//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
#include "SALOME_ContainerManager.hxx"
#include "SALOME_NamingService.hxx"
#include "SALOME_ModuleCatalog.hh"
#include "Basics_Utils.hxx"
#include "Basics_DirUtils.hxx"
#include <sys/types.h>
#include <sys/stat.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <vector>
#include "Utils_CorbaException.hxx"
#include <sstream>

#ifdef WITH_PACO_PARALLEL
#include "PaCOPP.hxx"
#endif

#define TIME_OUT_TO_LAUNCH_CONT 61

using namespace std;

const char *SALOME_ContainerManager::_ContainerManagerNameInNS = 
  "/ContainerManager";

omni_mutex SALOME_ContainerManager::_numInstanceMutex;


//=============================================================================
/*! 
 *  Constructor
 *  \param orb
 *  Define a CORBA single thread policy for the server, which avoid to deal
 *  with non thread-safe usage like Change_Directory in SALOME naming service
 */
//=============================================================================

SALOME_ContainerManager::SALOME_ContainerManager(CORBA::ORB_ptr orb, PortableServer::POA_var poa, SALOME_ResourcesManager *rm, SALOME_NamingService *ns):_nbprocUsed(0)
{
  MESSAGE("constructor");
  _NS = ns;
  _ResManager = rm;

  PortableServer::POAManager_var pman = poa->the_POAManager();
  _orb = CORBA::ORB::_duplicate(orb) ;
  CORBA::PolicyList policies;
  policies.length(1);
  PortableServer::ThreadPolicy_var threadPol = 
    poa->create_thread_policy(PortableServer::SINGLE_THREAD_MODEL);
  policies[0] = PortableServer::ThreadPolicy::_duplicate(threadPol);

  _poa = poa->create_POA("SThreadPOA",pman,policies);
  threadPol->destroy();
  PortableServer::ObjectId_var id = _poa->activate_object(this);
  CORBA::Object_var obj = _poa->id_to_reference(id);
  Engines::ContainerManager_var refContMan =
    Engines::ContainerManager::_narrow(obj);

  _NS->Register(refContMan,_ContainerManagerNameInNS);
  _isAppliSalomeDefined = (getenv("APPLI") != 0);

#ifdef HAVE_MPI2
#ifdef WITHOPENMPI
  if( getenv("OMPI_URI_FILE") != NULL ){
    system("killall ompi-server");
    string command;
    command = "ompi-server -r ";
    command += getenv("OMPI_URI_FILE");
    int status=system(command.c_str());
    if(status!=0)
      throw SALOME_Exception("Error when launching ompi-server");
  }
#endif
#endif

  MESSAGE("constructor end");
}

//=============================================================================
/*! 
 * destructor
 */
//=============================================================================

SALOME_ContainerManager::~SALOME_ContainerManager()
{
  MESSAGE("destructor");
#ifdef HAVE_MPI2
#ifdef WITHOPENMPI
  if( getenv("OMPI_URI_FILE") != NULL )
    system("killall ompi-server");
#endif
#endif
}

//=============================================================================
//! shutdown all the containers, then the ContainerManager servant
/*! CORBA method:
 */
//=============================================================================

void SALOME_ContainerManager::Shutdown()
{
  MESSAGE("Shutdown");
  ShutdownContainers();
  _NS->Destroy_Name(_ContainerManagerNameInNS);
  PortableServer::ObjectId_var oid = _poa->servant_to_id(this);
  _poa->deactivate_object(oid);
}

//=============================================================================
//! Loop on all the containers listed in naming service, ask shutdown on each
/*! CORBA Method:
 */
//=============================================================================

void SALOME_ContainerManager::ShutdownContainers()
{
  MESSAGE("ShutdownContainers");
  bool isOK;
  isOK = _NS->Change_Directory("/Containers");
  if( isOK ){
    vector<string> vec = _NS->list_directory_recurs();
    list<string> lstCont;
    for(vector<string>::iterator iter = vec.begin();iter!=vec.end();iter++)
      {
        SCRUTE((*iter));
        CORBA::Object_var obj=_NS->Resolve((*iter).c_str());
        try
          {
            Engines::Container_var cont=Engines::Container::_narrow(obj);
            if(!CORBA::is_nil(cont))
              lstCont.push_back((*iter));
          }
        catch(const CORBA::Exception& e)
          {
            // ignore this entry and continue
          }
      }
    MESSAGE("Container list: ");
    for(list<string>::iterator iter=lstCont.begin();iter!=lstCont.end();iter++){
      SCRUTE((*iter));
    }
    for(list<string>::iterator iter=lstCont.begin();iter!=lstCont.end();iter++)
    {
      try
      {
        SCRUTE((*iter));
        CORBA::Object_var obj=_NS->Resolve((*iter).c_str());
        Engines::Container_var cont=Engines::Container::_narrow(obj);
        if(!CORBA::is_nil(cont))
        {
          MESSAGE("ShutdownContainers: " << (*iter));
          cont->Shutdown();
        }
        else 
          MESSAGE("ShutdownContainers: no container ref for " << (*iter));
      }
      catch(CORBA::SystemException& e)
      {
        INFOS("CORBA::SystemException ignored : " << e);
      }
      catch(CORBA::Exception&)
      {
        INFOS("CORBA::Exception ignored.");
      }
      catch(...)
      {
        INFOS("Unknown exception ignored.");
      }
    }
  }
}

//=============================================================================
//! Give a suitable Container given constraints
/*! CORBA Method:
 *  \param params Container Parameters required for the container
 *  \return the container or nil
 */
