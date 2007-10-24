// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
#include "SALOME_ResourcesManager.hxx" 
#include "Utils_ExceptHandlers.hxx"
#include "Utils_CorbaException.hxx"
#include "OpUtil.hxx"

#include <stdlib.h>
#ifndef WNT
#include <unistd.h>
#else
#include <io.h>
#include <process.h>
#endif
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <map>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>
#include "utilities.h"

#define MAX_SIZE_FOR_HOSTNAME 256;

using namespace std;

const char *SALOME_ResourcesManager::_ResourcesManagerNameInNS = "/ResourcesManager";

//=============================================================================
/*!
 * just for test
 */ 
//=============================================================================

SALOME_ResourcesManager::
SALOME_ResourcesManager(CORBA::ORB_ptr orb, 
			PortableServer::POA_var poa, 
			SALOME_NamingService *ns,
                        const char *xmlFilePath) :
    _path_resources(xmlFilePath)
{
  MESSAGE("constructor");
  _NS = ns;
  _orb = CORBA::ORB::_duplicate(orb) ;
  _poa = PortableServer::POA::_duplicate(poa) ;
  PortableServer::ObjectId_var id = _poa->activate_object(this);
  CORBA::Object_var obj = _poa->id_to_reference(id);
  Engines::SalomeLauncher_var refContMan =
    Engines::SalomeLauncher::_narrow(obj);

  _NS->Register(refContMan,_ResourcesManagerNameInNS);
  _MpiStarted = false;
  MESSAGE("constructor end");
}

//=============================================================================
/*!
 *  Standard constructor, parse resource file.
 *  - if ${APPLI} exists in environment,
 *    look for ${HOME}/*{APPLI}/CatalogResources.xml
 *  - else look for default:
 *    ${KERNEL_ROOT_DIR}/share/salome/resources/kernel/CatalogResources.xml
 *  - parse XML resource file.
 */ 
//=============================================================================

SALOME_ResourcesManager::SALOME_ResourcesManager(CORBA::ORB_ptr orb, 
						 PortableServer::POA_var poa, 
						 SALOME_NamingService *ns)
{
  MESSAGE("constructor");
  _NS = ns;
  _orb = CORBA::ORB::_duplicate(orb) ;
  _poa = PortableServer::POA::_duplicate(poa) ;
  PortableServer::ObjectId_var id = _poa->activate_object(this);
  CORBA::Object_var obj = _poa->id_to_reference(id);
  Engines::ResourcesManager_var refContMan = Engines::ResourcesManager::_narrow(obj);
  _NS->Register(refContMan,_ResourcesManagerNameInNS);

  _isAppliSalomeDefined = (getenv("APPLI") != 0);
  _MpiStarted = false;

  if (_isAppliSalomeDefined)
    {
      _path_resources = getenv("HOME");
      _path_resources += "/";
      _path_resources += getenv("APPLI");
      _path_resources += "/CatalogResources.xml";
    }

  else
    {
      _path_resources = getenv("KERNEL_ROOT_DIR");
      _path_resources += "/share/salome/resources/kernel/CatalogResources.xml";
    }

  ParseXmlFile();
  MESSAGE("constructor end");
}

//=============================================================================
/*!
 *  Standard Destructor
 */ 
//=============================================================================

SALOME_ResourcesManager::~SALOME_ResourcesManager()
{
  MESSAGE("destructor");
}


//=============================================================================
/*! CORBA method:
 *  shutdown all the containers, then the ContainerManager servant
 */
//=============================================================================

void SALOME_ResourcesManager::Shutdown()
{
  MESSAGE("Shutdown");
  _NS->Destroy_Name(_ResourcesManagerNameInNS);
  PortableServer::ObjectId_var oid = _poa->servant_to_id(this);
  _poa->deactivate_object(oid);
  _remove_ref();
}

//=============================================================================
/*!
 *  get the list of name of ressources fitting for the specified module.
 *  If hostname specified, check it is local or known in resources catalog.
 *
 *  Else
 *  - select first machines with corresponding OS (all machines if
 *    parameter OS empty),
 *  - then select the sublist of machines on witch the module is known
 *    (if the result is empty, that probably means that the inventory of
 *    modules is probably not done, so give complete list from previous step)
 */ 
//=============================================================================

Engines::MachineList *
SALOME_ResourcesManager::GetFittingResources(const Engines::MachineParameters& params,
					     const Engines::CompoList& componentList)
