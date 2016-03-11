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

//  SALOME Container : implementation of container and engine for Kernel
//  File   : Component_i.cxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$
//#define private protected  // for pd_refCount trace
//
#include "SALOME_Component_i.hxx"
#include "SALOME_Container_i.hxx"
#include "RegistryConnexion.hxx"
#include "Basics_Utils.hxx"
#include <stdio.h>
#ifndef WIN32
#include <dlfcn.h>
#endif
#include <cstdlib>
#include "utilities.h"

#ifndef WIN32
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#else
#include <sys/timeb.h>
int SIGUSR11 = 1000;
#include <process.h>
#endif

extern bool _Sleeping ;
static Engines_Component_i * theEngines_Component ;

bool Engines_Component_i::_isMultiStudy = true;
bool Engines_Component_i::_isMultiInstance = false;

/*! \class Engines_Component_i
 *  \brief C++ implementation of Engines::Component interface
 *
 */

//=============================================================================
/*!
 *  Default constructor, not for use
 */
//=============================================================================

Engines_Component_i::Engines_Component_i():_myConnexionToRegistry(0), _notifSupplier(0), _id(0)
{
  //ASSERT(0);
  MESSAGE("Default Constructor, not for normal use...");
}

//=============================================================================
/*!  \brief  Standard Constructor for generic Component, used in derived class
 *
 *  Connection to Registry and Notification
 *  \param orb Object Request broker given by Container
 *  \param poa Portable Object Adapter from Container (normally root_poa)
 *  \param contId container CORBA id inside the server
 *  \param instanceName unique instance name for this object (see Container_i)
 *  \param interfaceName component class name
 *  \param notif use of notification
 *  \param regist (true or false) use of registry (default true)
 */
//=============================================================================

Engines_Component_i::Engines_Component_i(CORBA::ORB_ptr orb,
                                         PortableServer::POA_ptr poa,
                                         PortableServer::ObjectId * contId,
                                         const char *instanceName,
                                         const char *interfaceName,
                                         bool notif,
                                         bool regist ) :
  _instanceName(instanceName),
  _interfaceName(interfaceName),
  _myConnexionToRegistry(0),
  _notifSupplier(0),
  _ThreadId(0) ,
  _ThreadCpuUsed(0) ,
  _Executed(false) ,
  _graphName("") ,
  _nodeName(""),
  _studyId(-1),
  _id(0),
  _contId(0),
  _CanceledThread(false)
{
  MESSAGE("Component constructor with instanceName "<< _instanceName);
  _orb = CORBA::ORB::_duplicate(orb);
  _poa = PortableServer::POA::_duplicate(poa);
  _contId = contId ;
  CORBA::Object_var o = _poa->id_to_reference(*contId); // container ior...
  _container=Engines::Container::_narrow(o);
  setContainerName();

  if(regist)
    {
      const CORBA::String_var ior = _orb->object_to_string(o);
      _myConnexionToRegistry = new RegistryConnexion(0, 0, ior,"theSession",
                                                     _instanceName.c_str());
    }

  if(notif)
    _notifSupplier = new NOTIFICATION_Supplier(instanceName, notif);
}

//=============================================================================
/*!  \brief  Standard Constructor for standalone Component, used in derived class
 *
 *  Connection to Registry and Notification
 *  \param orb Object Request broker given by Container
 *  \param poa Portable Object Adapter from Container (normally root_poa)
 *  \param container container CORBA reference
 *  \param instanceName unique instance name for this object (see Container_i)
 *  \param interfaceName component class name
 *  \param notif use of notification
 *  \param regist (true or false) use of registry (default true)
 */
//=============================================================================

