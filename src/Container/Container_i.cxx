//  SALOME Container : implementation of container and engine for Kernel
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
//  File   : Container_i.cxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA 
//  Module : SALOME
//  $Header$

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)
#include "SALOME_Container_i.hxx"
#include "SALOME_NamingService.hxx"
#include "Utils_SINGLETON.hxx"
#include "OpUtil.hxx"
#include <stdio.h>
#include <dlfcn.h>
#include <unistd.h>

#include "utilities.h"
using namespace std;

bool _Sleeping = false ;

// Needed by multi-threaded Python
int _ArgC ;
char ** _ArgV ;


// Containers with name FactoryServer are started via rsh in LifeCycleCORBA
// Other Containers are started via start_impl of FactoryServer

extern "C" {void ActSigIntHandler() ; }
extern "C" {void SigIntHandler(int, siginfo_t *, void *) ; }

Engines_Container_i::Engines_Container_i () :
 _numInstance(0)
{
}

Engines_Container_i::Engines_Container_i (CORBA::ORB_ptr orb, 
					  PortableServer::POA_ptr poa,
					  char *containerName ,
                                          int argc , char* argv[],
					  bool regist,
					  bool activ ) :
 _numInstance(0)
{
  _pid = (long)getpid();

  if(regist)
    ActSigIntHandler() ;

  _ArgC = argc ;
  _ArgV = argv ;

  _argc = argc ;
  _argv = argv ;
  int i = strlen( _argv[ 0 ] ) - 1 ;
  while ( i >= 0 ) {
    if ( _argv[ 0 ][ i ] == '/' ) {
      _argv[ 0 ][ i+1 ] = '\0' ;
      break ;
    }
    i -= 1 ;
  }
  string hostname = GetHostname();
  MESSAGE(hostname << " " << getpid() << " Engines_Container_i starting argc "
	  << _argc << " Thread " << pthread_self() ) ;
  i = 0 ;
  while ( _argv[ i ] ) {
    MESSAGE("           argv" << i << " " << _argv[ i ]) ;
    i++ ;
  }
  if ( argc != 4 ) {
    MESSAGE("SALOME_Container usage : SALOME_Container ServerName -ORBInitRef NameService=corbaname::hostname:tcpipPortNumber") ;
//    exit(0) ;
  }

  SCRUTE(hostname);

  _containerName = "/Containers/";
  if (strlen(containerName)== 0)
    {
      _containerName += hostname;
    }
  else
    {
      _containerName += hostname;
      _containerName += "/" ;
      _containerName += containerName;
    }

  _orb = CORBA::ORB::_duplicate(orb) ;
  _poa = PortableServer::POA::_duplicate(poa) ;
  // Pour les containers paralleles: il ne faut pas activer le container generique, mais le container specialise
  if(activ){
    MESSAGE("activate object");
    _id = _poa->activate_object(this);
  }

  // Pour les containers paralleles: il ne faut pas enregistrer le container generique, mais le container specialise
  if(regist){
    //   _NS = new SALOME_NamingService(_orb);
    _NS = SINGLETON_<SALOME_NamingService>::Instance() ;
    ASSERT(SINGLETON_<SALOME_NamingService>::IsAlreadyExisting()) ;
    _NS->init_orb( orb ) ;

    Engines::Container_ptr pCont 
      = Engines::Container::_narrow(_this());
    SCRUTE(_containerName);
    _NS->Register(pCont, _containerName.c_str()); 
  }
}

Engines_Container_i::~Engines_Container_i()
{
  MESSAGE("Container_i::~Container_i()");
}

char* Engines_Container_i::name()
{
   return CORBA::string_dup(_containerName.c_str()) ;
}

char* Engines_Container_i::machineName()
{
  string s = GetHostname();
  MESSAGE("Engines_Container_i::machineName " << s);
   return CORBA::string_dup(s.c_str()) ;
}

