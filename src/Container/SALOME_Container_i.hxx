//=============================================================================
// File      : SALOME_Container_i.hxx
// Created   : jeu jui 12 08:04:44 CEST 2001
// Author    : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
// Project   : SALOME
// Copyright : EDF 2001 - CEA 2001
// $Header$
//=============================================================================

#ifndef _SALOME_CONTAINER_I_HXX_
#define _SALOME_CONTAINER_I_HXX_

#include <iostream.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)
#include <omnithread.h>
#include <map>
#include <string>

class SALOME_NamingService;

class Engines_Container_i: public POA_Engines::Container,
                public PortableServer::RefCountServantBase
{
public:
  Engines_Container_i();
  Engines_Container_i(CORBA::ORB_ptr orb, 
		      PortableServer::POA_ptr poa,
		      char * containerName ,
                      int argc, char* argv[]);
// Constructeur pour composant parallele : ne pas faire appel au naming service
  Engines_Container_i(CORBA::ORB_ptr orb, 
		      PortableServer::POA_ptr poa,
		      char * containerName,
		      int flag);
  virtual ~Engines_Container_i();


  Engines::Container_ptr start_impl(const char* ContainerName);

  Engines::Component_ptr load_impl(const char* nameToRegister,
				   const char* componentName);
  void remove_impl(Engines::Component_ptr component_i);
  void finalize_removal();

  char* name();
  char* machineName();
  void ping();
  bool Kill_impl() ;

protected:

  SALOME_NamingService *_NS ;
  string _library_path;
  string _containerName;
  CORBA::ORB_var _orb;
  PortableServer::POA_var _poa;
  PortableServer::ObjectId * _id ;
  int _numInstance ;
  map<string, void *> handle_map ;
  map<string, void *> remove_map ;
  omni_mutex _numInstanceMutex ; // if several threads on the same object

private: 

  int   _argc ;
  char** _argv ;

};

#endif