Engines_Component_i::Engines_Component_i(CORBA::ORB_ptr orb,
                                         PortableServer::POA_ptr poa,
                                         Engines::Container_ptr container,
                                         const char *instanceName,
                                         const char *interfaceName,
                                         bool notif,
                                         bool regist) :
  _instanceName(instanceName),
  _interfaceName(interfaceName),
  _myConnexionToRegistry(0),
  _notifSupplier(0),
  _ThreadId(0) ,
  _ThreadCpuUsed(0) ,
  _Executed(false) ,
  _graphName("") ,
  _nodeName(""),
  _studyId(-1),
  _id(0),
  _contId(0),
  _CanceledThread(false)
{
  MESSAGE("Component constructor with instanceName "<< _instanceName);
  _orb = CORBA::ORB::_duplicate(orb);
  _poa = PortableServer::POA::_duplicate(poa);
  _container=Engines::Container::_duplicate(container);
  setContainerName();
  const CORBA::String_var ior = _orb->object_to_string(_container);
  if(regist)
  {
    _myConnexionToRegistry = new RegistryConnexion(0, 0, ior,"theSession", _instanceName.c_str());
  }
  if(notif)
    _notifSupplier = new NOTIFICATION_Supplier(instanceName, notif);

}


//=============================================================================
/*!
 *  Destructor: call Container for decrement of instances count.
 *  When instances count falls to 0, the container tries to remove the
 *  component library (dlclose)
 */
//=============================================================================

Engines_Component_i::~Engines_Component_i()
{
  MESSAGE("Component destructor");
  Engines_Container_i::decInstanceCnt(_interfaceName);
  if(_myConnexionToRegistry)delete _myConnexionToRegistry;
  _myConnexionToRegistry = 0 ;

  if(_id) delete _id;
  _id=0;

  if(_notifSupplier)
    {
      SCRUTE(_notifSupplier->_refcount_value());
      PortableServer::POA_var poa=_notifSupplier->_default_POA();
      PortableServer::ObjectId_var anObjectId = poa->servant_to_id(_notifSupplier);
      poa->deactivate_object(anObjectId.in());
      SCRUTE(_notifSupplier->_refcount_value());
      _notifSupplier->_remove_ref();
    }
}

//=============================================================================
/*!
 *  CORBA method: return name of the instance, unique in this Container
 */
//=============================================================================

char* Engines_Component_i::instanceName()
{
   return CORBA::string_dup(_instanceName.c_str()) ;
}

//=============================================================================
/*!
 *  CORBA method: return name of the component class
 */
//=============================================================================

char* Engines_Component_i::interfaceName()
{
  return CORBA::string_dup(_interfaceName.c_str()) ;
}

//=============================================================================
/*!
 *  CORBA method: Get study Id
 *  \return -1: not initialised (Internal Error)
 *           0: multistudy component instance
 *          >0: study id associated to this instance
 */
//=============================================================================

CORBA::Long Engines_Component_i::getStudyId()
{
  return _studyId;
}

//=============================================================================
/*!
 *  CORBA method: Test if instance is alive and responds
 */
//=============================================================================

void Engines_Component_i::ping()
{
#ifndef WIN32
  MESSAGE("Engines_Component_i::ping() pid "<< getpid() << " threadid "
          << pthread_self());
#else
  MESSAGE("Engines_Component_i::ping() pid "<< _getpid()<< " threadid "
          << pthread_self().p );
#endif
}

//=============================================================================
/*!
 *  CORBA method: Deactivate this instance. CORBA object is deactivated (do not
 *  respond any more to CORBA calls), the connection to Regsitry is removed
 *  (Registry informed of deactivation), internal server reference counter on
 *  the derived servant class is decremented, to allow destruction of the class
 *  (delete) by POA, when there are no more references.
 *  -- TO BE USED BY CONTAINER ONLY (Container housekeeping) --
 */
//=============================================================================

void Engines_Component_i::destroy()
{
  MESSAGE("Engines_Component_i::destroy()");
  //SCRUTE(_refcount_value());
  _poa->deactivate_object(*_id);
  //SCRUTE(_refcount_value());
  _remove_ref();
  //SCRUTE(_refcount_value());
  MESSAGE("Engines_Component_i::destroyed") ;
}

//=============================================================================
/*!
 *  CORBA method: return CORBA reference of the Container
 *
 */
//=============================================================================

Engines::Container_ptr Engines_Component_i::GetContainerRef()
{
  return Engines::Container::_duplicate(_container);
}

//=============================================================================
/*!
 *  CORBA method:
 *  Gives a sequence of (key=string,value=any) to the component.
 *  Base class component stores the sequence in a map.
 *  The map is cleared before.
 *  This map is for use by derived classes.
 *  \param dico sequence of (key=string,value=any)
 */
//=============================================================================