//throw(SALOME_Exception)
{
//   MESSAGE("ResourcesManager::GetFittingResources");
  vector <std::string> vec;
  Engines::MachineList *ret=new Engines::MachineList;

  try{
    // --- To be sure that we search in a correct list.
    ParseXmlFile();

    const char *hostname = (const char *)params.hostname;
    MESSAGE("GetFittingResources " << hostname << " " << GetHostname().c_str());

    if (hostname[0] != '\0')
      {
//       MESSAGE("ResourcesManager::GetFittingResources : hostname specified" );

	if ( strcmp(hostname, "localhost") == 0 ||
	     strcmp(hostname, GetHostname().c_str()) == 0 )
	  {
	    //           MESSAGE("ResourcesManager::GetFittingResources : localhost" );
	    vec.push_back(GetHostname().c_str());
	    // 	  MESSAGE("ResourcesManager::GetFittingResources : " << vec.size());
	  }
	
	else if (_resourcesList.find(hostname) != _resourcesList.end())
	  {
	    // --- params.hostname is in the list of resources so return it.
	    vec.push_back(hostname);
	  }
	
	else
	  {
	    // Cas d'un cluster: nombre de noeuds > 1
	    int cpt=0;
	    for (map<string, ParserResourcesType>::const_iterator iter = _resourcesList.begin(); iter != _resourcesList.end(); iter++){
	      if( (*iter).second.DataForSort._nbOfNodes > 1 ){
		if( strncmp(hostname,(*iter).first.c_str(),strlen(hostname)) == 0 ){
		  vec.push_back((*iter).first.c_str());
		  //cout << "SALOME_ResourcesManager::GetFittingResources vector["
		  //     << cpt << "] = " << (*iter).first.c_str() << endl ;
		  cpt++;
		}
	      }
	    }
	    if(cpt==0){
	      // --- user specified an unknown hostame so notify him.
	      MESSAGE("ResourcesManager::GetFittingResources : SALOME_Exception");
	      throw SALOME_Exception("unknown host");
	    }
	  }
      }
    
    else
      // --- Search for available resources sorted by priority
      {
	SelectOnlyResourcesWithOS(vec, params.OS);
	
	KeepOnlyResourcesWithModule(vec, componentList);
	
	if (vec.size() == 0)
	  SelectOnlyResourcesWithOS(vec, params.OS);
	
	// --- set wanted parameters
	ResourceDataToSort::_nbOfNodesWanted = params.nb_node;
	
	ResourceDataToSort::_nbOfProcPerNodeWanted = params.nb_proc_per_node;
	
	ResourceDataToSort::_CPUFreqMHzWanted = params.cpu_clock;
	
	ResourceDataToSort::_memInMBWanted = params.mem_mb;
	
	// --- end of set
	
	list<ResourceDataToSort> li;
	
	for (vector<string>::iterator iter = vec.begin();
           iter != vec.end();
	     iter++)
	  li.push_back(_resourcesList[(*iter)].DataForSort);
	
	li.sort();
	
	unsigned int i = 0;
	
	for (list<ResourceDataToSort>::iterator iter2 = li.begin();
	     iter2 != li.end();
	     iter2++)
	  vec[i++] = (*iter2)._hostName;
      }
    
    //  MESSAGE("ResourcesManager::GetFittingResources : return" << ret.size());
    ret->length(vec.size());
    for(unsigned int i=0;i<vec.size();i++)
      (*ret)[i]=(vec[i]).c_str();

  }
  catch(const SALOME_Exception &ex)
    {
      INFOS("Caught exception.");
      THROW_SALOME_CORBA_EXCEPTION(ex.what(),SALOME::BAD_PARAM);
      //return ret;
    }  

  return ret;
}

//=============================================================================
/*!
 *  add an entry in the ressources catalog  xml file.
 *  Return 0 if OK (KERNEL found in new resources modules) else throw exception
 */ 
//=============================================================================

int
SALOME_ResourcesManager::
AddResourceInCatalog(const Engines::MachineParameters& paramsOfNewResources,
                     const vector<string>& modulesOnNewResources,
                     const char *alias,
                     const char *userName,
                     AccessModeType mode,
                     AccessProtocolType prot)
