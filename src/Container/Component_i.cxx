using namespace std;
//=============================================================================
// File      : Component_i.cxx
// Created   : jeu jui 12 08:04:40 CEST 2001
// Author    : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
// Project   : SALOME
// Copyright : EDF 2001 - CEA 2001
// $Header$
//=============================================================================

#include "SALOME_Component_i.hxx"
#include "RegistryConnexion.hxx"
#include "OpUtil.hxx"
#include <stdio.h>
#include <dlfcn.h>
#include "utilities.h"

extern bool _Sleeping ;

Engines_Component_i::Engines_Component_i()
{
//  MESSAGE("Component constructor");
}

Engines_Component_i::Engines_Component_i(CORBA::ORB_ptr orb,
					 PortableServer::POA_ptr poa, 
					 PortableServer::ObjectId * contId, 
					 const char *instanceName,
					 const char *interfaceName,
                                         bool notif) :
  _instanceName(instanceName), _interfaceName(interfaceName),
  _myConnexionToRegistry(0), _ThreadId(0) , _graphName("") , _nodeName("") {
  MESSAGE("Component constructor with instanceName "<< _instanceName);
  _orb = CORBA::ORB::_duplicate(orb);
  _poa = PortableServer::POA::_duplicate(poa);
  _contId = contId ;
  CORBA::Object_var o = _poa->id_to_reference(*contId); // container ior...
  const CORBA::String_var ior = _orb->object_to_string(o);
  _myConnexionToRegistry = new RegistryConnexion(0, 0, ior, "theSession", _instanceName.c_str());

  _notifSupplier = new NOTIFICATION_Supplier(instanceName, notif);
}

// Constructeur pour composant parallele: ne pas faire appel au registry!!
Engines_Component_i::Engines_Component_i(CORBA::ORB_ptr orb,
					 PortableServer::POA_ptr poa, 
					 PortableServer::ObjectId * contId, 
					 const char *instanceName,
					 const char *interfaceName,
					 int flag,
                                         bool notif )
  : _instanceName(instanceName),
    _interfaceName(interfaceName),
    _myConnexionToRegistry(0),
    _ThreadId(0)
{
//  MESSAGE("Component constructor with instanceName "<< _instanceName);
  _orb = CORBA::ORB::_duplicate(orb);
  _poa = PortableServer::POA::_duplicate(poa);
  _contId = contId ;
  //  CORBA::Object_var myself = this->_this(); //appel a _this = increment reference

  _notifSupplier = new NOTIFICATION_Supplier(instanceName, notif);
}

Engines_Component_i::~Engines_Component_i()
{
  MESSAGE("Component destructor");
//   delete _myConnexionToRegistry;
//   _myConnexionToRegistry = 0 ;
}

char* Engines_Component_i::instanceName() {
   return CORBA::string_dup(_instanceName.c_str()) ;
}

char* Engines_Component_i::interfaceName() {
   return CORBA::string_dup(_interfaceName.c_str()) ;
}

void Engines_Component_i::ping()
{
  MESSAGE("Engines_Component_i::ping() pid "<< getpid() << " threadid "
          << pthread_self());
}

void Engines_Component_i::destroy()
{
  MESSAGE("Engines_Component_i::destroy()");

  delete _notifSupplier;
  _notifSupplier = 0;

  delete _myConnexionToRegistry;
  _myConnexionToRegistry = 0 ;
  _poa->deactivate_object(*_id) ;
  CORBA::release(_poa) ;
  delete(_id) ;
  _thisObj->_remove_ref();
  MESSAGE("Engines_Component_i::destroyed") ;
}

Engines::Container_ptr Engines_Component_i::GetContainerRef()
{
  MESSAGE("Engines_Component_i::GetContainerRef");
  CORBA::Object_ptr o = _poa->id_to_reference(*_contId) ;
  return Engines::Container::_narrow(o);
}

PortableServer::ObjectId * Engines_Component_i::getId()
{
//  MESSAGE("PortableServer::ObjectId * Engines_Component_i::getId()");
  return _id ;
}

void Engines_Component_i::beginService(const char *serviceName)
{
  MESSAGE("Send BeginService notification for " << serviceName << endl
	  << "Component instance : " << _instanceName << endl << endl);
  _ThreadId = pthread_self() ;
  _serviceName = serviceName ;
  if ( pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS , NULL ) ) {
    perror("pthread_setcanceltype ") ;
    exit(0) ;
  }
  if ( pthread_setcancelstate( PTHREAD_CANCEL_ENABLE , NULL ) ) {
    perror("pthread_setcancelstate ") ;
    exit(0) ;
  }
  MESSAGE("Return from BeginService for " << serviceName
          << " ThreadId " << _ThreadId
          << " _graphName " << _graphName << " _nodeName " << _nodeName );
}

