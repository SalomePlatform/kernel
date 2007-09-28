//  SALOME_ParallelContainer : implementation of container and engine for ParallelKernel
//
//  Copyright (C) 2007 OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//  File   : SALOME_ParallelContainer_i.cxx
//  Author : Andr� RIBES, EDF
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA 

#include <SALOMEconfig.h>
#ifndef WNT
#else
#include <SALOME_Component.hxx>
#endif
#include "SALOME_ParallelContainer_i.hxx"

#include "SALOME_Component_i.hxx"

#include "SALOME_FileRef_i.hxx"
#include "SALOME_FileTransfer_i.hxx"
#include "SALOME_NamingService.hxx"
#include "OpUtil.hxx"

#include "OpUtil.hxx"
#include <string.h>
#include <stdio.h>
#ifndef WNT
#include <dlfcn.h>
#include <unistd.h>
#else
#include "../../adm/win32/SALOME_WNT.hxx"
#include <signal.h>
#include <process.h>
int SIGUSR1 = 1000;
#endif

#include "utilities.h"
using namespace std;

bool _Sleeping = false ;

// Containers with name FactoryServer are started via rsh in LifeCycleCORBA
// Other Containers are started via start_impl of FactoryServer

extern "C" {void ActSigIntHandler() ; }
#ifndef WNT
extern "C" {void SigIntHandler(int, siginfo_t *, void *) ; }
#else
extern "C" {void SigIntHandler( int ) ; }
#endif


map<std::string, int> Engines_Parallel_Container_i::_cntInstances_map;
map<std::string, void *> Engines_Parallel_Container_i::_library_map;
map<std::string, void *> Engines_Parallel_Container_i::_toRemove_map;
omni_mutex Engines_Parallel_Container_i::_numInstanceMutex ;

//=============================================================================
/*! 
 *  Default constructor, not for use
 */
//=============================================================================

Engines_Parallel_Container_i::Engines_Parallel_Container_i (CORBA::ORB_ptr orb, char * ior) : 
  InterfaceParallel_impl(orb,ior), Engines::Container_serv(orb,ior),
  _numInstance(0)
{
}

//=============================================================================
/*! 
 *  Construtor to use
 */
//=============================================================================

Engines_Parallel_Container_i::Engines_Parallel_Container_i (CORBA::ORB_ptr orb, char * ior,
							    PortableServer::POA_ptr poa,
							    char *containerName ,
							    int argc , char* argv[],
							    bool activAndRegist,
							    bool isServantAloneInProcess
							   ) :
  InterfaceParallel_impl(orb,ior), Engines::Container_serv(orb,ior),
  _numInstance(0),_isServantAloneInProcess(isServantAloneInProcess)
{
  _pid = (long)getpid();

  if(activAndRegist)
    ActSigIntHandler() ;

  _argc = argc ;
  _argv = argv ;

  string hostname = GetHostname();

  _orb = CORBA::ORB::_duplicate(orb) ;
  _poa = PortableServer::POA::_duplicate(poa) ;
  _NS = new SALOME_NamingService();
  _NS->init_orb( CORBA::ORB::_duplicate(_orb) );
  _containerName = _NS->BuildContainerNameForNS(containerName, hostname.c_str());

  fileTransfer_i* aFileTransfer = new fileTransfer_i();
  _fileTransfer = Engines::fileTransfer::_narrow(aFileTransfer->_this());
}

//=============================================================================
/*! 
 *  Destructor
 */
//=============================================================================

Engines_Parallel_Container_i::~Engines_Parallel_Container_i()
{
  MESSAGE("Container_i::~Container_i()");
  delete _id;
}

//=============================================================================
/*! 
 *  CORBA attribute: Container name (see constructor)
 */
//=============================================================================

char* Engines_Parallel_Container_i::name()
{
  return CORBA::string_dup(_containerName.c_str()) ;
}

//=============================================================================
/*! 
 *  CORBA method: Get the hostName of the Container (without domain extensions)
 */
//=============================================================================

char* Engines_Parallel_Container_i::getHostName()
{
  string s = GetHostname();
  MESSAGE("Engines_Parallel_Container_i::getHostName " << s);
  return CORBA::string_dup(s.c_str()) ;
}

