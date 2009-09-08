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
//  SALOME MPIContainer : implemenation of container based on MPI libraries
//  File   : MPIContainer_i.cxx
//  Module : SALOME
//
#include <iostream>
#include <dlfcn.h>
#include <stdio.h>
#include "Basics_Utils.hxx"
#include "SALOME_Component_i.hxx"
#include "MPIContainer_i.hxx"
#include "SALOME_NamingService.hxx"
#include "Utils_SINGLETON.hxx"
#include "OpUtil.hxx"
#include "utilities.h"
#include <time.h>
#include <sys/time.h>
#include <pthread.h>  // must be before Python.h !
#include <Python.h>
#include "Container_init_python.hxx"
using namespace std;

// L'appel au registry SALOME ne se fait que pour le process 0
Engines_MPIContainer_i::Engines_MPIContainer_i(int nbproc, int numproc,
					       CORBA::ORB_ptr orb, 
					       PortableServer::POA_ptr poa,
					       char * containerName,
					       int argc, char *argv[]) 
  : Engines_Container_i(orb,poa,containerName,argc,argv,false), MPIObject_i(nbproc,numproc)
{

  _id = _poa->activate_object(this);
  CORBA::Object_var obj=_poa->id_to_reference(*_id);
  Engines::Container_var pCont = Engines::Container::_narrow(obj);
  _remove_ref();

  if(numproc==0){

    _NS = new SALOME_NamingService();
    _NS->init_orb( CORBA::ORB::_duplicate(_orb) ) ;

    string hostname = Kernel_Utils::GetHostname();
    _containerName = _NS->BuildContainerNameForNS(containerName,hostname.c_str());
    SCRUTE(_containerName);
    _NS->Register(pCont, _containerName.c_str());

  }

  // Root recupere les ior des container des autre process
  Engines::MPIObject_var pobj = POA_Engines::MPIContainer::_this();
  BCastIOR(_orb,pobj,true);
}

Engines_MPIContainer_i::Engines_MPIContainer_i(int nbproc, int numproc) 
  : Engines_Container_i(), MPIObject_i(nbproc,numproc)
{
}

Engines_MPIContainer_i::~Engines_MPIContainer_i(void)
{
  MESSAGE("[" << _numproc << "] Engines_MPIContainer_i::~Engines_MPIContainer_i()");
}

// Load component
void Engines_MPIContainer_i::Shutdown()
{
  int ip;
  MESSAGE("[" << _numproc << "] shutdown of MPI Corba Server");
  if( _numproc == 0 ){
    _NS->Destroy_FullDirectory(_containerName.c_str());
    for(ip= 1;ip<_nbproc;ip++)
      (Engines::MPIContainer::_narrow((*_tior)[ip]))->Shutdown();
  }

  std::map<std::string, Engines::Component_var>::iterator itm;
  for (itm = _listInstances_map.begin(); itm != _listInstances_map.end(); itm++)
    {
      try
        {
          itm->second->destroy();
        }
      catch(const CORBA::Exception& e)
        {
          // ignore this entry and continue
        }
      catch(...)
        {
          // ignore this entry and continue
        }
    }

  _orb->shutdown(0);

}

// Load a component library
bool Engines_MPIContainer_i::load_component_Library(const char* componentName)
{
  pthread_t *th;
  if(_numproc == 0){
    th = new pthread_t[_nbproc];
    for(int ip=1;ip<_nbproc;ip++){
      thread_st *st = new thread_st;
      st->ip = ip;
      st->tior = _tior;
      st->compoName = componentName;
      pthread_create(&(th[ip]),NULL,th_loadcomponentlibrary,(void*)st);
    }
  }

  bool ret = Lload_component_Library(componentName);

  if(_numproc == 0){
    for(int ip=1;ip<_nbproc;ip++)
      pthread_join(th[ip],NULL);
    delete th;
  }
  return ret;
}