void Engines_Container_i::ping()
{
  MESSAGE("Engines_Container_i::ping() pid "<< getpid());
}

//! Kill current container
bool Engines_Container_i::Kill_impl() {
  MESSAGE("Engines_Container_i::Kill() pid "<< getpid() << " containerName "
          << _containerName.c_str() << " machineName "
          << GetHostname().c_str());
  exit( 0 ) ;
}

//! Launch a new container from the current container
Engines::Container_ptr Engines_Container_i::start_impl(
                                      const char* ContainerName ) {
  MESSAGE("start_impl argc " << _argc << " ContainerName " << ContainerName
          << hex << this << dec) ;
  _numInstanceMutex.lock() ; // lock on the instance number

  CORBA::Object_var obj = Engines::Container::_nil() ;
  bool nilvar = true ;
  try {
    string cont("/Containers/");
    cont += machineName() ;
    cont += "/" ;
    cont += ContainerName;
    INFOS(machineName() << " start_impl unknown container " << cont.c_str()
          << " try to Resolve" );
    obj = _NS->Resolve( cont.c_str() );
    nilvar = CORBA::is_nil( obj ) ;
    if ( nilvar ) {
      INFOS(machineName() << " start_impl unknown container "
            << ContainerName);
    }
  }
  catch (ServiceUnreachable&) {
    INFOS(machineName() << "Caught exception: Naming Service Unreachable");
  }
  catch (...) {
    INFOS(machineName() << "Caught unknown exception.");
  }
  if ( !nilvar ) {
    _numInstanceMutex.unlock() ;
    MESSAGE("start_impl container found without new launch") ;
    return Engines::Container::_narrow(obj);
  }
  int i = 0 ;
  while ( _argv[ i ] ) {
    MESSAGE("           argv" << i << " " << _argv[ i ]) ;
    i++ ;
  }
  string shstr = string(getenv("KERNEL_ROOT_DIR")) + "/bin/salome/SALOME_Container ";
//   string shstr( "./runSession SALOME_Container " ) ;
  shstr += ContainerName ;
  if ( _argc == 4 ) {
    shstr += " " ;
    shstr += _argv[ 2 ] ;
    shstr += " " ;
    shstr += _argv[ 3 ] ;
  }
  shstr += " > /tmp/" ;
  shstr += ContainerName ;
  shstr += ".log 2>&1 &" ;
  MESSAGE("system(" << shstr << ")") ;
  int status = system( shstr.c_str() ) ;
  if (status == -1) {
    INFOS("Engines_Container_i::start_impl SALOME_Container failed (system command status -1)") ;
  }
  else if (status == 217) {
    INFOS("Engines_Container_i::start_impl SALOME_Container failed (system command status 217)") ;
  }
  INFOS(machineName() << " Engines_Container_i::start_impl SALOME_Container launch done");

//   pid_t pid = fork() ;
//   if ( pid == 0 ) {
//     string anExe( _argv[ 0 ] ) ;
//     anExe += "runSession" ;
//     char * args[ 6 ] ;
//     args[ 0 ] = "runSession" ;
//     args[ 1 ] = "SALOME_Container" ;
//     args[ 2 ] = strdup( ContainerName ) ;
//     args[ 3 ] = strdup( _argv[ 2 ] ) ;
//     args[ 4 ] = strdup( _argv[ 3 ] ) ;
//     args[ 5 ] = NULL ;
//     MESSAGE("execl(" << anExe.c_str() << " , " << args[ 0 ] << " , "
//                      << args[ 1 ] << " , " << args[ 2 ] << " , " << args[ 3 ]
//                      << " , " << args[ 4 ] << ")") ;
//     int status = execv( anExe.c_str() , args ) ;
//     if (status == -1) {
//       INFOS("Engines_Container_i::start_impl execl failed (system command status -1)") ;
//       perror( "Engines_Container_i::start_impl execl error ") ;
//     }
//     else {
//       INFOS(machineName() << " Engines_Container_i::start_impl execl done");
//     }
//     exit(0) ;
//   }

  obj = Engines::Container::_nil() ;
  try {
    string cont("/Containers/");
    cont += machineName() ;
    cont += "/" ;
    cont += ContainerName;
    nilvar = true ;
    int count = 20 ;
    while ( nilvar && count >= 0) {
      sleep( 1 ) ;
      obj = _NS->Resolve(cont.c_str());
      nilvar = CORBA::is_nil( obj ) ;
      if ( nilvar ) {
        INFOS(count << ". " << machineName()
              << " start_impl unknown container " << cont.c_str());
        count -= 1 ;
      }
    }
    _numInstanceMutex.unlock() ;
    if ( !nilvar ) {
      MESSAGE("start_impl container found after new launch of SALOME_Container") ;
    }
    return Engines::Container::_narrow(obj);
  }
  catch (ServiceUnreachable&) {
    INFOS(machineName() << "Caught exception: Naming Service Unreachable");
  }
  catch (...) {
    INFOS(machineName() << "Caught unknown exception.");
  }
  _numInstanceMutex.unlock() ;
  MESSAGE("start_impl container not found after new launch of SALOME_Container") ;
  return Engines::Container::_nil() ;
}