void Engines_Component_i::setProperties(const Engines::FieldsDict& dico)
{
  _fieldsDict.clear();
  for (CORBA::ULong i=0; i<dico.length(); i++)
    {
      std::string cle(dico[i].key);
      _fieldsDict[cle] = dico[i].value;
    }
}

//=============================================================================
/*!
 *  CORBA method:
 *  returns a previously stored map (key=string,value=any) as a sequence.
 *  (see setProperties)
 */
//=============================================================================

Engines::FieldsDict* Engines_Component_i::getProperties()
{
  Engines::FieldsDict_var copie = new Engines::FieldsDict;
  copie->length(_fieldsDict.size());
  std::map<std::string,CORBA::Any>::iterator it;
  CORBA::ULong i = 0;
  for (it = _fieldsDict.begin(); it != _fieldsDict.end(); it++, i++)
    {
      std::string cle((*it).first);
      copie[i].key = CORBA::string_dup(cle.c_str());
      copie[i].value = _fieldsDict[cle];
    }
  return copie._retn();
}

//=============================================================================
/*!
 *  CORBA method:
 *  This method is to set an option specific to a certain EngineComponent.
 */
//=============================================================================

void Engines_Component_i::SetOption(const char*, const char*)
{
}

//=============================================================================
/*!
 *  CORBA method:
 *  This method is to get value of an option specific to a certain EngineComponent.
 */
//=============================================================================

char* Engines_Component_i::GetOption(const char*)
{
  return CORBA::string_dup("") ;
}

//=============================================================================
/*!
 *  CORBA method: used by Supervision to give names to this instance
 */
//=============================================================================

void Engines_Component_i::Names( const char * graphName ,
                                 const char * nodeName )
{
  _graphName = graphName ;
  _nodeName = nodeName ;
  //  MESSAGE("Engines_Component_i::Names( '" << _graphName << "' , '"
  //          << _nodeName << "' )");
}

//=============================================================================
/*!
 *  CORBA method: used in Supervision
 */
//=============================================================================

bool Engines_Component_i::Kill_impl()
{
//  MESSAGE("Engines_Component_i::Kill_i() pthread_t "<< pthread_self()
//          << " pid " << getpid() << " instanceName "
//          << _instanceName.c_str() << " interface " << _interfaceName.c_str()
//          << " machineName " << Kernel_Utils::GetHostname().c_str()<< " _id " << hex << _id
//          << dec << " _ThreadId " << _ThreadId << " this " << hex << this
//          << dec ) ;

  bool RetVal = false ;
#ifndef WIN32
  if ( _ThreadId > 0 && pthread_self() != _ThreadId )
    {
      RetVal = Killer( _ThreadId , SIGUSR2 ) ;
      _ThreadId = (pthread_t ) -1 ;
    }

#else
  if ( _ThreadId > 0 && pthread_self().p != _ThreadId->p )
    {
      RetVal = Killer( *_ThreadId , 0 ) ;
      _ThreadId = (pthread_t* ) 0 ;
    }

#endif
  return RetVal ;
}

//=============================================================================
/*!
 *  CORBA method: used in Supervision
 */
//=============================================================================

bool Engines_Component_i::Stop_impl()
{
#ifndef WIN32
  MESSAGE("Engines_Component_i::Stop_i() pthread_t "<< pthread_self()
          << " pid " << getpid() << " instanceName "
          << _instanceName.c_str() << " interface " << _interfaceName.c_str()
          << " machineName " << Kernel_Utils::GetHostname().c_str()<< " _id " << std::hex << _id
          << std::dec << " _ThreadId " << _ThreadId );
#else
  MESSAGE("Engines_Component_i::Stop_i() pthread_t "<< pthread_self().p
          << " pid " << _getpid() << " instanceName "
          << _instanceName.c_str() << " interface " << _interfaceName.c_str()
          << " machineName " << Kernel_Utils::GetHostname().c_str()<< " _id " << std::hex << _id
          << std::dec << " _ThreadId " << _ThreadId );
#endif


  bool RetVal = false ;
#ifndef WIN32
  if ( _ThreadId > 0 && pthread_self() != _ThreadId )
    {
      RetVal = Killer( _ThreadId , 0 ) ;
      _ThreadId = (pthread_t ) -1 ;
    }
#else
  if ( _ThreadId > 0 && pthread_self().p != _ThreadId->p )
    {
      RetVal = Killer( *_ThreadId , 0 ) ;
      _ThreadId = (pthread_t* ) 0 ;
    }
#endif
  return RetVal ;
}