bool Engines_MPIContainer_i::Lload_component_Library(const char* componentName)
{
  string aCompName = componentName;

  // --- try dlopen C++ component

  string impl_name = string ("lib") + aCompName + string("Engine.so");
  
  _numInstanceMutex.lock(); // lock to be alone 
  // (see decInstanceCnt, finalize_removal))
  if (_toRemove_map[impl_name]) _toRemove_map.erase(impl_name);
  if (_library_map[impl_name])
    {
      MESSAGE("[" << _numproc << "] Library " << impl_name << " already loaded");
      _numInstanceMutex.unlock();
      return true;
    }
  
  void* handle;
  handle = dlopen( impl_name.c_str() , RTLD_LAZY ) ;
  if ( handle )
    {
      _library_map[impl_name] = handle;
      _numInstanceMutex.unlock();
      MESSAGE("[" << _numproc << "] Library " << impl_name << " loaded");
      return true;
    }
  else
    {
      MESSAGE("[" << _numproc << "] Can't load shared library : " << impl_name);
      MESSAGE("[" << _numproc << "] error dlopen: " << dlerror());
    }
  _numInstanceMutex.unlock();

  // --- try import Python component

  INFOS("[" << _numproc << "] try import Python component "<<componentName);
  if (_isSupervContainer)
    {
      INFOS("[" << _numproc << "] Supervision Container does not support Python Component Engines");
      return false;
    }
  if (_library_map[aCompName])
    {
      return true; // Python Component, already imported
    }
  else
    {
      Py_ACQUIRE_NEW_THREAD;
      PyObject *mainmod = PyImport_AddModule((char *)"__main__");
      PyObject *globals = PyModule_GetDict(mainmod);
      PyObject *pyCont = PyDict_GetItemString(globals, "pyCont");
      PyObject *result = PyObject_CallMethod(pyCont,
					     (char*)"import_component",
					     (char*)"s",componentName);
      std::string ret= PyString_AsString(result);
      SCRUTE(ret);
      Py_RELEASE_NEW_THREAD;
  
      if (ret=="") // import possible: Python component
	{
	  _library_map[aCompName] = (void *)pyCont; // any non O value OK
	  MESSAGE("[" << _numproc << "] import Python: "<<aCompName<<" OK");
	  return true;
	}
    }
  return false;
}

// Create an instance of component
Engines::Component_ptr
Engines_MPIContainer_i::create_component_instance( const char* componentName,
						   CORBA::Long studyId)
{
  pthread_t *th;
  if(_numproc == 0){
    th = new pthread_t[_nbproc];
    for(int ip=1;ip<_nbproc;ip++){
      thread_st *st = new thread_st;
      st->ip = ip;
      st->tior = _tior;
      st->compoName = componentName;
      st->studyId = studyId;
      pthread_create(&(th[ip]),NULL,th_createcomponentinstance,(void*)st);
    }
  }

  Engines::Component_ptr cptr = Lcreate_component_instance(componentName,studyId);

  if(_numproc == 0){
    for(int ip=1;ip<_nbproc;ip++)
      pthread_join(th[ip],NULL);
    delete th;
  }

  return cptr;
}

