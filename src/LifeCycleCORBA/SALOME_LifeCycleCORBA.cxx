// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

//  SALOME LifeCycleCORBA : implementation of containers and engines life cycle both in Python and C++
//  File   : SALOME_LifeCycleCORBA.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

#include <time.h>
#ifndef WIN32
  #include <sys/time.h>
  #include <unistd.h>
#endif

#include "Basics_Utils.hxx"
#include "utilities.h"

#include <ServiceUnreachable.hxx>

#include "SALOME_LifeCycleCORBA.hxx"
#include "SALOME_ResourcesManager.hxx"
#include "SALOMESDS_DataServerManager.hxx"

#include CORBA_CLIENT_HEADER(SALOME_ModuleCatalog)
#include CORBA_CLIENT_HEADER(SALOME_Session)
#include CORBA_CLIENT_HEADER(DSC_Engines)
#include CORBA_CLIENT_HEADER(SALOME_Registry)
#include CORBA_CLIENT_HEADER(SALOMEDS)
#include CORBA_CLIENT_HEADER(SALOME_SDS)
#include CORBA_CLIENT_HEADER(Logger)
#include CORBA_CLIENT_HEADER(SALOME_Launcher)

#include "SALOME_ResourcesManager.hxx"
#include "SALOME_ContainerManager.hxx"
#include "SALOME_Component_i.hxx"
#include "SALOME_NamingService.hxx"
#include "SALOME_FileTransferCORBA.hxx"

IncompatibleComponent::IncompatibleComponent( void ):
  SALOME_Exception( "IncompatibleComponent" )
{
}

IncompatibleComponent::IncompatibleComponent(const IncompatibleComponent &ex):
  SALOME_Exception( ex )
{
}

/*! \class SALOME_LifeCycleCORBA
    \brief A class to manage life cycle of SALOME components.

*/

//=============================================================================
/*!
 *  Constructor
 */
//=============================================================================

SALOME_LifeCycleCORBA::SALOME_LifeCycleCORBA(SALOME_NamingService *ns)
{
  // be sure to have an instance of traceCollector, when used via SWIG
  // in a Python module
  int argc = 0;
  char *xargv = (char*)"";
  char **argv = &xargv;
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
  //  LocalTraceCollector *myThreadTrace = SALOMETraceCollector::instance(orb);
  _NSnew=0;
  if (!ns)
    {
      _NS = new SALOME_NamingService(orb);
      _NSnew=_NS;
    }
  else _NS = ns;
  //add try catch
  _NS->Change_Directory("/"); // mpv 250105: current directory may be not root
                              // (in SALOMEDS for an example)
  // not enough: set a current directory in naming service is not thread safe
  // if naming service instance is shared among several threads...
  // ==> allways use absolute path and dot rely on current directory!

  CORBA::Object_var obj =
    _NS->Resolve(SALOME_ContainerManager::_ContainerManagerNameInNS);
  if (CORBA::is_nil(obj))
    throw SALOME_Exception("Error: Cannot resolve ContainerManager in Naming Service");
  _ContManager=Engines::ContainerManager::_narrow(obj);

  obj = _NS->Resolve(SALOME_ResourcesManager::_ResourcesManagerNameInNS);
  if (CORBA::is_nil(obj))
    throw SALOME_Exception("Error: Cannot resolve ResourceManager in Naming Service");
  _ResManager=Engines::ResourcesManager::_narrow(obj);
}

//=============================================================================
/*!
 *  Destructor
 */
//=============================================================================

SALOME_LifeCycleCORBA::~SALOME_LifeCycleCORBA()
{
  if(_NSnew)delete _NSnew;
}

//=============================================================================
/*! \brief Find an already existing and registered component instance.
 *
 *  \param params         container parameters like type or name...
 *  \param componentName  the name of component class
 *  \param studyId        default = 0  : multistudy instance
 *  \return a CORBA reference of the component instance, or _nil if not found
 */
