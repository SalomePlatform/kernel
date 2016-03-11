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
//  File   : SALOME_ParallelContainer_i.hxx
//  Author : André RIBES, EDF

#ifndef _SALOME_PARALLEL_CONTAINER_I_HXX_
#define _SALOME_PARALLEL_CONTAINER_I_HXX_

#include <SALOMEconfig.h>

#include "SALOME_PACOExtensionPaCO_Engines_PACO_Container_server.hxx"
#include "SALOME_ParallelGlobalProcessVar_i.hxx"

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <sys/types.h>
#include <omnithread.h>
#include <map>
#include <string>

class SALOME_NamingService;

#ifdef WIN32
# if defined CONTAINER_EXPORTS || defined SalomeParallelContainer_EXPORTS
#  define CONTAINER_EXPORT __declspec( dllexport )
# else
#  define CONTAINER_EXPORT __declspec( dllimport )
# endif
#else
# define CONTAINER_EXPORT
#endif

class CONTAINER_EXPORT Engines_Parallel_Container_i:
  virtual public Engines::PACO_Container_serv,
  virtual public ParallelGlobalProcessVar_i,
  virtual public PortableServer::RefCountServantBase
{
public:
  Engines_Parallel_Container_i(CORBA::ORB_ptr orb, 
                               char * ior, 
                               int rank,
                               PortableServer::POA_ptr poa,
                               std::string containerName ,
                               bool isServantAloneInProcess = true);

  virtual ~Engines_Parallel_Container_i();

  // --- CORBA methods

  virtual bool load_component_Library(const char* componentName, CORBA::String_out reason);

  virtual Engines::EngineComponent_ptr
  create_component_instance( const char* componentName,
                             CORBA::Long studyId); // 0 for multiStudy

  virtual Engines::EngineComponent_ptr
  create_component_instance_env( const char* componentName,
                                 CORBA::Long studyId,          // 0 for multiStudy
                                 const Engines::FieldsDict& env,
                                 CORBA::String_out reason);

  virtual char*
  create_python_service_instance(const char* CompName,
                                 CORBA::String_out reason);

  Engines::EngineComponent_ptr
  find_component_instance( const char* registeredName,
                           CORBA::Long studyId); // 0 for multiStudy

  Engines::EngineComponent_ptr
  load_impl(const char* nameToRegister,
            const char* componentName);

  void
  create_paco_component_node_instance(const char* componentName,
                                      const char* proxy_containerName,
                                      CORBA::Long studyId); // 0 for multiStudy

  void updateInstanceNumber();

  void remove_impl(Engines::EngineComponent_ptr component_i);
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
  Engines::EngineComponent_ptr
  find_or_create_instance(std::string genericRegisterName);

  Engines::EngineComponent_ptr
  createCPPInstance(std::string genericRegisterName,
                    void *handle,
                    int studyId);

  Engines::EngineComponent_ptr
  createPythonInstance(std::string genericRegisterName,
                       int studyId);

  static bool isPythonContainer(const char* ContainerName);
  static void decInstanceCnt(std::string genericRegisterName);

  // --- needed for parallel components, Numerical Platon
  void set_id(PortableServer::ObjectId * id) { _id = id;}

  Engines::fileRef_ptr createFileRef(const char* origFileName);
  Engines::fileTransfer_ptr getFileTransfer();

  virtual Engines::Salome_file_ptr createSalome_file(const char* origFileName);
  void copyFile(Engines::Container_ptr container, const char* remoteFile, const char* localFile);
  Engines::PyNode_ptr createPyNode(const char* nodeName, const char* code);
  Engines::PyNode_ptr getDefaultPyNode(const char *nodeName);
  Engines::PyScriptNode_ptr createPyScriptNode(const char* nodeName, const char* code);
  Engines::PyScriptNode_ptr getDefaultPyScriptNode(const char *nodeName);
protected:

  SALOME_NamingService *_NS;
  std::string _hostname;
  std::string _library_path;
  std::string _containerName;
  std::string _logfilename;
  CORBA::ORB_var _orb;
  PortableServer::POA_var _poa;
  PortableServer::ObjectId * _id;
  int _numInstance;
  CORBA::Long   _pid;
  bool   _isServantAloneInProcess;
  Engines::fileTransfer_var _fileTransfer;

  typedef std::map<std::string,Engines::EngineComponent_var> _listInstances_map_t;
  typedef std::map<std::string,Engines::fileRef_var> _fileRef_map_t;
  typedef std::map<std::string,Engines::Salome_file_var> _Salome_file_map_t;
  _listInstances_map_t _listInstances_map;
  _fileRef_map_t _fileRef_map;
  _Salome_file_map_t _Salome_file_map;

  // Cette map contient les references vers les différentes
  // instances d'objets parallèles.
  std::map<std::string, PortableServer::ObjectId *> _par_obj_inst_map;

  typedef  PortableServer::ObjectId * (*FACTORY_FUNCTION) (CORBA::ORB_ptr, char *, int,
                                                           PortableServer::POA_ptr, 
                                                           PortableServer::ObjectId *, 
                                                           const char *, 
                                                           const char *);
};

#endif