//=============================================================================
Engines::Container_ptr
SALOME_ContainerManager::GiveContainer(const Engines::ContainerParameters& params)
{
  Engines::Container_ptr ret = Engines::Container::_nil();

  // Step 0: Default mode is start
  Engines::ContainerParameters local_params(params);
  if (std::string(local_params.mode.in()) == "")
    local_params.mode = CORBA::string_dup("start");
  std::string mode = local_params.mode.in();
  MESSAGE("[GiveContainer] starting with mode: " << mode);

  // Step 1: Find Container for find and findorstart mode
  if (mode == "find" or mode == "findorstart")
  {
    ret = FindContainer(params, params.resource_params.resList);
    if(!CORBA::is_nil(ret))
      return ret;
    else
    {
      if (mode == "find")
      {
	MESSAGE("[GiveContainer] no container found");
	return ret;
      }
      else
      {
	mode = "start";
      }
    }
  }

  // Step 2: Get all possibleResources from the parameters
  Engines::ResourceList_var possibleResources = _ResManager->GetFittingResources(local_params.resource_params);
  MESSAGE("[GiveContainer] - length of possible resources " << possibleResources->length());
  std::vector<std::string> local_resources;

  // Step 3: if mode is "get" keep only machines with existing containers 
  if(mode == "get")
  {
    for(unsigned int i=0; i < possibleResources->length(); i++)
    {
      Engines::Container_ptr cont = FindContainer(params, possibleResources[i].in());
      try
      {
	if(!cont->_non_existent())
	  local_resources.push_back(string(possibleResources[i]));
      }
      catch(CORBA::Exception&) {}
    }

    // if local_resources is empty, we cannot give a container
    if (local_resources.size() == 0)
    {
      MESSAGE("[GiveContainer] cannot find a container for mode get");
      return ret;
    }
  }
  else
    for(unsigned int i=0; i < possibleResources->length(); i++)
      local_resources.push_back(string(possibleResources[i]));

  // Step 4: select the resource where to get/start the container
  std::string resource_selected;
  try
  {
    resource_selected = _ResManager->GetImpl()->Find(params.resource_params.policy.in(), local_resources);
  }
  catch(const SALOME_Exception &ex)
  {
    MESSAGE("[GiveContainer] Exception in ResourceManager find !: " << ex.what());
    return ret;
  }
  MESSAGE("[GiveContainer] Resource selected is: " << resource_selected);

  _numInstanceMutex.lock();

  // Step 5: get container in the naming service
  Engines::ResourceDefinition_var resource_definition = _ResManager->GetResourceDefinition(resource_selected.c_str());
  std::string hostname(resource_definition->name.in());
  std::string containerNameInNS;
  if(params.isMPI){
    int nbproc;
    if ( (params.resource_params.nb_node <= 0) && (params.resource_params.nb_proc_per_node <= 0) )
      nbproc = 1;
    else if ( params.resource_params.nb_node == 0 )
      nbproc = params.resource_params.nb_proc_per_node;
    else if ( params.resource_params.nb_proc_per_node == 0 )
      nbproc = params.resource_params.nb_node;
    else
      nbproc = params.resource_params.nb_node * params.resource_params.nb_proc_per_node;
    if( getenv("LIBBATCH_NODEFILE") != NULL )
      machinesFile(nbproc);
    // A mpi parallel container register on zero node in NS
    containerNameInNS = _NS->BuildContainerNameForNS(params, GetMPIZeroNode(hostname).c_str());
  }
  else
    containerNameInNS = _NS->BuildContainerNameForNS(params, hostname.c_str());
  MESSAGE("[GiveContainer] Container name in the naming service: " << containerNameInNS);

  // Step 6: check if the name exists in naming service
  //if params.mode == "getorstart" or "get" use the existing container
  //if params.mode == "start" shutdown the existing container before launching a new one with that name
  CORBA::Object_var obj = _NS->Resolve(containerNameInNS.c_str());
  if (!CORBA::is_nil(obj))
  {
    try
    {
      Engines::Container_var cont=Engines::Container::_narrow(obj);
      if(!cont->_non_existent())
      {
	if(std::string(params.mode.in())=="getorstart" or std::string(params.mode.in())=="get"){
	  _numInstanceMutex.unlock();
	  return cont._retn(); /* the container exists and params.mode is getorstart or get use it*/
	}
	else
	{
	  INFOS("[GiveContainer] A container is already registered with the name: " << containerNameInNS << ", shutdown the existing container");
	  cont->Shutdown(); // shutdown the registered container if it exists
	}
      }
    }
    catch(CORBA::Exception&)
    {
      INFOS("[GiveContainer] CORBA::Exception ignored when trying to get the container - we start a new one");
    }
  }

  // Step 7: type of container: PaCO, Exe, Mpi or Classic
  // Mpi already tested in step 5, specific code on BuildCommandToLaunch Local/Remote Container methods
  // TODO -> separates Mpi from Classic/Exe
  // PaCO++
  std::string parallelLib(params.parallelLib);
  if (std::string(local_params.parallelLib.in()) != "")
  {
    INFOS("[GiveContainer] PaCO++ container are not currently available");
    _numInstanceMutex.unlock();
    return ret;
  }
  // Classic or Exe ?
  std::string container_exe = "SALOME_Container"; // Classic container
  int found=0;
  try
  {
    CORBA::String_var container_exe_tmp;
    CORBA::Object_var obj = _NS->Resolve("/Kernel/ModulCatalog");
    SALOME_ModuleCatalog::ModuleCatalog_var Catalog = SALOME_ModuleCatalog::ModuleCatalog::_narrow(obj) ;
    if (CORBA::is_nil (Catalog))
    {
      INFOS("[GiveContainer] Module Catalog is not found -> cannot launch a container");
      _numInstanceMutex.unlock();
      return ret;
    }
    // Loop through component list
    for(unsigned int i=0; i < local_params.resource_params.componentList.length(); i++)
    {
      const char* compoi = local_params.resource_params.componentList[i];
      SALOME_ModuleCatalog::Acomponent_var compoInfo = Catalog->GetComponent(compoi);
      if (CORBA::is_nil (compoInfo))
      {
	continue;
      }
      SALOME_ModuleCatalog::ImplType impl=compoInfo->implementation_type();
      container_exe_tmp=compoInfo->implementation_name();
      if(impl==SALOME_ModuleCatalog::CEXE)
      {
	if(found)
	{
	  INFOS("ContainerManager Error: you can't have 2 CEXE component in the same container" );
	  _numInstanceMutex.unlock();
	  return Engines::Container::_nil();
	}
	MESSAGE("[GiveContainer] Exe container found !: " << container_exe_tmp);
	container_exe = container_exe_tmp.in();
	found=1;
      }
    }
  }
  catch (ServiceUnreachable&)
  {
    INFOS("Caught exception: Naming Service Unreachable");
    _numInstanceMutex.unlock();
    return ret;
  }
  catch (...)
  {
    INFOS("Caught unknown exception.");
    _numInstanceMutex.unlock();
    return ret;
  }

  // Step 8: start a new container
  MESSAGE("[GiveContainer] Try to launch a new container on " << resource_selected);
  std::string command;
  // if a parallel container is launched in batch job, command is: "mpirun -np nbproc -machinefile nodesfile SALOME_MPIContainer"
  if( getenv("LIBBATCH_NODEFILE") != NULL && params.isMPI )
    command = BuildCommandToLaunchLocalContainer(params,container_exe);
  // if a container is launched on localhost, command is "SALOME_Container" or "mpirun -np nbproc SALOME_MPIContainer"
  else if(hostname == Kernel_Utils::GetHostname())
    command = BuildCommandToLaunchLocalContainer(params, container_exe);
  // if a container is launched in remote mode, command is "ssh resource_selected SALOME_Container" or "ssh resource_selected mpirun -np nbproc SALOME_MPIContainer"
  else
    command = BuildCommandToLaunchRemoteContainer(resource_selected, params, container_exe);

  //redirect stdout and stderr in a file
#ifdef WNT
  string logFilename=getenv("TEMP");
  logFilename += "\\";
#else
  string logFilename="/tmp";
  char* val = getenv("SALOME_TMP_DIR");
  if(val)
  {
    struct stat file_info;
    stat(val, &file_info);
    bool is_dir = S_ISDIR(file_info.st_mode);
    if (is_dir)logFilename=val;
    else std::cerr << "SALOME_TMP_DIR environment variable is not a directory use /tmp instead" << std::endl;
  }
  logFilename += "/";
#endif
  logFilename += _NS->ContainerName(params)+"_"+ resource_selected +"_"+getenv( "USER" )+".log" ;
  command += " > " + logFilename + " 2>&1";
#ifdef WNT
  command = "%PYTHONBIN% -c \"import win32pm ; win32pm.spawnpid(r'" + command + "', '')\"";
#else
  command += " &";
#endif

  // launch container with a system call
  int status=system(command.c_str());

  _numInstanceMutex.unlock();

  if (status == -1){
    MESSAGE("SALOME_ContainerManager::StartContainer rsh failed (system command status -1)");
    RmTmpFile(_TmpFileName); // command file can be removed here
    return Engines::Container::_nil();
  }
  else if (status == 217){
    MESSAGE("SALOME_ContainerManager::StartContainer rsh failed (system command status 217)");
    RmTmpFile(_TmpFileName); // command file can be removed here
    return Engines::Container::_nil();
  }
  else
  {
    int count = TIME_OUT_TO_LAUNCH_CONT;
    MESSAGE("[GiveContainer] waiting " << count << " second steps");
    while (CORBA::is_nil(ret) && count)
    {
#ifndef WIN32
      sleep( 1 ) ;
#else
      Sleep(1000);
#endif
      count--;
      MESSAGE("[GiveContainer] step " << count << " Waiting for container on " << resource_selected);
      CORBA::Object_var obj = _NS->Resolve(containerNameInNS.c_str());
      ret=Engines::Container::_narrow(obj);
    }
    if (CORBA::is_nil(ret))
    {
      INFOS("[GiveContainer] was not able to launch container " << containerNameInNS);
    }
    else
    {
      // Setting log file name
      logFilename=":"+logFilename;
      logFilename="@"+Kernel_Utils::GetHostname()+logFilename;
      logFilename=getenv( "USER" )+logFilename;
      ret->logfilename(logFilename.c_str());
      RmTmpFile(_TmpFileName); // command file can be removed here
    }
  }
  return ret;
}