//=============================================================================
Engines::EngineComponent_ptr
SALOME_LifeCycleCORBA::FindComponent(const Engines::ContainerParameters& params,
                                     const char *componentName,
                                     int studyId)
{
  if (! isKnownComponentClass(componentName))
    return Engines::EngineComponent::_nil();

  Engines::ContainerParameters new_params(params);
  new_params.resource_params.componentList.length(1);
  new_params.resource_params.componentList[0] = componentName;
  new_params.resource_params.can_run_containers = true;
  Engines::ResourceList_var listOfResources;
  try
    {
      listOfResources = _ResManager->GetFittingResources(new_params.resource_params);
    }
  catch( const SALOME::SALOME_Exception& ex )
    {
      return Engines::EngineComponent::_nil();
    }

  Engines::EngineComponent_var compo = _FindComponent(new_params,
                                                componentName,
                                                studyId,
                                                listOfResources);

  return compo._retn();
}

//=============================================================================
/*! \brief Load a component instance on a container defined by its parameters
 *
 *  \param params         container parameters like type or name...
 *  \param componentName  the name of component class
 *  \param studyId        default = 0  : multistudy instance
 *  \return a CORBA reference of the component instance, or _nil if problem
 */
//=============================================================================

Engines::EngineComponent_ptr
SALOME_LifeCycleCORBA::LoadComponent(const Engines::ContainerParameters& params,
                                     const char *componentName,
                                     int studyId)
{
  // --- Check if Component Name is known in ModuleCatalog

  if (! isKnownComponentClass(componentName))
    return Engines::EngineComponent::_nil();

  Engines::ContainerParameters new_params(params);
  new_params.resource_params.componentList.length(1);
  new_params.resource_params.componentList[0] = componentName;
  new_params.resource_params.can_run_containers = true;

  Engines::ResourceList_var listOfResources;
  try
    {
      listOfResources = _ResManager->GetFittingResources(new_params.resource_params);
    }
  catch( const SALOME::SALOME_Exception& ex )
    {
      return Engines::EngineComponent::_nil();
    }
  new_params.resource_params.resList = listOfResources;

  Engines::EngineComponent_var compo = _LoadComponent(new_params,
                                                componentName,
                                                studyId);

  return compo._retn();
}

//=============================================================================
/*! \brief Find an already existing and registered component instance or load a new
 *         component instance on a container defined by its parameters.
 *
 *  \param params         container parameters like type or name...
 *  \param componentName  the name of component class
 *  \param studyId        default = 0  : multistudy instance
 *  \return a CORBA reference of the component instance, or _nil if problem
 */
//=============================================================================

Engines::EngineComponent_ptr
SALOME_LifeCycleCORBA::
FindOrLoad_Component(const Engines::ContainerParameters& params,
                     const char *componentName,
                     int studyId)
{
  // --- Check if Component Name is known in ModuleCatalog

  if (! isKnownComponentClass(componentName))
    return Engines::EngineComponent::_nil();

  Engines::ContainerParameters new_params(params);
  new_params.resource_params.componentList.length(1);
  new_params.resource_params.componentList[0] = componentName;
  new_params.resource_params.can_run_containers = true;

  Engines::ResourceList_var listOfResources;
  try
    {
      listOfResources = _ResManager->GetFittingResources(new_params.resource_params);
    }
  catch( const SALOME::SALOME_Exception& ex )
    {
      return Engines::EngineComponent::_nil();
    }

  Engines::EngineComponent_var compo = _FindComponent(new_params,
                                                componentName,
                                                studyId,
                                                listOfResources);

  if(CORBA::is_nil(compo))
  {
    new_params.resource_params.resList = listOfResources;
    compo = _LoadComponent(new_params,
                           componentName,
                           studyId);
  }

  return compo._retn();
}

//=============================================================================
/*! \brief Find an already existing and registered component instance or load a new
 *         component instance on a container defined by name
 *
 *  \param containerName  the name of container, under one of the forms
 *           - 1 aContainer (local container)
 *           - 2 machine/aContainer (container on hostname = machine)
 *  \param componentName  the name of component class
 *  \return a CORBA reference of the component instance, or _nil if problem
 */
//=============================================================================

Engines::EngineComponent_ptr
SALOME_LifeCycleCORBA::FindOrLoad_Component(const char *containerName,
                                            const char *componentName)
{
  MESSAGE("SALOME_LifeCycleCORBA::FindOrLoad_Component INTERACTIF " << containerName << " " << componentName ) ;

  // --- Check if Component Name is known in ModuleCatalog
  if (! isKnownComponentClass(componentName))
    return Engines::EngineComponent::_nil();

  // --- Check if containerName contains machine name (if yes: rg>0)
  char *stContainer=strdup(containerName);
  std::string st2Container(stContainer);
  int rg=st2Container.find("/");

  Engines::ContainerParameters params;
  preSet(params);
  if (rg<0)
  {
    // containerName doesn't contain "/" => Local container
    params.container_name = CORBA::string_dup(stContainer);
  }
  else
  {
    stContainer[rg]='\0';
    params.container_name = CORBA::string_dup(stContainer+rg+1);
    params.resource_params.hostname = CORBA::string_dup(stContainer);
  }
  params.isMPI = false;
  SCRUTE(params.container_name);
  free(stContainer);
  return FindOrLoad_Component(params, componentName);
}

