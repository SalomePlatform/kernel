//=============================================================================
// File      : SALOME_Component_i.hxx
// Created   : jeu jui 12 08:04:40 CEST 2001
// Author    : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
// Project   : SALOME
// Copyright : EDF 2001 - CEA 2001
// $Header$
//=============================================================================

#ifndef _SALOME_COMPONENT_I_HXX_
#define _SALOME_COMPONENT_I_HXX_

#include <iostream.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)
#include "NOTIFICATION.hxx"

class RegistryConnexion;

class Engines_Component_i: public virtual POA_Engines::Component,
			   public virtual PortableServer::RefCountServantBase
{
public:
  Engines_Component_i();
  Engines_Component_i(CORBA::ORB_ptr orb,
		      PortableServer::POA_ptr poa,
		      PortableServer::ObjectId * contId, 
		      const char *instanceName, 
		      const char *interfaceName,
                      bool notif = false);
  // Consructeur pour composant parallele: ne pas faire appel au registry
  Engines_Component_i(CORBA::ORB_ptr orb,
		      PortableServer::POA_ptr poa,
		      PortableServer::ObjectId * contId, 
		      const char *instanceName, 
		      const char *interfaceName,
		      int flag,
                      bool notif = false);

  virtual ~Engines_Component_i();

  char* instanceName();
  char* interfaceName();

  void destroy();
  void ping();

  Engines::Container_ptr GetContainerRef();
  PortableServer::ObjectId * getId(); 

  void beginService(const char *serviceName);
  void endService(const char *serviceName);
  void sendMessage(const char *event_type, const char *message);

  void Names( const char * graphName , const char * nodeName ) ;
  char * graphName() ;
  char * nodeName() ;
  bool Kill_impl();
  bool Stop_impl();
  bool Suspend_impl();
  bool Resume_impl();

protected:
  string _instanceName ;
  string _interfaceName ;
  string _serviceName ;
  string _graphName ;
  string _nodeName ;
  CORBA::ORB_ptr _orb;
  PortableServer::POA_ptr _poa;
  PortableServer::ObjectId * _id;
  PortableServer::ObjectId * _contId;
  Engines_Component_i * _thisObj ;
  RegistryConnexion *_myConnexionToRegistry;
  NOTIFICATION_Supplier* _notifSupplier;

private:
  pthread_t _ThreadId ;
};

#endif
