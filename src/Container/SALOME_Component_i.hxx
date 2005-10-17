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
//  File   : SALOME_Component_i.hxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$

#ifndef _SALOME_COMPONENT_I_HXX_
#define _SALOME_COMPONENT_I_HXX_

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#ifndef WNT
#include <unistd.h>
#endif
#include <sys/types.h>
#include <string>
#include <map>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)
#include "NOTIFICATION.hxx"

class RegistryConnexion;
class Engines_Container_i;

#if defined CONTAINER_EXPORTS
#if defined WIN32
#define CONTAINER_EXPORT __declspec( dllexport )
#else
#define CONTAINER_EXPORT
#endif
#else
#if defined WNT
#define CONTAINER_EXPORT __declspec( dllimport )
#else
#define CONTAINER_EXPORT
#endif
#endif

class CONTAINER_EXPORT Engines_Component_i: 
  public virtual POA_Engines::Component,
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

  // --- CORBA methods

  char* instanceName();
  char* interfaceName();

  void ping();
  void destroy();

  CORBA::Long getStudyId();
  Engines::Container_ptr GetContainerRef();

  void setProperties(const Engines::FieldsDict& dico);
  Engines::FieldsDict* getProperties();

  void Names( const char * graphName , const char * nodeName ) ;
  bool Kill_impl();
  bool Stop_impl();
  bool Suspend_impl();
  bool Resume_impl();
  CORBA::Long CpuUsed_impl() ;

 virtual Engines::TMPFile* DumpPython(CORBA::Object_ptr theStudy,
				      CORBA::Boolean isPublished,
				      CORBA::Boolean& isValidScript);


  // --- local C++ methods

  PortableServer::ObjectId * getId(); 
  Engines_Container_i *GetContainerPtr();

  bool setStudyId(CORBA::Long studyId);
  static bool isMultiStudy();
  static bool isMultiInstance();
  static std::string GetDynLibraryName(const char *componentName);

  void beginService(const char *serviceName);
  void endService(const char *serviceName);
  void sendMessage(const char *event_type, const char *message);
  char * graphName() ;
  char * nodeName() ;
  bool Killer( pthread_t ThreadId , int signum );
  void SetCurCpu() ;
  long CpuUsed() ;

protected:
  int _studyId; // -1: not initialised; 0: multiStudy; >0: study
  static bool _isMultiStudy;
  static bool _isMultiInstance;

  std::string _instanceName ;
  std::string _interfaceName ;

  CORBA::ORB_ptr _orb;
  PortableServer::POA_ptr _poa;
  PortableServer::ObjectId * _id;
  PortableServer::ObjectId * _contId;
  Engines_Component_i * _thisObj ;
  RegistryConnexion *_myConnexionToRegistry;
  NOTIFICATION_Supplier* _notifSupplier;
  std::map<std::string,CORBA::Any>_fieldsDict;

  std::string _serviceName ;
  std::string _graphName ;
  std::string _nodeName ;

private:
#ifndef WNT
  pthread_t _ThreadId ;
#else
  pthread_t* _ThreadId ;
#endif
  long      _StartUsed ;
  long      _ThreadCpuUsed ;
  bool      _Executed ;
};

#endif