Engines::Component_ptr
Engines_MPIContainer_i::Lcreate_component_instance( const char* genericRegisterName, CORBA::Long studyId)
{
  if (studyId < 0) {
    INFOS("studyId must be > 0 for mono study instance, =0 for multiStudy");
    return Engines::Component::_nil() ;
  }

  Engines::Component_var iobject = Engines::Component::_nil() ;
  Engines::MPIObject_var pobj;

  string aCompName = genericRegisterName;
  if (_library_map[aCompName]) { // Python component
    if (_isSupervContainer) {
      INFOS("Supervision Container does not support Python Component Engines");
      return Engines::Component::_nil();
    }
    _numInstanceMutex.lock() ; // lock on the instance number
    _numInstance++ ;
    int numInstance = _numInstance ;
    _numInstanceMutex.unlock() ;

    char aNumI[12];
    sprintf( aNumI , "%d" , numInstance ) ;
    string instanceName = aCompName + "_inst_" + aNumI ;
    string component_registerName =
      _containerName + "/" + instanceName;

    Py_ACQUIRE_NEW_THREAD;
    PyObject *mainmod = PyImport_AddModule((char*)"__main__");
    PyObject *globals = PyModule_GetDict(mainmod);
    PyObject *pyCont = PyDict_GetItemString(globals, "pyCont");
    PyObject *result = PyObject_CallMethod(pyCont,
					   (char*)"create_component_instance",
					   (char*)"ssl",
					   aCompName.c_str(),
					   instanceName.c_str(),
					   studyId);
    string iors = PyString_AsString(result);
    SCRUTE(iors);
    Py_RELEASE_NEW_THREAD;
  
    CORBA::Object_var obj = _orb->string_to_object(iors.c_str());
    iobject = Engines::Component::_narrow( obj ) ;
    pobj = Engines::MPIObject::_narrow(obj) ;
    if( _numproc == 0 )
      _NS->Register(iobject, component_registerName.c_str()) ;
    // Root recupere les ior des composants des autre process
    BCastIOR(_orb,pobj,false);

    return iobject._retn();
  }
  
  //--- try C++

  string impl_name = string ("lib") + genericRegisterName +string("Engine.so");
  if (_library_map.count(impl_name) != 0) // C++ component
    {
      void* handle = _library_map[impl_name];
      iobject = createMPIInstance(genericRegisterName,
				    handle,
				    studyId);
      return iobject._retn();
    }

  return Engines::Component::_nil() ;
}

Engines::Component_ptr
Engines_MPIContainer_i::createMPIInstance(string genericRegisterName,
					  void *handle,
					  int studyId)
{
  Engines::Component_var iobject;
  Engines::MPIObject_var pobj;
  // --- find the factory

  string aGenRegisterName = genericRegisterName;
  string factory_name = aGenRegisterName + string("Engine_factory");

  typedef  PortableServer::ObjectId * (*MPIFACTORY_FUNCTION)
    (int,int,
     CORBA::ORB_ptr,
     PortableServer::POA_ptr, 
     PortableServer::ObjectId *, 
     const char *, 
     const char *) ;

  dlerror();
  MPIFACTORY_FUNCTION MPIComponent_factory = (MPIFACTORY_FUNCTION) dlsym(handle, factory_name.c_str());

  if ( !MPIComponent_factory )
    {
      INFOS( "[" << _numproc << "] Can't resolve symbol: " + factory_name );
      SCRUTE( dlerror() );
      pobj = Engines::MPIObject::_nil();
      BCastIOR(_orb,pobj,false);
      return Engines::Component::_nil();
    }

  // --- create instance

  iobject = Engines::Component::_nil() ;

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
      id = (MPIComponent_factory) ( _nbproc,_numproc,_orb, _poa, _id, instanceName.c_str(),
				 aGenRegisterName.c_str() ) ;

      // --- get reference & servant from id

      CORBA::Object_var obj = _poa->id_to_reference(*id);
      iobject = Engines::Component::_narrow( obj ) ;
      pobj = Engines::MPIObject::_narrow(obj) ;

      Engines_Component_i *servant =
	dynamic_cast<Engines_Component_i*>(_poa->reference_to_servant(iobject));
      ASSERT(servant);
      //SCRUTE(servant->pd_refCount);
      servant->_remove_ref(); // compensate previous id_to_reference 
      //SCRUTE(servant->pd_refCount);
      _listInstances_map[instanceName] = iobject;
      _cntInstances_map[aGenRegisterName] += 1;
      //SCRUTE(servant->pd_refCount);
      bool ret_studyId = servant->setStudyId(studyId);
      ASSERT(ret_studyId);

      // --- register the engine under the name
      //     containerName(.dir)/instanceName(.object)

      if( _numproc == 0 ){
	_NS->Register( iobject , component_registerName.c_str() ) ;
	MESSAGE( component_registerName.c_str() << " bound" ) ;
      }
      // Root recupere les ior des composants des autre process
      BCastIOR(_orb,pobj,false);

    }
  catch(const POException &ex){
    INFOS( ex.msg << " on process number " << ex.numproc ) ;
    return Engines::Component::_nil();
  }
  catch (...){
    INFOS( "Container_i::createInstance exception catched" ) ;
    return Engines::Component::_nil();
  }
  return iobject._retn();
}

