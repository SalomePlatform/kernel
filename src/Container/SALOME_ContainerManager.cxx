// Copyright (C) 2007-2025  CEA, EDF, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
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

#include "SALOME_ContainerManager.hxx"
#include "SALOME_ResourcesManager.hxx"
#include "SALOME_LoadRateManager.hxx"
#include "SALOME_NamingService.hxx"
#include "SALOME_Container_i.hxx"
#include "SALOME_ResourcesManager_Client.hxx"
#include "SALOME_Embedded_NamingService.hxx"
#include "SALOME_ModuleCatalog.hh"
#include "Basics_Utils.hxx"
#include "Basics_DirUtils.hxx"
#include "PythonCppUtils.hxx"
#include "KernelBasis.hxx"
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <vector>
#include "Utils_CorbaException.hxx"
#include <sstream>
#include <string>
#include <queue>
#include <thread>
#include <chrono>

#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOME_Session)

#ifdef HAVE_MPI2
#include <mpi.h>
#include <sys/wait.h>
#endif

#ifdef WIN32
#include <process.h>
#define getpid _getpid

#ifndef S_ISREG
#define S_ISREG(mode)  (((mode) & S_IFMT) == S_IFREG)
#endif

#endif

#ifdef WITH_PACO_PARALLEL
#include "PaCOPP.hxx"
#endif

const int SALOME_ContainerManager::TIME_OUT_TO_LAUNCH_CONT=60;

const int SALOME_ContainerManager::DFT_DELTA_TIME_NS_LOOKUP_IN_MS=1000;

const char *SALOME_ContainerManager::_ContainerManagerNameInNS = "/ContainerManager";

omni_mutex SALOME_ContainerManager::_numInstanceMutex;

Utils_Mutex SALOME_ContainerManager::_getenvMutex;

Utils_Mutex SALOME_ContainerManager::_systemMutex;

//=============================================================================
/*!
 *  Constructor
 *  \param orb
 *  Define a CORBA single thread policy for the server, which avoid to deal
 *  with non thread-safe usage like Change_Directory in SALOME naming service
 */
//=============================================================================

SALOME_ContainerManager::SALOME_ContainerManager(CORBA::ORB_ptr orb, PortableServer::POA_var poa, SALOME_NamingService_Abstract *ns)
  : _nbprocUsed(1),_delta_time_ns_lookup_in_ms(DFT_DELTA_TIME_NS_LOOKUP_IN_MS),_delta_time_measure_in_ms(Abstract_Engines_Container_i::DFT_TIME_INTERVAL_BTW_MEASURE)
{
  _NS = ns;
  _resManager = new SALOME_ResourcesManager_Client(ns);
  _time_out_in_second = GetTimeOutToLoaunchServer();

  PortableServer::POAManager_var pman = poa->the_POAManager();
  _orb = CORBA::ORB::_duplicate(orb) ;
  CORBA::PolicyList policies;
  policies.length(1);
  PortableServer::ThreadPolicy_var threadPol(poa->create_thread_policy(PortableServer::ORB_CTRL_MODEL));
  policies[0] = PortableServer::ThreadPolicy::_duplicate(threadPol);

  _poa = poa->create_POA("MThreadPOA",pman,policies);
  threadPol->destroy();
  PortableServer::ObjectId_var id = _poa->activate_object(this);
  CORBA::Object_var obj = _poa->id_to_reference(id);
  Engines::ContainerManager_var refContMan = Engines::ContainerManager::_narrow(obj);
  if(_NS)
    _NS->Register(refContMan,_ContainerManagerNameInNS);
  _isAppliSalomeDefined = (GetenvThreadSafe("APPLI") != 0);

#ifdef HAVE_MPI2
#ifdef OPEN_MPI
  _pid_mpiServer = -1;
  // the urifile name depends on pid of the process
  std::stringstream urifile;
  urifile << GetenvThreadSafeAsString("HOME") << "/.urifile_" << getpid();
  setenv("OMPI_URI_FILE",urifile.str().c_str(),1);
  if( GetenvThreadSafe("OMPI_URI_FILE") != NULL ){
    // Linux specific code
    pid_t pid = fork(); // spawn a child process, following code is executed in both processes
    if ( pid == 0 ) // I'm a child, replace myself with a new ompi-server
    {
      std::string uriarg = GetenvThreadSafeAsString("OMPI_URI_FILE");
      execlp( "ompi-server", "ompi-server", "-r", uriarg.c_str(), NULL );
      throw SALOME_Exception("Error when launching ompi-server"); // execlp failed
    }
    else if ( pid < 0 )
    {
      throw SALOME_Exception("fork() failed");
    }
    else // I'm a parent
    {
      //wait(NULL); // wait(?) for a child end
      _pid_mpiServer = pid;
    }
  }
#elif defined(MPICH)
  _pid_mpiServer = -1;
  // Linux specific code
  pid_t pid = fork(); // spawn a child process, following code is executed in both processes
  if ( pid == 0 ) // I'm a child, replace myself with a new hydra_nameserver
  {
    execlp( "hydra_nameserver", "hydra_nameserver", NULL );
    throw SALOME_Exception("Error when launching hydra_nameserver"); // execlp failed
  }
  else if ( pid < 0 )
  {
    throw SALOME_Exception("fork() failed");
  }
  else // I'm a parent
  {
    //wait(NULL);
    _pid_mpiServer = pid;
  }
#endif
#endif
}

//=============================================================================
/*!
 * destructor
 */
//=============================================================================

SALOME_ContainerManager::~SALOME_ContainerManager()
{
  MESSAGE("destructor");
  delete _resManager;
#ifdef HAVE_MPI2
#ifdef OPEN_MPI
  if( GetenvThreadSafe("OMPI_URI_FILE") != NULL ){
    // kill my ompi-server
    if( kill(_pid_mpiServer,SIGTERM) != 0 )
      throw SALOME_Exception("Error when killing ompi-server");
    // delete my urifile
    int status=SystemThreadSafe("rm -f ${OMPI_URI_FILE}");
    if(status!=0)
      throw SALOME_Exception("Error when removing urifile");
  }
#elif defined(MPICH)
  // kill my hydra_nameserver
  if(_pid_mpiServer > -1)
    if( kill(_pid_mpiServer,SIGTERM) != 0 )
      throw SALOME_Exception("Error when killing hydra_nameserver");
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
  if(_NS)
    _NS->Destroy_Name(_ContainerManagerNameInNS);
  PortableServer::ObjectId_var oid = _poa->servant_to_id(this);
  _poa->deactivate_object(oid);
}

CORBA::Long SALOME_ContainerManager::GetTimeOutToLaunchServerInSecond()
{
  return this->_time_out_in_second;
}

void SALOME_ContainerManager::SetTimeOutToLaunchServerInSecond(CORBA::Long timeInSecond)
{
  this->_time_out_in_second = timeInSecond;
}

CORBA::Long SALOME_ContainerManager::GetDeltaTimeBetweenNSLookupAtLaunchTimeInMilliSecond()
{
  return this->_delta_time_ns_lookup_in_ms;
}

void SALOME_ContainerManager::SetDeltaTimeBetweenNSLookupAtLaunchTimeInMilliSecond(CORBA::Long timeInMS)
{
  this->_delta_time_ns_lookup_in_ms = timeInMS;
}

CORBA::Long SALOME_ContainerManager::GetDeltaTimeBetweenCPUMemMeasureInMilliSecond()
{
  return this->_delta_time_measure_in_ms;
}

void SALOME_ContainerManager::SetDeltaTimeBetweenCPUMemMeasureInMilliSecond(CORBA::Long timeInMS)
{
  this->_delta_time_measure_in_ms = timeInMS;
}

void SALOME_ContainerManager::SetBigObjOnDiskThreshold(CORBA::Long thresholdInByte)
{
  SALOME::SetBigObjOnDiskThreshold(thresholdInByte);
}

void SALOME_ContainerManager::SetBigObjOnDiskDirectory(const char *directory)
{
  SALOME::SetBigObjOnDiskDirectory(directory);
}

void SALOME_ContainerManager::SetDirectoryForReplayFiles(const char *directory)
{
  SALOME::SetDirectoryForReplayFiles( directory );
}

 void SALOME_ContainerManager::SetNumberOfRetry(CORBA::Long nbRetry)
 {
    SALOME::SetNumberOfRetry( nbRetry );
 }

CORBA::Long SALOME_ContainerManager::GetNumberOfRetry()
{
  return SALOME::GetNumberOfRetry();
}

//=============================================================================
//! Loop on all the containers listed in naming service, ask shutdown on each
/*! CORBA Method:
 */
//=============================================================================