//=============================================================================
/*! 
 *  CORBA method: Get the PID (process identification) of the Container
 */
//=============================================================================

CORBA::Long Engines_Parallel_Container_i::getPID()
{
  return (CORBA::Long)getpid();
}

//=============================================================================
/*! 
 *  CORBA method: check if servant is still alive
 */
//=============================================================================

void Engines_Parallel_Container_i::ping()
{
  MESSAGE("Engines_Parallel_Container_i::ping() pid "<< getpid());
}

//=============================================================================
/*! 
 *  CORBA method, oneway: Server shutdown. 
 *  - Container name removed from naming service,
 *  - servant deactivation,
 *  - orb shutdown if no other servants in the process 
 */
//=============================================================================

void Engines_Parallel_Container_i::Shutdown()
{
  MESSAGE("Engines_Parallel_Container_i::Shutdown()");
  _NS->Destroy_FullDirectory(_containerName.c_str());
  //_remove_ref();
  //_poa->deactivate_object(*_id);
  if(_isServantAloneInProcess)
  {
    MESSAGE("Effective Shutdown of container Begins...");
    LocalTraceBufferPool* bp1 = LocalTraceBufferPool::instance();
    bp1->deleteInstance(bp1);
    _orb->shutdown(0);
  }
}


//=============================================================================
/*! 
 *  CORBA method: load a new component class (Python or C++ implementation)
 *  \param componentName like COMPONENT
 *                          try to make a Python import of COMPONENT,
 *                          then a lib open of libCOMPONENTEngine.so
 *  \return true if dlopen successfull or already done, false otherwise
 */
//=============================================================================

bool
Engines_Parallel_Container_i::load_component_Library(const char* componentName)
{
  bool ret = false;
  string aCompName = componentName;
  // --- try dlopen C++ component

#ifndef WNT
  string impl_name = string ("lib") + aCompName + string("Engine.so");
#else
  string impl_name = aCompName + string("Engine.dll");
#endif

  SCRUTE(impl_name);

  _numInstanceMutex.lock(); // lock to be alone 
  if (_toRemove_map[impl_name]) _toRemove_map.erase(impl_name);
  if (_library_map[impl_name])
  {
    MESSAGE("Library " << impl_name << " already loaded");
    _numInstanceMutex.unlock();
    ret = true;
  }

  void* handle;
#ifndef WNT
  handle = dlopen( impl_name.c_str() , RTLD_LAZY ) ;
#else
  handle = dlopen( impl_name.c_str() , 0 ) ;
#endif
  if ( handle )
  {
    _library_map[impl_name] = handle;
    _numInstanceMutex.unlock();
    ret = true;
  }
  else
  {
    cerr << "Can't load shared library : " << impl_name << endl;
    cerr << "error dlopen: " << dlerror() << endl;
    _numInstanceMutex.unlock();
    ret = false;
  }

  // To be sure that all the nodes of the component as loaded the library
  global_paco_context_ptr->my_com->paco_barrier();

  return ret;
}

//=============================================================================
/*! 
 *  CORBA method: Creates a new servant instance of a component.
 *  The servant registers itself to naming service and Registry.
 *  \param genericRegisterName  Name of the component instance to register
 *                         in Registry & Name Service (without _inst_n suffix)
 *  \param studyId         0 for multiStudy instance, 
 *                         study Id (>0) otherwise
 *  \return a loaded component
 */
//=============================================================================