//=============================================================================
/*! \brief Check if the component class is known in module catalog
 *
 *  \param componentName  the name of component class
 *  \return true if found, false otherwise
 */
//=============================================================================

bool SALOME_LifeCycleCORBA::isKnownComponentClass(const char *componentName)
{
  try
  {
    CORBA::Object_var obj = _NS->Resolve("/Kernel/ModulCatalog");
    SALOME_ModuleCatalog::ModuleCatalog_var Catalog =
      SALOME_ModuleCatalog::ModuleCatalog::_narrow(obj) ;
    ASSERT(! CORBA::is_nil(Catalog));
    SALOME_ModuleCatalog::Acomponent_var compoInfo =
      Catalog->GetComponent(componentName);
    if (CORBA::is_nil (compoInfo))
    {
      MESSAGE("Catalog Error: Component not found in the catalog " << componentName);
      return false;
    }
    else return true;
  }
  catch (ServiceUnreachable&)
  {
    INFOS("Caught exception: Naming Service Unreachable");
  }
  catch (...)
  {
    INFOS("Caught unknown exception.");
  }
  return false;
}

//=============================================================================
/*! \brief Initialisation of a given Engines::ResourceParameters with default values.
 */
//=============================================================================

void
SALOME_LifeCycleCORBA::preSet(Engines::ResourceParameters& params)
{
  params.name = "";
  params.hostname = "";
  params.OS = "";
  params.nb_proc = 0;
  params.mem_mb = 0;
  params.cpu_clock = 0;
  params.nb_node = 0;
  params.nb_proc_per_node = 0;
  params.policy = "";
  params.can_launch_batch_jobs = false;
  params.can_run_containers = false;
}

//=============================================================================
/*! \brief Initialisation of a given Engines::ContainerParameters with default values.
 */
//=============================================================================

void SALOME_LifeCycleCORBA::preSet( Engines::ContainerParameters& params)
{
  params.container_name = "";
  params.mode = "";
  params.workingdir = "";
  params.nb_proc = 0;
  params.isMPI = false;
  params.parallelLib = "";
  SALOME_LifeCycleCORBA::preSet(params.resource_params);
}

//=============================================================================
/*!
 *  \return a number of processors not 0, only for MPI containers
 */
//=============================================================================

int SALOME_LifeCycleCORBA::NbProc(const Engines::ContainerParameters& params)
{
  if( !params.isMPI )
    return 0;
  else if( params.nb_proc <= 0 )
    return 1;
  else
    return params.nb_proc;
}

//=============================================================================
/*! \brief Get the container manager
 *
 *  \return the container Manager
 */
//=============================================================================

Engines::ContainerManager_ptr SALOME_LifeCycleCORBA::getContainerManager()
{
 Engines::ContainerManager_var contManager =
   Engines::ContainerManager::_duplicate(_ContManager);
 return contManager._retn();
}

//=============================================================================
/*! \brief Get the resources manager
 *
 *  \return the container Manager
 */
//=============================================================================

Engines::ResourcesManager_ptr SALOME_LifeCycleCORBA::getResourcesManager()
{
 Engines::ResourcesManager_var resManager =
   Engines::ResourcesManager::_duplicate(_ResManager);
 return resManager._retn();
}

//=============================================================================
/*! \brief shutdown all the SALOME servers except SALOME_Session_Server and omniNames
 */
//=============================================================================