Engines::Component_ptr Engines_Container_i::load_impl( const char* nameToRegister,
	                                               const char* componentName ) {

  _numInstanceMutex.lock() ; // lock on the instance number
  BEGIN_OF( "Container_i::load_impl " << componentName ) ;
  _numInstance++ ;
  char _aNumI[12];
  sprintf( _aNumI , "%d" , _numInstance ) ;

  string _impl_name = componentName;
  string _nameToRegister = nameToRegister;
  string instanceName = _nameToRegister + "_inst_" + _aNumI ;
  //SCRUTE(instanceName);

  //string absolute_impl_name = _library_path + "lib" + _impl_name + ".so";
  string absolute_impl_name( _impl_name ) ;
  SCRUTE(absolute_impl_name);
  void* handle;
  handle = dlopen( absolute_impl_name.c_str() , RTLD_LAZY ) ;
  if ( !handle ) {
    INFOS("Can't load shared library : " << absolute_impl_name);
    INFOS("error dlopen: " << dlerror());
    _numInstanceMutex.unlock() ;
    return Engines::Component::_nil() ;
  }
  
  string factory_name = _nameToRegister + string("Engine_factory");
  //  SCRUTE(factory_name) ;

  typedef  PortableServer::ObjectId * (*FACTORY_FUNCTION)
                            (CORBA::ORB_ptr,
			     PortableServer::POA_ptr, 
			     PortableServer::ObjectId *, 
			     const char *, 
			     const char *) ; 
  FACTORY_FUNCTION Component_factory = (FACTORY_FUNCTION) dlsym(handle, factory_name.c_str());

  char *error ;
  if ( (error = dlerror() ) != NULL) {
      INFOS("Can't resolve symbol: " + factory_name);
      SCRUTE(error);
      _numInstanceMutex.unlock() ;
      return Engines::Component::_nil() ;
    }

  string component_registerName = _containerName + "/" + _nameToRegister;
  Engines::Component_var iobject = Engines::Component::_nil() ;
  try {
    CORBA::Object_var obj = _NS->Resolve( component_registerName.c_str() ) ;
    if ( CORBA::is_nil( obj ) ) {
// Instanciate required CORBA object
      PortableServer::ObjectId * id ;
      id = (Component_factory) ( _orb, _poa, _id, instanceName.c_str() ,
                                 _nameToRegister.c_str() ) ;
  // get reference from id
      obj = _poa->id_to_reference(*id);
      iobject = Engines::Component::_narrow( obj ) ;

//  _numInstanceMutex.lock() ; // lock on the add on handle_map (necessary ?)
  // register the engine under the name containerName.dir/nameToRegister.object
      _NS->Register( iobject , component_registerName.c_str() ) ;
      MESSAGE( "Container_i::load_impl " << component_registerName.c_str() << " bound" ) ;
    }
    else { // JR : No ReBind !!!
      MESSAGE( "Container_i::load_impl " << component_registerName.c_str() << " already bound" ) ;
      iobject = Engines::Component::_narrow( obj ) ;
    }
  }
  catch (...) {
    INFOS( "Container_i::load_impl catched" ) ;
  }

//Jr  _numInstanceMutex.lock() ; // lock on the add on handle_map (necessary ?)
  handle_map[instanceName] = handle;
  END_OF("Container_i::load_impl");
  _numInstanceMutex.unlock() ;
  return Engines::Component::_duplicate(iobject);
}