Engines::Component_ptr
Engines_Parallel_Container_i::create_component_instance(const char*genericRegisterName,
							CORBA::Long studyId)
{
  cerr << "----------------- create_component_instance node : " << getMyRank() << endl;

  if (studyId < 0)
  {
    INFOS("studyId must be > 0 for mono study instance, =0 for multiStudy");
    return Engines::Component::_nil() ;
  }

  Engines::Component_var iobject = Engines::Component::_nil() ;

  // is it a parallel component ?
  bool parallel = false;
  string aCompName = genericRegisterName;
  int par = aCompName.find("@PARALLEL@");
  if (par>0) {
    parallel = true;
    aCompName = aCompName.substr(0,par);
  }

  //--- try C++
#ifndef WNT
  string impl_name = string ("lib") + aCompName +string("Engine.so");
#else
  string impl_name = aCompName +string("Engine.dll");
#endif
  void* handle = _library_map[impl_name];

  if ( !handle )
  {
    cerr << "shared library " << impl_name <<"must be loaded before instance" << endl;;
    return Engines::Component::_nil() ;
  }
  else
  {
    if (parallel) {
      // Sequential component case
      // Component parallel proxy created on node 0
      iobject = createParallelInstance(aCompName,
				       handle,
				       studyId);

    }
    else {
      // Sequential component case
      iobject = createInstance(aCompName,
			       handle,
			       studyId);
    }

    return iobject._retn();
  }
}

//=============================================================================
/*! 
 *  CORBA method: Finds a servant instance of a component
 *  \param registeredName  Name of the component in Registry or Name Service,
 *                         without instance suffix number
 *  \param studyId         0 if instance is not associated to a study, 
 *                         >0 otherwise (== study id)
 *  \return the first instance found with same studyId
 */
//=============================================================================

Engines::Component_ptr Engines_Parallel_Container_i::find_component_instance( const char* registeredName,
									      CORBA::Long studyId)
{
  Engines::Component_var anEngine = Engines::Component::_nil();
  map<string,Engines::Component_var>::iterator itm =_listInstances_map.begin();
  while (itm != _listInstances_map.end())
  {
    string instance = (*itm).first;
    SCRUTE(instance);
    if (instance.find(registeredName) == 0)
    {
      anEngine = (*itm).second;
      if (studyId == anEngine->getStudyId())
      {
	return anEngine._retn();
      }
    }
    itm++;
  }
  return anEngine._retn();  
}

//=============================================================================
/*! 
 *  CORBA method: find or create an instance of the component (servant),
 *  load a new component class (dynamic library) if required,
 *  ---- FOR COMPATIBILITY WITH 2.2 ---- 
 *  ---- USE ONLY FOR MULTISTUDY INSTANCES ! --------
 *  The servant registers itself to naming service and Registry.
 *  \param genericRegisterName  Name of the component to register
 *                              in Registry & Name Service
 *  \param componentName       Name of the constructed library of the component
 *  \return a loaded component
 */
//=============================================================================

Engines::Component_ptr Engines_Parallel_Container_i::load_impl( const char* genericRegisterName,
								const char* componentName )
{
  string impl_name = string ("lib") + genericRegisterName +string("Engine.so");
  Engines::Component_var iobject = Engines::Component::_nil() ;
  if (load_component_Library(genericRegisterName))
    iobject = find_or_create_instance(genericRegisterName, impl_name);
  return iobject._retn();
}


//=============================================================================
/*! 
 *  CORBA method: Stops the component servant, and deletes all related objects
 *  \param component_i     Component to be removed
 */
//=============================================================================

void Engines_Parallel_Container_i::remove_impl(Engines::Component_ptr component_i)
{
  ASSERT(! CORBA::is_nil(component_i));
  string instanceName = component_i->instanceName() ;
  MESSAGE("unload component " << instanceName);
  _listInstances_map.erase(instanceName);
  component_i->destroy() ;
  _NS->Destroy_Name(instanceName.c_str());
}

//=============================================================================
/*! 
 *  CORBA method: Discharges unused libraries from the container.
 */
//=============================================================================

void Engines_Parallel_Container_i::finalize_removal()
{
  MESSAGE("finalize unload : dlclose");
  _numInstanceMutex.lock(); // lock to be alone
  // (see decInstanceCnt, load_component_Library)
  map<string, void *>::iterator ith;
  for (ith = _toRemove_map.begin(); ith != _toRemove_map.end(); ith++)
  {
    void *handle = (*ith).second;
    string impl_name= (*ith).first;
    if (handle)
    {
      SCRUTE(handle);
      SCRUTE(impl_name);
      // 	  dlclose(handle);                // SALOME unstable after ...
      // 	  _library_map.erase(impl_name);
    }
  }
  _toRemove_map.clear();
  _numInstanceMutex.unlock();
}

