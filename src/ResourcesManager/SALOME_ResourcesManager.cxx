#include "SALOME_ResourcesManager.hxx"
#include "SALOME_Container_i.hxx"
#include "Utils_ExceptHandlers.hxx"
#include "OpUtil.hxx"

#include <qdom.h>

#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <map>
#include <list>

#include <sys/types.h>
#include <sys/stat.h>

#define MAX_SIZE_FOR_HOSTNAME 256;

using namespace std;

//just for test
SALOME_ResourcesManager::SALOME_ResourcesManager(const char *xmlFilePath):_path_resources(xmlFilePath)
{
}

SALOME_ResourcesManager::SALOME_ResourcesManager(CORBA::ORB_ptr orb)
{
  _NS = new SALOME_NamingService(orb);
  _path_resources=getenv("KERNEL_ROOT_DIR");
  _path_resources+="/share/salome/resources/CatalogResources.xml";
  ParseXmlFile();
}

SALOME_ResourcesManager::~SALOME_ResourcesManager()
{
  delete _NS;
}

vector<string> SALOME_ResourcesManager::GetFittingResources(const Engines::MachineParameters& params,const char *moduleName) throw(SALOME_Exception)
{
  vector <std::string> ret;
  //To be sure that we search in a correct list.
  ParseXmlFile();
  const char *hostname=(const char *)params.hostname;
  if(hostname[0]!='\0')
    {
      if(_resourcesList.find(hostname)!=_resourcesList.end())
	// params.hostame is in the list of resources so return it.
	ret.push_back(hostname);
      else
	//user specified an unknown hostame so notify to him.
	throw SALOME_Exception("unknown host");
    }
  else
    // Search for available resources sorted by priority
    {
      SelectOnlyResourcesWithOS(ret,params.OS);
      KeepOnlyResourcesWithModule(ret,moduleName);
      //set wanted parameters
      ResourceDataToSort::_nbOfNodesWanted=params.nb_node;
      ResourceDataToSort::_nbOfProcPerNodeWanted=params.nb_proc_per_node;
      ResourceDataToSort::_CPUFreqMHzWanted=params.cpu_clock;
      ResourceDataToSort::_memInMBWanted=params.mem_mb;
      //end of set
      list<ResourceDataToSort> li;
      for(vector<string>::iterator iter=ret.begin();iter!=ret.end();iter++)
	li.push_back(_resourcesList[(*iter)].DataForSort);
      li.sort();
      unsigned int i=0;
      for(list<ResourceDataToSort>::iterator iter2=li.begin();iter2!=li.end();iter2++)
	ret[i++]=(*iter2)._hostName;
    }
  return ret;
}

int SALOME_ResourcesManager::AddResourceInCatalog(const Engines::MachineParameters& paramsOfNewResources, const map<string,string>& modulesOnNewResources,
						  const char *environPathOfPrerequired,
						  const char *alias, const char *userName, AccessModeType mode, AccessProtocolType prot) throw(SALOME_Exception)
{
  map<string,string>::const_iterator iter=modulesOnNewResources.find("KERNEL");
  if(iter!=modulesOnNewResources.end())
    {
      ParserResourcesType newElt;
      newElt.DataForSort._hostName=paramsOfNewResources.hostname;
      newElt.Alias=alias;
      newElt.Protocol=prot;
      newElt.Mode=mode;
      newElt.UserName=userName;
      newElt.ModulesPath=modulesOnNewResources;
      newElt.PreReqFilePath=environPathOfPrerequired;
      newElt.OS=paramsOfNewResources.OS;
      newElt.DataForSort._memInMB=paramsOfNewResources.mem_mb;
      newElt.DataForSort._CPUFreqMHz=paramsOfNewResources.cpu_clock;
      newElt.DataForSort._nbOfNodes=paramsOfNewResources.nb_node;
      newElt.DataForSort._nbOfProcPerNode=paramsOfNewResources.nb_proc_per_node;
      _resourcesList[newElt.DataForSort._hostName]=newElt;
      return 0;
    }
  else
    throw SALOME_Exception("KERNEL is not present in this resource");
}

void SALOME_ResourcesManager::DeleteResourceInCatalog(const char *hostname)
{
  _resourcesList.erase(hostname);
}