void SALOME_LifeCycleCORBA::shutdownServers()
{
  // get each Container from NamingService => shutdown it
  // (the order is inverse to the order of servers initialization)

  SALOME::Session_var session = SALOME::Session::_nil();
  CORBA::Long pid = 0;
  CORBA::Object_var objS = _NS->Resolve("/Kernel/Session");
  if (!CORBA::is_nil(objS))
  {
    session = SALOME::Session::_narrow(objS);
    if (!CORBA::is_nil(session))
    {
      pid = session->getPID();
      session->Shutdown();
    }
  }

  std::string hostname = Kernel_Utils::GetHostname();

  // 1) ConnectionManager
  try
    {
      CORBA::Object_var objCnM=_NS->Resolve("/ConnectionManager");
      Engines::ConnectionManager_var connMan=Engines::ConnectionManager::_narrow(objCnM);
      if ( !CORBA::is_nil(connMan) && ( pid != connMan->getPID() ) )
        connMan->ShutdownWithExit();
    }
  catch(const CORBA::Exception& e)
    {
       // ignore and continue
    }

  timespec ts_req;
  ts_req.tv_nsec=100000000;
  ts_req.tv_sec=0;

//Wait some time so that ConnectionManager be completely shutdown
#ifndef WIN32
  nanosleep(&ts_req,0);
#endif

  // 2) SALOMEDS
  try
    {
      CORBA::Object_var objSDS = _NS->Resolve("/myStudyManager");
      SALOMEDS::StudyManager_var studyManager = SALOMEDS::StudyManager::_narrow(objSDS) ;
      if ( !CORBA::is_nil(studyManager) && ( pid != studyManager->getPID() ) )
        studyManager->Shutdown();
    }
  catch(const CORBA::Exception& e)
    {
       // ignore and continue
    }

//Wait some time so that study be completely shutdown
#ifndef WIN32
  nanosleep(&ts_req,0);
#endif

  // 3) ModuleCatalog
  try
    {
      CORBA::Object_var objMC=_NS->Resolve("/Kernel/ModulCatalog");
      SALOME_ModuleCatalog::ModuleCatalog_var catalog = SALOME_ModuleCatalog::ModuleCatalog::_narrow(objMC);
      if ( !CORBA::is_nil(catalog) && ( pid != catalog->getPID() ) )
        catalog->shutdown();
    }
  catch(const CORBA::Exception& e)
    {
       // ignore and continue
    }

//Wait some time so that ModulCatalog be completely shutdown
#ifndef WIN32
  nanosleep(&ts_req,0);
#endif
  // 4 ) Remote ScopeServer (the DataServer is hosted by SalomeLauncher shutdown right after)
  try
    {
      CORBA::Object_var objDSM(_NS->Resolve(SALOMESDS::DataServerManager::NAME_IN_NS));
      SALOME::DataServerManager_var dsm(SALOME::DataServerManager::_narrow(objDSM));
      if ( !CORBA::is_nil(dsm) )
        dsm->shutdownScopes();
    }
  catch(const CORBA::Exception& e)
    {
       // ignore and continue
    }

  // 5) SalomeLauncher
  try
    {
      CORBA::Object_var objSL = _NS->Resolve("/SalomeLauncher");
      Engines::SalomeLauncher_var launcher = Engines::SalomeLauncher::_narrow(objSL);
      if (!CORBA::is_nil(launcher) && (pid != launcher->getPID()))
        launcher->Shutdown();
    }
  catch(const CORBA::Exception& e)
    {
       // ignore and continue
    }

//Wait some time so that launcher be completely shutdown
#ifndef WIN32
  nanosleep(&ts_req,0);
#endif

  // 6) Registry
  try
    {
      CORBA::Object_var objR = _NS->Resolve("/Registry");
      Registry::Components_var registry = Registry::Components::_narrow(objR);
      if ( !CORBA::is_nil(registry) && ( pid != registry->getPID() ) )
          registry->Shutdown();
    }
  catch(const CORBA::Exception& e)
    {
       // ignore and continue
    }

  // 7) Logger
  int argc = 0;
  char *xargv = (char*)"";
  char **argv = &xargv;
  CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

  CORBA::Object_var objLog = CORBA::Object::_nil();
  CosNaming::NamingContext_var inc;
  CORBA::Object_var theObj = CORBA::Object::_nil();
  std::string stdname = "Logger";
  CosNaming::Name name;
  name.length(1);
  name[0].id = CORBA::string_dup(stdname.c_str());
  try
  {
    if(!CORBA::is_nil(orb))
      theObj = orb->resolve_initial_references("NameService");
    if (!CORBA::is_nil(theObj))
      inc = CosNaming::NamingContext::_narrow(theObj);
  }
  catch(...)
  {
  }
  if(!CORBA::is_nil(inc))
  {
    try
    {
      objLog = inc->resolve(name);
      SALOME_Logger::Logger_var logger = SALOME_Logger::Logger::_narrow(objLog);
      if ( !CORBA::is_nil(logger) )
        logger->shutdown();
    }
    catch(...)
    {
    }
  }
}