//=============================================================================
/*! 
 *  CORBA method: Kill the container process with exit(0).
 *  To remove :  never returns !
 */
//=============================================================================

bool Engines_Parallel_Container_i::Kill_impl()
{
  MESSAGE("Engines_Parallel_Container_i::Kill() pid "<< getpid() << " containerName "
	  << _containerName.c_str() << " machineName "
	  << GetHostname().c_str());
  INFOS("===============================================================");
  INFOS("= REMOVE calls to Kill_impl in C++ container                  =");
  INFOS("===============================================================");
  //exit( 0 ) ;
  ASSERT(0);
  return false;
}

//=============================================================================
/*! 
 *  C++ method: Finds an already existing servant instance of a component, or
 *              create an instance.
 *  ---- USE ONLY FOR MULTISTUDY INSTANCES ! --------
 *  \param genericRegisterName    Name of the component instance to register
 *                                in Registry & Name Service,
 *                                (without _inst_n suffix, like "COMPONENT")
 *  \param componentLibraryName   like "libCOMPONENTEngine.so"
 *  \return a loaded component
 * 
 *  example with names:
 *  aGenRegisterName = COMPONENT (= first argument)
 *  impl_name = libCOMPONENTEngine.so (= second argument)
 *  _containerName = /Containers/cli76ce/FactoryServer
 *  factoryName = COMPONENTEngine_factory
 *  component_registerBase = /Containers/cli76ce/FactoryServer/COMPONENT
 *
 *  instanceName = COMPONENT_inst_1
 *  component_registerName = /Containers/cli76ce/FactoryServer/COMPONENT_inst_1
 */
//=============================================================================

Engines::Component_ptr
Engines_Parallel_Container_i::find_or_create_instance(string genericRegisterName,
						      string componentLibraryName)
{
  string aGenRegisterName = genericRegisterName;
  string impl_name = componentLibraryName;
  void* handle = _library_map[impl_name];
  if ( !handle )
  {
    INFOS("shared library " << impl_name <<"must be loaded before instance");
    return Engines::Component::_nil() ;
  }
  else
  {
    // --- find a registered instance in naming service, or create

    string component_registerBase =
      _containerName + "/" + aGenRegisterName;
    Engines::Component_var iobject = Engines::Component::_nil() ;
    try
    {
      CORBA::Object_var obj =
	_NS->ResolveFirst( component_registerBase.c_str());
      if ( CORBA::is_nil( obj ) )
      {
	iobject = createInstance(genericRegisterName,
				 handle,
				 0); // force multiStudy instance here !
      }
      else
      { 
	iobject = Engines::Component::_narrow( obj ) ;
	Engines_Component_i *servant =
	  dynamic_cast<Engines_Component_i*>
	  (_poa->reference_to_servant(iobject));
	ASSERT(servant)
	  int studyId = servant->getStudyId();
	ASSERT (studyId >= 0);
	if (studyId == 0) // multiStudy instance, OK
	{
	  // No ReBind !
	  MESSAGE(component_registerBase.c_str()<<" already bound");
	}
	else // monoStudy instance: NOK
	{
	  iobject = Engines::Component::_nil();
	  INFOS("load_impl & find_component_instance methods "
		<< "NOT SUITABLE for mono study components");
	}
      }
    }
    catch (...)
    {
      INFOS( "Container_i::load_impl catched" ) ;
    }
    return iobject._retn();
  }
}

//=============================================================================
/*! 
 *  C++ method: create a servant instance of a component.
 *  \param genericRegisterName    Name of the component instance to register
 *                                in Registry & Name Service,
 *                                (without _inst_n suffix, like "COMPONENT")
 *  \param handle                 loaded library handle
 *  \param studyId                0 for multiStudy instance, 
 *                                study Id (>0) otherwise
 *  \return a loaded component
 * 
 *  example with names:
 *  aGenRegisterName = COMPONENT (= first argument)
 *  _containerName = /Containers/cli76ce/FactoryServer
 *  factoryName = COMPONENTEngine_factory
 *  component_registerBase = /Containers/cli76ce/FactoryServer/COMPONENT
 *  instanceName = COMPONENT_inst_1
 *  component_registerName = /Containers/cli76ce/FactoryServer/COMPONENT_inst_1
 */