//=============================================================================
/*!
 *  CORBA method: used in Supervision
 */
//=============================================================================

bool Engines_Component_i::Suspend_impl()
{
#ifndef WIN32
  MESSAGE("Engines_Component_i::Suspend_i() pthread_t "<< pthread_self()
          << " pid " << getpid() << " instanceName "
          << _instanceName.c_str() << " interface " << _interfaceName.c_str()
          << " machineName " << Kernel_Utils::GetHostname().c_str()<< " _id " << std::hex << _id
          << std::dec << " _ThreadId " << _ThreadId );
#else
  MESSAGE("Engines_Component_i::Suspend_i() pthread_t "<< pthread_self().p
          << " pid " << _getpid() << " instanceName "
          << _instanceName.c_str() << " interface " << _interfaceName.c_str()
          << " machineName " << Kernel_Utils::GetHostname().c_str()<< " _id " << std::hex << _id
          << std::dec << " _ThreadId " << _ThreadId );
#endif

  bool RetVal = false ;
#ifndef WIN32
  if ( _ThreadId > 0 && pthread_self() != _ThreadId )
#else
  if ( _ThreadId > 0 && pthread_self().p != _ThreadId->p )
#endif
    {
      if ( _Sleeping )
        {
          return false ;
        }
    else
      {
#ifndef WIN32
        RetVal = Killer( _ThreadId ,SIGINT ) ;
#else
        RetVal = Killer( *_ThreadId ,SIGINT ) ;
#endif
        //if ( RetVal ) _Sleeping = true;

      }
    }
  return RetVal ;
}

//=============================================================================
/*!
 *  CORBA method: used in Supervision
 */
//=============================================================================

bool Engines_Component_i::Resume_impl()
{
#ifndef WIN32
  MESSAGE("Engines_Component_i::Resume_i() pthread_t "<< pthread_self()
          << " pid " << getpid() << " instanceName "
          << _instanceName.c_str() << " interface " << _interfaceName.c_str()
          << " machineName " << Kernel_Utils::GetHostname().c_str()<< " _id " << std::hex << _id
          << std::dec << " _ThreadId " << _ThreadId );
#else
  MESSAGE("Engines_Component_i::Resume_i() pthread_t "<< pthread_self().p
          << " pid " << _getpid() << " instanceName "
          << _instanceName.c_str() << " interface " << _interfaceName.c_str()
          << " machineName " << Kernel_Utils::GetHostname().c_str()<< " _id " << std::hex << _id
          << std::dec << " _ThreadId " << _ThreadId );
#endif
  bool RetVal = false ;
#ifndef WIN32
  if ( _ThreadId > 0 && pthread_self() != _ThreadId )
#else
  if ( _ThreadId > 0 && pthread_self().p != _ThreadId->p )
#endif
    {
    if ( _Sleeping )
      {
        _Sleeping = false ;
        RetVal = true ;
      }
    else
      {
        RetVal = false ;
      }
    }
  return RetVal ;
}

//=============================================================================
/*!
 *  CORBA method:
 */
//=============================================================================

CORBA::Long Engines_Component_i::CpuUsed_impl()
{
  long cpu = 0 ;
  if ( _ThreadId || _Executed )
    {
    if ( _ThreadId > 0 )
      {
#ifndef WIN32
      if ( pthread_self() != _ThreadId )
#else
      if ( pthread_self().p != _ThreadId->p )
#endif
        {
        if ( _Sleeping )
          {
          }
        else
          {
            // Get Cpu in the appropriate thread with that object !...
            theEngines_Component = this ;
#ifndef WIN32
            Killer( _ThreadId ,SIGUSR1 ) ;
#else
            Killer( *_ThreadId ,SIGUSR11 ) ;
#endif
          }
        cpu = _ThreadCpuUsed ;
        }
      else
        {
          _ThreadCpuUsed = CpuUsed() ;
          cpu = _ThreadCpuUsed ;
          // cout << pthread_self() << " Engines_Component_i::CpuUsed_impl "
          //      << _serviceName << " " << cpu << endl ;
      }
    }
    else
      {
        cpu = _ThreadCpuUsed ;
        // cout << pthread_self() << " Engines_Component_i::CpuUsed_impl "
        //      << _serviceName << " " << cpu<< endl ;
      }
    }
  else
    {
      // cout<< pthread_self()<<"Engines_Component_i::CpuUsed_impl _ThreadId "
      //     <<_ThreadId <<" "<<_serviceName<<" _StartUsed "<<_StartUsed<<endl;
    }
  return cpu ;
}