throw(SALOME_Exception)
{
  vector<string>::const_iterator iter = find(modulesOnNewResources.begin(),
					     modulesOnNewResources.end(),
					     "KERNEL");

  if (iter != modulesOnNewResources.end())
    {
      ParserResourcesType newElt;
      newElt.DataForSort._hostName = paramsOfNewResources.hostname;
      newElt.Alias = alias;
      newElt.Protocol = prot;
      newElt.Mode = mode;
      newElt.UserName = userName;
      newElt.ModulesList = modulesOnNewResources;
      newElt.OS = paramsOfNewResources.OS;
      newElt.DataForSort._memInMB = paramsOfNewResources.mem_mb;
      newElt.DataForSort._CPUFreqMHz = paramsOfNewResources.cpu_clock;
      newElt.DataForSort._nbOfNodes = paramsOfNewResources.nb_node;
      newElt.DataForSort._nbOfProcPerNode =
        paramsOfNewResources.nb_proc_per_node;
      _resourcesList[newElt.DataForSort._hostName] = newElt;
      return 0;
    }

  else
    throw SALOME_Exception("KERNEL is not present in this resource");
}

//=============================================================================
/*!
 *  Deletes a resource from the catalog
 */ 
//=============================================================================

void SALOME_ResourcesManager::DeleteResourceInCatalog(const char *hostname)
{
  _resourcesList.erase(hostname);
}

//=============================================================================
/*!
 *  write the current data in memory in file.
 */ 
//=============================================================================

void SALOME_ResourcesManager::WriteInXmlFile()
{
  const char* aFilePath = _path_resources.c_str();
  
  FILE* aFile = fopen(aFilePath, "w");

  if (aFile == NULL)
    {
      INFOS("Error opening file !");
      return;
    }
  
  xmlDocPtr aDoc = xmlNewDoc(BAD_CAST "1.0");
  xmlNewDocComment(aDoc, BAD_CAST "ResourcesCatalog");

  SALOME_ResourcesCatalog_Handler* handler =
    new SALOME_ResourcesCatalog_Handler(_resourcesList);
  handler->PrepareDocToXmlFile(aDoc);
  delete handler;

  int isOk = xmlSaveFile(aFilePath, aDoc);
  
  if (!isOk)
    INFOS("Error while XML file saving.");
  
  // Free the document
  xmlFreeDoc(aDoc);

  // Free the global variables that may have been allocated by the parser
  xmlCleanupParser();
  
  fclose(aFile);
  
  MESSAGE("WRITING DONE!");
}

//=============================================================================
/*!
 *  parse the data type catalog
 */ 
//=============================================================================

const MapOfParserResourcesType& SALOME_ResourcesManager::ParseXmlFile()
{
  SALOME_ResourcesCatalog_Handler* handler =
    new SALOME_ResourcesCatalog_Handler(_resourcesList);

  const char* aFilePath = _path_resources.c_str();
  FILE* aFile = fopen(aFilePath, "r");
  
  if (aFile != NULL)
    {
      xmlDocPtr aDoc = xmlReadFile(aFilePath, NULL, 0);
      
      if (aDoc != NULL)
	handler->ProcessXmlDocument(aDoc);
      else
	INFOS("ResourcesManager: could not parse file "<<aFilePath);
      
      // Free the document
      xmlFreeDoc(aDoc);

      // Free the global variables that may have been allocated by the parser
      xmlCleanupParser();
      fclose(aFile);
    }
  else
    INFOS("ResourcesManager: file "<<aFilePath<<" is not readable.");
  
  delete handler;

  return _resourcesList;
}

//=============================================================================
/*!
 *   consult the content of the list
 */ 
//=============================================================================

const MapOfParserResourcesType& SALOME_ResourcesManager::GetList() const
  {
    return _resourcesList;
  }


//=============================================================================
/*!
 *  dynamically obtains the first machines
 */ 
//=============================================================================

char *
SALOME_ResourcesManager::FindFirst(const Engines::MachineList& listOfMachines)
{
  return CORBA::string_dup(_dynamicResourcesSelecter.FindFirst(listOfMachines).c_str());
}

//=============================================================================
/*!
 *  dynamically obtains the best machines
 */ 
//=============================================================================

string
SALOME_ResourcesManager::FindNext(const Engines::MachineList& listOfMachines)
{
  return _dynamicResourcesSelecter.FindNext(listOfMachines,_resourcesList,_NS);
}
//=============================================================================
/*!
 *  dynamically obtains the best machines
 */ 
//=============================================================================

string
SALOME_ResourcesManager::FindBest(const Engines::MachineList& listOfMachines)
{
  return _dynamicResourcesSelecter.FindBest(listOfMachines);
}