void SALOME_ContainerManager::ShutdownContainersGeneric(std::function<void(Engines::Container_ptr)> funcToBeCalledOnContainer)
{
  MESSAGE("ShutdownContainersGeneric");
  if(!_NS)
    return ;
  SALOME_CMOD::Session_var session = SALOME_CMOD::Session::_nil();
  CORBA::Long pid = 0;
  CORBA::Object_var objS = _NS->Resolve("/Kernel/Session");
  if (!CORBA::is_nil(objS))
  {
    session = SALOME_CMOD::Session::_narrow(objS);
    if (!CORBA::is_nil(session))
      pid = session->getPID();
  }

  bool isOK;
  isOK = _NS->Change_Directory("/Containers");
  if( isOK ){
    std::vector<std::string> vec = _NS->list_directory_recurs();
    std::list<std::string> lstCont;
    for(std::vector<std::string>::iterator iter = vec.begin();iter!=vec.end();iter++)
      {
        SCRUTE((*iter));
        CORBA::Object_var obj=_NS->Resolve((*iter).c_str());
        try
          {
            Engines::Container_var cont=Engines::Container::_narrow(obj);
            if(!CORBA::is_nil(cont) && pid != cont->getPID())
              lstCont.push_back((*iter));
          }
        catch(const CORBA::Exception&)
          {
            // ignore this entry and continue
          }
      }
    MESSAGE("Container list: ");
    for(std::list<std::string>::iterator iter=lstCont.begin();iter!=lstCont.end();iter++){
      SCRUTE((*iter));
    }
    for(std::list<std::string>::iterator iter=lstCont.begin();iter!=lstCont.end();iter++)
    {
      try
      {
        SCRUTE((*iter));
        CORBA::Object_var obj=_NS->Resolve((*iter).c_str());
        Engines::Container_var cont=Engines::Container::_narrow(obj);
        if(!CORBA::is_nil(cont))
        {
          MESSAGE("ShutdownContainersGeneric: " << (*iter));
          funcToBeCalledOnContainer( cont );
          MESSAGE("ShutdownContainersGeneric: after call of shutdown" << (*iter));
        }
        else
          MESSAGE("ShutdownContainersGeneric: no container ref for " << (*iter));
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

void SALOME_ContainerManager::ShutdownContainers()
{
  this->ShutdownContainersGeneric( [](Engines::Container_ptr cont) { cont->Shutdown(); } );
}

void SALOME_ContainerManager::ShutdownContainersNow()
{
  this->ShutdownContainersGeneric( [](Engines::Container_ptr cont)
  {
    try
    {
      cont->ShutdownNow();
    }
    catch(...)
    {
    }
  }
    );
}

void SALOME_ContainerManager::SetOverrideEnvForContainers(const Engines::KeyValDict& env)
{
  this->_override_env.clear();
  auto sz = env.length();
  for(auto i = 0 ; i < sz ; ++i)
    _override_env.emplace_back(env[i].key.in(), env[i].val.in());
}

Engines::KeyValDict *SALOME_ContainerManager::GetOverrideEnvForContainers()
{
  std::unique_ptr<Engines::KeyValDict> ret( new Engines::KeyValDict );
  auto sz = _override_env.size();
  ret->length(sz);
  for(auto i = 0 ; i < sz ; ++i)
  {
    (*ret)[i].key = CORBA::string_dup( _override_env[i].first.c_str() );
    (*ret)[i].val = CORBA::string_dup( _override_env[i].second.c_str() );
  }
  return ret.release();
}

void SALOME_ContainerManager::SetCodeOnContainerStartUp(const char *code)
{
  _code_to_exe_on_startup = code;
}

char *SALOME_ContainerManager::GetCodeOnContainerStartUp()
{
  return CORBA::string_dup( _code_to_exe_on_startup.c_str() );
}

//=============================================================================
//! Give a suitable Container given constraints
/*! CORBA Method:
 *  \param params Container Parameters required for the container
 *  \return the container or nil
 */
//=============================================================================
Engines::Container_ptr SALOME_ContainerManager::GiveContainer(const Engines::ContainerParameters& params)
{
  std::string machFile;
  Engines::Container_ptr ret(Engines::Container::_nil());

  // Step 0: Default mode is start
  Engines::ContainerParameters local_params(params);
  if (std::string(local_params.mode.in()) == "")
    local_params.mode = CORBA::string_dup("start");
  std::string mode = local_params.mode.in();
  MESSAGE("[GiveContainer] starting with mode: " << mode);

  // Step 1: Find Container for find and findorstart mode
  if (mode == "find" || mode == "findorstart")
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
  // Consider only resources that can run containers
  resourceParams resource_params = resourceParameters_CORBAtoCPP(local_params.resource_params);
  resource_params.can_run_containers = true;
  std::vector<std::string> possibleResources = _resManager->GetFittingResources(resource_params);
  MESSAGE("[GiveContainer] - length of possible resources " << possibleResources.size());
  std::vector<std::string> local_resources;

  // Step 3: if mode is "get" keep only machines with existing containers
  if(mode == "get")
  {
    for(unsigned int i=0; i < possibleResources.size(); i++)
    {
      Engines::Container_ptr cont = FindContainer(params, possibleResources[i]);
      try
      {
        if(!cont->_non_existent())
          local_resources.push_back(possibleResources[i]);
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
    local_resources = possibleResources;

  // Step 4: select the resource where to get/start the container
  bool resource_available = true;
  std::string resource_selected;
  std::vector<std::string> resources = local_resources;
  while (resource_available)
  {
    if (resources.size() == 0)
      resource_available = false;
    else
    {
      try
      {
        resource_selected = _resManager->Find(params.resource_params.policy.in(), resources);
        // Remove resource_selected from vector
        std::vector<std::string>::iterator it;
        for (it=resources.begin() ; it < resources.end(); it++ )
          if (*it == resource_selected)
          {
            resources.erase(it);
            break;
          }
      }
      catch(const SALOME_Exception &ex) //!< TODO: unused variable
      {
        MESSAGE("[GiveContainer] Exception in ResourceManager find !: " << ex.what());
        return ret;
      }
      MESSAGE("[GiveContainer] Resource selected is: " << resource_selected);

      // Step 5: Create container name
      ParserResourcesType resource_definition = _resManager->GetResourceDefinition(resource_selected);
      std::string hostname(resource_definition.HostName);
      std::string containerNameInNS;
      if(params.isMPI){
        int nbproc = params.nb_proc <= 0 ? 1 : params.nb_proc;
        try
        {
          if( GetenvThreadSafe("LIBBATCH_NODEFILE") != NULL )
            machFile = machinesFile(nbproc);
        }
        catch(const SALOME_Exception & ex)
        {
          std::string err_msg = ex.what();
          err_msg += params.container_name;
          INFOS(err_msg.c_str());
          return ret;
        }
        // A mpi parallel container register on zero node in NS
        std::string mpiZeroNode = GetMPIZeroNode(resource_selected,machFile).c_str();
        containerNameInNS = _NS->BuildContainerNameForNS(params, mpiZeroNode.c_str());
      }
      else
        containerNameInNS = _NS->BuildContainerNameForNS(params, hostname.c_str());
      MESSAGE("[GiveContainer] Container name in the naming service: " << containerNameInNS);

      // Step 6: check if the name exists in naming service
      //if params.mode == "getorstart" or "get" use the existing container
      //if params.mode == "start" shutdown the existing container before launching a new one with that name

      { // critical section
        Utils_Locker lock (&_giveContainerMutex1);
        CORBA::Object_var obj = _NS->Resolve(containerNameInNS.c_str());
        if (!CORBA::is_nil(obj))
          {
            try
            {
                Engines::Container_var cont=Engines::Container::_narrow(obj);
                if(!cont->_non_existent())
                  {
                    if(std::string(params.mode.in())=="getorstart" || std::string(params.mode.in())=="get"){
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
      } // end critical section
      Engines::Container_var cont = LaunchContainer(params, resource_selected, hostname, machFile, containerNameInNS);
      if (!CORBA::is_nil(cont))
      {
        INFOS("[GiveContainer] container " << containerNameInNS << " launched");
        cont->monitoringtimeresms( this->_delta_time_measure_in_ms );
        INFOS("[GiveContainer] container " << containerNameInNS << " first CORBA invocation OK");
        std::ostringstream envInfo;
        std::for_each( _override_env.begin(), _override_env.end(), [&envInfo](const std::pair<std::string,std::string>& p) { envInfo << p.first << " = " << p.second << " "; } );
        INFOS("[GiveContainer] container " << containerNameInNS << " override " << envInfo.str());
        cont->set_big_obj_on_disk_directory( SALOME::GetBigObjOnDiskDirectoryCoarse().c_str() );
        cont->set_big_obj_on_disk_threshold( SALOME::GetBigObjOnDiskThreshold() );
        cont->set_directory_for_replay_files( SALOME::GetDirectoryForReplayFiles().c_str() );
        cont->set_number_of_retry( SALOME::GetNumberOfRetry() );
        cont->set_timeout_before_retrying( SALOME::GetExecutionTimeOut() );
        Engines::FieldsDict envCorba;
        {
          auto sz = _override_env.size();
          envCorba.length(sz);
          for(auto i = 0 ; i < sz ; ++i)
          {
            envCorba[i].key = CORBA::string_dup( _override_env[i].first.c_str() );
            envCorba[i].value <<= CORBA::string_dup( _override_env[i].second.c_str() );
          }
        }
        cont->override_environment_python( envCorba );
        if( SALOME::GetForwardCurrentDirectoryStatus() )
          cont->set_current_directory( SALOME::GetCurrentWorkingDirectory().c_str() );
        if( !_code_to_exe_on_startup.empty() )
        {
          INFOS("[GiveContainer] container " << containerNameInNS << " python code executed " << _code_to_exe_on_startup);
          cont->set_startup_code( _code_to_exe_on_startup.c_str() );
          cont->execute_python_code( _code_to_exe_on_startup.c_str() );
        }
        INFOS("[GiveContainer] container " << containerNameInNS << " verbosity positionning Activation = " << SALOME::VerbosityActivated() << " Verbosity Level = " << SALOME::VerbosityLevelStr());
        cont->setVerbosity( SALOME::VerbosityActivated(), SALOME::VerbosityLevelStr().c_str() );
        return cont._retn();
      }
      else
      {
        INFOS("[GiveContainer] Failed to launch container on resource " << resource_selected);
      }
    }
  }

  // We were not able to launch the container
  INFOS("[GiveContainer] Cannot launch the container on the following selected resources:")
  std::vector<std::string>::iterator it;
  for (it=local_resources.begin() ; it < local_resources.end(); it++ )
    INFOS("[GiveContainer] " << *it)
  return ret;
}

std::string SALOME_ContainerManager::GetCppBinaryOfKernelSSLContainer() const
{
  switch( SALOME::GetPyExecutionMode() )
  {
    case SALOME::PyExecutionMode::InProcess:
      return "SALOME_Container_No_NS_Serv";
    case SALOME::PyExecutionMode::OutOfProcessNoReplay:
      return "SALOME_Container_No_NS_Serv_OutProcess";
    case SALOME::PyExecutionMode::OutOfProcessWithReplay:
      return "SALOME_Container_No_NS_Serv_OutProcess_Replay";
    case SALOME::PyExecutionMode::OutOfProcessNoReplayFT:
      return "SALOME_Container_No_NS_Serv_OutProcess_FT";
    case SALOME::PyExecutionMode::OutOfProcessWithReplayFT:
      return "SALOME_Container_No_NS_Serv_OutProcess_Replay_FT";
    default:
      {
        ERROR_MESSAGE("Not manager py execution mode");
        THROW_SALOME_EXCEPTION("GetCppBinaryOfKernelSSLContainer : Not manager py execution mode");
      }
  }
}

std::string SALOME_ContainerManager::GetCppBinaryOfKernelContainer() const
{
  std::string ret = this->_isSSL ?  GetCppBinaryOfKernelSSLContainer() : "SALOME_Container";
  return ret;
}

std::string SALOME_ContainerManager::GetRunRemoteExecutableScript() const
{
  std::string ret = this->_isSSL ? "runRemoteSSL.sh" : "runRemote.sh";
  return ret;
}

Engines::Container_ptr
SALOME_ContainerManager::LaunchContainer(const Engines::ContainerParameters& params,
                                         const std::string & resource_selected,
                                         const std::string & hostname,
                                         const std::string & machFile,
                                         const std::string & containerNameInNS)
{
  std::string user,command,logFilename,tmpFileName;
  int status;
  Engines::Container_ptr ret(Engines::Container::_nil());
  {//start of critical section
    Utils_Locker lock (&_giveContainerMutex1);
    // Step 1: type of container: PaCO, Exe, Mpi or Classic
    // Mpi already tested in step 5, specific code on BuildCommandToLaunch Local/Remote Container methods
    // TODO -> separates Mpi from Classic/Exe
    // Classic or Exe ?
    std::string container_exe = this->GetCppBinaryOfKernelContainer();
    Engines::ContainerParameters local_params(params);
    int found=0;
    try
    {
        CORBA::String_var container_exe_tmp;
        CORBA::Object_var obj = _NS->Resolve("/Kernel/ModulCatalog");
        SALOME_ModuleCatalog::ModuleCatalog_var Catalog = SALOME_ModuleCatalog::ModuleCatalog::_narrow(obj) ;
        if (CORBA::is_nil (Catalog))
          {
            INFOS("[GiveContainer] Module Catalog is not found -> cannot launch a container");
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
        return ret;
    }
    catch (...)
    {
        INFOS("Caught unknown exception.");
        return ret;
    }

    // Step 2: test resource
    // Only if an application directory is set
    if(hostname != Kernel_Utils::GetHostname() && _isAppliSalomeDefined)
      {

        const ParserResourcesType resInfo(_resManager->GetResourceDefinition(resource_selected));
        std::string command = getCommandToRunRemoteProcess(resInfo.Protocol, resInfo.HostName, 
                                                           resInfo.UserName, resInfo.AppliPath);

        // Launch remote command
          command += " \"ls /tmp >/dev/null 2>&1\"";
        // Anthony : command is NO MORE launched to improve dramatically time to launch containers
        int status = 0;
        if (status != 0)
          {
            // Error on resource - cannot launch commands
            INFOS("[LaunchContainer] Cannot launch commands on machine " << hostname);
            INFOS("[LaunchContainer] Command was " << command);
#ifndef WIN32
            INFOS("[LaunchContainer] Command status is " << WEXITSTATUS(status));
#endif
            return Engines::Container::_nil();
          }
      }

    // Step 3: start a new container
    // Check if a PaCO container
    // PaCO++
    if (std::string(local_params.parallelLib.in()) != "")
      {
        ret = StartPaCOPPContainer(params, resource_selected);
        return ret;
      }
    // Other type of containers...
    MESSAGE("[GiveContainer] Try to launch a new container on " << resource_selected);
    // if a parallel container is launched in batch job, command is: "mpirun -np nbproc -machinefile nodesfile SALOME_MPIContainer"
    if( GetenvThreadSafe("LIBBATCH_NODEFILE") != NULL && params.isMPI )
    {
      command = BuildCommandToLaunchLocalContainer(params, machFile, container_exe, tmpFileName);
      MESSAGE("[LaunchContainer] LIBBATCH_NODEFILE : \"" << command << "\"");
    }
    // if a container is launched on localhost, command is "SALOME_Container" or "mpirun -np nbproc SALOME_MPIContainer"
    else if(hostname == Kernel_Utils::GetHostname())
    {
      command = BuildCommandToLaunchLocalContainer(params, machFile, container_exe, tmpFileName);
      MESSAGE("[LaunchContainer] hostname local : \"" << command << "\"");
    }
    // if a container is launched in remote mode, command is "ssh resource_selected SALOME_Container" or "ssh resource_selected mpirun -np nbproc SALOME_MPIContainer"
    else
    {
      command = BuildCommandToLaunchRemoteContainer(resource_selected, params, container_exe);
      MESSAGE("[LaunchContainer] remote : \"" << command << "\"");
    }

    //redirect stdout and stderr in a file
#ifdef WIN32
    logFilename=GetenvThreadSafeAsString("TEMP");
    logFilename += "\\";
    user = GetenvThreadSafeAsString( "USERNAME" );
#else
    user = GetenvThreadSafeAsString( "USER" );
    if (user.empty())
      user = GetenvThreadSafeAsString( "LOGNAME" );
    logFilename="/tmp";
    char* val = GetenvThreadSafe("SALOME_TMP_DIR");
    if(val)
      {
        struct stat file_info;
        stat(val, &file_info);
        bool is_dir = S_ISDIR(file_info.st_mode);
        if (is_dir)
          logFilename=val;
        else
          MESSAGE( "SALOME_TMP_DIR environment variable is not a directory use /tmp instead" << std::endl );
      }
    logFilename += "/";
#endif
    logFilename += _NS->ContainerName(params)+"_"+ resource_selected +"_"+user;
    std::ostringstream tmp;
    tmp << "_" << getpid();
    logFilename += tmp.str();
    logFilename += ".log" ;
    command += " > " + logFilename + " 2>&1";
    MakeTheCommandToBeLaunchedASync(command);
    
    MESSAGE("[LaunchContainer] SYSTEM COMMAND that will be launched : \"" << command << "\"");
    // launch container with a system call
    status=SystemThreadSafe(command.c_str());
  }//end of critical of section

  if (status == -1)
    {
      INFOS("[LaunchContainer] command failed (system command status -1): " << command);
      RmTmpFile(tmpFileName); // command file can be removed here
      return Engines::Container::_nil();
    }
  else if (status == 217)
    {
      INFOS("[LaunchContainer] command failed (system command status 217): " << command);
      RmTmpFile(tmpFileName); // command file can be removed here
      return Engines::Container::_nil();
    }
  else
    {
      // Step 4: Wait for the container
      double nbTurn = ( (double)this->_time_out_in_second ) * ( 1000.0 / ( (double) this->_delta_time_ns_lookup_in_ms) );
      int count( (int)nbTurn );
      INFOS("[GiveContainer] # attempts : " << count << " name in NS : \"" << containerNameInNS << "\"");
      INFOS("[GiveContainer] # attempts : Time in second before time out : " << this->_time_out_in_second << " Delta time in ms between NS lookup : " << this->_delta_time_ns_lookup_in_ms);
      while (CORBA::is_nil(ret) && count)
        {
          std::this_thread::sleep_for(std::chrono::milliseconds(_delta_time_ns_lookup_in_ms));
          count--;
          MESSAGE("[GiveContainer] step " << count << " Waiting for container on " << resource_selected << " with entry in NS = \"" << containerNameInNS << "\"" );
          CORBA::Object_var obj(_NS->Resolve(containerNameInNS.c_str()));
          ret=Engines::Container::_narrow(obj);
        }
      if (CORBA::is_nil(ret))
        {
          INFOS("[GiveContainer] was not able to launch container " << containerNameInNS);
        }
      else
        {
          // Setting log file name
          ret->locallogfilename( logFilename.c_str() );
          logFilename=":"+logFilename;
          logFilename="@"+Kernel_Utils::GetHostname()+logFilename;//threadsafe
          logFilename=user+logFilename;
          ret->logfilename(logFilename.c_str());
          RmTmpFile(tmpFileName); // command file can be removed here
        }
    }
  return ret;
}

//=============================================================================
//! Find a container given constraints (params) on a list of machines (possibleComputers)
//! agy : this method is ThreadSafe
/*!
 *
 */
//=============================================================================

Engines::Container_ptr SALOME_ContainerManager::FindContainer(const Engines::ContainerParameters& params, const Engines::ResourceList& possibleResources)
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
//! agy : this method is ThreadSafe
/*!
 *
 */
//=============================================================================

Engines::Container_ptr
SALOME_ContainerManager::FindContainer(const Engines::ContainerParameters& params, const std::string& resource)
{
  ParserResourcesType resource_definition = _resManager->GetResourceDefinition(resource);
  std::string hostname(resource_definition.HostName);
  std::string containerNameInNS(_NS->BuildContainerNameForNS(params, hostname.c_str()));
  MESSAGE("[FindContainer] Try to find a container  " << containerNameInNS << " on resource " << resource);
  CORBA::Object_var obj = _NS->Resolve(containerNameInNS.c_str());
  try
  {
    if(obj->_non_existent())
      return Engines::Container::_nil();
    else
      return Engines::Container::_narrow(obj);
  }
  catch(const CORBA::Exception&)
  {
    return Engines::Container::_nil();
  }
}


bool isPythonContainer(const char* ContainerName);

//=============================================================================
/*!
 *  This is no longer valid (C++ container are also python containers)
 */
//=============================================================================
bool isPythonContainer(const char* ContainerName)
{
  return false; // VSR 02/08/2013: Python containers are no more supported
  bool ret = false;
  size_t len = strlen(ContainerName);

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

 *  - where user is omitted if not specified in CatalogResources,
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

std::string
SALOME_ContainerManager::BuildCommandToLaunchRemoteContainer(const std::string& resource_name, const Engines::ContainerParameters& params, const std::string& container_exe) const
{
  std::string command,tmpFileName;
  const ParserResourcesType resInfo(_resManager->GetResourceDefinition(resource_name));
  std::string wdir = params.workingdir.in();
  if (!_isAppliSalomeDefined)
  {
      MESSAGE("[BuildCommandToLaunchRemoteContainer] NO APPLI MODE : " << " Protocol :" << resInfo.Protocol << " hostname :" << resInfo.HostName << " username : " << resInfo.UserName << " appli : " << resInfo.AppliPath << " wdir : \"" << wdir << "\"");
      command = getCommandToRunRemoteProcessNoAppli(resInfo.Protocol, resInfo.HostName, 
                                                    resInfo.UserName, resInfo.AppliPath,
                                                    wdir);
  }
  else
  {
    MESSAGE("[BuildCommandToLaunchRemoteContainer] WITH APPLI MODE : " << " Protocol :" << resInfo.Protocol << " hostname :" << resInfo.HostName << " username : " << resInfo.UserName << " appli : " << resInfo.AppliPath << " wdir : \"" << wdir << "\"");
    // "ssh -l user machine distantPath/runRemote.sh hostNS portNS WORKINGDIR workingdir
    //      SALOME_Container containerName -ORBInitRef NameService=IOR:01000..."
    //  or 
    //  "ssh -l user machine distantLauncher remote -p hostNS -m portNS -d dir
    //      --  SALOME_Container contName -ORBInitRef NameService=IOR:01000..."
    command = getCommandToRunRemoteProcess(resInfo.Protocol, resInfo.HostName, 
                                           resInfo.UserName, resInfo.AppliPath,
                                           wdir);
  }
  if(params.isMPI)
  {
    int nbproc = params.nb_proc <= 0 ? 1 : params.nb_proc;
    command += " mpirun -np ";
    std::ostringstream o;
    o << nbproc << " ";
    command += o.str();
#ifdef LAM_MPI
    command += "-x PATH,LD_LIBRARY_PATH,OMNIORB_CONFIG,SALOME_trace ";
#elif defined(OPEN_MPI)
    if( GetenvThreadSafe("OMPI_URI_FILE") == NULL )
      command += "-x PATH -x LD_LIBRARY_PATH -x OMNIORB_CONFIG -x SALOME_trace";
    else{
      command += "-x PATH -x LD_LIBRARY_PATH -x OMNIORB_CONFIG -x SALOME_trace -ompi-server file:";
      command += GetenvThreadSafeAsString("OMPI_URI_FILE");
    }
#elif defined(MPICH)
    command += "-nameserver " + Kernel_Utils::GetHostname();
#endif
    command += " SALOME_MPIContainer ";
  }
  else
    command += " " +container_exe+ " ";

  command += _NS->ContainerName(params) + " ";
  if(this->_isSSL)
  {
    Engines::EmbeddedNamingService_var ns = GetEmbeddedNamingService();
    CORBA::String_var iorNS = _orb->object_to_string(ns);
    command += std::string(iorNS);
  }
  else //if(!this->_isSSL)
  {
    command += " -";
    AddOmninamesParams(command);
  }
  MESSAGE("command =" << command);

  return command;
}

//=============================================================================
//! Return a path to the directory with scripts templates
/*! 
 *  \return the path pointed by SALOME_KERNEL_SCRIPTS_DIR environment variable, if it is defined,
 *  ${KERNEL_ROOT_DIR}/share/salome/resources/separator/kernel/ScriptsTemplate - otherwise
 */
//=============================================================================
std::string getScriptTemplateFilePath()
{
  auto parseScriptTemplateFilePath = []() -> std::string
  {
    std::string scriptTemplateFilePath = SALOME_ContainerManager::GetenvThreadSafeAsString("SALOME_KERNEL_SCRIPTS_DIR");
    if (!scriptTemplateFilePath.empty())
    {
      return scriptTemplateFilePath;
    }
    else
    {
      AutoGIL agil;
      AutoPyRef kc = PyImport_ImportModule("salome.kernel.KernelContainer");
      AutoPyRef locOfScriptsPy = PyObject_GetAttrString(kc,"getDftLocOfScripts");
      AutoPyRef resPy = PyObject_CallFunction(locOfScriptsPy,"");
      std::string ret( PyUnicode_AsUTF8( resPy ) );
      return ret;
    }
  };

  static const std::string scriptTemplateFilePath = parseScriptTemplateFilePath();
  return scriptTemplateFilePath;
}

//=============================================================================
//! Return a command line constructed based on Python scripts templates
/*! 
 *  \param theScriptName        the name of Python script template
 *  \param theScriptParameters  the queue of parameter values
 *  \return the command line constructed according to the given parameters
 */
//=============================================================================
std::string GetCommandFromTemplate(const std::string& theScriptName,
                                   std::queue<std::string>& theScriptParameters)
{
  std::string command;
  AutoGIL agil;
  // manage GIL

  PyObject* mod(PyImport_ImportModule(theScriptName.c_str()));
  MESSAGE("Template name :"<< theScriptName.c_str());

  if (!mod)
  {
    AutoPyRef sys = PyImport_ImportModule("sys");
    AutoPyRef sys_path = PyObject_GetAttrString(sys, "path");
    AutoPyRef folder_path = PyUnicode_FromString(getScriptTemplateFilePath().c_str());
    PyList_Append(sys_path, folder_path);

    mod = PyImport_ImportModule(theScriptName.c_str());

  }

  if (mod)
  {
    PyObject* meth(PyObject_GetAttrString(mod, "command"));
    if (!meth)
    {
      Py_XDECREF(mod);
    }
    else
    {
      int id = -1;
      PyObject* tuple(PyTuple_New(theScriptParameters.size()));

      auto insert_parameter = [&tuple, &theScriptParameters, &id]()
      {
        if (!theScriptParameters.empty())
        {
          PyTuple_SetItem(tuple, ++id, PyUnicode_FromString(theScriptParameters.front().c_str()));
          theScriptParameters.pop();
        }
      };

      while (!theScriptParameters.empty())
      {
        insert_parameter();
      }
      
      PyObject *args(PyTuple_New(1));
      PyTuple_SetItem(args, 0, tuple);

      PyObject *res(PyObject_CallObject(meth, args));
      if (res)
      {
        command = PyUnicode_AsUTF8(res);
        Py_XDECREF(res);
      }

      Py_XDECREF(args);
      Py_XDECREF(tuple);
      Py_XDECREF(meth);
      Py_XDECREF(mod);
    }
  }
  else
  {
    ERROR_MESSAGE("Can not import the template script \"" << theScriptName << "\" !");
  }

  MESSAGE("Command from template is ... " << command << std::endl);
  return command;
}
//=============================================================================

//=============================================================================
/*!
 *  builds the command to be launched.
 */
//=============================================================================
std::string SALOME_ContainerManager::BuildCommandToLaunchLocalContainer(const Engines::ContainerParameters& params, const std::string& machinesFile, const std::string& container_exe, std::string& tmpFileName) const
{
  // Prepare name of the script to be used
  std::string script_name = "SALOME_CM_LOCAL_NO_MPI";
  if (params.isMPI)
  {
#ifdef LAM_MPI
    script_name = "SALOME_CM_LOCAL_MPI_LAN";
#elif defined(OPEN_MPI)
    script_name = "SALOME_CM_LOCAL_MPI_OPENMPI";
#elif defined(MPICH)
    script_name = "SALOME_CM_LOCAL_MPI_MPICH";
#endif
  }
  
  // Prepare parameters to use in the Python script:
  // 1. All parameters are strings.
  // 2. For some booleans use "1" = True, "0" = False.
  // 3. If a parameter is NULL, then its value is "NULL".

  std::queue<std::string> script_parameters;
  
  // ===== Number of processes (key = "nb_proc")
  script_parameters.push(params.isMPI ? std::to_string(params.nb_proc <= 0 ? 1 : params.nb_proc) : "NULL");

  // ===== Working directory (key = "workdir") and temporary directory flag (key = "isTmpDir")
  // A working directory is requested
  std::string workdir = params.workingdir.in();
  std::string isTmpDir = std::to_string(0);
  if (workdir == "$TEMPDIR")
  {
    // A new temporary directory is requested
    isTmpDir = std::to_string(1);
    workdir = Kernel_Utils::GetTmpDir();
  }
  script_parameters.push(workdir);
  script_parameters.push(isTmpDir);
  
  // ===== Server name (key = "name_server")
  script_parameters.push(Kernel_Utils::GetHostname());

  // ===== Container (key = "container")
  std::string container;
  if (params.isMPI)
  {
    container = isPythonContainer(params.container_name) ? "pyMPI SALOME_ContainerPy.py" : "SALOME_MPIContainer";
  }
  else
  {
    container = isPythonContainer(params.container_name) ? "SALOME_ContainerPy.py" : container_exe;
  }
  script_parameters.push(container);

  // ===== Container name (key = "container_name")
  script_parameters.push(_NS->ContainerName(params));

  // ===== LIBBATCH node file (key = "libbatch_nodefile")
  script_parameters.push(std::to_string(GetenvThreadSafe("LIBBATCH_NODEFILE") != NULL ? 1 : 0));

  // ===== Machine file (key = "machine_file")
  script_parameters.push(machinesFile.empty() ? "NULL" : machinesFile);

  // ===== OMPI uri file (key = "ompi_uri_file")
  std::string ompi_uri_file = GetenvThreadSafeAsString("OMPI_URI_FILE");
  script_parameters.push(ompi_uri_file.empty() ? "NULL" : ompi_uri_file);

  MESSAGE("Retrieving command from template (python module) \"" << script_name << "\"" << std::endl);
  std::string command_from_template = GetCommandFromTemplate(script_name, script_parameters);

  std::ostringstream o;
  o << command_from_template << " ";
  
  //==================================================================================== */

  if( this->_isSSL )
  {
    Engines::EmbeddedNamingService_var ns = GetEmbeddedNamingService();
    CORBA::String_var iorNS = _orb->object_to_string(ns);
    o << iorNS;
  }
  else
  {
    o << "-";
    AddOmninamesParams(o);
  }
  
  tmpFileName = BuildTemporaryFileName();
  std::ofstream command_file( tmpFileName.c_str() );
  command_file << o.str();
  command_file.close();

#ifndef WIN32
  chmod(tmpFileName.c_str(), 0x1ED);
#endif
  
  std::string command = tmpFileName;
  MESSAGE("Command is file ... " << command);
  MESSAGE("Command is ... " << o.str());
  return command;
}


//=============================================================================
/*!
 *  removes the generated temporary file in case of a remote launch.
 *  This method is thread safe
 */
//=============================================================================

void SALOME_ContainerManager::RmTmpFile(std::string& tmpFileName)
{
  size_t length = tmpFileName.size();
  if ( length  > 0)
    {
#ifdef WIN32
      std::string command = "del /F ";
#else
      std::string command = "rm ";
#endif
      if ( length > 4 )
        command += tmpFileName.substr(0, length - 3 );
      else
        command += tmpFileName;
      command += '*';
      SystemThreadSafe(command.c_str());
      //if dir is empty - remove it
      std::string tmp_dir = Kernel_Utils::GetDirByPath( tmpFileName );
      if ( Kernel_Utils::IsEmptyDir( tmp_dir ) )
        {
#ifdef WIN32
          command = "del /F " + tmp_dir;
#else
          command = "rmdir " + tmp_dir;
#endif
          SystemThreadSafe(command.c_str());
        }
    }
}

//=============================================================================
/*!
 *   add to command all options relative to naming service.
 */
//=============================================================================

void SALOME_ContainerManager::AddOmninamesParams(std::string& command) const
{
  std::ostringstream oss;
  AddOmninamesParams(oss);
  command+=oss.str();
}

//=============================================================================
/*!
 *  add to command all options relative to naming service.
 */
//=============================================================================

void SALOME_ContainerManager::AddOmninamesParams(std::ostream& fileStream) const
{
  AddOmninamesParams(fileStream,_NS);
}

//=============================================================================
/*!
 *  add to command all options relative to naming service.
 */
//=============================================================================

void SALOME_ContainerManager::AddOmninamesParams(std::ostream& fileStream, SALOME_NamingService_Abstract *ns)
{
  SALOME_NamingService *nsTrad(dynamic_cast<SALOME_NamingService *>(ns));
  if(nsTrad)
  {
    CORBA::String_var iorstr(nsTrad->getIORaddr());
    fileStream << "ORBInitRef NameService=";
    fileStream << iorstr;
  }
}

void SALOME_ContainerManager::MakeTheCommandToBeLaunchedASync(std::string& command)
{
#ifdef WIN32
    command = "%PYTHONBIN% -c \"import subprocess ; subprocess.Popen(r'" + command + "').pid\"";
#else
    command += " &";
#endif
}

/*!
 * Return in second the time out to give chance to server to be launched and
 * to register into NS
 */
int SALOME_ContainerManager::GetTimeOutToLoaunchServer()
{
  int count(TIME_OUT_TO_LAUNCH_CONT);
  if (GetenvThreadSafe("TIMEOUT_TO_LAUNCH_CONTAINER") != 0)
    {
      std::string new_count_str(GetenvThreadSafeAsString("TIMEOUT_TO_LAUNCH_CONTAINER"));
      int new_count;
      std::istringstream ss(new_count_str);
      if (!(ss >> new_count))
        {
          INFOS("[LaunchContainer] TIMEOUT_TO_LAUNCH_CONTAINER should be an int");
        }
      else
        count = new_count;
    }
  return count;
}

void SALOME_ContainerManager::SleepInSecond(int ellapseTimeInSecond)
{
#ifndef WIN32
  sleep( ellapseTimeInSecond ) ;
#else
  int timeInMS(1000*ellapseTimeInSecond);
  Sleep(timeInMS);
#endif
}

//=============================================================================
/*!
 *  generate a file name in /tmp directory
 */
//=============================================================================

std::string SALOME_ContainerManager::BuildTemporaryFileName()
{
  //build more complex file name to support multiple salome session
  std::string aFileName = Kernel_Utils::GetTmpFileName();
  std::ostringstream str_pid;
  str_pid << ::getpid();
  aFileName = aFileName + "-" + str_pid.str();
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

std::string SALOME_ContainerManager::BuildTempFileToLaunchRemoteContainer (const std::string& resource_name, const Engines::ContainerParameters& params, std::string& tmpFileName) const
{
  int status;

  tmpFileName = BuildTemporaryFileName();
  std::ofstream tempOutputFile;
  tempOutputFile.open(tmpFileName.c_str(), std::ofstream::out );
  const ParserResourcesType resInfo(_resManager->GetResourceDefinition(resource_name));
  tempOutputFile << "#! /bin/sh" << std::endl;

  // --- set env vars

  tempOutputFile << "export SALOME_trace=local" << std::endl; // mkr : 27.11.2006 : PAL13967 - Distributed supervision graphs - Problem with "SALOME_trace"
  //tempOutputFile << "source " << resInfo.PreReqFilePath << endl;

  // ! env vars

  if (params.isMPI)
    {
      int nbproc = params.nb_proc <= 0 ? 1 : params.nb_proc;

      tempOutputFile << "mpirun -np ";

      tempOutputFile << nbproc << " ";
#ifdef LAM_MPI
      tempOutputFile << "-x PATH,LD_LIBRARY_PATH,OMNIORB_CONFIG,SALOME_trace ";
#elif defined(OPEN_MPI)
      if( GetenvThreadSafe("OMPI_URI_FILE") == NULL )
        tempOutputFile << "-x PATH -x LD_LIBRARY_PATH -x OMNIORB_CONFIG -x SALOME_trace";
      else{
        tempOutputFile << "-x PATH -x LD_LIBRARY_PATH -x OMNIORB_CONFIG -x SALOME_trace -ompi-server file:";
        tempOutputFile << GetenvThreadSafeAsString("OMPI_URI_FILE");
      }
#elif defined(MPICH)
      tempOutputFile << "-nameserver " + Kernel_Utils::GetHostname();
#endif
    }

  tempOutputFile << GetenvThreadSafeAsString("KERNEL_ROOT_DIR") << "/bin/salome/";

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
  tempOutputFile << " &" << std::endl;
  tempOutputFile.flush();
  tempOutputFile.close();
#ifndef WIN32
  chmod(tmpFileName.c_str(), 0x1ED);
#endif

  // --- Build command

  std::string command;

  if (resInfo.Protocol == rsh)
    {
      command = "rsh ";
      std::string commandRcp = "rcp ";
      commandRcp += tmpFileName;
      commandRcp += " ";
      commandRcp += resInfo.HostName;
      commandRcp += ":";
      commandRcp += tmpFileName;
      status = SystemThreadSafe(commandRcp.c_str());
    }

  else if (resInfo.Protocol == ssh)
    {
      command = "ssh ";
      std::string commandRcp = "scp ";
      commandRcp += tmpFileName;
      commandRcp += " ";
      commandRcp += resInfo.HostName;
      commandRcp += ":";
      commandRcp += tmpFileName;
      status = SystemThreadSafe(commandRcp.c_str());
    }

  else if (resInfo.Protocol == srun)
    {
      command = "srun -n 1 -N 1 --overlap --mem-per-cpu=0 --cpu-bind=none --nodelist=";
      std::string commandRcp = "rcp ";
      commandRcp += tmpFileName;
      commandRcp += " ";
      commandRcp += resInfo.HostName;
      commandRcp += ":";
      commandRcp += tmpFileName;
      status = SystemThreadSafe(commandRcp.c_str());
    }
  else
    throw SALOME_Exception("Unknown protocol");

  if(status)
    throw SALOME_Exception("Error of connection on remote host");

  command += resInfo.HostName;
  command += " ";
  command += tmpFileName;

  SCRUTE(command);

  return command;

}

std::string SALOME_ContainerManager::GetMPIZeroNode(const std::string machine, const std::string machinesFile) const
{
  int status;
  std::string zeronode;
  std::string command;
  std::string tmpFile = BuildTemporaryFileName();
  const ParserResourcesType resInfo(_resManager->GetResourceDefinition(machine));

  if(resInfo.Protocol == sh)
  {
    return resInfo.HostName;
  }

  if( GetenvThreadSafe("LIBBATCH_NODEFILE") == NULL )
    {
      if (_isAppliSalomeDefined)
        {
          command = getCommandToRunRemoteProcess(resInfo.Protocol, resInfo.HostName, 
                                                 resInfo.UserName, resInfo.AppliPath);
          command += " mpirun -np 1 hostname -s > " + tmpFile;
        }
      else
        command = "mpirun -np 1 hostname -s > " + tmpFile;
    }
  else
    command = "mpirun -np 1 -machinefile " + machinesFile + " hostname -s > " + tmpFile;

  status = SystemThreadSafe(command.c_str());
  if( status == 0 ){
    std::ifstream fp(tmpFile.c_str(),std::ios::in);
    while(fp >> zeronode);
  }

  RmTmpFile(tmpFile);

  return zeronode;
}

std::string SALOME_ContainerManager::machinesFile(const int nbproc)
{
  std::string tmp;
  std::string nodesFile = GetenvThreadSafeAsString("LIBBATCH_NODEFILE");
  std::string machinesFile = Kernel_Utils::GetTmpFileName();
  std::ifstream fpi(nodesFile.c_str(),std::ios::in);
  std::ofstream fpo(machinesFile.c_str(),std::ios::out);

  _numInstanceMutex.lock();

  for(int i=0;i<_nbprocUsed;i++)
    fpi >> tmp;

  for(int i=0;i<nbproc;i++)
    if( fpi >> tmp )
      fpo << tmp << std::endl;
    else
      throw SALOME_Exception("You need more processors than batch session have allocated for you! Unable to launch the mpi container: ");

  _nbprocUsed += nbproc;
  fpi.close();
  fpo.close();

  _numInstanceMutex.unlock();

  return machinesFile;

}

std::string SALOME_ContainerManager::getCommandToRunRemoteProcessNoAppli(AccessProtocolType protocol, const std::string & hostname, const std::string & username, const std::string & applipath, const std::string & workdir) const
{
  return getCommandToRunRemoteProcessCommon("SALOME_CM_REMOTE","salome shell --",protocol,hostname,username,applipath,workdir);
}

std::string SALOME_ContainerManager::getCommandToRunRemoteProcess(AccessProtocolType protocol, const std::string & hostname, const std::string & username, const std::string & applipath, const std::string & workdir) const
{
  return getCommandToRunRemoteProcessCommon("SALOME_CM_REMOTE_OLD",this->GetRunRemoteExecutableScript(),protocol,hostname,username,applipath,workdir);
}

std::string SALOME_ContainerManager::getCommandToRunRemoteProcessCommon(const std::string& templateName,
                                                                  const std::string& remoteScript,
                                                                  AccessProtocolType protocol,
                                                                  const std::string & hostname,
                                                                  const std::string & username,
                                                                  const std::string & applipath,
                                                                  const std::string & workdir) const
{
  std::ostringstream command;
  
  // Prepare parameters to use in the Python script:
  // 1. All parameters are strings.
  // 2. For some booleans use "1" = True, "0" = False.
  // 3. If a parameter is NULL, then its value is "NULL".
  
  std::queue<std::string> script_parameters;

  // ===== Protocol (key = "protocol")
  std::string strProtocol;
  switch (protocol)
  {
  case rsh: strProtocol = "rsh"; break;
  case ssh: strProtocol = "ssh"; break;
  case srun: strProtocol = "srun"; break;
  case pbsdsh: strProtocol = "pbsdsh"; break;
  case blaunch: strProtocol = "blaunch"; break;
  default:
    throw SALOME_Exception("Unknown protocol");
  }
  script_parameters.push(strProtocol);

  // ===== User name (key = "user")
  script_parameters.push(username.empty() ? "NULL" : username);
  
  // ===== Host name (key = "host")
  script_parameters.push(hostname.empty() ? "NULL" : hostname);
  
 
  // ===== Remote APPLI path (key = "appli")
  script_parameters.push(applipath.empty() ? GetenvThreadSafeAsString("APPLI") : applipath);
  
  if(!this->_isSSL)
  {
    ASSERT(GetenvThreadSafe("NSHOST"));
    ASSERT(GetenvThreadSafe("NSPORT"));
  }

  struct stat statbuf;
  std::string appli_mode = (stat(GetenvThreadSafe("APPLI"), &statbuf) == 0 && S_ISREG(statbuf.st_mode)) ? "launcher" : "dir";

  // ===== Working directory (key = "workdir")
  script_parameters.push(workdir == "$TEMPDIR" ? "\\$TEMPDIR" : workdir);
  
  // ===== SSL (key = "ssl")
  script_parameters.push(this->_isSSL ? "1" : "0");
  
  // ===== Hostname of CORBA name server (key = "nshost")
  std::string nshost = GetenvThreadSafeAsString("NSHOST");
  script_parameters.push(nshost.empty() ? "NULL" : nshost);

  // ===== Port of CORBA name server (key = "nsport")
  std::string nsport = GetenvThreadSafeAsString("NSPORT");
  script_parameters.push(nsport.empty() ? "NULL" : nsport);

  // ===== Remote script (key = "remote_script")
  script_parameters.push(remoteScript.empty() ? "NONE" : remoteScript);
  
  // ===== Naming service (key = "naming_service")
  std::string namingService = "NONE";
  if(this->_isSSL)
  {
    Engines::EmbeddedNamingService_var ns = GetEmbeddedNamingService();
    CORBA::String_var iorNS = _orb->object_to_string(ns);
    namingService = iorNS;
  }
  script_parameters.push(namingService);

  // ===== APPLI mode (key = "appli_mode")
  // $APPLI points either to an application directory, or to a salome launcher file
  // we prepare the remote command according to the case
  script_parameters.push(appli_mode);

  command << GetCommandFromTemplate(templateName, script_parameters);

  return command.str();
}

bool
SALOME_ContainerManager::checkPaCOParameters(Engines::ContainerParameters & params, std::string resource_selected)
{
  bool result = true;

  // Step 1 : check ContainerParameters
  // Check container_name, has to be defined
  if (std::string(params.container_name.in()) == "")
  {
    INFOS("[checkPaCOParameters] You must define a container_name to launch a PaCO++ container");
    result = false;
  }
  // Check parallelLib
  std::string parallelLib = params.parallelLib.in();
  if (parallelLib != "Mpi" && parallelLib != "Dummy")
  {
    INFOS("[checkPaCOParameters] parallelLib is not correctly defined");
    INFOS("[checkPaCOParameters] you can chosse between: Mpi and Dummy");
    INFOS("[checkPaCOParameters] you entered: " << parallelLib);
    result = false;
  }
  // Check nb_proc
  if (params.nb_proc <= 0)
  {
    INFOS("[checkPaCOParameters] You must define a nb_proc > 0");
    result = false;
  }

  // Step 2 : check resource_selected
  const ParserResourcesType resource_definition = _resManager->GetResourceDefinition(resource_selected);
  //std::string protocol = resource_definition->protocol.in();
  std::string username = resource_definition.UserName;
  std::string applipath = resource_definition.AppliPath;

  //if (protocol == "" || username == "" || applipath == "")
  if (username == "" || applipath == "")
  {
    INFOS("[checkPaCOParameters] resource selected is not well defined");
    INFOS("[checkPaCOParameters] resource name: " << resource_definition.Name);
    INFOS("[checkPaCOParameters] resource hostname: " << resource_definition.HostName);
    INFOS("[checkPaCOParameters] resource protocol: " << resource_definition.getAccessProtocolTypeStr());
    INFOS("[checkPaCOParameters] resource username: " << username);
    INFOS("[checkPaCOParameters] resource applipath: " << applipath);
    result = false;
  }

  return result;
}

/*
 * :WARNING: Do not directly convert returned value to std::string
 * This function may return NULL if env variable is not defined.
 * And std::string(NULL) causes undefined behavior.
 * Use GetenvThreadSafeAsString to properly get a std::string.
*/
char *SALOME_ContainerManager::GetenvThreadSafe(const char *name)
{// getenv is not thread safe. See man 7 pthread.
  Utils_Locker lock (&_getenvMutex);
  return getenv(name);
}

/*
 * Return env variable as a std::string.
 * Return empty string if env variable is not set.
 */
std::string SALOME_ContainerManager::GetenvThreadSafeAsString(const char *name)
{
  char* var = GetenvThreadSafe(name);
  return var ? std::string(var) : std::string();
}

int SALOME_ContainerManager::SystemThreadSafe(const char *command)
{
  Utils_Locker lock (&_systemMutex);
  return system(command);
}

long SALOME_ContainerManager::SystemWithPIDThreadSafe(const std::vector<std::string>& command)
{
  Utils_Locker lock(&_systemMutex);
  if(command.size()<1)
    throw SALOME_Exception("SystemWithPIDThreadSafe : command is expected to have a length of size 1 at least !");
#ifndef WIN32
  pid_t pid ( fork() ) ; // spawn a child process, following code is executed in both processes
#else 
  pid_t pid = -1; //Throw SALOME_Exception on Windows
#endif
  if ( pid == 0 ) // I'm a child, replace myself with a new ompi-server
    {
      std::size_t sz(command.size());
      char **args = new char *[sz+1];
      for(std::size_t i=0;i<sz;i++)
        args[i] = strdup(command[i].c_str());
      args[sz] = nullptr;
      execvp( command[0].c_str() , args ); 
      std::ostringstream oss;
      oss << "Error when launching " << command[0];
      throw SALOME_Exception(oss.str().c_str()); // execvp failed
    }
  else if ( pid < 0 )
    {
      throw SALOME_Exception("fork() failed");
    }
  else // I'm a parent
    {
      return pid;
    }
}

#ifdef WITH_PACO_PARALLEL

//=============================================================================
/*! CORBA Method:
 *  Start a suitable PaCO++ Parallel Container in a list of machines.
 *  \param params           Container Parameters required for the container
 *  \return CORBA container reference.
 */
//=============================================================================
Engines::Container_ptr
SALOME_ContainerManager::StartPaCOPPContainer(const Engines::ContainerParameters& params_const,
                                              std::string resource_selected)
{
  CORBA::Object_var obj;
  PaCO::InterfaceManager_var container_proxy;
  Engines::Container_ptr ret = Engines::Container::_nil();
  Engines::ContainerParameters params(params_const);
  params.resource_params.name = CORBA::string_dup(resource_selected.c_str());

  // Step 0 : Check parameters
  if (!checkPaCOParameters(params, resource_selected))
  {
    INFOS("[StartPaCOPPContainer] check parameters failed ! see logs...");
    return ret;
  }

  // Step 1 : Starting a new parallel container !
  INFOS("[StartPaCOPPContainer] Starting a PaCO++ parallel container");
  INFOS("[StartPaCOPPContainer] on resource : " << resource_selected);

  // Step 2 : Get a MachineFile for the parallel container
  std::string machine_file_name = _resManager->getMachineFile(resource_selected,
                                                              params.nb_proc,
                                                              params.parallelLib.in());

  if (machine_file_name == "")
  {
    INFOS("[StartPaCOPPContainer] Machine file generation failed");
    return ret;
  }

  // Step 3 : starting parallel container proxy
  std::string command_proxy("");
  std::string proxy_machine;
  try
  {
    command_proxy = BuildCommandToLaunchPaCOProxyContainer(params, machine_file_name, proxy_machine);
  }
  catch(const SALOME_Exception & ex)
  {
    INFOS("[StartPaCOPPContainer] Exception in BuildCommandToLaunchPaCOContainer");
    INFOS(ex.what());
    return ret;
  }
  obj = LaunchPaCOProxyContainer(command_proxy, params, proxy_machine);
  if (CORBA::is_nil(obj))
  {
    INFOS("[StartPaCOPPContainer] LaunchPaCOContainer for proxy returns NIL !");
    return ret;
  }
  container_proxy = PaCO::InterfaceManager::_narrow(obj);
  MESSAGE("[StartPaCOPPContainer] PaCO container proxy is launched");

  // Step 4 : starting parallel container nodes
  std::string command_nodes("");
  SALOME_ContainerManager::actual_launch_machine_t nodes_machines;
  try
  {
    command_nodes = BuildCommandToLaunchPaCONodeContainer(params, machine_file_name, nodes_machines, proxy_machine);
  }
  catch(const SALOME_Exception & ex)
  {
    INFOS("[StarPaCOPPContainer] Exception in BuildCommandToLaunchPaCONodeContainer");
    INFOS(ex.what());
    return ret;
  }

  std::string container_generic_node_name = std::string(params.container_name.in()) + std::string("Node");
  bool result = LaunchPaCONodeContainer(command_nodes, params, container_generic_node_name, nodes_machines);
  if (!result)
  {
    INFOS("[StarPaCOPPContainer] LaunchPaCONodeContainer failed !");
    // Il faut tuer le proxy
    try
    {
      Engines::Container_var proxy = Engines::Container::_narrow(container_proxy);
      proxy->Shutdown();
    }
    catch (...)
    {
      INFOS("[StarPaCOPPContainer] Exception caught from proxy Shutdown...");
    }
    return ret;
  }

  // Step 4 : connecting nodes and the proxy to actually create a parallel container
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
      INFOS("[StarPaCOPPContainer] CONNECTION FAILED From Naming Service !");
      INFOS("[StarPaCOPPContainer] Container name is " << containerNameInNS);
      return ret;
    }
    try
    {
      MESSAGE("[StarPaCOPPContainer] Deploying node : " << container_node_name);
      PaCO::InterfaceParallel_var node = PaCO::InterfaceParallel::_narrow(obj);
      node->deploy();
      MESSAGE("[StarPaCOPPContainer] node " << container_node_name << " is deployed");
    }
    catch(CORBA::SystemException& e)
    {
      INFOS("[StarPaCOPPContainer] Exception in deploying node : " << containerNameInNS);
      INFOS("CORBA::SystemException : " << e);
      return ret;
    }
    catch(CORBA::Exception& e)
    {
      INFOS("[StarPaCOPPContainer] Exception in deploying node : " << containerNameInNS);
      INFOS("CORBA::Exception" << e);
      return ret;
    }
    catch(...)
    {
      INFOS("[StarPaCOPPContainer] Exception in deploying node : " << containerNameInNS);
      INFOS("Unknown exception !");
      return ret;
    }
  }

  // Step 5 : starting parallel container
  try
  {
    MESSAGE ("[StarPaCOPPContainer] Starting parallel object");
    container_proxy->start();
    MESSAGE ("[StarPaCOPPContainer] Parallel object is started");
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
  return ret;
}

std::string
SALOME_ContainerManager::BuildCommandToLaunchPaCOProxyContainer(const Engines::ContainerParameters& params,
                                                                std::string machine_file_name,
                                                                std::string & proxy_hostname)
{
  // In the proxy case, we always launch a Dummy Proxy
  std::string exe_name = "SALOME_ParallelContainerProxyDummy";
  std::string container_name = params.container_name.in();

  // Convert nb_proc in string
  std::ostringstream tmp_string;
  tmp_string << params.nb_proc;
  std::string nb_proc_str = tmp_string.str();

  // Get resource definition
  ParserResourcesType resource_definition =
      _resManager->GetResourceDefinition(params.resource_params.name.in());

  // Choose hostname
  std::string hostname;
  std::ifstream machine_file(machine_file_name.c_str());
  std::getline(machine_file, hostname, ' ');
  size_t found = hostname.find('\n');
  if (found!=std::string::npos)
    hostname.erase(found, 1); // Remove \n
  proxy_hostname = hostname;
  MESSAGE("[BuildCommandToLaunchPaCOProxyContainer] machine file name extracted is " << hostname);

  // Remote execution
  bool remote_execution = false;
  if (hostname != std::string(Kernel_Utils::GetHostname()))
  {
    MESSAGE("[BuildCommandToLaunchPaCOProxyContainer] remote machine case detected !");
    remote_execution = true;
  }

  // Log environment
  std::string log_type("");
  char * get_val = GetenvThreadSafe("PARALLEL_LOG");
  if (get_val)
    log_type = get_val;

  // Generating the command
  std::string command_begin("");
  std::string command_end("");
  std::ostringstream command;

  LogConfiguration(log_type, "proxy", container_name, hostname, command_begin, command_end);
  command << command_begin;

  // Adding connection command
  // We can only have a remote execution with
  // a SALOME application
  if (remote_execution)
  {
    ASSERT(GetenvThreadSafe("NSHOST"));
    ASSERT(GetenvThreadSafe("NSPORT"));

    command << resource_definition.getAccessProtocolTypeStr();
    command << " -l ";
    command << resource_definition.UserName;
    command << " " << hostname;
    command << " " << resource_definition.AppliPath;
    command << "/runRemote.sh ";
    command << GetenvThreadSafeAsString("NSHOST") << " "; // hostname of CORBA name server
    command << GetenvThreadSafeAsString("NSPORT") << " "; // port of CORBA name server
  }

  command << exe_name;
  command << " " << container_name;
  command << " Dummy";
  command << " " << hostname;
  command << " " << nb_proc_str;
  command << " -";
  AddOmninamesParams(command);

  // Final command
  command << command_end;
  MESSAGE("[BuildCommandToLaunchPaCOProxyContainer] Command is: " << command.str());

  return command.str();
}

std::string
SALOME_ContainerManager::BuildCommandToLaunchPaCONodeContainer(const Engines::ContainerParameters& params,
                                                               const std::string & machine_file_name,
                                                               SALOME_ContainerManager::actual_launch_machine_t & vect_machine,
                                                               const std::string & proxy_hostname)
{
  // Name of exe
  std::string exe_name = "SALOME_ParallelContainerNode";
  exe_name += params.parallelLib.in();
  std::string container_name = params.container_name.in();

  // Convert nb_proc in string
  std::ostringstream nb_proc_stream;
  nb_proc_stream << params.nb_proc;

  // Get resource definition
  ParserResourcesType resource_definition =
      _resManager->GetResourceDefinition(params.resource_params.name.in());

  // Log environment
  std::string log_type("");
  char * get_val = GetenvThreadSafe("PARALLEL_LOG");
  if (get_val)
    log_type = get_val;

  // Now the command is different according to paralleLib
  std::ostringstream command_nodes;
  std::ifstream machine_file(machine_file_name.c_str());
  if (std::string(params.parallelLib.in()) == "Dummy")
  {
    for (int i= 0; i < params.nb_proc; i++)
    {
      // Choose hostname
      std::string hostname;
      std::getline(machine_file, hostname);
      MESSAGE("[BuildCommandToLaunchPaCONodeContainer] machine file name extracted is " << hostname);

      // Remote execution
      bool remote_execution = false;
      if (hostname != std::string(Kernel_Utils::GetHostname()))
      {
        MESSAGE("[BuildCommandToLaunchPaCONodeContainer] remote machine case detected !");
        remote_execution = true;
      }

      // For each node we have a new command
      // Generating the command
      std::ostringstream command_node_stream;
      std::string command_node_begin("");
      std::string command_node_end("");
      std::ostringstream node_number;
      node_number << i;
      std::string container_node_name = container_name + node_number.str();
      LogConfiguration(log_type, "node", container_node_name, hostname, command_node_begin, command_node_end);

      // Adding connection command
      // We can only have a remote execution with
      // a SALOME application
      if (remote_execution)
      {
        ASSERT(GetenvThreadSafe("NSHOST"));
        ASSERT(GetenvThreadSafe("NSPORT"));

        command_node_stream << resource_definition.getAccessProtocolTypeStr();
        command_node_stream << " -l ";
        command_node_stream << resource_definition.UserName;
        command_node_stream << " " << hostname;
        command_node_stream << " " << resource_definition.AppliPath;
        command_node_stream << "/runRemote.sh ";
        command_node_stream << GetenvThreadSafeAsString("NSHOST") << " "; // hostname of CORBA name server
        command_node_stream << GetenvThreadSafeAsString("NSPORT") << " "; // port of CORBA name server
      }

      command_node_stream << exe_name;
      command_node_stream << " " << container_name;
      command_node_stream << " " << params.parallelLib.in();
      command_node_stream << " " << proxy_hostname;
      command_node_stream << " " << node_number.str();
      command_node_stream << " -";
      AddOmninamesParams(command_node_stream);

      command_nodes << command_node_begin << command_node_stream.str() << command_node_end;
      vect_machine.push_back(hostname);
    }
  }

  else if (std::string(params.parallelLib.in()) == "Mpi")
  {
    // Choose hostname
    std::string hostname;
    std::getline(machine_file, hostname, ' ');
    MESSAGE("[BuildCommandToLaunchPaCONodeContainer] machine file name extracted is " << hostname);

    // Remote execution
    bool remote_execution = false;
    if (hostname != std::string(Kernel_Utils::GetHostname()))
    {
      MESSAGE("[BuildCommandToLaunchPaCONodeContainer] remote machine case detected !");
      remote_execution = true;
    }

    // In case of Mpi and Remote, we copy machine_file in the applipath
    // scp mpi_machine_file user@machine:Path
    std::ostringstream command_remote_stream;
    std::string::size_type last = machine_file_name.find_last_of("/");
    if (last == std::string::npos)
      last = -1;

    if (resource_definition.Protocol == rsh)
      command_remote_stream << "rcp ";
    else
      command_remote_stream << "scp ";
    command_remote_stream << machine_file_name << " ";
    command_remote_stream << resource_definition.UserName << "@";
    command_remote_stream << hostname << ":" << resource_definition.AppliPath;
    command_remote_stream <<  "/" << machine_file_name.substr(last+1);

    int status = SystemThreadSafe(command_remote_stream.str().c_str());
    if (status == -1)
    {
      INFOS("copy of the MPI machine file failed ! - sorry !");
      return "";
    }

    // Generating the command
    std::string command_begin("");
    std::string command_end("");

    LogConfiguration(log_type, "nodes", container_name, hostname, command_begin, command_end);
    command_nodes << command_begin;

    // Adding connection command
    // We can only have a remote execution with
    // a SALOME application
    if (remote_execution)
    {
      ASSERT(GetenvThreadSafe("NSHOST"));
      ASSERT(GetenvThreadSafe("NSPORT"));

      command_nodes << resource_definition.getAccessProtocolTypeStr();
      command_nodes << " -l ";
      command_nodes << resource_definition.UserName;
      command_nodes << " " << hostname;
      command_nodes << " " << resource_definition.AppliPath;
      command_nodes << "/runRemote.sh ";
      command_nodes << GetenvThreadSafeAsString("NSHOST") << " "; // hostname of CORBA name server
      command_nodes << GetenvThreadSafeAsString("NSPORT") << " "; // port of CORBA name server
    }

    if (resource_definition.mpi == lam)
    {
      command_nodes << "mpiexec -ssi boot ";
      command_nodes << "-machinefile "  << machine_file_name << " ";
      command_nodes <<  "-n " << params.nb_proc;
    }
    else
    {
      command_nodes << "mpirun -np " << params.nb_proc;
    }
    command_nodes << " " << exe_name;
    command_nodes << " " << container_name;
    command_nodes << " " << params.parallelLib.in();
    command_nodes << " " << proxy_hostname;
    command_nodes << " -";
    AddOmninamesParams(command_nodes);

    // We don't put hostname, because nodes are registered in the resource of the proxy
    for (int i= 0; i < params.nb_proc; i++)
      vect_machine.push_back(proxy_hostname);

    command_nodes << command_end;
  }
  return command_nodes.str();
}

void
SALOME_ContainerManager::LogConfiguration(const std::string & log_type,
                                          const std::string & exe_type,
                                          const std::string & container_name,
                                          const std::string & hostname,
                                          std::string & begin,
                                          std::string & end)
{
  if(log_type == "xterm")
  {
    begin = "xterm -e \"export LD_LIBRARY_PATH=$LD_LIBRARY_PATH; export PATH=$PATH;";
    end   = "\"&";
  }
  else if(log_type == "xterm_debug")
  {
    begin = "xterm -e \"export LD_LIBRARY_PATH=$LD_LIBRARY_PATH; export PATH=$PATH;";
    end   = "; cat \" &";
  }
  else
  {
    // default into a file...
    std::string logFilename = "/tmp/" + container_name + "_" + hostname + "_" + exe_type + "_";
    std::string user = GetenvThreadSafeAsString("USER");
    if (user.empty())
      user = GetenvThreadSafeAsString("LOGNAME");
    logFilename += user + ".log";
    end = " > " + logFilename + " 2>&1 & ";
  }
}

CORBA::Object_ptr
SALOME_ContainerManager::LaunchPaCOProxyContainer(const std::string& command,
                                                  const Engines::ContainerParameters& params,
                                                  const std::string & hostname)
{
  PaCO::InterfaceManager_ptr container_proxy = PaCO::InterfaceManager::_nil();

  MESSAGE("[LaunchPaCOProxyContainer] Launch command");
  int status = SystemThreadSafe(command.c_str());
  if (status == -1) {
    INFOS("[LaunchPaCOProxyContainer] failed : system command status -1");
    return container_proxy;
  }
  else if (status == 217) {
    INFOS("[LaunchPaCOProxyContainer] failed : system command status 217");
    return container_proxy;
  }

  int count(GetTimeOutToLoaunchServer());
  CORBA::Object_var obj = CORBA::Object::_nil();
  std::string containerNameInNS = _NS->BuildContainerNameForNS(params.container_name.in(),
                                                               hostname.c_str());
  MESSAGE("[LaunchParallelContainer]  Waiting for Parallel Container proxy : " << containerNameInNS);

  while (CORBA::is_nil(obj) && count)
  {
    sleep(1);
    count--;
    obj = _NS->Resolve(containerNameInNS.c_str());
  }

  try
  {
    container_proxy = PaCO::InterfaceManager::_narrow(obj);
  }
  catch(CORBA::SystemException& e)
  {
    INFOS("[StarPaCOPPContainer] Exception in _narrow after LaunchParallelContainer for proxy !");
    INFOS("CORBA::SystemException : " << e);
    return container_proxy;
  }
  catch(CORBA::Exception& e)
  {
    INFOS("[StarPaCOPPContainer] Exception in _narrow after LaunchParallelContainer for proxy !");
    INFOS("CORBA::Exception" << e);
    return container_proxy;
  }
  catch(...)
  {
    INFOS("[StarPaCOPPContainer] Exception in _narrow after LaunchParallelContainer for proxy !");
    INFOS("Unknown exception !");
    return container_proxy;
  }
  if (CORBA::is_nil(container_proxy))
  {
    INFOS("[StarPaCOPPContainer] PaCO::InterfaceManager::_narrow returns NIL !");
    return container_proxy;
  }
  return obj._retn();
}

//=============================================================================
/*! This method launches the parallel container.
 *  It will may be placed on the resources manager.
 *
 * \param command to launch
 * \param container's parameters
 * \param name of the container
 *
 * \return CORBA container reference
 */
//=============================================================================
bool
SALOME_ContainerManager::LaunchPaCONodeContainer(const std::string& command,
                                                 const Engines::ContainerParameters& params,
                                                 const std::string& name,
                                                 SALOME_ContainerManager::actual_launch_machine_t & vect_machine)
{
  INFOS("[LaunchPaCONodeContainer] Launch command");
  int status = SystemThreadSafe(command.c_str());
  if (status == -1) {
    INFOS("[LaunchPaCONodeContainer] failed : system command status -1");
    return false;
  }
  else if (status == 217) {
    INFOS("[LaunchPaCONodeContainer] failed : system command status 217");
    return false;
  }

  INFOS("[LaunchPaCONodeContainer] Waiting for the nodes of the parallel container");
  // We are waiting all the nodes
  for (int i = 0; i < params.nb_proc; i++)
  {
    CORBA::Object_var obj = CORBA::Object::_nil();
    std::string theMachine(vect_machine[i]);
    // Name of the node
    std::ostringstream tmp;
    tmp << i;
    std::string proc_number = tmp.str();
    std::string container_node_name = name + proc_number;
    std::string containerNameInNS = _NS->BuildContainerNameForNS((char*) container_node_name.c_str(), theMachine.c_str());
    INFOS("[LaunchPaCONodeContainer]  Waiting for Parallel Container node " << containerNameInNS << " on " << theMachine);
    int count(GetTimeOutToLoaunchServer());
    while (CORBA::is_nil(obj) && count) {
      SleepInSecond(1);
      count-- ;
      obj = _NS->Resolve(containerNameInNS.c_str());
    }
    if (CORBA::is_nil(obj))
    {
      INFOS("[LaunchPaCONodeContainer] Launch of node failed (or not found) !");
      return false;
    }
  }
  return true;
}

#else

Engines::Container_ptr
SALOME_ContainerManager::StartPaCOPPContainer(const Engines::ContainerParameters& /*params*/,
                                              std::string /*resource_selected*/)
{
  Engines::Container_ptr ret = Engines::Container::_nil();
  INFOS("[StarPaCOPPContainer] is disabled !");
  INFOS("[StarPaCOPPContainer] recompile SALOME Kernel to enable PaCO++ parallel extension");
  return ret;
}

std::string
SALOME_ContainerManager::BuildCommandToLaunchPaCOProxyContainer(const Engines::ContainerParameters& /*params*/,
                                                                std::string /*machine_file_name*/,
                                                                std::string & /*proxy_hostname*/)
{
  return "";
}

std::string
SALOME_ContainerManager::BuildCommandToLaunchPaCONodeContainer(const Engines::ContainerParameters& /*params*/,
                                                               const std::string & /*machine_file_name*/,
                                                               SALOME_ContainerManager::actual_launch_machine_t & /*vect_machine*/,
                                                               const std::string & /*proxy_hostname*/)
{
  return "";
}
void
SALOME_ContainerManager::LogConfiguration(const std::string & /*log_type*/,
                                          const std::string & /*exe_type*/,
                                          const std::string & /*container_name*/,
                                          const std::string & /*hostname*/,
                                          std::string & /*begin*/,
                                          std::string & /*end*/)
{
}

CORBA::Object_ptr
SALOME_ContainerManager::LaunchPaCOProxyContainer(const std::string& /*command*/,
                                                  const Engines::ContainerParameters& /*params*/,
                                                  const std::string& /*hostname*/)
{
  CORBA::Object_ptr ret = CORBA::Object::_nil();
  return ret;
}

bool
SALOME_ContainerManager::LaunchPaCONodeContainer(const std::string& /*command*/,
                        const Engines::ContainerParameters& /*params*/,
                        const std::string& /*name*/,
                        SALOME_ContainerManager::actual_launch_machine_t & /*vect_machine*/)
{
  return false;
}
#endif