//=============================================================================
/*!
 *  C++ method: return Container Servant
 */
//=============================================================================

Engines_Container_i *Engines_Component_i::GetContainerPtr()
{
  PortableServer::ObjectId_var  contId=_poa->reference_to_id(_container);
  return dynamic_cast<Engines_Container_i*>(_poa->id_to_servant(contId)) ;
}

//=============================================================================
/*!
 *  C++ method: set study Id
 *  \param studyId         0 if instance is not associated to a study,
 *                         >0 otherwise (== study id)
 *  \return true if the set of study Id is OK
 *  must be set once by Container, at instance creation,
 *  and cannot be changed after.
 */
//=============================================================================

CORBA::Boolean Engines_Component_i::setStudyId(CORBA::Long studyId)
{
  ASSERT( studyId >= 0);
  CORBA::Boolean ret = false;
  if (_studyId < 0) // --- not yet initialized
    {
      _studyId = studyId;
      ret = true;
    }
  else
    if ( _studyId == studyId) ret = true;
  return ret;
}

//=============================================================================
/*!
 *  C++ method: return CORBA instance id, the id is set in derived class
 *  constructor, when instance is activated.
 */
//=============================================================================

PortableServer::ObjectId * Engines_Component_i::getId()
{
//  MESSAGE("PortableServer::ObjectId * Engines_Component_i::getId()");
  return _id ;
}

//=============================================================================
/*!
 *  C++ method: used by derived classes for supervision
 */
//=============================================================================

void Engines_Component_i::beginService(const char *serviceName)
{
  std::cerr << "beginService for " << serviceName << " Component instance : " << _instanceName << std::endl;

#ifndef WIN32
  _ThreadId = pthread_self() ;
#else
  _ThreadId = new pthread_t;
  _ThreadId->p = pthread_self().p ;
  _ThreadId->x = pthread_self().x ;
#endif
  _StartUsed = 0 ;
  _StartUsed = CpuUsed_impl() ;
  _ThreadCpuUsed = 0 ;
  _Executed = true ;
  _serviceName = serviceName ;
  theEngines_Component = this ;
  if ( pthread_setcanceltype( PTHREAD_CANCEL_ASYNCHRONOUS , NULL ) )
    {
      perror("pthread_setcanceltype ") ;
      exit(0) ;
    }
  if ( pthread_setcancelstate( PTHREAD_CANCEL_ENABLE , NULL ) )
    {
      perror("pthread_setcancelstate ") ;
      exit(0) ;
    }

  // --- all strings given with setProperties are set in environment
  std::map<std::string,CORBA::Any>::iterator it;
  for (it = _fieldsDict.begin(); it != _fieldsDict.end(); it++)
    {
      std::string cle((*it).first);
      if ((*it).second.type()->kind() == CORBA::tk_string)
        {
          const char* value;
          (*it).second >>= value;
          std::string s(cle);
          s+='=';
          s+=value;
          putenv((char *)s.c_str());
          MESSAGE("--- setenv: "<<cle<<" = "<< value);
        }
    }
}

//=============================================================================
/*!
 *  C++ method: used by derived classes for supervision
 */
//=============================================================================

void Engines_Component_i::endService(const char *serviceName)
{
  if ( !_CanceledThread )
    _ThreadCpuUsed = CpuUsed_impl() ;

  float cpus=_ThreadCpuUsed/1000.;
  std::cerr << "endService for " << serviceName << " Component instance : " << _instanceName ;
  std::cerr << " Cpu Used: " << cpus << " (s) " << std::endl;
  MESSAGE("Send EndService notification for " << serviceName
          << std::endl << " Component instance : " << _instanceName << " StartUsed "
          << _StartUsed << " _ThreadCpuUsed "<< _ThreadCpuUsed << std::endl <<std::endl);
  _ThreadId = 0 ;
}