//=============================================================================
/*! \brief shutdown  omniNames
 */
//=============================================================================

void SALOME_LifeCycleCORBA::killOmniNames()
{
  std::string portNumber (::getenv ("NSPORT") );
  std::string python_exe;

  python_exe = std::string("python");

  if ( !portNumber.empty() )
  {
    std::string cmd;

    cmd  = std::string("from salome_utils import killOmniNames; ");
    cmd += std::string("killOmniNames(") + portNumber + "); ";
    cmd  = python_exe + std::string(" -c \"") + cmd +"\"";
    MESSAGE(cmd);
    system( cmd.c_str() );

    cmd  = std::string("from killSalomeWithPort import cleanApplication; ");
    cmd += std::string("cleanApplication(") + portNumber + "); ";
    cmd  = python_exe + std::string(" -c \"") + cmd +"\"";
    MESSAGE(cmd);
    system( cmd.c_str() );
  }

  // shutdown portmanager
  if ( !portNumber.empty() )
  {
    std::string cmd;

    cmd  = std::string("from PortManager import releasePort; ");
    cmd += std::string("releasePort(") + portNumber + "); ";
    cmd  = python_exe + std::string(" -c \"") + cmd +"\"";
    MESSAGE(cmd);
    system( cmd.c_str() );
  }
}

//=============================================================================
/*! \brief Find an already existing and registered component instance.
 *
 * - build a list of machines on which an instance of the component is running,
 * - find the best machine among the list
 *
 *  \param params         machine parameters like type or name...
 *  \param componentName  the name of component class
 *  \param studyId        default = 0  : multistudy instance
 *  \param listOfMachines list of machine address
 *  \return a CORBA reference of the component instance, or _nil if not found
 */
//=============================================================================

Engines::EngineComponent_ptr
SALOME_LifeCycleCORBA::
_FindComponent(const Engines::ContainerParameters& params,
               const char *componentName,
               int studyId,
               const Engines::ResourceList& listOfResources)
{
  // --- build the list of machines on which the component is already running
  const char *containerName = params.container_name;
  int nbproc = NbProc(params);

  Engines::ResourceList_var resourcesOK = new Engines::ResourceList;

  unsigned int lghtOfresourcesOK = 0;
  resourcesOK->length(listOfResources.length());

  for(unsigned int i=0; i < listOfResources.length(); i++)
  {
    const char * currentResource = listOfResources[i];
    Engines::ResourceDefinition_var resource_definition =
        _ResManager->GetResourceDefinition(currentResource);
    CORBA::Object_var obj = _NS->ResolveComponent(resource_definition->hostname.in(),
                                                  containerName,
                                                  componentName,
                                                  nbproc);
    if (!CORBA::is_nil(obj))
      resourcesOK[lghtOfresourcesOK++] = CORBA::string_dup(currentResource);
  }

  // --- find the best machine among the list
  if(lghtOfresourcesOK != 0)
  {
    resourcesOK->length(lghtOfresourcesOK);
    CORBA::String_var bestResource = _ResManager->FindFirst(resourcesOK);
    Engines::ResourceDefinition_var resource_definition =
        _ResManager->GetResourceDefinition(bestResource);
    CORBA::Object_var obj = _NS->ResolveComponent(resource_definition->hostname.in(),
                                                  containerName,
                                                  componentName,
                                                  nbproc);
    return Engines::EngineComponent::_narrow(obj);
  }
  else
    return Engines::EngineComponent::_nil();
}

//=============================================================================
/*! \brief  Load a component instance.
 *
 *  - Finds a container in the list of machine or start one.
 *  - Try to load the component library in the container,
 *  - then create an instance of the component.
 *
 *  \param params         machine parameters like type or name...
 *  \param componentName  the name of component class
 *  \param studyId        default = 0  : multistudy instance
 *  \return a CORBA reference of the component instance, or _nil if problem
 */
//=============================================================================