//=============================================================================

Engines::Component_ptr
Engines_Parallel_Container_i::createInstance(string genericRegisterName,
					     void *handle,
					     int studyId)
{
  // --- find the factory

  string aGenRegisterName = genericRegisterName;
  string factory_name = aGenRegisterName + string("Engine_factory");

  SCRUTE(factory_name) ;

  typedef  PortableServer::ObjectId * (*FACTORY_FUNCTION)
    (CORBA::ORB_ptr,
     PortableServer::POA_ptr, 
     PortableServer::ObjectId *, 
     const char *, 
     const char *) ;

  FACTORY_FUNCTION Component_factory
    = (FACTORY_FUNCTION) dlsym(handle, factory_name.c_str());

  char *error ;
  if ( (error = dlerror() ) != NULL)
  {
    INFOS("Can't resolve symbol: " + factory_name);
    SCRUTE(error);
    return Engines::Component::_nil() ;
  }

  // --- create instance
  Engines::Component_var iobject = Engines::Component::_nil() ;
  try
  {
    _numInstanceMutex.lock() ; // lock on the instance number
    _numInstance++ ;
    int numInstance = _numInstance ;
    _numInstanceMutex.unlock() ;

    char aNumI[12];
    sprintf( aNumI , "%d" , numInstance ) ;
    string instanceName = aGenRegisterName + "_inst_" + aNumI ;
    string component_registerName =
      _containerName + "/" + instanceName;

    // --- Instanciate required CORBA object

    PortableServer::ObjectId *id ; //not owner, do not delete (nore use var)
    id = (Component_factory) ( _orb, _poa, _id, instanceName.c_str(),
			       aGenRegisterName.c_str() ) ;

    // --- get reference & servant from id
    CORBA::Object_var obj = _poa->id_to_reference(*id);
    iobject = Engines::Component::_narrow(obj) ;

    Engines_Component_i *servant = 
      dynamic_cast<Engines_Component_i*>(_poa->reference_to_servant(iobject));
    ASSERT(servant);
    servant->_remove_ref(); // compensate previous id_to_reference 
    _listInstances_map[instanceName] = iobject;
    _cntInstances_map[aGenRegisterName] += 1;
    bool ret_studyId = servant->setStudyId(studyId);
    ASSERT(ret_studyId);

    // --- register the engine under the name
    //     containerName(.dir)/instanceName(.object)
    _NS->Register(iobject , component_registerName.c_str()) ;
    MESSAGE( component_registerName.c_str() << " bound" ) ;
  }
  catch (...)
  {
    INFOS( "Container_i::createInstance exception catched" ) ;
  }
  return iobject._retn();
}