//=============================================================================
/*!
 *  C++ method: -- CHECK IF USED --
 */
//=============================================================================

char* Engines_Component_i::graphName()
{
  return CORBA::string_dup( _graphName.c_str() ) ;
}

//=============================================================================
/*!
 *  C++ method: -- CHECK IF USED --
 */
//=============================================================================

char* Engines_Component_i::nodeName()
{
  return CORBA::string_dup( _nodeName.c_str() ) ;
}

//=============================================================================
/*!
 *  C++ method: used in Supervision (see kill_impl)
 */
//=============================================================================

bool Engines_Component_i::Killer( pthread_t ThreadId , int signum )
{
#ifndef WIN32
  if ( ThreadId )
#else
  if ( ThreadId.p )
#endif
    {
      if ( signum == 0 )
        {
          if ( pthread_cancel( ThreadId ) )
            {
              perror("Killer pthread_cancel error") ;
              return false ;
            }
          else
            {
#ifdef WIN32
              MESSAGE("Killer : ThreadId " << ThreadId.p << " pthread_canceled") ;
#else
              MESSAGE("Killer : ThreadId " << ThreadId << " pthread_canceled") ;
#endif
            }
        }
      else
        {
          if ( pthread_kill( ThreadId , signum ) == -1 )
            {
              perror("Killer pthread_kill error") ;
              return false ;
            }
          else
            {
#ifdef WIN32
              MESSAGE("Killer : ThreadId " << ThreadId.p << " pthread_killed(" << signum << ")") ;
#else
              MESSAGE("Killer : ThreadId " << ThreadId << " pthread_killed(" << signum << ")") ;
#endif
            }
        }
    }
  return true ;
}

void SetCpuUsed();
void CallCancelThread();

//=============================================================================
/*!
 *  C++ method:
 */
//=============================================================================

void SetCpuUsed()
{
  if ( theEngines_Component )
    theEngines_Component->SetCurCpu() ;
}

//=============================================================================
/*!
 *  C++ method:
 */
//=============================================================================

void Engines_Component_i::SetCurCpu()
{
  _ThreadCpuUsed =  CpuUsed() ;
  //  MESSAGE(pthread_self() <<
  //  " Engines_Component_i::SetCurCpu() _ThreadCpuUsed " << _ThreadCpuUsed) ;
}

//=============================================================================
/*!
 *  C++ method:
 */
//=============================================================================

long Engines_Component_i::CpuUsed()
{
  long cpu = 0 ;
#ifndef WIN32
  struct rusage usage ;
  if ( _ThreadId || _Executed )
    {
      if ( getrusage( RUSAGE_SELF , &usage ) == -1 )
        {
          perror("Engines_Component_i::CpuUsed") ;
          return 0 ;
        }
      //cpu time is calculated in millisecond (user+system times)
      cpu = usage.ru_utime.tv_sec*1000 +usage.ru_utime.tv_usec/1000;
      cpu = cpu+ usage.ru_stime.tv_sec*1000 +usage.ru_stime.tv_usec/1000;
      cpu=cpu-_StartUsed ;
      // std::cout << pthread_self() << " Engines_Component_i::CpuUsed " << " "
      //      << _serviceName   << usage.ru_utime.tv_sec << " - " << _StartUsed
      //      << " = " << cpu << std::endl ;
    }
  else
    {
      // std::cout << pthread_self() << "Engines_Component_i::CpuUsed _ThreadId "
      //      << _ThreadId << " " << _serviceName<< " _StartUsed "
      //      << _StartUsed << std::endl ;
    }
#else
        // NOT implementet yet
#endif


  return cpu ;
}

void CallCancelThread()
{
  if ( theEngines_Component )
    theEngines_Component->CancelThread() ;
}

//=============================================================================
/*!
 *  C++ method:
 */
//=============================================================================

void Engines_Component_i::CancelThread()
{
  _CanceledThread = true;
}

//=============================================================================
/*!
 *  C++ method: Send message to event channel
 */
//=============================================================================

void Engines_Component_i::sendMessage(const char *event_type,
                                      const char *message)
{
    _notifSupplier->Send(_graphName.c_str(), _nodeName.c_str(), event_type, message);
}