//=============================================================================
/*!
 *  This is no longer valid (C++ container are also python containers)
 */ 
//=============================================================================

bool isPythonContainer(const char* ContainerName)
{
  bool ret = false;
  int len = strlen(ContainerName);

  if (len >= 2)
    if (strcmp(ContainerName + len - 2, "Py") == 0)
      ret = true;

  return ret;
}


//=============================================================================
/*!
 *  Builds the script to be launched
 *
 *  If SALOME Application not defined ($APPLI),
 *  see BuildTempFileToLaunchRemoteContainer()
 *
 *  Else rely on distant configuration. Command is under the form (example):
 *  ssh user@machine distantPath/runRemote.sh hostNS portNS \
 *                   SALOME_Container containerName &"

 *  - where user is ommited if not specified in CatalogResources,
 *  - where distant path is always relative to user@machine $HOME, and
 *    equal to $APPLI if not specified in CatalogResources,
 *  - where hostNS is the hostname of CORBA naming server (set by scripts to
 *    use to launch SALOME and servers in $APPLI: runAppli.sh, runRemote.sh)
 *  - where portNS is the port used by CORBA naming server (set by scripts to
 *    use to launch SALOME and servers in $APPLI: runAppli.sh, runRemote.sh)
 */ 
//=============================================================================

string
SALOME_ResourcesManager::BuildCommandToLaunchRemoteContainer
(const string& machine,
 const Engines::MachineParameters& params, const long id)
{
  string command;
  int nbproc;
  char idc[3*sizeof(long)];
	  
  if ( ! _isAppliSalomeDefined )
    command = BuildTempFileToLaunchRemoteContainer(machine, params);

  else
    {
      const ParserResourcesType& resInfo = _resourcesList[machine];

      if (params.isMPI)
        {
          if ( (params.nb_node <= 0) && (params.nb_proc_per_node <= 0) )
            nbproc = 1;
          else if ( params.nb_node == 0 )
            nbproc = params.nb_proc_per_node;
          else if ( params.nb_proc_per_node == 0 )
            nbproc = params.nb_node;
          else
            nbproc = params.nb_node * params.nb_proc_per_node;
        }

      // "ssh user@machine distantPath/runRemote.sh hostNS portNS \
      //  SALOME_Container containerName &"

      if (resInfo.Protocol == rsh)
        command = "rsh ";
      else if (resInfo.Protocol == ssh)
        command = "ssh ";
      else
        throw SALOME_Exception("Unknown protocol");

      if (resInfo.UserName != "")
	{
	  command += resInfo.UserName;
	  command += "@";
	}

      command += machine;
      command += " ";

      if (resInfo.AppliPath != "")
	command += resInfo.AppliPath; // path relative to user@machine $HOME
      else
	{
	  ASSERT(getenv("APPLI"));
	  command += getenv("APPLI"); // path relative to user@machine $HOME
	}

      command += "/runRemote.sh ";

      ASSERT(getenv("NSHOST")); 
      command += getenv("NSHOST"); // hostname of CORBA name server

      command += " ";
      ASSERT(getenv("NSPORT"));
      command += getenv("NSPORT"); // port of CORBA name server

      if(params.isMPI)
	{
	  command += " mpirun -np ";
	  std::ostringstream o;
	  o << nbproc << " ";
	  command += o.str();
#ifdef WITHLAM
	  command += "-x PATH,LD_LIBRARY_PATH,OMNIORB_CONFIG,SALOME_trace ";
#endif	
	  command += " SALOME_MPIContainer ";
	}
      else
	command += " SALOME_Container ";

      command += _NS->ContainerName(params);
      command += " -id ";
      sprintf(idc,"%ld",id);
      command += idc;
      command += " -";
      AddOmninamesParams(command);
      command += " > /tmp/";
      command += _NS->ContainerName(params);
      command += "_";
      command += GetHostname();
      command += "_";
      command += getenv( "USER" ) ;
      command += ".log 2>&1 &" ;

      MESSAGE("command =" << command);
    }

  return command;
}

//=============================================================================
/*!
 *  builds the command to be launched.
 */ 
//=============================================================================