// Load component
Engines::Component_ptr Engines_MPIContainer_i::load_impl(const char* nameToRegister,
						 const char* componentName)
{
  pthread_t *th;
  if(_numproc == 0){
    th = new pthread_t[_nbproc];
    for(int ip=1;ip<_nbproc;ip++){
      thread_st *st = new thread_st;
      st->ip = ip;
      st->tior = _tior;
      st->nameToRegister = nameToRegister;
      st->compoName = componentName;
      pthread_create(&(th[ip]),NULL,th_loadimpl,(void*)st);
    }
  }

  Engines::Component_ptr cptr =  Lload_impl(nameToRegister,componentName);

  if(_numproc == 0){
    for(int ip=1;ip<_nbproc;ip++)
      pthread_join(th[ip],NULL);
    delete th;
  }

  return cptr;
}

// Load component
Engines::Component_ptr Engines_MPIContainer_i::Lload_impl(
				   const char* nameToRegister,
				   const char* componentName)
{
  Engines::Component_var iobject;
  Engines::MPIObject_var pobj;
  char cproc[4];

  sprintf(cproc,"_%d",_numproc);

  BEGIN_OF("[" << _numproc << "] MPIContainer_i::Lload_impl");

  _numInstanceMutex.lock() ; // lock on the instance number
  _numInstance++ ;
  char _aNumI[12];
  sprintf(_aNumI,"%d",_numInstance) ;

  string _impl_name = componentName;
  string _nameToRegister = nameToRegister;
  string instanceName = _nameToRegister + "_inst_" + _aNumI + cproc;
  MESSAGE("[" << _numproc << "] instanceName=" << instanceName);

  string absolute_impl_name(_impl_name);
  MESSAGE("[" << _numproc << "] absolute_impl_name=" << absolute_impl_name);
  void * handle = dlopen(absolute_impl_name.c_str(), RTLD_LAZY);
  if(!handle){
    INFOS("[" << _numproc << "] Can't load shared library : " << absolute_impl_name);
    INFOS("[" << _numproc << "] error dlopen: " << dlerror());
    return Engines::Component::_nil() ;
  }

  string factory_name = _nameToRegister + string("Engine_factory");
  MESSAGE("[" << _numproc << "] factory_name=" << factory_name) ;

  dlerror();
  PortableServer::ObjectId * (*MPIComponent_factory) (int,int,
						  CORBA::ORB_ptr,
						  PortableServer::POA_ptr,
						  PortableServer::ObjectId *,
						  const char *,
						  const char *) =
    (PortableServer::ObjectId * (*) (int,int,
				     CORBA::ORB_ptr,
				     PortableServer::POA_ptr, 
				     PortableServer::ObjectId *, 
				     const char *, 
				     const char *)) 
    dlsym(handle, factory_name.c_str());

  char *error ;
  if ((error = dlerror()) != NULL){
    // Try to load a sequential component
    MESSAGE("[" << _numproc << "] Try to load a sequential component");
    _numInstanceMutex.unlock() ;
    iobject = Engines_Container_i::load_impl(nameToRegister,componentName);
    if( CORBA::is_nil(iobject) ) return Engines::Component::_duplicate(iobject);
  }
  else{
    // Instanciation du composant parallele
    MESSAGE("[" << _numproc << "] Try to load a parallel component");
    PortableServer::ObjectId * id = (MPIComponent_factory)
      (_nbproc,_numproc,_orb, _poa, _id, instanceName.c_str(), _nameToRegister.c_str());
    // get reference from id
    CORBA::Object_var o = _poa->id_to_reference(*id);
    pobj = Engines::MPIObject::_narrow(o) ;
    iobject = Engines::Component::_narrow(o) ;
  }

  if( _numproc == 0 ){
    // utiliser + tard le registry ici :
    // register the engine under the name containerName.dir/nameToRegister.object
    string component_registerName = _containerName + "/" + _nameToRegister;
    _NS->Register(iobject, component_registerName.c_str()) ;
  }

  _numInstanceMutex.unlock() ;

  // Root recupere les ior des composants des autre process
  BCastIOR(_orb,pobj,false);

  END_OF("[" <<_numproc << "] MPIContainer_i::Lload_impl");
  return Engines::Component::_duplicate(iobject);

}