Engines::Component_ptr
Engines_Parallel_Container_i::createParallelInstance(string genericRegisterName,
						     void *handle,
						     int studyId)
{
  cerr << "----------------- createParallelInstance node : " << getMyRank() << endl;
  // --- create instance
  Engines::Component_var iobject = Engines::Component::_nil();
  string aGenRegisterName = genericRegisterName;

  //////////////////////////////////////////////////////////////////////////
  // 1: Proxy Step
  // Node 0 create the proxy
  if (getMyRank() == 0) {
    // --- find the factory
    string factory_name = aGenRegisterName + string("EngineProxy_factory");

    typedef  PortableServer::ObjectId * (*FACTORY_FUNCTION)
      (CORBA::ORB_ptr,
       PortableServer::POA_ptr,
       PortableServer::ObjectId *, 
       const char *,
       int) ;

    FACTORY_FUNCTION Component_factory
      = (FACTORY_FUNCTION) dlsym(handle, factory_name.c_str());

    char *error ;
    if ( (error = dlerror() ) != NULL) {
      INFOS("Can't resolve symbol: " + factory_name);
      SCRUTE(error);
      return Engines::Component::_nil();
    }
    try {
      _numInstanceMutex.lock() ; // lock on the instance number
      _numInstance++ ;
      int numInstance = _numInstance ;
      _numInstanceMutex.unlock() ;

      char aNumI[12];
      sprintf( aNumI , "%d" , numInstance ) ;
      string instanceName = aGenRegisterName + "_inst_" + aNumI ;
      string component_registerName =
	_containerName + "/" + instanceName;

      // --- Instanciate required CORBA object
      PortableServer::ObjectId *id ; //not owner, do not delete (nore use var)
      id = (Component_factory) ( _orb, _poa, _id, instanceName.c_str(), getTotalNode()) ;

      // --- get reference & servant from id
      CORBA::Object_var obj = _poa->id_to_reference(*id);
      iobject = Engines::Component::_narrow(obj) ;

      _listInstances_map[instanceName] = iobject;
      _cntInstances_map[aGenRegisterName] += 1;

      // --- register the engine under the name
      //     containerName(.dir)/instanceName(.object)
      _NS->Register(iobject , component_registerName.c_str()) ;
      MESSAGE( component_registerName.c_str() << " bound" ) ;
    }
    catch (...)
    {
      INFOS( "Container_i::createParallelInstance exception catched in Proxy creation" ) ;
    }
  }
  else {
    // We have to have the same numIntance to be able to get the proxy reference 
    // in the nameing service.
    _numInstanceMutex.lock() ; // lock on the instance number
    _numInstance++ ;
//    int numInstance = _numInstance ;
    _numInstanceMutex.unlock() ;
  }
  cerr << "Node " << getMyRank() << " entering in paco_barrier()" << endl;
  global_paco_context_ptr->my_com->paco_barrier();
  cerr << "Node " << getMyRank() << " quitting paco_barrier()" << endl;

  //////////////////////////////////////////////////////////////////////////
  // 2: Nodes Step

  char * proxy_ior;
  Engines::Component_PaCO_var iobject2;

  char aNumI[12];
  sprintf( aNumI , "%d" , _numInstance ) ;
  string instanceName = aGenRegisterName + "_inst_" + aNumI ;

  string component_registerName = _containerName + "/" + instanceName;
  string hostname = GetHostname();

  CORBA::Object_var temp = _NS->Resolve(component_registerName.c_str());
  Engines::Component_var obj_proxy = Engines::Component::_narrow(temp);
  proxy_ior = _orb->object_to_string(obj_proxy);

  // --- find the factory
  string factory_name = aGenRegisterName + string("Engine_factory");

  typedef  PortableServer::ObjectId * (*FACTORY_FUNCTION)
    (CORBA::ORB_ptr, char * ior,
     PortableServer::POA_ptr, 
     PortableServer::ObjectId *, 
     const char *, 
     const char *) ;

  FACTORY_FUNCTION Component_factory
    = (FACTORY_FUNCTION) dlsym(handle, factory_name.c_str());

  char *error ;
  if ( (error = dlerror() ) != NULL)
  {
    INFOS("Can't resolve symbol: " + factory_name);
    SCRUTE(error);
    return Engines::Component::_nil() ;
  }
  try
  {
    char aNumI2[12];
    sprintf(aNumI2 , "%d" , getMyRank()) ;
    string instanceName = aGenRegisterName + "_inst_node_" + aNumI2;
    string component_registerName = _containerName + aNumI2 + "/" + instanceName;

    // --- Instanciate required CORBA object

    PortableServer::ObjectId *id ; //not owner, do not delete (nore use var)
    id = (Component_factory) ( _orb, proxy_ior, _poa, _id, instanceName.c_str(),
			       aGenRegisterName.c_str() ) ;

    // --- get reference & servant from id
    CORBA::Object_var obj = _poa->id_to_reference(*id);
    iobject2 = Engines::Component_PaCO::_narrow(obj) ;

    // --- register the engine under the name
    _NS->Register(iobject2 , component_registerName.c_str()) ;
    MESSAGE( component_registerName.c_str() << " bound" ) ;
  }
  catch (...)
  {
    INFOS( "Container_i::createParallelInstance exception catched" ) ;
  }

  //////////////////////////////////////////////////////////////////////////
  // 3: Deployment Step

  iobject2->deploy(getMyRank());
  global_paco_context_ptr->my_com->paco_barrier();
  cerr << "--------- createParallelInstance : End Deploy step ----------" << endl;  

  //  return obj_proxy._retn();
  return iobject._retn();
}