//=============================================================================
/*!
 *  C++ method: return standard library name built on component name
 */
//=============================================================================

std::string Engines_Component_i::GetDynLibraryName(const char *componentName)
{
#ifndef WIN32
  std::string ret="lib";
  ret+=componentName;
  ret+="Engine.so";
#else
  std::string ret=componentName;
  ret+="Engine.dll";
#endif
  return ret;
}

//=============================================================================
/*!
 *  C++ method: DumpPython default implementation
 */
//=============================================================================

Engines::TMPFile* Engines_Component_i::DumpPython(CORBA::Object_ptr theStudy,
                                                  CORBA::Boolean isPublished,
                                                  CORBA::Boolean isMultiFile,
                                                  CORBA::Boolean& isValidScript)
{
  const char* aScript = isMultiFile ? "def RebuildData(theStudy): pass" : "";
  char* aBuffer = new char[strlen(aScript)+1];
  strcpy(aBuffer, aScript);
  CORBA::Octet* anOctetBuf =  (CORBA::Octet*)aBuffer;
  int aBufferSize = strlen(aBuffer)+1;
  Engines::TMPFile_var aStreamFile = new Engines::TMPFile(aBufferSize, aBufferSize, anOctetBuf, 1);
  isValidScript = true;
  return aStreamFile._retn();
}

Engines::Salome_file_ptr
Engines_Component_i::getInputFileToService(const char* service_name,
                                           const char* Salome_file_name)
{
  // Try to find the service, if it doesn't exist, we throw an exception.
  _Service_file_map_it = _Input_Service_file_map.find(service_name);
  if (_Service_file_map_it ==  _Input_Service_file_map.end()) {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    es.text = "service doesn't have salome files";
    throw SALOME::SALOME_Exception(es);
  }
  _t_Salome_file_map * _map = _Input_Service_file_map[service_name];

  // Try to find the Salome_file ...
  _Salome_file_map_it = _map->find(Salome_file_name);
  if (_Salome_file_map_it ==  _map->end()) {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    es.text = "service doesn't have this Salome_file";
    throw SALOME::SALOME_Exception(es);
  }
  Salome_file_i * Sfile = (*_map)[Salome_file_name];

  return Sfile->_this();
}

Engines::Salome_file_ptr
Engines_Component_i::setInputFileToService(const char* service_name,
                                           const char* Salome_file_name)
{
  // Try to find the service, if it doesn't exist, we add it.
  _Service_file_map_it = _Input_Service_file_map.find(service_name);
  if (_Service_file_map_it ==  _Input_Service_file_map.end()) {
    _t_Salome_file_map * _map = new _t_Salome_file_map();
    _Input_Service_file_map[service_name] = _map;
  }
  _t_Salome_file_map * _map = _Input_Service_file_map[service_name];

  // Try to find the Salome_file ...
  _Salome_file_map_it = _map->find(Salome_file_name);
  if (_Salome_file_map_it ==  _map->end()) {
    Salome_file_i * Sfile = new Salome_file_i();
    Engines::Container_ptr container = this->GetContainerRef();
    Sfile->setContainer(Engines::Container::_duplicate(container));
    (*_map)[Salome_file_name] = Sfile;
  }

  Salome_file_i * Sfile = (*_map)[Salome_file_name];
  return Sfile->_this();
}

void
Engines_Component_i::checkInputFilesToService(const char* service_name)
{
  // Try to find the service, if it doesn't exist, nothing to do.
  _Service_file_map_it = _Input_Service_file_map.find(service_name);
  if (_Service_file_map_it !=  _Input_Service_file_map.end()) {
    _t_Salome_file_map * _map = _Input_Service_file_map[service_name];
    _t_Salome_file_map::iterator begin = _map->begin();
    _t_Salome_file_map::iterator end = _map->end();

    for(;begin!=end;begin++) {
      Salome_file_i * file = begin->second;
      std::string file_port_name = begin->first;
      configureSalome_file(service_name, file_port_name, file);
      file->recvFiles();
    }
  }
}