void Engines_Container_i::remove_impl(Engines::Component_ptr component_i)
{
  ASSERT(! CORBA::is_nil(component_i));
  string instanceName = component_i->instanceName() ;
  MESSAGE("unload component " << instanceName);
  component_i->destroy() ;
  MESSAGE("test key handle_map");
  _numInstanceMutex.lock() ; // lock on the remove on handle_map
  if (handle_map[instanceName]) // if key does not exist, created & initialized null
    {
      remove_map[instanceName] = handle_map[instanceName] ;
    }
  else MESSAGE("pas d'entree handle_map");
  handle_map.erase(instanceName) ;   
  _numInstanceMutex.unlock() ;
  MESSAGE("contenu handle_map");
  map<string, void *>::iterator im ;
  for (im = handle_map.begin() ; im != handle_map.end() ; im ++)
    {
      MESSAGE("reste " << (*im).first);
    }
}

void Engines_Container_i::finalize_removal()
{
  MESSAGE("finalize unload : dlclose");
  map<string, void *>::iterator im ;
  _numInstanceMutex.lock() ; // lock on the explore remove_map & dlclose
  for (im = remove_map.begin() ; im != remove_map.end() ; im ++)
    {
      void * handle = (*im).second ;
      dlclose(handle) ;
      MESSAGE("dlclose " << (*im).first);
    }
  remove_map.clear() ;  
  _numInstanceMutex.unlock() ;
  MESSAGE("remove_map.clear()");
}

void ActSigIntHandler() {
  struct sigaction SigIntAct ;
  SigIntAct.sa_sigaction = &SigIntHandler ;
  SigIntAct.sa_flags = SA_SIGINFO ;
  if ( sigaction( SIGINT | SIGUSR1 , &SigIntAct, NULL ) ) {
    perror("SALOME_Container main ") ;
    exit(0) ;
  }
  else {
    INFOS(pthread_self() << "SigIntHandler activated") ;
  }
}

void SetCpuUsed() ;

void SigIntHandler(int what , siginfo_t * siginfo ,
                                        void * toto ) {
  MESSAGE(pthread_self() << "SigIntHandler what     " << what << endl
          << "              si_signo " << siginfo->si_signo << endl
          << "              si_code  " << siginfo->si_code << endl
          << "              si_pid   " << siginfo->si_pid) ;
  if ( _Sleeping ) {
    _Sleeping = false ;
    MESSAGE("SigIntHandler END sleeping.") ;
    return ;
  }
  else {
    ActSigIntHandler() ;
    if ( siginfo->si_signo == SIGUSR1 ) {
      SetCpuUsed() ;
    }
    else {
      _Sleeping = true ;
      MESSAGE("SigIntHandler BEGIN sleeping.") ;
      int count = 0 ;
      while( _Sleeping ) {
        sleep( 1 ) ;
        count += 1 ;
      }
      MESSAGE("SigIntHandler LEAVE sleeping after " << count << " s.") ;
    }
    return ;
  }
}

// Get the PID of the Container

CORBA::Long Engines_Container_i::getPID() {
    return (long)getpid();
}

// Get the hostName of the Container

char* Engines_Container_i::getHostName() {
    return((char*)(GetHostname().c_str()));
}