//=============================================================================
/*! 
 *
 */
//=============================================================================

void Engines_Parallel_Container_i::decInstanceCnt(string genericRegisterName)
{
  string aGenRegisterName =genericRegisterName;
  MESSAGE("Engines_Parallel_Container_i::decInstanceCnt " << aGenRegisterName);
  ASSERT(_cntInstances_map[aGenRegisterName] > 0); 
  _numInstanceMutex.lock(); // lock to be alone
  // (see finalize_removal, load_component_Library)
  _cntInstances_map[aGenRegisterName] -= 1;
  SCRUTE(_cntInstances_map[aGenRegisterName]);
  if (_cntInstances_map[aGenRegisterName] == 0)
  {
    string impl_name =
      Engines_Component_i::GetDynLibraryName(aGenRegisterName.c_str());
    SCRUTE(impl_name);
    void* handle = _library_map[impl_name];
    ASSERT(handle);
    _toRemove_map[impl_name] = handle;
  }
  _numInstanceMutex.unlock();
}

//=============================================================================
/*! 
 *  Retrieves only with container naming convention if it is a python container
 */
//=============================================================================

bool Engines_Parallel_Container_i::isPythonContainer(const char* ContainerName)
{
  bool ret=false;
  int len=strlen(ContainerName);
  if(len>=2)
    if(strcmp(ContainerName+len-2,"Py")==0)
      ret=true;
  return ret;
}

//=============================================================================
/*! 
 *  
 */
//=============================================================================

void ActSigIntHandler()
{
#ifndef WNT
  struct sigaction SigIntAct ;
  SigIntAct.sa_sigaction = &SigIntHandler ;
  SigIntAct.sa_flags = SA_SIGINFO ;
#endif

  // DEBUG 03.02.2005 : the first parameter of sigaction is not a mask of signals
  // (SIGINT | SIGUSR1) :
  // it must be only one signal ===> one call for SIGINT 
  // and an other one for SIGUSR1
#ifndef WNT
  if ( sigaction( SIGINT , &SigIntAct, NULL ) ) {
    perror("SALOME_Container main ") ;
    exit(0) ;
  }
  if ( sigaction( SIGUSR1 , &SigIntAct, NULL ) ) {
    perror("SALOME_Container main ") ;
    exit(0) ;
  }
  //PAL9042 JR : during the execution of a Signal Handler (and of methods called through Signal Handlers)
  //             use of streams (and so on) should never be used because :
  //             streams of C++ are naturally thread-safe and use pthread_mutex_lock ===>
  //             A stream operation may be interrupted by a signal and if the Handler use stream we
  //             may have a "Dead-Lock" ===HangUp
  //==INFOS is commented
  //  INFOS(pthread_self() << "SigIntHandler activated") ;
#else  
  signal( SIGINT, SigIntHandler );
  signal( SIGUSR1, SigIntHandler );
#endif

}

void SetCpuUsed() ;