//=============================================================================
//! Find a container given constraints (params) on a list of machines (possibleComputers)
/*!
 *
 */
//=============================================================================

Engines::Container_ptr
SALOME_ContainerManager::FindContainer(const Engines::ContainerParameters& params,
                                       const Engines::ResourceList& possibleResources)
{
  MESSAGE("[FindContainer] FindContainer on " << possibleResources.length() << " resources");
  for(unsigned int i=0; i < possibleResources.length();i++)
    {
      Engines::Container_ptr cont = FindContainer(params, possibleResources[i].in());
      if(!CORBA::is_nil(cont))
        return cont;
    }
  MESSAGE("[FindContainer] no container found");
  return Engines::Container::_nil();
}

//=============================================================================
//! Find a container given constraints (params) on a machine (theMachine)
/*!
 *
 */
//=============================================================================

Engines::Container_ptr
SALOME_ContainerManager::FindContainer(const Engines::ContainerParameters& params,
                                       const std::string& resource)
{
  std::string containerNameInNS(_NS->BuildContainerNameForNS(params, resource.c_str()));
  MESSAGE("[FindContainer] Try to find a container  " << containerNameInNS << " on resource " << resource);
  CORBA::Object_var obj = _NS->Resolve(containerNameInNS.c_str());
  try
  {
    if(obj->_non_existent())
      return Engines::Container::_nil();
    else
      return Engines::Container::_narrow(obj);
  }
  catch(const CORBA::Exception& e)
  {
    return Engines::Container::_nil();
  }
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
 *  ssh user@machine distantPath/runRemote.sh hostNS portNS WORKINGDIR workingdir \
 *                   SALOME_Container containerName &"

 *  - where user is ommited if not specified in CatalogResources,
 *  - where distant path is always relative to user@machine $HOME, and
 *    equal to $APPLI if not specified in CatalogResources,
 *  - where hostNS is the hostname of CORBA naming server (set by scripts to
 *    use to launch SALOME and servers in $APPLI: runAppli.sh, runRemote.sh)
 *  - where portNS is the port used by CORBA naming server (set by scripts to
 *    use to launch SALOME and servers in $APPLI: runAppli.sh, runRemote.sh)
 *  - where workingdir is the requested working directory for the container.
 *    If WORKINGDIR (and workingdir) is not present the working dir will be $HOME
 */ 
//=============================================================================

string
SALOME_ContainerManager::BuildCommandToLaunchRemoteContainer
(const string& resource_name,
 const Engines::ContainerParameters& params, const std::string& container_exe)
{
          
  string command;
  if (!_isAppliSalomeDefined)
    command = BuildTempFileToLaunchRemoteContainer(resource_name, params);
  else
  {
    int nbproc;
    Engines::ResourceDefinition_var resource_definition = _ResManager->GetResourceDefinition(resource_name.c_str());
    std::string hostname(resource_definition->name.in());
    const ParserResourcesType& resInfo = _ResManager->GetImpl()->GetResourcesDescr(resource_name);

    if (params.isMPI)
    {
      if ((params.resource_params.nb_node <= 0) && (params.resource_params.nb_proc_per_node <= 0))
	nbproc = 1;
      else if (params.resource_params.nb_node == 0)
	nbproc = params.resource_params.nb_proc_per_node;
      else if (params.resource_params.nb_proc_per_node == 0)
	nbproc = params.resource_params.nb_node;
      else
	nbproc = params.resource_params.nb_node * params.resource_params.nb_proc_per_node;
    }

    // "ssh -l user machine distantPath/runRemote.sh hostNS portNS WORKINGDIR workingdir \
    //  SALOME_Container containerName &"
    if (resInfo.Protocol == rsh)
      command = "rsh ";
    else if (resInfo.Protocol == ssh)
      command = "ssh ";
    else
      throw SALOME_Exception("Unknown protocol");

    if (resInfo.UserName != "")
    {
      command += "-l ";
      command += resInfo.UserName;
      command += " ";
    }

    command += resInfo.HostName;
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

    std::string wdir = params.workingdir.in();
    if(wdir != "")
    {
      command += " WORKINGDIR ";
      command += " '";
      if(wdir == "$TEMPDIR")
	wdir="\\$TEMPDIR";
      command += wdir; // requested working directory
      command += "'"; 
    }

    if(params.isMPI)
    {
      command += " mpirun -np ";
      std::ostringstream o;
      o << nbproc << " ";
      command += o.str();
#ifdef WITHLAM
      command += "-x PATH,LD_LIBRARY_PATH,OMNIORB_CONFIG,SALOME_trace ";
#elif defined(WITHOPENMPI)
      if( getenv("OMPI_URI_FILE") == NULL )
	command += "-x PATH -x LD_LIBRARY_PATH -x OMNIORB_CONFIG -x SALOME_trace";
      else{
	command += "-x PATH -x LD_LIBRARY_PATH -x OMNIORB_CONFIG -x SALOME_trace -ompi-server file:";
	command += getenv("OMPI_URI_FILE");
      }
#endif        
      command += " SALOME_MPIContainer ";
    }
    else
      command += " " +container_exe+ " ";

    command += _NS->ContainerName(params);
    command += " -";
    AddOmninamesParams(command);

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
SALOME_ContainerManager::BuildCommandToLaunchLocalContainer
(const Engines::ContainerParameters& params, const std::string& container_exe)
{
  _TmpFileName = BuildTemporaryFileName();
  string command;
  int nbproc = 0;

  ostringstream o;

  if (params.isMPI)
    {
      o << "mpirun -np ";

      if ( (params.resource_params.nb_node <= 0) && (params.resource_params.nb_proc_per_node <= 0) )
        nbproc = 1;
      else if ( params.resource_params.nb_node == 0 )
        nbproc = params.resource_params.nb_proc_per_node;
      else if ( params.resource_params.nb_proc_per_node == 0 )
        nbproc = params.resource_params.nb_node;
      else
        nbproc = params.resource_params.nb_node * params.resource_params.nb_proc_per_node;

      o << nbproc << " ";

      if( getenv("LIBBATCH_NODEFILE") != NULL )
	o << "-machinefile " << _machinesFile << " ";

#ifdef WITHLAM
      o << "-x PATH,LD_LIBRARY_PATH,OMNIORB_CONFIG,SALOME_trace ";
#elif defined(WITHOPENMPI)
      if( getenv("OMPI_URI_FILE") == NULL )
        o << "-x PATH -x LD_LIBRARY_PATH -x OMNIORB_CONFIG -x SALOME_trace";
      else
        {
          o << "-x PATH -x LD_LIBRARY_PATH -x OMNIORB_CONFIG -x SALOME_trace -ompi-server file:";
          o << getenv("OMPI_URI_FILE");
        }
#endif

      if (isPythonContainer(params.container_name))
        o << " pyMPI SALOME_ContainerPy.py ";
      else
        o << " SALOME_MPIContainer ";
    }

  else
    {
      std::string wdir=params.workingdir.in();
      if(wdir != "")
        {
          // a working directory is requested
          if(wdir == "$TEMPDIR")
            {
              // a new temporary directory is requested
              string dir = Kernel_Utils::GetTmpDir();
#ifdef WIN32
              o << "cd /d " << dir << endl;
#else
              o << "cd " << dir << ";";
#endif

            }
          else
            {
              // a permanent directory is requested use it or create it
#ifdef WIN32
              o << "mkdir " + wdir << endl;
              o << "cd /D " + wdir << endl;
#else
              o << "mkdir -p " << wdir << " && cd " << wdir + ";";
#endif
            }
        }
      if (isPythonContainer(params.container_name))
        o << "SALOME_ContainerPy.py ";
      else
        o << container_exe + " ";

    }

  o << _NS->ContainerName(params);
  o << " -";
  AddOmninamesParams(o);

  ofstream command_file( _TmpFileName.c_str() );
  command_file << o.str();
  command_file.close();

#ifndef WIN32
  chmod(_TmpFileName.c_str(), 0x1ED);
#endif
  command = _TmpFileName;

  MESSAGE("Command is file ... " << command);
  MESSAGE("Command is ... " << o.str());
  return command;
}


//=============================================================================
/*!
 *  removes the generated temporary file in case of a remote launch.
 */ 
//=============================================================================

void SALOME_ContainerManager::RmTmpFile(std::string& tmpFileName)
{
  int lenght = tmpFileName.size();
  if ( lenght  > 0)
    {
#ifdef WIN32
      string command = "del /F ";
#else
      string command = "rm ";      
#endif
      if ( lenght > 4 )
        command += tmpFileName.substr(0, lenght - 3 );
      else
        command += tmpFileName;
      command += '*';
      system(command.c_str());
      //if dir is empty - remove it
      string tmp_dir = Kernel_Utils::GetDirByPath( tmpFileName );
      if ( Kernel_Utils::IsEmptyDir( tmp_dir ) )
        {
#ifdef WIN32
          command = "del /F " + tmp_dir;
#else
          command = "rmdir " + tmp_dir;
#endif
          system(command.c_str());
        }
    }
}

//=============================================================================
/*!
 *   add to command all options relative to naming service.
 */ 
//=============================================================================

void SALOME_ContainerManager::AddOmninamesParams(string& command) const
{
  CORBA::String_var iorstr = _NS->getIORaddr();
  command += "ORBInitRef NameService=";
  command += iorstr;
}

//=============================================================================
/*!
 *  add to command all options relative to naming service.
 */ 
//=============================================================================

void SALOME_ContainerManager::AddOmninamesParams(ofstream& fileStream) const
{
  CORBA::String_var iorstr = _NS->getIORaddr();
  fileStream << "ORBInitRef NameService=";
  fileStream << iorstr;
}

//=============================================================================
/*!
 *  add to command all options relative to naming service.
 */ 
//=============================================================================

void SALOME_ContainerManager::AddOmninamesParams(ostringstream& oss) const
{
  CORBA::String_var iorstr = _NS->getIORaddr();
  oss << "ORBInitRef NameService=";
  oss << iorstr;
}

//=============================================================================
/*!
 *  generate a file name in /tmp directory
 */ 
//=============================================================================

string SALOME_ContainerManager::BuildTemporaryFileName() const
{
  //build more complex file name to support multiple salome session
  string aFileName = Kernel_Utils::GetTmpFileName();
#ifndef WIN32
  aFileName += ".sh";
#else
  aFileName += ".bat";
#endif
  return aFileName;
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
SALOME_ContainerManager::BuildTempFileToLaunchRemoteContainer
(const string& resource_name,
 const Engines::ContainerParameters& params) throw(SALOME_Exception)
{
  int status;

  _TmpFileName = BuildTemporaryFileName();
  ofstream tempOutputFile;
  tempOutputFile.open(_TmpFileName.c_str(), ofstream::out );
  const ParserResourcesType& resInfo = _ResManager->GetImpl()->GetResourcesDescr(resource_name);
  tempOutputFile << "#! /bin/sh" << endl;

  // --- set env vars

  tempOutputFile << "export SALOME_trace=local" << endl; // mkr : 27.11.2006 : PAL13967 - Distributed supervision graphs - Problem with "SALOME_trace"
  //tempOutputFile << "source " << resInfo.PreReqFilePath << endl;

  // ! env vars

  if (params.isMPI)
    {
      tempOutputFile << "mpirun -np ";
      int nbproc;

      if ( (params.resource_params.nb_node <= 0) && (params.resource_params.nb_proc_per_node <= 0) )
        nbproc = 1;
      else if ( params.resource_params.nb_node == 0 )
        nbproc = params.resource_params.nb_proc_per_node;
      else if ( params.resource_params.nb_proc_per_node == 0 )
        nbproc = params.resource_params.nb_node;
      else
        nbproc = params.resource_params.nb_node * params.resource_params.nb_proc_per_node;

      std::ostringstream o;

      tempOutputFile << nbproc << " ";
#ifdef WITHLAM
      tempOutputFile << "-x PATH,LD_LIBRARY_PATH,OMNIORB_CONFIG,SALOME_trace ";
#elif defined(WITHOPENMPI)
      if( getenv("OMPI_URI_FILE") == NULL )
        tempOutputFile << "-x PATH -x LD_LIBRARY_PATH -x OMNIORB_CONFIG -x SALOME_trace";
      else{
        tempOutputFile << "-x PATH -x LD_LIBRARY_PATH -x OMNIORB_CONFIG -x SALOME_trace -ompi-server file:";
        tempOutputFile << getenv("OMPI_URI_FILE");
      }
#endif
    }

  tempOutputFile << getenv("KERNEL_ROOT_DIR") << "/bin/salome/";

  if (params.isMPI)
    {
      if (isPythonContainer(params.container_name))
        tempOutputFile << " pyMPI SALOME_ContainerPy.py ";
      else
        tempOutputFile << " SALOME_MPIContainer ";
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
#ifndef WIN32
  chmod(_TmpFileName.c_str(), 0x1ED);
#endif

  // --- Build command

  string command;

  if (resInfo.Protocol == rsh)
    {
      command = "rsh ";
      string commandRcp = "rcp ";
      commandRcp += _TmpFileName;
      commandRcp += " ";
      commandRcp += resInfo.HostName;
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
      commandRcp += resInfo.HostName;
      commandRcp += ":";
      commandRcp += _TmpFileName;
      status = system(commandRcp.c_str());
    }
  else
    throw SALOME_Exception("Unknown protocol");

  if(status)
    throw SALOME_Exception("Error of connection on remote host");    

  command += resInfo.HostName;
  _CommandForRemAccess = command;
  command += " ";
  command += _TmpFileName;

  SCRUTE(command);

  return command;

}

#ifdef WITH_PACO_PARALLEL
//=============================================================================
/*! CORBA Method:
 *  Find or Start a suitable PaCO++ Parallel Container in a list of machines.
 *  \param params            Machine Parameters required for the container
 *  \return CORBA container reference.
 */
//=============================================================================
Engines::Container_ptr
SALOME_ContainerManager::StartPaCOPPContainer(const Engines::ContainerParameters& params_const)
{
  CORBA::Object_var obj;
  PaCO::InterfaceManager_var container_proxy;
  Engines::Container_ptr ret = Engines::Container::_nil();
  Engines::MachineParameters params(params_const);

  // Step 1 : Try to find a suitable container
  // Currently not as good as could be since
  // we have to verified the number of nodes of the container
  // if a user tell that.
  ret = FindContainer(params, params.computerList);
  if(CORBA::is_nil(ret)) {
    // Step 2 : Starting a new parallel container !
    INFOS("[StartParallelContainer] Starting a PaCO++ parallel container");

    // Step 3 : Choose a computer
    std::string theMachine = _ResManager->FindFirst(params.computerList);
    //If the machine name is localhost use the real name
    if(theMachine == "localhost")
      theMachine=Kernel_Utils::GetHostname();

    if(theMachine == "") {
      INFOS("[StartParallelContainer] !!!!!!!!!!!!!!!!!!!!!!!!!!");
      INFOS("[StartParallelContainer] No possible computer found");
      INFOS("[StartParallelContainer] !!!!!!!!!!!!!!!!!!!!!!!!!!");
      return ret;
    }
    INFOS("[StartParallelContainer] on machine : " << theMachine);
    params.hostname = CORBA::string_dup(theMachine.c_str());

    // Step 4 : starting parallel container proxy
    Engines::MachineParameters params_proxy(params);
    std::string command_proxy;
    SALOME_ContainerManager::actual_launch_machine_t proxy_machine;
    try 
    {
      command_proxy = BuildCommandToLaunchParallelContainer("SALOME_ParallelContainerProxy", params_proxy, proxy_machine);
    }
    catch(const SALOME_Exception & ex)
    {
      INFOS("[StartParallelContainer] Exception in BuildCommandToLaunchParallelContainer");
      INFOS(ex.what());
      return ret;
    }
    params_proxy.nb_proc = 0; // LaunchParallelContainer uses this value to know if it launches the proxy or the nodes
    obj = LaunchParallelContainer(command_proxy, params_proxy, _NS->ContainerName(params_proxy), proxy_machine);
    if (CORBA::is_nil(obj))
    {
      INFOS("[StartParallelContainer] LaunchParallelContainer for proxy returns NIL !");
      return ret;
    }
    try 
    {
      container_proxy = PaCO::InterfaceManager::_narrow(obj);
    }
    catch(CORBA::SystemException& e)
    {
      INFOS("[StartParallelContainer] Exception in _narrow after LaunchParallelContainer for proxy !");
      INFOS("CORBA::SystemException : " << e);
      return ret;
    }
    catch(CORBA::Exception& e)
    {
      INFOS("[StartParallelContainer] Exception in _narrow after LaunchParallelContainer for proxy !");
      INFOS("CORBA::Exception" << e);
      return ret;
    }
    catch(...)
    {
      INFOS("[StartParallelContainer] Exception in _narrow after LaunchParallelContainer for proxy !");
      INFOS("Unknown exception !");
      return ret;
    }
    if (CORBA::is_nil(container_proxy))
    {
      INFOS("[StartParallelContainer] PaCO::InterfaceManager::_narrow returns NIL !");
      return ret;
    }

    // Step 5 : starting parallel container nodes
    std::string command_nodes;
    Engines::MachineParameters params_nodes(params);
    SALOME_ContainerManager::actual_launch_machine_t nodes_machines;
    try 
    {
      command_nodes = BuildCommandToLaunchParallelContainer("SALOME_ParallelContainerNode", params_nodes, nodes_machines, proxy_machine[0]);
    }
    catch(const SALOME_Exception & ex)
    {
      INFOS("[StartParallelContainer] Exception in BuildCommandToLaunchParallelContainer");
      INFOS(ex.what());
      return ret;
    }
    std::string container_generic_node_name = _NS->ContainerName(params) + "Node";
    obj = LaunchParallelContainer(command_nodes, params_nodes, container_generic_node_name, nodes_machines);
    if (CORBA::is_nil(obj))
    {
      INFOS("[StartParallelContainer] LaunchParallelContainer for nodes returns NIL !");
      // Il faut tuer le proxy
      try 
      {
        Engines::Container_var proxy = Engines::Container::_narrow(container_proxy);
        proxy->Shutdown();
      }
      catch (...)
      {
        INFOS("[StartParallelContainer] Exception catched from proxy Shutdown...");
      }
      return ret;
    }

    // Step 6 : connecting nodes and the proxy to actually create a parallel container
    for (int i = 0; i < params.nb_proc; i++) 
    {
      std::ostringstream tmp;
      tmp << i;
      std::string proc_number = tmp.str();
      std::string container_node_name = container_generic_node_name + proc_number;

      std::string theNodeMachine(nodes_machines[i]);
      std::string containerNameInNS = _NS->BuildContainerNameForNS(container_node_name.c_str(), theNodeMachine.c_str());
      obj = _NS->Resolve(containerNameInNS.c_str());
      if (CORBA::is_nil(obj)) 
      {
        INFOS("[StartParallelContainer] CONNECTION FAILED From Naming Service !");
        INFOS("[StartParallelContainer] Container name is " << containerNameInNS);
        return ret;
      }
      try
      {
        MESSAGE("[StartParallelContainer] Deploying node : " << container_node_name);
        PaCO::InterfaceParallel_var node = PaCO::InterfaceParallel::_narrow(obj);
        node->deploy();
        MESSAGE("[StartParallelContainer] node " << container_node_name << " is deployed");
      }
      catch(CORBA::SystemException& e)
      {
        INFOS("[StartParallelContainer] Exception in deploying node : " << containerNameInNS);
        INFOS("CORBA::SystemException : " << e);
        return ret;
      }
      catch(CORBA::Exception& e)
      {
        INFOS("[StartParallelContainer] Exception in deploying node : " << containerNameInNS);
        INFOS("CORBA::Exception" << e);
        return ret;
      }
      catch(...)
      {
        INFOS("[StartParallelContainer] Exception in deploying node : " << containerNameInNS);
        INFOS("Unknown exception !");
        return ret;
      }
    }

    // Step 7 : starting parallel container
    try 
    {
      MESSAGE ("[StartParallelContainer] Starting parallel object");
      container_proxy->start();
      MESSAGE ("[StartParallelContainer] Parallel object is started");
      ret = Engines::Container::_narrow(container_proxy);
    }
    catch(CORBA::SystemException& e)
    {
      INFOS("Caught CORBA::SystemException. : " << e);
    }
    catch(PortableServer::POA::ServantAlreadyActive&)
    {
      INFOS("Caught CORBA::ServantAlreadyActiveException");
    }
    catch(CORBA::Exception&)
    {
      INFOS("Caught CORBA::Exception.");
    }
    catch(std::exception& exc)
    {
      INFOS("Caught std::exception - "<<exc.what()); 
    }
    catch(...)
    {
      INFOS("Caught unknown exception.");
    }
  }
  return ret;
}
#else
//=============================================================================
/*! CORBA Method:
 *  Find or Start a suitable PaCO++ Parallel Container in a list of machines.
 *  \param params            Machine Parameters required for the container
 *  \return CORBA container reference.
 */
//=============================================================================
Engines::Container_ptr
SALOME_ContainerManager::StartPaCOPPContainer(const Engines::ContainerParameters& params)
{
  Engines::Container_ptr ret = Engines::Container::_nil();
  INFOS("[StartParallelContainer] is disabled !");
  INFOS("[StartParallelContainer] recompile SALOME Kernel to enable parallel extension");
  return ret;
}
#endif

#ifndef WITH_PACO_PARALLEL
CORBA::Object_ptr 
SALOME_ContainerManager::LaunchParallelContainer(const std::string& command, 
                                                 const Engines::ContainerParameters& params,
                                                 const std::string& name,
                                                 SALOME_ContainerManager::actual_launch_machine_t & vect_machine)
{
  CORBA::Object_ptr obj = CORBA::Object::_nil();
  return obj;
}
#else
//=============================================================================
/*! This method launches the parallel container.
 *  It will may be placed on the ressources manager.
 *
 * \param command to launch
 * \param container's parameters
 * \param name of the container
 *
 * \return CORBA container reference
 */
//=============================================================================
CORBA::Object_ptr 
SALOME_ContainerManager::LaunchParallelContainer(const std::string& command, 
                                                 const Engines::ContainerParameters& params,
                                                 const std::string& name,
                                                 SALOME_ContainerManager::actual_launch_machine_t & vect_machine)
{
  CORBA::Object_ptr obj = CORBA::Object::_nil();
  std::string containerNameInNS;
  int count = TIME_OUT_TO_LAUNCH_CONT;

  INFOS("[LaunchParallelContainer] Begin");
  int status = system(command.c_str());
  if (status == -1) {
    INFOS("[LaunchParallelContainer] failed : system command status -1");
    return obj;
  }
  else if (status == 217) {
    INFOS("[LaunchParallelContainer] failed : system command status 217");
    return obj;
  }

  if (params.nb_proc == 0) 
  {
    std::string theMachine(vect_machine[0]);
    // Proxy We have launch a proxy
    containerNameInNS = _NS->BuildContainerNameForNS((char*) name.c_str(), theMachine.c_str());
    INFOS("[LaunchParallelContainer]  Waiting for Parallel Container proxy " << containerNameInNS << " on " << theMachine);
    while (CORBA::is_nil(obj) && count) 
    {
#ifndef WIN32
      sleep(1) ;
#else
      Sleep(1000);
#endif
      count-- ;
      obj = _NS->Resolve(containerNameInNS.c_str());
    }
  }
  else 
  {
    INFOS("[LaunchParallelContainer] launching the nodes of the parallel container");
    // We are waiting all the nodes
    for (int i = 0; i < params.nb_proc; i++) 
    {
      obj = CORBA::Object::_nil();
      std::string theMachine(vect_machine[i]);
      // Name of the node
      std::ostringstream tmp;
      tmp << i;
      std::string proc_number = tmp.str();
      std::string container_node_name = name + proc_number;
      containerNameInNS = _NS->BuildContainerNameForNS((char*) container_node_name.c_str(), theMachine.c_str());
      INFOS("[LaunchParallelContainer]  Waiting for Parallel Container node " << containerNameInNS << " on " << theMachine);
      while (CORBA::is_nil(obj) && count) {
#ifndef WIN32
        sleep(1) ;
#else
        Sleep(1000);
#endif
        count-- ;
        obj = _NS->Resolve(containerNameInNS.c_str());
      }
      if (CORBA::is_nil(obj))
      {
        INFOS("[LaunchParallelContainer] Launch of node failed (or not found) !");
        return obj;
      }
    }
  }
  if (CORBA::is_nil(obj)) 
    INFOS("[LaunchParallelContainer] failed");
  
  return obj;
}
#endif

#ifndef WITH_PACO_PARALLEL
string 
SALOME_ContainerManager::BuildCommandToLaunchParallelContainer(const std::string& exe_name,
                                                               const Engines::ContainerParameters& params,
                                                               SALOME_ContainerManager::actual_launch_machine_t & vect_machine,
                                                               const std::string proxy_hostname)
{
  return "";
}
#else
//=============================================================================
/*! Creates a command line that the container manager uses to launch
 * a parallel container.
 */ 
//=============================================================================
string 
SALOME_ContainerManager::BuildCommandToLaunchParallelContainer(const std::string& exe_name,
                                                               const Engines::ContainerParameters& params,
                                                               SALOME_ContainerManager::actual_launch_machine_t & vect_machine,
                                                               const std::string proxy_hostname)
{
  // This method knows the differences between the proxy and the nodes.
  // nb_proc is not used in the same way if it is a proxy or 
  // a node.
  
  //command = "gdb --args ";
  //command = "valgrind --tool=memcheck --log-file=val_log ";
  //command += real_exe_name;

  // Step 0 : init some variables...
  std::string parallelLib(CORBA::string_dup(params.parallelLib));
  std::string real_exe_name  = exe_name + parallelLib;
  std::string machine_file_name("");
  bool remote = false;
  bool is_a_proxy = false; 
  std::string hostname(CORBA::string_dup(params.hostname));

  std::ostringstream tmp_string;
  CORBA::Long nb_nodes = params.nb_proc;
  tmp_string << nb_nodes;
  std::string nbproc = tmp_string.str();

  Engines::MachineParameters_var rtn = new Engines::MachineParameters();
  rtn->container_name = params.container_name;
  rtn->hostname = params.hostname;
  rtn->OS = params.OS;
  rtn->mem_mb = params.mem_mb;
  rtn->cpu_clock = params.cpu_clock;
  rtn->nb_proc_per_node = params.nb_proc_per_node;
  rtn->nb_node = params.nb_node;
  rtn->nb_proc = params.nb_proc;
  rtn->isMPI = params.isMPI;

  // Step 1 : local or remote launch ?
  if (hostname != std::string(Kernel_Utils::GetHostname()) )
  {
    MESSAGE("[BuildCommandToLaunchParallelContainer] remote machine case detected !");
    remote = true;
  }

  // Step 2 : proxy or nodes launch ?
  std::string::size_type loc_proxy = exe_name.find("Proxy");
  if( loc_proxy != string::npos ) {
    is_a_proxy = true;
  } 

  // Step 3 : Depending of the parallelLib, getting the machine file
  // ParallelLib Dummy has is own machine for this method
  if (remote)
  {
    if (is_a_proxy)
    {
      machine_file_name = _ResManager->getMachineFile(hostname,
                                                      1,
                                                      parallelLib);
    }
    else
    {
      machine_file_name = _ResManager->getMachineFile(hostname, 
                                                      params.nb_proc,
                                                      parallelLib);
    }
    if (machine_file_name == "")
    {
      INFOS("[BuildCommandToLaunchParallelContainer] Error machine_file was not generated for machine " << hostname);
      throw SALOME_Exception("Error machine_file was not generated");
    }
    MESSAGE("[BuildCommandToLaunchParallelContainer] machine_file_name is : " << machine_file_name);
  }

  // Step 4 : Log type choosen by the user
  std::string log_env("");
  char * get_val = getenv("PARALLEL_LOG");
  if (get_val)
    log_env = get_val;
  std::string command_begin("");
  std::string command_end("");
  if(log_env == "xterm")
  {
    command_begin = "/usr/X11R6/bin/xterm -e \"export LD_LIBRARY_PATH=$LD_LIBRARY_PATH; export PATH=$PATH;";
    command_end   = "\"&";
  }
  else if(log_env == "xterm_debug")
  {
    command_begin = "/usr/X11R6/bin/xterm -e \"export LD_LIBRARY_PATH=$LD_LIBRARY_PATH; export PATH=$PATH;";
    command_end   = "; cat \" &";
  }
  else
  {
    // default into a file...
    std::string logFilename = "/tmp/" + _NS->ContainerName(params) + "_" + hostname;
    if (is_a_proxy)
      logFilename += "_Proxy_";
    else
      logFilename += "_Node_";
    logFilename += std::string(getenv("USER")) + ".log";
    command_end = " > " + logFilename + " 2>&1 & ";
  }

  // Step 5 : Building the command
  std::string command("");
  if (parallelLib == "Dummy")
  {
    if (is_a_proxy)
    {
      std::string command_remote("");
      if (remote)
      {
        std::string machine_name;
        std::ifstream machine_file(machine_file_name.c_str());
        std::getline(machine_file, machine_name);
        MESSAGE("[BuildCommandToLaunchParallelContainer] machine file name extracted is " << machine_name)

        // We want to launch a command like : 
        // ssh -l user machine distantPath/runRemote.sh hostNS portNS
        const ParserResourcesType& resInfo = _ResManager->GetImpl()->GetResourcesList(machine_name);
        if (resInfo.Protocol == rsh)
          command_remote = "rsh ";
        else 
          command_remote = "ssh ";
        command_remote += "-l ";
        command_remote += resInfo.UserName;
        command_remote += " ";
        command_remote += machine_name;
        command_remote += " ";
        command_remote += resInfo.AppliPath; // path relative to user@machine $HOME
        command_remote += "/runRemote.sh ";
        ASSERT(getenv("NSHOST")); 
        command_remote += getenv("NSHOST"); // hostname of CORBA name server
        command_remote += " ";
        ASSERT(getenv("NSPORT"));
        command_remote += getenv("NSPORT"); // port of CORBA name server
        command_remote += " ";

        hostname = machine_name;
      }

      command =  real_exe_name;
      command += " " + _NS->ContainerName(rtn);
      command += " " + parallelLib;
      command += " " + hostname;
      command += " " + nbproc;
      command += " -";
      AddOmninamesParams(command);

      command = command_begin + command_remote + command + command_end;
      vect_machine.push_back(hostname);
    }
    else
    {
      std::ifstream * machine_file = NULL;
      if (remote)
        machine_file = new std::ifstream(machine_file_name.c_str());
      for (int i= 0; i < nb_nodes; i++)
      {
        std::string command_remote("");
        if (remote)
        {
          std::string machine_name;
          std::getline(*machine_file, machine_name);
          MESSAGE("[BuildCommandToLaunchParallelContainer] machine file name extracted is " << machine_name)

            // We want to launch a command like : 
            // ssh -l user machine distantPath/runRemote.sh hostNS portNS
            const ParserResourcesType& resInfo = _ResManager->GetImpl()->GetResourcesList(machine_name);
          if (resInfo.Protocol == rsh)
            command_remote = "rsh ";
          else 
            command_remote = "ssh ";
          command_remote += "-l ";
          command_remote += resInfo.UserName;
          command_remote += " ";
          command_remote += machine_name;
          command_remote += " ";
          command_remote += resInfo.AppliPath; // path relative to user@machine $HOME
          command_remote += "/runRemote.sh ";
          ASSERT(getenv("NSHOST")); 
          command_remote += getenv("NSHOST"); // hostname of CORBA name server
          command_remote += " ";
          ASSERT(getenv("NSPORT"));
          command_remote += getenv("NSPORT"); // port of CORBA name server
          command_remote += " ";

          hostname = machine_name;
        }

        std::ostringstream tmp;
        tmp << i;
        std::string proc_number = tmp.str();

        std::string command_tmp("");
        command_tmp += real_exe_name;
        command_tmp += " " + _NS->ContainerName(rtn);
        command_tmp += " " + parallelLib;
        command_tmp += " " + proxy_hostname;
        command_tmp += " " + proc_number;
        command_tmp += " -";
        AddOmninamesParams(command_tmp);

        // On change _Node_ par _Nodex_ pour avoir chaque noeud
        // sur un fichier
        std::string command_end_tmp = command_end;
        std::string::size_type loc_node = command_end_tmp.find("_Node_");
        if (loc_node != std::string::npos)
          command_end_tmp.insert(loc_node+5, proc_number);
        command += command_begin + command_remote + command_tmp + command_end_tmp;
        vect_machine.push_back(hostname);
      }
      if (machine_file)
        delete machine_file;
    }
  }
  else if (parallelLib == "Mpi")
  {
    // Step 0: if remote we have to copy the file
    // to the first machine of the file
    std::string remote_machine("");
    if (remote)
    {
      std::ifstream * machine_file = NULL;
      machine_file = new std::ifstream(machine_file_name.c_str());
      // Get first word of the line
      // For MPI implementation the first word is the 
      // machine name
      std::getline(*machine_file, remote_machine, ' ');
      machine_file->close();
      MESSAGE("[BuildCommandToLaunchParallelContainer] machine file name extracted is " << remote_machine)

      // We want to launch a command like : 
      // scp mpi_machine_file user@machine:Path
      std::string command_remote("");
      const ParserResourcesType& resInfo = _ResManager->GetImpl()->GetResourcesList(remote_machine);
      if (resInfo.Protocol == rsh)
        command_remote = "rcp ";
      else 
        command_remote = "scp ";

      command_remote += machine_file_name;
      command_remote += " ";
      command_remote += resInfo.UserName;
      command_remote += "@";
      command_remote += remote_machine;
      command_remote += ":";
      command_remote += machine_file_name;

      int status = system(command_remote.c_str());
      if (status == -1)
      {
        INFOS("copy of the mpi machine file failed !");
        return "";
      }
    }

    if (is_a_proxy)
    {
      std::string command_remote("");
      if (remote)
      {
        // We want to launch a command like : 
        // ssh -l user machine distantPath/runRemote.sh hostNS portNS
        const ParserResourcesType& resInfo = _ResManager->GetImpl()->GetResourcesList(remote_machine);
        if (resInfo.Protocol == rsh)
          command_remote = "rsh ";
        else 
          command_remote = "ssh ";
        command_remote += "-l ";
        command_remote += resInfo.UserName;
        command_remote += " ";
        command_remote += remote_machine;
        command_remote += " ";
        command_remote += resInfo.AppliPath; // path relative to user@machine $HOME
        command_remote += "/runRemote.sh ";
        ASSERT(getenv("NSHOST")); 
        command_remote += getenv("NSHOST"); // hostname of CORBA name server
        command_remote += " ";
        ASSERT(getenv("NSPORT"));
        command_remote += getenv("NSPORT"); // port of CORBA name server
        command_remote += " ";

        hostname = remote_machine;
      }

      // We use Dummy proxy for MPI parallel containers
      real_exe_name  = exe_name + "Dummy";
      command =  real_exe_name;
      command += " " + _NS->ContainerName(rtn);
      command += " Dummy";
      command += " " + hostname;
      command += " " + nbproc;
      command += " -";
      AddOmninamesParams(command);

      command = command_begin + command_remote + command + command_end;
      vect_machine.push_back(hostname);
    }
    else                                          
    {
      std::string command_remote("");
      if (remote)
      {
        const ParserResourcesType& resInfo = _ResManager->GetImpl()->GetResourcesList(remote_machine);
        if (resInfo.Protocol == rsh)
          command_remote = "rsh ";
        else 
          command_remote = "ssh ";
        command_remote += "-l ";
        command_remote += resInfo.UserName;
        command_remote += " ";
        command_remote += remote_machine;
        command_remote += " ";

        std::string new_real_exe_name("");
        new_real_exe_name += resInfo.AppliPath; // path relative to user@machine $HOME
        new_real_exe_name += "/runRemote.sh ";
        ASSERT(getenv("NSHOST")); 
        new_real_exe_name += getenv("NSHOST"); // hostname of CORBA name server
        new_real_exe_name += " ";
        ASSERT(getenv("NSPORT"));
        new_real_exe_name += getenv("NSPORT"); // port of CORBA name server
        new_real_exe_name += " ";

        real_exe_name = new_real_exe_name + real_exe_name;
        hostname = remote_machine;
      }

      const ParserResourcesType& resInfo = _ResManager->GetImpl()->GetResourcesList(hostname);
      if (resInfo.mpi == lam)
      {
        command = "mpiexec -ssi boot ";
        if (resInfo.Protocol == rsh)
          command += "rsh ";
        else 
          command += "ssh ";
        command += "-machinefile " + machine_file_name + " "; 
        command += "-n " + nbproc + " ";
        command += real_exe_name;
        command += " " + _NS->ContainerName(rtn);
        command += " " + parallelLib;
        command += " " + proxy_hostname;
        command += " -";
        AddOmninamesParams(command);
      }
      else
      {
        command = "mpirun -np " + nbproc + " ";
        command += real_exe_name;
        command += " " + _NS->ContainerName(rtn);
        command += " " + parallelLib;
        command += " " + proxy_hostname;
        command += " -";
        AddOmninamesParams(command);
      }

      command = command_begin + command_remote + command + command_end;
      for (int i= 0; i < nb_nodes; i++)
        vect_machine.push_back(proxy_hostname);
    }
  }
  else
  {
    std::string message("Unknown parallelLib : " + parallelLib);
    throw SALOME_Exception(message.c_str());
  }

  MESSAGE("Parallel launch is: " << command);
  return command;
}
#endif

string SALOME_ContainerManager::GetMPIZeroNode(string machine)
{
  int status;
  string zeronode;
  string cmd;
  string tmpFile = BuildTemporaryFileName();

  if( getenv("LIBBATCH_NODEFILE") == NULL )
    cmd = "ssh " + machine + " mpirun -np 1 hostname > " + tmpFile;
  else
    cmd = "mpirun -np 1 -machinefile " + _machinesFile + " hostname > " + tmpFile;

  status = system(cmd.c_str());
  if( status == 0 ){
    ifstream fp(tmpFile.c_str(),ios::in);
    fp >> zeronode;
  }

  RmTmpFile(tmpFile);

  return zeronode;
}

void SALOME_ContainerManager::machinesFile(const int nbproc)
{
  string tmp;
  string nodesFile = getenv("LIBBATCH_NODEFILE");
  _machinesFile = Kernel_Utils::GetTmpFileName();
  ifstream fpi(nodesFile.c_str(),ios::in);
  ofstream fpo(_machinesFile.c_str(),ios::out);

  for(int i=0;i<_nbprocUsed;i++)
    fpi >> tmp;

  for(int i=0;i<nbproc;i++)
    if( fpi >> tmp )
      fpo << tmp << endl;
    else
      throw SALOME_Exception("You ask more processes than batch session have allocated!");

  _nbprocUsed += nbproc;
  fpi.close();
  fpo.close();

}