Engines::Salome_file_ptr
Engines_Component_i::getOutputFileToService(const char* service_name,
                                            const char* Salome_file_name)
{
  // Try to find the service, if it doesn't exist, we throw an exception.
  _Service_file_map_it = _Output_Service_file_map.find(service_name);
  if (_Service_file_map_it ==  _Output_Service_file_map.end()) {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    es.text = "service doesn't have salome files";
    throw SALOME::SALOME_Exception(es);
  }
  _t_Salome_file_map * _map = _Output_Service_file_map[service_name];

  // Try to find the Salome_file ...
  _Salome_file_map_it = _map->find(Salome_file_name);
  if (_Salome_file_map_it ==  _map->end()) {
    SALOME::ExceptionStruct es;
    es.type = SALOME::INTERNAL_ERROR;
    es.text = "service doesn't have this Salome_file";
    throw SALOME::SALOME_Exception(es);
  }
  Salome_file_i * Sfile = (*_map)[Salome_file_name];

  return Sfile->_this();
}

Engines::Salome_file_ptr
Engines_Component_i::setOutputFileToService(const char* service_name,
                                           const char* Salome_file_name)
{
  // Try to find the service, if it doesn't exist, we add it.
  _Service_file_map_it = _Output_Service_file_map.find(service_name);
  if (_Service_file_map_it ==  _Output_Service_file_map.end()) {
    _t_Salome_file_map * _map = new _t_Salome_file_map();
    _Output_Service_file_map[service_name] = _map;
  }
  _t_Salome_file_map * _map = _Output_Service_file_map[service_name];

  // Try to find the Salome_file ...
  _Salome_file_map_it = _map->find(Salome_file_name);
  if (_Salome_file_map_it ==  _map->end()) {
    Salome_file_i * Sfile = new Salome_file_i();
    Engines::Container_ptr container = this->GetContainerRef();
    Sfile->setContainer(Engines::Container::_duplicate(container));
    (*_map)[Salome_file_name] = Sfile;
  }

  Salome_file_i * Sfile = (*_map)[Salome_file_name];
  return Sfile->_this();
}

void
Engines_Component_i::checkOutputFilesToService(const char* service_name)
{
  // Try to find the service, if it doesn't exist, nothing to do.
  _Service_file_map_it = _Output_Service_file_map.find(service_name);
  if (_Service_file_map_it !=  _Output_Service_file_map.end()) {
    _t_Salome_file_map * _map = _Output_Service_file_map[service_name];
    _t_Salome_file_map::iterator begin = _map->begin();
    _t_Salome_file_map::iterator end = _map->end();

    for(;begin!=end;begin++) {
      Salome_file_i * file = begin->second;
      std::string file_port_name = begin->first;
      configureSalome_file(service_name, file_port_name, file);
      file->recvFiles();
    }
  }

}

//=============================================================================
/*!
 *  C++ method: used to configure the Salome_file into the runtime.
 *  \param service_name name of the service that use this Salome_file
 *  \param file_port_name name of the Salome_file
 *  \param file Salome_file C++ object
 */
//=============================================================================
void
Engines_Component_i::configureSalome_file(std::string service_name,
                                          std::string file_port_name,
                                          Salome_file_i * file)
{
  // By default this method does nothing
}

//=============================================================================
/*!
 *  C++ method: return the name of the container associated with this component
 *  This name does not contains the "/Containers" string and all "/" are replaced by "_"
 *  \return the container name (reformatted)
 */
//=============================================================================
std::string Engines_Component_i::getContainerName()
{
  return _containerName;
}
//=============================================================================
/*!
 *  C++ method: set the name of the container associated with this component (attribute _containerName)
 *  This name does not contains the "/Containers" string and all "/" are replaced by "_"
 *  \return the container name (reformatted)
 */
//=============================================================================
void Engines_Component_i::setContainerName()
{
  CORBA::String_var containerName=_container->name();
  std::string name(containerName);
  name.erase(0,12);
  std::string::size_type slash =name.find_first_of('/');
  if(slash != std::string::npos)
    name[slash]='_';
  _containerName=name;
}

//=============================================================================
/*!
  \brief Get version of the component

  This method is supposed to be implemented in all derived classes; default implementation
  returns empty string that means that no version information about the component is available.

  \note The version of the component is stored to the study, as a part of general persistence
  mechanism; once stored, version information in the study cannot be changed.

  \return string containing component's version, e.g. "1.0"
*/
char* Engines_Component_i::getVersion()
{
  return CORBA::string_dup( "" );
}