void Engines_Component_i::endService(const char *serviceName)
{
  MESSAGE("Send EndService notification for " << serviceName << endl
	  << "Component instance : " << _instanceName << endl << endl);
  _ThreadId = 0 ;
}

void Engines_Component_i::Names( const char * graphName ,
                                 const char * nodeName ) {
  _graphName = graphName ;
  _nodeName = nodeName ;
//  MESSAGE("Engines_Component_i::Names( '" << _graphName << "' , '"
//          << _nodeName << "' )");
}

char* Engines_Component_i::graphName() {
  return CORBA::string_dup( _graphName.c_str() ) ;
}

char* Engines_Component_i::nodeName() {
  return CORBA::string_dup( _nodeName.c_str() ) ;
}

bool Killer( int ThreadId , int signum ) {
  if ( ThreadId ) {
    if ( signum == 0 ) {
      if ( pthread_cancel( ThreadId ) ) {
        perror("Killer pthread_cancel error") ;
        return false ;
      }
      else {
        MESSAGE("Killer : ThreadId " << ThreadId << " pthread_cancel") ;
      }
    }
    else {
      if ( pthread_kill( ThreadId , signum ) == -1 ) {
        perror("Killer pthread_kill error") ;
        return false ;
      }
      else {
        MESSAGE("Killer : ThreadId " << ThreadId << " pthread_killed("
                << signum << ")") ;
      }
    }
  }
  return true ;
}

bool Engines_Component_i::Kill_impl() {
  MESSAGE("Engines_Component_i::Kill_i() pthread_t "<< pthread_self()
          << " pid " << getpid() << " instanceName "
          << _instanceName.c_str() << " interface " << _interfaceName.c_str()
          << " machineName " << GetHostname().c_str()<< " _id " << hex << _id
          << dec << " _ThreadId " << _ThreadId << " this " << hex << this
          << dec ) ;
  bool RetVal = false ;
  if ( _ThreadId > 0 && pthread_self() != _ThreadId ) {
    RetVal = Killer( _ThreadId , 0 ) ;
    _ThreadId = (pthread_t ) -1 ;
  }
  return RetVal ;
}

bool Engines_Component_i::Stop_impl() {
  MESSAGE("Engines_Component_i::Stop_i() pthread_t "<< pthread_self()
          << " pid " << getpid() << " instanceName "
          << _instanceName.c_str() << " interface " << _interfaceName.c_str()
          << " machineName " << GetHostname().c_str()<< " _id " << hex << _id
          << dec << " _ThreadId " << _ThreadId );
  bool RetVal = false ;
  if ( _ThreadId > 0 && pthread_self() != _ThreadId ) {
    RetVal = Killer( _ThreadId , 0 ) ;
    _ThreadId = (pthread_t ) -1 ;
  }
  return RetVal ;
}

bool Engines_Component_i::Suspend_impl() {
  MESSAGE("Engines_Component_i::Suspend_i() pthread_t "<< pthread_self()
          << " pid " << getpid() << " instanceName "
          << _instanceName.c_str() << " interface " << _interfaceName.c_str()
          << " machineName " << GetHostname().c_str()<< " _id " << hex << _id
          << dec << " _ThreadId " << _ThreadId );
  bool RetVal = false ;
  if ( _ThreadId > 0 && pthread_self() != _ThreadId ) {
    if ( _Sleeping ) {
      return false ;
    }
    else {
      RetVal = Killer( _ThreadId ,SIGINT ) ;
    }
  }
  return RetVal ;
}

bool Engines_Component_i::Resume_impl() {
  MESSAGE("Engines_Component_i::Resume_i() pthread_t "<< pthread_self()
          << " pid " << getpid() << " instanceName "
          << _instanceName.c_str() << " interface " << _interfaceName.c_str()
          << " machineName " << GetHostname().c_str()<< " _id " << hex << _id
          << dec << " _ThreadId " << _ThreadId );
  bool RetVal = false ;
  if ( _ThreadId > 0 && pthread_self() != _ThreadId ) {
    if ( _Sleeping ) {
      _Sleeping = false ;
      RetVal = true ;
    }
    else {
      RetVal = false ;
    }
  }
  return RetVal ;

}

// Send message to event channel

void Engines_Component_i::sendMessage(const char *event_type, const char *message) {
    _notifSupplier->Send(graphName(), nodeName(), event_type, message);
}