string
SALOME_ResourcesManager::BuildCommandToLaunchLocalContainer
(const Engines::MachineParameters& params, const long id)
{
  _TmpFileName = "";
  string command;
  int nbproc = 0;
  char idc[3*sizeof(long)];

  if (params.isMPI)
    {
      command = "mpirun -np ";

      if ( (params.nb_node <= 0) && (params.nb_proc_per_node <= 0) )
        nbproc = 1;
      else if ( params.nb_node == 0 )
        nbproc = params.nb_proc_per_node;
      else if ( params.nb_proc_per_node == 0 )
        nbproc = params.nb_node;
      else
        nbproc = params.nb_node * params.nb_proc_per_node;

      std::ostringstream o;

      o << nbproc << " ";

      command += o.str();
#ifdef WITHLAM
      command += "-x PATH,LD_LIBRARY_PATH,OMNIORB_CONFIG,SALOME_trace ";
#endif

      if (isPythonContainer(params.container_name))
        command += "pyMPI SALOME_ContainerPy.py ";
      else
        command += "SALOME_MPIContainer ";
    }

  else
    {
      if (isPythonContainer(params.container_name))
        command = "SALOME_ContainerPy.py ";
      else
        command = "SALOME_Container ";
    }

  command += _NS->ContainerName(params);
  command += " -id ";
  sprintf(idc,"%ld",id);
  command += idc;
  command += " -";
  AddOmninamesParams(command);
  command += " > /tmp/";
  command += _NS->ContainerName(params);
  command += "_";
  command += GetHostname();
  command += "_";
  command += getenv( "USER" ) ;
  command += ".log 2>&1 &" ;
  MESSAGE("Command is ... " << command);
  return command;
}


//=============================================================================
/*!
 *  removes the generated temporary file in case of a remote launch.
 */ 
//=============================================================================

void SALOME_ResourcesManager::RmTmpFile()
{
  if (_TmpFileName != "")
    {
#ifndef WNT
      string command = "rm ";
#else
      string command = "del /F ";
#endif
      command += _TmpFileName;
      char *temp = strdup(command.c_str());
      int lgthTemp = strlen(temp);
      temp[lgthTemp - 3] = '*';
      temp[lgthTemp - 2] = '\0';
      system(temp);
      free(temp);
    }
}


//=============================================================================
/*!
 *  builds the script to be launched
 */ 
//=============================================================================

string
SALOME_ResourcesManager::BuildCommand
(const string& machine,
 const char *containerName)
{
  // rsh -n ikkyo /export/home/rahuel/SALOME_ROOT/bin/runSession SALOME_Container -ORBInitRef NameService=corbaname::dm2s0017:1515 &
  const ParserResourcesType& resInfo = _resourcesList[machine];
  bool pyCont = isPythonContainer(containerName);

  string command;

  if (resInfo.Protocol == rsh)
    command = "rsh -n " ;
  else if (resInfo.Protocol == ssh)
    command = "ssh -f -n ";
  else
    throw SALOME_Exception("Not implemented yet...");

  command += machine;
  command += " ";
  string path = getenv("KERNEL_ROOT_DIR");
  command += path;
  command += "/bin/salome/";

  if ( pyCont )
    command += "SALOME_ContainerPy.py ";
  else
    command += "SALOME_Container ";

  command += containerName;
  command += " -";
  AddOmninamesParams(command);
  command += " > /tmp/";
  command += containerName;
  command += "_";
  command += machine;
  command += "_";
  command += getenv( "USER" ) ;
  command += ".log 2>&1 &" ;

  SCRUTE( command );
  return command;
}

//=============================================================================
/*!
 *  Gives a sublist of machines with matching OS.
 *  If parameter OS is empty, gives the complete list of machines
 */ 
//=============================================================================

// Warning need an updated parsed list : _resourcesList
void
SALOME_ResourcesManager::SelectOnlyResourcesWithOS
( vector<string>& hosts,
  const char *OS) const
throw(SALOME_Exception)
{
  string base(OS);

  for (map<string, ParserResourcesType>::const_iterator iter =
         _resourcesList.begin();
       iter != _resourcesList.end();
       iter++)
    {
      if ( (*iter).second.OS == base || base.size() == 0)
        hosts.push_back((*iter).first);
    }
}


//=============================================================================
/*!
 *  Gives a sublist of machines on which the module is known.
 */ 
//=============================================================================

//Warning need an updated parsed list : _resourcesList
void
SALOME_ResourcesManager::KeepOnlyResourcesWithModule
( vector<string>& hosts,
  const Engines::CompoList& componentList) const
