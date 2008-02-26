//  SALOME_ParallelContainer : implementation of container and engine for Parallel Kernel
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
//  File   : SALOME_ParallelContainer_i.hxx
//  Author : André RIBES, EDF
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA

#ifndef _SALOME_PARALLEL_CONTAINER_I_HXX_
#define _SALOME_PARALLEL_CONTAINER_I_HXX_

#include <SALOMEconfig.h>

#include "SALOME_ComponentPaCO_Engines_Container_server.h"

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#ifndef WNT
#include <unistd.h>
#endif
#include <sys/types.h>
#include <omnithread.h>
#include <map>
#include <string>

class SALOME_NamingService;


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

class CONTAINER_EXPORT Engines_Parallel_Container_i:
  /*  public virtual POA_Engines::Container, */
  public Engines::Container_serv,
  public virtual PortableServer::RefCountServantBase
{
public:
  Engines_Parallel_Container_i(CORBA::ORB_ptr orb, char * ior, int rank);
  Engines_Parallel_Container_i(CORBA::ORB_ptr orb, char * ior, int rank,
		      PortableServer::POA_ptr poa,
		      char * containerName ,
                      int argc, char* argv[],
		      bool activAndRegist = true,
		      bool isServantAloneInProcess = true);
  virtual ~Engines_Parallel_Container_i();

  // --- CORBA methods

  virtual bool load_component_Library(const char* componentName);

  virtual Engines::Component_ptr
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

  virtual void ping();
  char* name();
  char* workingdir();
  char* logfilename();
  void logfilename(const char* name);

  virtual void Shutdown();
  char* getHostName();
  CORBA::Long getPID();
  //! Kill current container
  bool Kill_impl() ;

  // --- local C++ methods

  Engines::Component_ptr
  find_or_create_instance( std::string genericRegisterName,
			   std::string componentLibraryName);

  Engines::Component_ptr
  createInstance(std::string genericRegisterName,
		 void *handle,
		 int studyId);

  Engines::Component_ptr
  createParallelInstance(std::string genericRegisterName,
			 void *handle,
			 int studyId);

  static bool isPythonContainer(const char* ContainerName);
  static void decInstanceCnt(std::string genericRegisterName);

  // --- needed for parallel components, Numerical Platon

  int getArgc() { return _argc; }
  char **getArgv() { return _argv; }

  void set_id(PortableServer::ObjectId * id) { _id = id;}

  Engines::fileRef_ptr createFileRef(const char* origFileName);
  Engines::fileTransfer_ptr getFileTransfer();

  virtual Engines::Salome_file_ptr createSalome_file(const char* origFileName);
protected:

  static std::map<std::string, int> _cntInstances_map;
  static std::map<std::string, void *> _library_map; // library names, loaded
  static std::map<std::string, void *> _toRemove_map;// library names to remove
  static omni_mutex _numInstanceMutex ; // lib and instance protection

  bool _isSupervContainer;

  SALOME_NamingService *_NS ;
  std::string _library_path;
  std::string _containerName;
  std::string _logfilename;
  CORBA::ORB_var _orb;
  PortableServer::POA_var _poa;
  PortableServer::ObjectId * _id ;
  int _numInstance ;
  std::map<std::string,Engines::Component_var> _listInstances_map;
  std::map<std::string,Engines::fileRef_var> _fileRef_map;
  std::map<std::string,Engines::Salome_file_var> _Salome_file_map;
  Engines::fileTransfer_var _fileTransfer;

  int    _argc ;
  char** _argv ;
  long   _pid;
  bool   _isServantAloneInProcess;
};

#endif