#ifndef WNT
void SigIntHandler(int what , siginfo_t * siginfo ,
		   void * toto ) {
  //PAL9042 JR : during the execution of a Signal Handler (and of methods called through Signal Handlers)
  //             use of streams (and so on) should never be used because :
  //             streams of C++ are naturally thread-safe and use pthread_mutex_lock ===>
  //             A stream operation may be interrupted by a signal and if the Handler use stream we
  //             may have a "Dead-Lock" ===HangUp
  //==MESSAGE is commented
  //  MESSAGE(pthread_self() << "SigIntHandler what     " << what << endl
  //          << "              si_signo " << siginfo->si_signo << endl
  //          << "              si_code  " << siginfo->si_code << endl
  //          << "              si_pid   " << siginfo->si_pid) ;
  if ( _Sleeping ) {
    _Sleeping = false ;
    //     MESSAGE("SigIntHandler END sleeping.") ;
    return ;
  }
  else {
    ActSigIntHandler() ;
    if ( siginfo->si_signo == SIGUSR1 ) {
      SetCpuUsed() ;
    }
    else {
      _Sleeping = true ;
      //      MESSAGE("SigIntHandler BEGIN sleeping.") ;
      int count = 0 ;
      while( _Sleeping ) {
	sleep( 1 ) ;
	count += 1 ;
      }
      //      MESSAGE("SigIntHandler LEAVE sleeping after " << count << " s.") ;
    }
    return ;
  }
}
#else // Case WNT
void SigIntHandler( int what ) {
  MESSAGE( pthread_self() << "SigIntHandler what     " << what << endl );
  if ( _Sleeping ) {
    _Sleeping = false ;
    MESSAGE("SigIntHandler END sleeping.") ;
    return ;
  }
  else {
    ActSigIntHandler() ;
    if ( what == SIGUSR1 ) {
      SetCpuUsed() ;
    }
    else {
      _Sleeping = true ;
      MESSAGE("SigIntHandler BEGIN sleeping.") ;
      int count = 0 ;
      while( _Sleeping ) {
	Sleep( 1000 ) ;
	count += 1 ;
      }
      MESSAGE("SigIntHandler LEAVE sleeping after " << count << " s.") ;
    }
    return ;
  }
}
#endif

//=============================================================================
/*! 
 *  CORBA method: get or create a fileRef object associated to a local file
 *  (a file on the computer on which runs the container server), which stores
 *  a list of (machine, localFileName) corresponding to copies already done.
 * 
 *  \param  origFileName absolute path for a local file to copy on other
 *          computers
 *  \return a fileRef object associated to the file.
 */
//=============================================================================

Engines::fileRef_ptr
Engines_Parallel_Container_i::createFileRef(const char* origFileName)
{
  string origName(origFileName);
  Engines::fileRef_var theFileRef = Engines::fileRef::_nil();

  if (origName[0] != '/')
    {
      INFOS("path of file to copy must be an absolute path begining with '/'");
      return Engines::fileRef::_nil();
    }

  if (CORBA::is_nil(_fileRef_map[origName]))
    {
      CORBA::Object_var obj=_poa->id_to_reference(*_id);
      Engines::Container_var pCont = Engines::Container::_narrow(obj);
      fileRef_i* aFileRef = new fileRef_i(pCont, origFileName);
      theFileRef = Engines::fileRef::_narrow(aFileRef->_this());
      _numInstanceMutex.lock() ; // lock to be alone (stl container write)
      _fileRef_map[origName] = theFileRef;
      _numInstanceMutex.unlock() ;
    }
  
  theFileRef =  Engines::fileRef::_duplicate(_fileRef_map[origName]);
  ASSERT(! CORBA::is_nil(theFileRef));
  return theFileRef._retn();
}

//=============================================================================
/*! 
 *  CORBA method:
 *  \return a reference to the fileTransfer object
 */
//=============================================================================

Engines::fileTransfer_ptr
Engines_Parallel_Container_i::getFileTransfer()
{
  Engines::fileTransfer_var aFileTransfer
    = Engines::fileTransfer::_duplicate(_fileTransfer);
  return aFileTransfer._retn();
}


Engines::Salome_file_ptr 
Engines_Parallel_Container_i::createSalome_file(const char* origFileName) 
{
  string origName(origFileName);
  if (CORBA::is_nil(_Salome_file_map[origName]))
    {
      Salome_file_i* aSalome_file = new Salome_file_i();
      try 
      {
	aSalome_file->setLocalFile(origFileName);
	aSalome_file->recvFiles();
      }
      catch (const SALOME::SALOME_Exception& e)
      {
	return Engines::Salome_file::_nil();
      }

      Engines::Salome_file_var theSalome_file = Engines::Salome_file::_nil();
      theSalome_file = Engines::Salome_file::_narrow(aSalome_file->_this());
      _numInstanceMutex.lock() ; // lock to be alone (stl container write)
      _Salome_file_map[origName] = theSalome_file;
      _numInstanceMutex.unlock() ;
    }
  
  Engines::Salome_file_ptr theSalome_file =  
    Engines::Salome_file::_duplicate(_Salome_file_map[origName]);
  ASSERT(!CORBA::is_nil(theSalome_file));
  return theSalome_file;
}