throw(SALOME_Exception)
{
  for (vector<string>::iterator iter = hosts.begin(); iter != hosts.end();)
    {
      MapOfParserResourcesType::const_iterator it = _resourcesList.find(*iter);
      const vector<string>& mapOfModulesOfCurrentHost = (((*it).second).ModulesList);

      bool erasedHost = false;
      if( mapOfModulesOfCurrentHost.size() > 0 ){
	for(int i=0;i<componentList.length();i++){
          const char* compoi = componentList[i];
	  vector<string>::const_iterator itt = find(mapOfModulesOfCurrentHost.begin(),
					      mapOfModulesOfCurrentHost.end(),
					      compoi);
// 					      componentList[i]);
	  if (itt == mapOfModulesOfCurrentHost.end()){
	    erasedHost = true;
	    break;
	  }
	}
      }
      if(erasedHost)
        hosts.erase(iter);
      else
        iter++;
    }
}


//=============================================================================
/*!
 *   add to command all options relative to naming service.
 */ 
//=============================================================================

void SALOME_ResourcesManager::AddOmninamesParams(string& command) const
  {
    // If env variable OMNIORB_CONFIG is not defined or the file is more complex than one line
    // does not work
    // Even if we use it we have to check if env variable exists
    //string omniORBcfg( getenv( "OMNIORB_CONFIG" ) ) ;
    //ifstream omniORBfile( omniORBcfg.c_str() ) ;
    //char ORBInitRef[11] ;
    //char egal[3] ;
    //char nameservice[132] ;
    //omniORBfile >> ORBInitRef ;
    //command += "ORBInitRef " ;
    //omniORBfile >> egal ;
    //omniORBfile >> nameservice ;
    //omniORBfile.close() ;
    //char * bsn = strchr( nameservice , '\n' ) ;
    //if ( bsn ) {
    //bsn[ 0 ] = '\0' ;
    //}
    //command += nameservice ;

    char *iorstr = _NS->getIORaddr();
    command += "ORBInitRef NameService=";
    command += iorstr;
  }


//=============================================================================
/*!
 *  add to command all options relative to naming service.
 */ 
//=============================================================================

void SALOME_ResourcesManager::AddOmninamesParams(ofstream& fileStream) const
  {
    fileStream << "ORBInitRef NameService=";
    fileStream << _NS->getIORaddr();
  }


//=============================================================================
/*!
 *  generate a file name in /tmp directory
 */ 
//=============================================================================

string SALOME_ResourcesManager::BuildTemporaryFileName() const
  {
    //build more complex file name to support multiple salome session
    char *temp = new char[19];
    strcpy(temp, "/tmp/command");
    strcat(temp, "XXXXXX");
#ifndef WNT

    mkstemp(temp);
#else

    char aPID[80];
    itoa(getpid(), aPID, 10);
    strcat(temp, aPID);
#endif

    string command(temp);
    delete [] temp;
    command += ".sh";
    return command;
  }


//=============================================================================
/*!
 *  Builds in a temporary file the script to be launched.
 *  
 *  Used if SALOME Application ($APPLI) is not defined.
 *  The command is build with data from CatalogResources, in which every path
 *  used on remote computer must be defined.
 */ 
//=============================================================================

