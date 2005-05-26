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
//  File   : SALOME_Container_i.hxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$

#ifndef _SALOME_CONTAINER_I_HXX_
#define _SALOME_CONTAINER_I_HXX_

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <omnithread.h>
#include <map>
#include <string>

class SALOME_NamingService;

class Engines_Container_i: public virtual POA_Engines::Container,
			   public virtual PortableServer::RefCountServantBase
{
public:
  Engines_Container_i();
  Engines_Container_i(CORBA::ORB_ptr orb, 
		      PortableServer::POA_ptr poa,
		      char * containerName ,
                      int argc, char* argv[],
		      bool activAndRegist = true,
		      bool isServantAloneInProcess = true);
  virtual ~Engines_Container_i();

  // --- CORBA methods

  bool load_component_Library(const char* componentName);

  Engines::Component_ptr
  create_component_instance( const char* componentName,
			     CORBA::Long studyId); // 0 for multiStudy

  Engines::Component_ptr
  find_component_instance( const char* registeredName,
			   CORBA::Long studyId); // 0 for multiStudy

  Engines::Component_ptr
  load_impl(const char* nameToRegister,
	    const char* componentName);


  void remove_impl(Engines::Component_ptr component_i);
  void finalize_removal();
  void Shutdown();

  char* getHostName();
  CORBA::Long getPID();
  char* name();
  void ping();

  bool Kill_impl() ;

  //char* machineName();
  //Engines::Component_ptr instance(const char* nameToRegister,
  //				   const char* componentName);

  // --- local C++ methods

  Engines::Component_ptr
  find_or_create_instance( std::string genericRegisterName,
			   std::string componentLibraryName);

  Engines::Component_ptr
  createInstance(std::string genericRegisterName,
		 void *handle,
		 int studyId);

  static bool isPythonContainer(const char* ContainerName);
  static std::string BuildContainerNameForNS(const char *ContainerName,
					     const char *hostname);
  static void decInstanceCnt(std::string genericRegisterName);

protected:

  static const char *_defaultContainerName;
  static std::map<std::string, int> _cntInstances_map;
  static std::map<std::string, void *> _library_map; // library names, loaded
  static std::map<std::string, void *> _toRemove_map;// library names to remove
  static omni_mutex _numInstanceMutex ; // lib and instance protection

  bool _isSupervContainer;

  SALOME_NamingService *_NS ;
  std::string _library_path;
  std::string _containerName;
  CORBA::ORB_var _orb;
  PortableServer::POA_var _poa;
  PortableServer::ObjectId * _id ;
  int _numInstance ;
  std::map<std::string,Engines::Component_var> _listInstances_map;

  //private: 

  int    _argc ;
  char** _argv ;
  long   _pid;
  bool   _isServantAloneInProcess;
};

#endif

