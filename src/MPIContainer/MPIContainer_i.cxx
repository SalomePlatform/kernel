//  SALOME MPIContainer : implemenation of container based on MPI libraries
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//
//
//  File   : MPIContainer_i.cxx
//  Module : SALOME

using namespace std;
#include <iostream.h>
#include <dlfcn.h>
#include <stdio.h>
#include "MPIContainer_i.hxx"
#include "utilities.h"

MPIContainer_i::MPIContainer_i(int nbproc, int numproc,
			       CORBA::ORB_ptr orb, 
			       PortableServer::POA_ptr poa,
			       char * containerName) 
  : Engines_Container_i(orb,poa,containerName,0), MPIObject_i(nbproc,numproc)
{
  _id = _poa->activate_object(this);

  MESSAGE("[" << _numproc << "] containerName=" << _containerName);
  if( _numproc ==  0 ){
    _NS = SINGLETON_<SALOME_NamingService>::Instance() ;
    ASSERT(SINGLETON_<SALOME_NamingService>::IsAlreadyExisting()) ;
    _NS->init_orb( orb ) ;

    Engines::Container_ptr pCont 
      = Engines::Container::_narrow(POA_Engines::MPIContainer::_this());
    _NS->Register(pCont, _containerName.c_str()); 
  }

  // Root recupere les ior des container des autre process
  Engines::MPIObject_var pobj = POA_Engines::MPIContainer::_this();
  BCastIOR(_orb,pobj,true);
}

MPIContainer_i::~MPIContainer_i(void)
{
  if( !handle_map.empty() ){
    MESSAGE("[" << _numproc << "] MPIContainer_i::~MPIContainer_i: warning destroy a not empty container");
  }
}

// Load component
Engines::Component_ptr MPIContainer_i::load_impl(const char* nameToRegister,
						 const char* componentName)
{
  int ip;
  Engines::Component_var iobject;
  Engines::MPIObject_var pobj;
  char cproc[4];

  if( _numproc == 0 ){
    // Invocation du chargement du composant dans les autres process
    for(ip= 1;ip<_nbproc;ip++)
      (Engines::MPIContainer::_narrow((*_tior)[ip]))->load_impl(nameToRegister,
								componentName);
  }

  sprintf(cproc,"_%d",_numproc);

  BEGIN_OF("[" << _numproc << "] MPIContainer_i::load_impl");

  _numInstanceMutex.lock() ; // lock on the instance number
  _numInstance++ ;
  char _aNumI[12];
  sprintf(_aNumI,"%d",_numInstance) ;
  _numInstanceMutex.unlock() ;

  string _impl_name = componentName;
  string instanceName = string(nameToRegister) + "_inst_" + _aNumI + cproc;

  string absolute_impl_name(_impl_name);
  MESSAGE("[" << _numproc << "] absolute_impl_name=" << absolute_impl_name);
  void * handle = dlopen(absolute_impl_name.c_str(), RTLD_LAZY);
  if(!handle){
    MESSAGE("[" << _numproc << "] Can't load shared library : " << absolute_impl_name);
    MESSAGE("[" << _numproc << "] error dlopen: " << dlerror());
    return Engines::Component::_nil() ;
  }

  string factory_name = string(nameToRegister) + string("Engine_factory");
  MESSAGE("[" << _numproc << "] factory_name=" << factory_name) ;

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
    iobject = Engines_Container_i::load_impl(nameToRegister,componentName);
    if( CORBA::is_nil(iobject) ) return Engines::Component::_duplicate(iobject);
  }
  else{
    // Instanciation du composant parallele
    MESSAGE("[" << _numproc << "] Try to load a parallel component");
    PortableServer::ObjectId * id = (MPIComponent_factory)
      (_nbproc,_numproc,_orb, _poa, _id, instanceName.c_str(), nameToRegister);
    // get reference from id
    CORBA::Object_var o = _poa->id_to_reference(*id);
    pobj = Engines::MPIObject::_narrow(o) ;
    iobject = Engines::Component::_narrow(pobj) ;
  }

  // Root recupere les ior des composants des autre process
  BCastIOR(_orb,pobj,false);

  if( _numproc == 0 ){
    // utiliser + tard le registry ici :
    // register the engine under the name containerName.dir/nameToRegister.object
    string component_registerName = _containerName + "/" + nameToRegister;
    _NS->Register(iobject, component_registerName.c_str()) ;
  }

  _numInstanceMutex.lock() ; // lock on the add on handle_map (necessary ?)
  handle_map[instanceName] = handle;
  _numInstanceMutex.unlock() ;
  END_OF("[" <<_numproc << "] MPIContainer_i::load_impl");
  return Engines::Component::_duplicate(iobject);

}

void MPIContainer_i::remove_impl(Engines::Component_ptr component_i)
{
  int ip;
  Engines::Component_ptr cptr;
  Engines::MPIObject_ptr pcptr;
  Engines::MPIObject_ptr spcptr;

  ASSERT(! CORBA::is_nil(component_i));

  if( _numproc == 0 ){
    // Invocation de la destruction du composant dans les autres process
    pcptr = (Engines::MPIObject_ptr)component_i;
    for(ip= 1;ip<_nbproc;ip++){
      spcptr = Engines::MPIObject::_narrow((*(pcptr->tior()))[ip]);
      cptr = (Engines::Component_ptr)spcptr;
      (Engines::MPIContainer::_narrow((*_tior)[ip]))->remove_impl(cptr);
    }
  }

  string instanceName = component_i->instanceName() ;
  MESSAGE("[" << _numproc << "] unload component " << instanceName);
  component_i->destroy() ;
  MESSAGE("[" << _numproc << "] test key handle_map");
  _numInstanceMutex.lock() ; // lock on the remove on handle_map
  if (handle_map[instanceName]) // if key does not exist, created & initialized null
    {
      remove_map[instanceName] = handle_map[instanceName] ;
    }
  else MESSAGE("[" << _numproc << "] no key handle_map");
  handle_map.erase(instanceName) ;   
  _numInstanceMutex.unlock() ;
  MESSAGE("[" << _numproc << "] list handle_map");
  map<string, void *>::iterator im ;
  for (im = handle_map.begin() ; im != handle_map.end() ; im ++)
    {
      MESSAGE("[" << _numproc << "] stay " << (*im).first);
    }
}

void MPIContainer_i::finalize_removal()
{
  int ip;

  MESSAGE("[" << _numproc << "] finalize unload : dlclose");

  if( _numproc == 0 ){
    // Invocation de la destruction du composant dans les autres process
    for(ip= 1;ip<_nbproc;ip++)
      (Engines::MPIContainer::_narrow((*_tior)[ip]))->finalize_removal();
  }

  map<string, void *>::iterator im ;
  _numInstanceMutex.lock() ; // lock on the explore remove_map & dlclose
  for (im = remove_map.begin() ; im != remove_map.end() ; im ++)
    {
      void * handle = (*im).second ;
      dlclose(handle) ;
      MESSAGE("[" << _numproc << "] dlclose " << (*im).first);
    }
  remove_map.clear() ;  
  _numInstanceMutex.unlock() ;
  MESSAGE("[" << _numproc << "] remove_map.clear()");
}