string
SALOME_ResourcesManager::BuildTempFileToLaunchRemoteContainer
(const string& machine,
 const Engines::MachineParameters& params) throw(SALOME_Exception)
{
  int status;

  _TmpFileName = BuildTemporaryFileName();
  ofstream tempOutputFile;
  tempOutputFile.open(_TmpFileName.c_str(), ofstream::out );
  const ParserResourcesType& resInfo = _resourcesList[machine];
  tempOutputFile << "#! /bin/sh" << endl;

  // --- set env vars

  tempOutputFile << "export SALOME_trace=local" << endl; // mkr : 27.11.2006 : PAL13967 - Distributed supervision graphs - Problem with "SALOME_trace"
  //tempOutputFile << "source " << resInfo.PreReqFilePath << endl;

  // ! env vars

  if (params.isMPI)
    {
      tempOutputFile << "mpirun -np ";
      int nbproc;

      if ( (params.nb_node <= 0) && (params.nb_proc_per_node <= 0) )
        nbproc = 1;
      else if ( params.nb_node == 0 )
        nbproc = params.nb_proc_per_node;
      else if ( params.nb_proc_per_node == 0 )
        nbproc = params.nb_node;
      else
        nbproc = params.nb_node * params.nb_proc_per_node;

      std::ostringstream o;

      tempOutputFile << nbproc << " ";
#ifdef WITHLAM
      tempOutputFile << "-x PATH,LD_LIBRARY_PATH,OMNIORB_CONFIG,SALOME_trace ";
#endif
    }

  tempOutputFile << getenv("KERNEL_ROOT_DIR") << "/bin/salome/";

  if (params.isMPI)
    {
      if (isPythonContainer(params.container_name))
        tempOutputFile << "pyMPI SALOME_ContainerPy.py ";
      else
        tempOutputFile << "SALOME_MPIContainer ";
    }

  else
    {
      if (isPythonContainer(params.container_name))
        tempOutputFile << "SALOME_ContainerPy.py ";
      else
        tempOutputFile << "SALOME_Container ";
    }

  tempOutputFile << _NS->ContainerName(params) << " -";
  AddOmninamesParams(tempOutputFile);
  tempOutputFile << " &" << endl;
  tempOutputFile.flush();
  tempOutputFile.close();
  chmod(_TmpFileName.c_str(), 0x1ED);

  // --- Build command

  string command;

  if (resInfo.Protocol == rsh)
    {
      command = "rsh ";
      string commandRcp = "rcp ";
      commandRcp += _TmpFileName;
      commandRcp += " ";
      commandRcp += machine;
      commandRcp += ":";
      commandRcp += _TmpFileName;
      status = system(commandRcp.c_str());
    }

  else if (resInfo.Protocol == ssh)
    {
      command = "ssh ";
      string commandRcp = "scp ";
      commandRcp += _TmpFileName;
      commandRcp += " ";
      commandRcp += machine;
      commandRcp += ":";
      commandRcp += _TmpFileName;
      status = system(commandRcp.c_str());
    }
  else
    throw SALOME_Exception("Unknown protocol");

  if(status)
    throw SALOME_Exception("Error of connection on remote host");    

  command += machine;
  _CommandForRemAccess = command;
  command += " ";
  command += _TmpFileName;
  command += " > ";
  command += "/tmp/";
  command += _NS->ContainerName(params);
  command += "_";
  command += machine;
  command += "_";
  command += getenv( "USER" ) ;
  command += ".log 2>&1 &";
  SCRUTE(command);

  return command;

}

//=============================================================================
/*! Creates a command line that the container manager uses to launch
 * a parallel container.
 */ 
//=============================================================================
string 
SALOME_ResourcesManager::BuildCommandToLaunchLocalParallelContainer(const std::string& exe_name,
								    const Engines::MachineParameters& params,
								    const std::string& log)
{
  // This method knows the differences between the proxy and the nodes.
  // nb_component_nodes is not used in the same way if it is a proxy or 
  // a node.

  string command;
  string parallelLib(CORBA::string_dup(params.parallelLib));
  string hostname(CORBA::string_dup(params.hostname));
  int par = exe_name.find("Proxy");
  int nbproc = params.nb_component_nodes;
  char buffer [33];
  sprintf(buffer,"%d",nbproc);

  Engines::MachineParameters_var rtn = new Engines::MachineParameters();
  rtn->container_name = params.container_name;
  rtn->hostname = params.hostname;
  rtn->OS = params.OS;
  rtn->mem_mb = params.mem_mb;
  rtn->cpu_clock = params.cpu_clock;
  rtn->nb_proc_per_node = params.nb_proc_per_node;
  rtn->nb_node = params.nb_node;
  rtn->isMPI = params.isMPI;

  string real_exe_name  = exe_name + parallelLib;

  if (parallelLib == "Dummy")
  {
    //command = "gdb --args ";
    //command = "valgrind --tool=memcheck --log-file=val_log ";
    //command += real_exe_name;
    
    command = real_exe_name;
    
    command += " " + _NS->ContainerName(rtn);
    command += " " + parallelLib;
    command += " " + hostname;
    command += " -";
    AddOmninamesParams(command);
  }

  if (parallelLib == "Mpi")
  {
    // Step 1 : check if MPI is started
    if (_MpiStarted == false)
    {
      startMPI();
    }

    if (par < 0)
    {
      // Nodes case

      command = "mpiexec -np " + string(buffer) + " ";
//      command += "gdb --args ";
      command += real_exe_name;
      command += " " + _NS->ContainerName(rtn);
      command += " " + parallelLib;
      command += " " + hostname;
      command += " -";
      AddOmninamesParams(command);
    }
    else                                          
    {
      // Proxy case
      command = "mpiexec -np 1 ";
      command += real_exe_name;
      command += " " + _NS->ContainerName(rtn);
      command += " " + string(buffer);
      command += " " + parallelLib;
      command += " " + hostname;
      command += " -";
      AddOmninamesParams(command);
    }
  }

  // log choice
  if (log == "default")
  {
    command += " > /tmp/";
    command += _NS->ContainerName(rtn);
    command += "_";
    command += GetHostname();
    command += "_";
    command += getenv( "USER" ) ;
    command += ".log 2>&1 &" ;
  }
  if (log == "xterm")
  {
    command = "/usr/X11R6/bin/xterm -e \"export LD_LIBRARY_PATH=$LD_LIBRARY_PATH; export PATH=$PATH;  " 
	      + command + " \" &";
//	      + command + "; echo $LD_LIBRARY_PATH; cat \" &";
  }
  return command;

/*  if (log == "xterm")
  {
    command = "/usr/X11R6/bin/xterm -e \"export LD_LIBRARY_PATH=$LD_LIBRARY_PATH; export PATH=$PATH; echo $LD_LIBRARY_PATH; echo $PATH; " + command + "; cat \" &";
  }
*/
/*  command = "cd ; rm " + fichier_commande + "; touch " + \
	     fichier_commande + "; echo \" export LD_LIBRARY_PATH=$LD_LIBRARY_PATH; " + \
	     command + " >& /tmp/ribes_" + fichier_commande + " & \" > " + fichier_commande + ";";
  command += "ssh cn01 sh " + fichier_commande + " &";
  cerr << "La commande : " << command << endl;
*/
}