void SALOME_ResourcesManager::WriteInXmlFile()
{
  QDomDocument doc("ResourcesCatalog");
  SALOME_ResourcesCatalog_Handler* handler = new SALOME_ResourcesCatalog_Handler(_resourcesList);
  handler->PrepareDocToXmlFile(doc);
  delete handler;
  QFile file( _path_resources );
  if( !file.open( IO_WriteOnly ) )
    cout << "WRITING ERROR !!!" << endl;

  QTextStream ts( &file );
  ts << doc.toString();

  file.close();
  cout << "WRITING DONE!!!" << endl;
}

const MapOfParserResourcesType& SALOME_ResourcesManager::ParseXmlFile() 
{
  SALOME_ResourcesCatalog_Handler* handler = new SALOME_ResourcesCatalog_Handler(_resourcesList);
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

bool SALOME_ResourcesManager::_verify_resources(MapOfParserResourcesType resourceslist)
{
//     bool _return_value = true;
//     bool _bool = false ;
//     vector<string> _machine_list;
//     _machine_list.resize(0);

//   // Fill a list of all computers indicated in the resources list
//   for (unsigned int ind = 0; ind < resourceslist.size(); ind++)
//           _machine_list.push_back(resourceslist[ind].HostName);   

//   // Parse if a computer name is twice in the list of computers
//   for (unsigned int ind = 0; ind < _machine_list.size(); ind++)
//     {
//      for (unsigned int ind1 = ind+1 ; ind1 < _machine_list.size(); ind1++)
//        {
//          if(_machine_list[ind].compare(_machine_list[ind1]) == 0)
//            {
//              MESSAGE("The computer " << _machine_list[ind] << " is indicated more than once in the resources list")
//              _return_value = false;
//           }
//        }
//     }

//    return _return_value;
  return true;
}

const MapOfParserResourcesType& SALOME_ResourcesManager::GetList() const
{
  return _resourcesList;
}

string SALOME_ResourcesManager::FindBest(const Engines::MachineList& listOfMachines)
{
  return _dynamicResourcesSelecter.FindBest(listOfMachines);
}

string SALOME_ResourcesManager::BuildTempFileToLaunchRemoteContainer(const string& machine,const char *containerName)
{
  _TmpFileName=BuildTemporaryFileName();
  ofstream tempOutputFile;
  tempOutputFile.open(_TmpFileName.c_str(),ofstream::out );
  const ParserResourcesType& resInfo=_resourcesList[machine];
  tempOutputFile << "#! /bin/sh" << endl;
  //set env vars
  tempOutputFile << "python_version=`python -c \"import sys; print sys.version[:3]\"`" << endl;
  for(map<string,string>::const_iterator iter=resInfo.ModulesPath.begin();iter!=resInfo.ModulesPath.end();iter++)
    {
      string curModulePath((*iter).second);
      tempOutputFile << (*iter).first << "_ROOT_DIR="<< curModulePath << endl;
      tempOutputFile << "export " << (*iter).first << "_ROOT_DIR" << endl;
      tempOutputFile << "LD_LIBRARY_PATH=" << curModulePath << "/lib/salome" << ":${LD_LIBRARY_PATH}" << endl;
      tempOutputFile << "PYTHONPATH=" << curModulePath << "/bin/salome:" << curModulePath << "/lib/salome:" << curModulePath << "/lib/python${python_version}/site-packages/salome:";
      tempOutputFile << curModulePath << "/lib/python${python_version}/site-packages/salome/shared_modules:${PYTHONPATH}" << endl;
    }
  tempOutputFile << "export LD_LIBRARY_PATH" << endl;
  tempOutputFile << "export PYTHONPATH" << endl;
  tempOutputFile << "source " << resInfo.PreReqFilePath << endl;
  // ! env vars
  tempOutputFile << (*(resInfo.ModulesPath.find("KERNEL"))).second << "/bin/salome/";
  if(Engines_Container_i::isPythonContainer(containerName))
    tempOutputFile << "SALOME_ContainerPy.py ";
  else
    tempOutputFile << "SALOME_Container ";
  tempOutputFile << containerName << " -";
  AddOmninamesParams(tempOutputFile);
  tempOutputFile << " &" << endl;
  tempOutputFile.flush();
  tempOutputFile.close();
  chmod(_TmpFileName.c_str(),0x1ED);
  //Build command
  string command;
  if(resInfo.Protocol==rsh)
    {
      command = "rsh ";
      string commandRcp="rcp ";
      commandRcp+=_TmpFileName;
      commandRcp+=" ";
      commandRcp+=machine;
      commandRcp+=":";
      commandRcp+=_TmpFileName;
      system(commandRcp.c_str());
    }
  else if(resInfo.Protocol==ssh)
    {
      command = "ssh ";
      string commandScp="scp ";
      commandScp+=_TmpFileName;
      commandScp+=" ";
      commandScp+=machine;
      commandScp+=":";
      commandScp+=_TmpFileName;
      system(commandScp.c_str());
    }
  else
    throw SALOME_Exception("Unknown protocol");
  command+=machine;
  _CommandForRemAccess=command;
  command+=" ";
  command+=_TmpFileName;
  command += " > ";
  command += "/tmp/";
  command += containerName;
  command += "_";
  command += machine;
  command += ".log &";
  cout << "Command is ... " << command << endl;
  return command;
}

string SALOME_ResourcesManager::BuildCommandToLaunchLocalContainer(const char *containerName)
{
  _TmpFileName="";
  ostringstream command;
  if(Engines_Container_i::isPythonContainer(containerName))
    command << "SALOME_ContainerPy.py ";
  else
    command << "SALOME_Container ";
  command << containerName << " -";
  AddOmninamesParams(command);
  command << " > /tmp/" << containerName << "_" << GetHostname() << ".log 2>&1 &";
  cout << "Command is ... " << command.str() << endl << flush;
  return command.str();
}

void SALOME_ResourcesManager::RmTmpFile()
{
  if(_TmpFileName!="")
    {
      string command="rm ";
      command+=_TmpFileName;
      char *temp=strdup(command.c_str());
      int lgthTemp=strlen(temp);
      temp[lgthTemp-3]='*';
      temp[lgthTemp-2]='\0';
      system(temp);
      free(temp);
    }
}

string SALOME_ResourcesManager::BuildCommand(const string& machine,const char *containerName)
{
// rsh -n ikkyo /export/home/rahuel/SALOME_ROOT/bin/runSession SALOME_Container -ORBInitRef NameService=corbaname::dm2s0017:1515 &
  const ParserResourcesType& resInfo=_resourcesList[machine];
  bool pyCont=Engines_Container_i::isPythonContainer(containerName);
  ostringstream command;
  if(resInfo.Protocol==rsh)
    command << "rsh -n " ;
  else if(resInfo.Protocol==ssh)
    command << "ssh -f -n ";
  else
    throw SALOME_Exception("Not implemented yet...");
  command << machine << " ";
  string path = (*(resInfo.ModulesPath.find("KERNEL"))).second;
  command << path <<"/bin/salome/";
  if ( pyCont )
    command << "SALOME_ContainerPy.py ";
  else
    command << "SALOME_Container ";
  command << containerName;
  command << " -";
  AddOmninamesParams(command);
  command << " > /tmp/" << containerName << "_" << machine << ".log 2>&1 &" ;
  SCRUTE( command );
  return command.str();
}

// Warning need an updated parsed list : _resourcesList
void SALOME_ResourcesManager::SelectOnlyResourcesWithOS(vector<string>& hosts,const char *OS) const throw(SALOME_Exception)
{
  string base(OS);
  for(map<string, ParserResourcesType>::const_iterator iter=_resourcesList.begin();iter!=_resourcesList.end();iter++)
    if((*iter).second.OS==base)
      hosts.push_back((*iter).first);
}

//Warning need an updated parsed list : _resourcesList
void SALOME_ResourcesManager::KeepOnlyResourcesWithModule(vector<string>& hosts,const char *moduleName) const throw(SALOME_Exception)
{
   for(vector<string>::iterator iter=hosts.begin();iter!=hosts.end();)
     {
       MapOfParserResourcesType::const_iterator it=_resourcesList.find(*iter);
       const map<string,string>& mapOfModulesOfCurrentHost=(((*it).second).ModulesPath);
       if(mapOfModulesOfCurrentHost.find(moduleName)==mapOfModulesOfCurrentHost.end())
	 hosts.erase(iter);
       else
	 iter++;
     }
}

void SALOME_ResourcesManager::AddOmninamesParams(ostream& fileStream) const
{
  char *iorstr = _NS->getIORaddr();
  fileStream << "ORBInitRef NameService=" << iorstr;
}

string SALOME_ResourcesManager::BuildTemporaryFileName() const
{
  //build more complex file name to support multiple salome session
  char *temp=new char[19];
  strcpy(temp,"/tmp/command");
  strcat(temp,"XXXXXX");
  mkstemp(temp);
  string command(temp);
  delete [] temp;
  command += ".sh";
  return command;
}