void Engines_MPIContainer_i::remove_impl(Engines::Component_ptr component_i)
{
  Engines::MPIObject_ptr pcptr;
  Engines::MPIObject_ptr spcptr;

  pthread_t *th;
  if(_numproc == 0){
    pcptr = (Engines::MPIObject_ptr)component_i;
    th = new pthread_t[_nbproc];
    for(int ip=1;ip<_nbproc;ip++){
      thread_st *st = new thread_st;
      st->ip = ip;
      st->tior = _tior;
      spcptr = Engines::MPIObject::_narrow((*(pcptr->tior()))[ip]);
      st->cptr = (Engines::Component_ptr)spcptr;
      pthread_create(&(th[ip]),NULL,th_removeimpl,(void*)st);
    }
  }

  ASSERT(! CORBA::is_nil(component_i));
  string instanceName = component_i->instanceName() ;
  MESSAGE("[" << _numproc << "] unload component " << instanceName);
  _numInstanceMutex.lock() ; // lock on the remove on handle_map
  _listInstances_map.erase(instanceName);
  _numInstanceMutex.unlock() ;
  component_i->destroy() ;
  if(_numproc == 0)
    _NS->Destroy_Name(instanceName.c_str());

  if(_numproc == 0){
    for(int ip=1;ip<_nbproc;ip++)
      pthread_join(th[ip],NULL);
    delete th;
  }

}

void Engines_MPIContainer_i::finalize_removal()
{
  pthread_t *th;
  if(_numproc == 0){
    th = new pthread_t[_nbproc];
    for(int ip=1;ip<_nbproc;ip++){
      thread_st *st = new thread_st;
      st->ip = ip;
      st->tior = _tior;
      pthread_create(&(th[ip]),NULL,th_finalizeremoval,(void*)st);
    }
  }

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
      //        dlclose(handle);                // SALOME unstable after ...
      //        _library_map.erase(impl_name);
    }
  }
  _toRemove_map.clear();
  _numInstanceMutex.unlock();

  if(_numproc == 0){
    for(int ip=1;ip<_nbproc;ip++)
      pthread_join(th[ip],NULL);
    delete th;
  }
}

void *th_loadcomponentlibrary(void *s)
{
  thread_st *st = (thread_st*)s;
  (Engines::MPIContainer::_narrow((*(st->tior))[st->ip]))->load_component_Library(st->compoName.c_str());
  return NULL;
}

void *th_createcomponentinstance(void *s)
{
  thread_st *st = (thread_st*)s;
  (Engines::MPIContainer::_narrow((*(st->tior))[st->ip]))->create_component_instance(st->compoName.c_str(),st->studyId);
  return NULL;
}

void *th_loadimpl(void *s)
{
  thread_st *st = (thread_st*)s;
  (Engines::MPIContainer::_narrow((*(st->tior))[st->ip]))->load_impl(st->nameToRegister.c_str(),st->compoName.c_str());
  return NULL;
}

void *th_removeimpl(void *s)
{
  thread_st *st = (thread_st*)s;
  (Engines::MPIContainer::_narrow((*(st->tior))[st->ip]))->remove_impl(st->cptr);
  return NULL;
}

void *th_finalizeremoval(void *s)
{
  thread_st *st = (thread_st*)s;
  (Engines::MPIContainer::_narrow((*(st->tior))[st->ip]))->finalize_removal();
  return NULL;
}