void SALOME_ResourcesManager::startMPI()
{
  cerr << "----------------------------------------------" << endl;
  cerr << "----------------------------------------------" << endl;
  cerr << "----------------------------------------------" << endl;
  cerr << "-Only Lam on Localhost is currently supported-" << endl;
  cerr << "----------------------------------------------" << endl;
  cerr << "----------------------------------------------" << endl;
  cerr << "----------------------------------------------" << endl;

  int status = system("lamboot");
  if (status == -1)
  {
    INFOS("lamboot failed : system command status -1");
  }
  else if (status == 217)
  {
    INFOS("lamboot failed : system command status 217");
  }
  else
  {
    _MpiStarted = true;
  }
}

Engines::MachineParameters* SALOME_ResourcesManager::GetMachineParameters(const char *hostname)
{
  ParserResourcesType resource = _resourcesList[string(hostname)];
  Engines::MachineParameters *p_ptr = new Engines::MachineParameters;
  p_ptr->container_name = CORBA::string_dup("");
  p_ptr->hostname = CORBA::string_dup("hostname");
  p_ptr->alias = CORBA::string_dup(resource.Alias.c_str());
  if( resource.Protocol == rsh )
    p_ptr->protocol = "rsh";
  else if( resource.Protocol == ssh )
    p_ptr->protocol = "ssh";
  p_ptr->username = CORBA::string_dup(resource.UserName.c_str());
  p_ptr->applipath = CORBA::string_dup(resource.AppliPath.c_str());
  p_ptr->modList.length(resource.ModulesList.size());
  for(int i=0;i<resource.ModulesList.size();i++)
    p_ptr->modList[i] = CORBA::string_dup(resource.ModulesList[i].c_str());
  p_ptr->OS = CORBA::string_dup(resource.OS.c_str());
  p_ptr->mem_mb = resource.DataForSort._memInMB;
  p_ptr->cpu_clock = resource.DataForSort._CPUFreqMHz;
  p_ptr->nb_proc_per_node = resource.DataForSort._nbOfProcPerNode;
  p_ptr->nb_node = resource.DataForSort._nbOfNodes;
  if( resource.mpi == indif )
    p_ptr->mpiImpl = "indif";
  else if( resource.mpi == lam )
    p_ptr->mpiImpl = "lam";
  else if( resource.mpi == mpich1 )
    p_ptr->mpiImpl = "mpich1";
  else if( resource.mpi == mpich2 )
    p_ptr->mpiImpl = "mpich2";
  else if( resource.mpi == openmpi )
    p_ptr->mpiImpl = "openmpi";
  if( resource.Batch == pbs )
    p_ptr->batch = "pbs";
  else if( resource.Batch == lsf )
    p_ptr->batch = "lsf";
  else if( resource.Batch == slurm )
    p_ptr->batch = "slurm";
  return p_ptr;
}
