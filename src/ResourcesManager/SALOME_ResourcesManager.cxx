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
//#include "SALOME_Container_i.hxx"
#include "Utils_ExceptHandlers.hxx"
#include "OpUtil.hxx"

#include <qdom.h>

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

//=============================================================================
/*!
 * just for test
 */ 
//=============================================================================

SALOME_ResourcesManager::
SALOME_ResourcesManager(CORBA::ORB_ptr orb,
                        const char *xmlFilePath) :
    _path_resources(xmlFilePath)
{
  _NS = new SALOME_NamingService(orb);
  _MpiStarted = false;
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

SALOME_ResourcesManager::SALOME_ResourcesManager(CORBA::ORB_ptr orb)
{
  _NS = new SALOME_NamingService(orb);
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
}

//=============================================================================
/*!
 *  Standard Destructor
 */ 
//=============================================================================

SALOME_ResourcesManager::~SALOME_ResourcesManager()
{
  delete _NS;
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

vector<string>
SALOME_ResourcesManager::
GetFittingResources(const Engines::MachineParameters& params,
                    const char *moduleName)
throw(SALOME_Exception)
{
//   MESSAGE("ResourcesManager::GetFittingResources");
  vector <std::string> ret;

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
          ret.push_back(GetHostname().c_str());
// 	  MESSAGE("ResourcesManager::GetFittingResources : " << ret.size());
        }

      else if (_resourcesList.find(hostname) != _resourcesList.end())
        {
          // --- params.hostname is in the list of resources so return it.
          ret.push_back(hostname);
        }

      else
        {
// Cas d'un cluster: nombre de noeuds > 1
          int cpt=0;
          for (map<string, ParserResourcesType>::const_iterator iter = _resourcesList.begin(); iter != _resourcesList.end(); iter++){
	    if( (*iter).second.DataForSort._nbOfNodes > 1 ){
   	      if( strncmp(hostname,(*iter).first.c_str(),strlen(hostname)) == 0 ){
                ret.push_back((*iter).first.c_str());
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
      SelectOnlyResourcesWithOS(ret, params.OS);

      KeepOnlyResourcesWithModule(ret, moduleName);

      if (ret.size() == 0)
        SelectOnlyResourcesWithOS(ret, params.OS);

      // --- set wanted parameters
      ResourceDataToSort::_nbOfNodesWanted = params.nb_node;

      ResourceDataToSort::_nbOfProcPerNodeWanted = params.nb_proc_per_node;

      ResourceDataToSort::_CPUFreqMHzWanted = params.cpu_clock;

      ResourceDataToSort::_memInMBWanted = params.mem_mb;

      // --- end of set

      list<ResourceDataToSort> li;

      for (vector<string>::iterator iter = ret.begin();
           iter != ret.end();
           iter++)
        li.push_back(_resourcesList[(*iter)].DataForSort);

      li.sort();

      unsigned int i = 0;

      for (list<ResourceDataToSort>::iterator iter2 = li.begin();
           iter2 != li.end();
           iter2++)
        ret[i++] = (*iter2)._hostName;
    }

  //  MESSAGE("ResourcesManager::GetFittingResources : return" << ret.size());
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
                     const map<string, string>& modulesOnNewResources,
                     const char *environPathOfPrerequired,
                     const char *alias,
                     const char *userName,
                     AccessModeType mode,
                     AccessProtocolType prot)
throw(SALOME_Exception)
{
  map<string, string>::const_iterator iter =
    modulesOnNewResources.find("KERNEL");

  if (iter != modulesOnNewResources.end())
    {
      ParserResourcesType newElt;
      newElt.DataForSort._hostName = paramsOfNewResources.hostname;
      newElt.Alias = alias;
      newElt.Protocol = prot;
      newElt.Mode = mode;
      newElt.UserName = userName;
      newElt.ModulesPath = modulesOnNewResources;
      newElt.PreReqFilePath = environPathOfPrerequired;
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
  QDomDocument doc("ResourcesCatalog");
  SALOME_ResourcesCatalog_Handler* handler =
    new SALOME_ResourcesCatalog_Handler(_resourcesList);
  handler->PrepareDocToXmlFile(doc);
  delete handler;

  QFile file( _path_resources );

  if ( !file.open( IO_WriteOnly ) )
    INFOS("WRITING ERROR !");

  QTextStream ts( &file );

  ts << doc.toString();

  file.close();

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
  QFile xmlFile(_path_resources);

  QXmlInputSource source(xmlFile);

  QXmlSimpleReader reader;
  reader.setContentHandler( handler );
  reader.setErrorHandler( handler );
  reader.parse( source );
  xmlFile.close();
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
  string path = (*(resInfo.ModulesPath.find("KERNEL"))).second;
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
  const char *moduleName) const
throw(SALOME_Exception)
{
  for (vector<string>::iterator iter = hosts.begin(); iter != hosts.end();)
    {
      MapOfParserResourcesType::const_iterator it = _resourcesList.find(*iter);
      const map<string, string>& mapOfModulesOfCurrentHost =
        (((*it).second).ModulesPath);

      if (mapOfModulesOfCurrentHost.find(moduleName) ==
          mapOfModulesOfCurrentHost.end())
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
 const Engines::MachineParameters& params)
{
  _TmpFileName = BuildTemporaryFileName();
  ofstream tempOutputFile;
  tempOutputFile.open(_TmpFileName.c_str(), ofstream::out );
  const ParserResourcesType& resInfo = _resourcesList[machine];
  tempOutputFile << "#! /bin/sh" << endl;

  // --- set env vars

  tempOutputFile << "source " << resInfo.PreReqFilePath << endl;

  for (map<string, string>::const_iterator iter = resInfo.ModulesPath.begin();
       iter != resInfo.ModulesPath.end();
       iter++)
    {
      string curModulePath((*iter).second);
      tempOutputFile << (*iter).first << "_ROOT_DIR=" << curModulePath << endl;
      tempOutputFile << "export " << (*iter).first << "_ROOT_DIR" << endl;
      tempOutputFile << "LD_LIBRARY_PATH=" << curModulePath
		     << "/lib/salome" << ":${LD_LIBRARY_PATH}" << endl;
      tempOutputFile << "PYTHONPATH=" << curModulePath << "/bin/salome:"
		     << curModulePath << "/lib/salome:" << curModulePath
		     << "/lib/python${PYTHON_VERSION}/site-packages/salome:";
      tempOutputFile << curModulePath
      << "/lib/python${PYTHON_VERSION}/site-packages/salome/shared_modules:${PYTHONPATH}"
      << endl;
    }

  tempOutputFile << "export LD_LIBRARY_PATH" << endl;
  tempOutputFile << "export PYTHONPATH" << endl;
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

  tempOutputFile << (*(resInfo.ModulesPath.find("KERNEL"))).second
		 << "/bin/salome/";

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
      system(commandRcp.c_str());
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
      system(commandRcp.c_str());
    }
  else
    throw SALOME_Exception("Unknown protocol");

  command += machine;
  _CommandForRemAccess = command;
  command += " ";
  command += _TmpFileName;
  command += " > ";
  command += "/tmp/";
  command += _NS->ContainerName(params);
  command += "_";
  command += machine;
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