Engines::EngineComponent_ptr
SALOME_LifeCycleCORBA::
_LoadComponent(const Engines::ContainerParameters& params,
              const char *componentName,
              int studyId)
{
  MESSAGE("_LoadComponent, required " << params.container_name <<
          " " << componentName << " " << NbProc(params));

  Engines::ContainerParameters local_params(params);
  local_params.mode = CORBA::string_dup("findorstart");
  Engines::Container_var cont = _ContManager->GiveContainer(local_params);
  if (CORBA::is_nil(cont)) return Engines::EngineComponent::_nil();

  char* reason;
  bool isLoadable = cont->load_component_Library(componentName,reason);
  if (!isLoadable)
    {
      //std::cerr << reason << std::endl;
      CORBA::string_free(reason);
      return Engines::EngineComponent::_nil();
    }
  CORBA::string_free(reason);

  Engines::EngineComponent_var myInstance =
    cont->create_component_instance(componentName, studyId);
  return myInstance._retn();
}

//=============================================================================
/*! \brief  Load a parallel component instance.
 *
 *  \param params         machine parameters like type or name...
 *  \param componentName  the name of component class
 *  \param studyId        default = 0  : multistudy instance
 *  \return a CORBA reference of the parallel component instance, or _nil if problem
 */
//=============================================================================
Engines::EngineComponent_ptr
SALOME_LifeCycleCORBA::Load_ParallelComponent(const Engines::ContainerParameters& params,
                                              const char *componentName,
                                              int studyId)
{
  MESSAGE("Entering LoadParallelComponent");

/*MESSAGE("Parameters : ");
  MESSAGE("Container name : " << params.container_name);
  MESSAGE("Number of component nodes : " << params.nb_component_nodes);
  MESSAGE("Component Name : " << componentName);*/

  Engines::ContainerParameters parms(params);
  parms.resource_params.componentList.length(1);
  parms.resource_params.componentList[0] = componentName;
  parms.mode = CORBA::string_dup("findorstart");

  MESSAGE("Starting Parallel Container");
  Engines::Container_var cont = _ContManager->GiveContainer(parms);
  if (CORBA::is_nil(cont)) {
    INFOS("FindOrStartParallelContainer() returns a NULL container !");
    return Engines::EngineComponent::_nil();
  }

  MESSAGE("Loading component library");
  char* reason;
  bool isLoadable = cont->load_component_Library(componentName,reason);
  if (!isLoadable) {
    INFOS(componentName <<" library is not loadable !");
    //std::cerr << reason << std::endl;
    CORBA::string_free(reason);
    return Engines::EngineComponent::_nil();
  }
  CORBA::string_free(reason);

  MESSAGE("Creating component instance");
  // @PARALLEL@ permits to identify that the component requested
  // is a parallel component.
  std::string name = std::string(componentName);
  Engines::EngineComponent_var myInstance = cont->create_component_instance(name.c_str(), studyId);
  if (CORBA::is_nil(myInstance))
    INFOS("create_component_instance returns a NULL component !");
  return myInstance._retn();
}

/*! \brief copy a file from a source host to a destination host
 * \param hostSrc the source host
 * \param fileSrc the file to copy from the source host to the destination host
 * \param hostDest the destination host
 * \param fileDest the destination file
 */
void SALOME_LifeCycleCORBA::copyFile(const char* hostSrc, const char* fileSrc, const char* hostDest, const char* fileDest)
{
  if(strcmp(hostDest,"localhost") == 0)
    {
      //if localhost use a shortcut
      SALOME_FileTransferCORBA transfer(hostSrc,fileSrc);
      transfer.getLocalFile(fileDest);
      return;
    }

  Engines::ContainerManager_var contManager = getContainerManager();

  Engines::ContainerParameters params;
  preSet(params);

  params.resource_params.hostname = hostDest;
  params.mode = CORBA::string_dup("findorstart");
  Engines::Container_var containerDest = contManager->GiveContainer(params);

  params.resource_params.hostname = hostSrc;
  Engines::Container_var containerSrc = contManager->GiveContainer(params);

  containerDest->copyFile(containerSrc,fileSrc,fileDest);
}

/*! \brief get the naming service used by the life cycle
 *
 *  \return the naming service
 */
SALOME_NamingService * SALOME_LifeCycleCORBA::namingService()
{
  return _NS;
}

/*! \brief get the orb used by the life cycle
 *
 *  \return the orb
 */
CORBA::ORB_ptr SALOME_LifeCycleCORBA::orb()
{
  return _NS->orb();
}
