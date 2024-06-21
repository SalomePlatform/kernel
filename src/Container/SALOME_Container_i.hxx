// Copyright (C) 2007-2024  CEA, EDF, OPEN CASCADE
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
//  File   : SALOME_Container_i.hxx
//  Author : Paul RASCLE, EDF - MARC TAJCHMAN, CEA
//  Module : SALOME
//  $Header$
//
#ifndef _SALOME_CONTAINER_I_HXX_
#define _SALOME_CONTAINER_I_HXX_

#include "SALOME_Container.hxx"
#include "Utils_Mutex.hxx"

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)
#include CORBA_SERVER_HEADER(SALOME_PyNode)

#include <iostream>
#include <signal.h>
#include <stdlib.h>
#ifndef WIN32
#include <unistd.h>
#endif
#include <sys/types.h>
#include <omnithread.h>
#include <map>
#include <list>
#include <string>
#include <vector>

class SALOME_NamingService_Container_Abstract;

class CONTAINER_EXPORT Abstract_Engines_Container_i : public virtual POA_Engines::Container,
                                                      public virtual PortableServer::ServantBase
{
public:
  Abstract_Engines_Container_i();
  Abstract_Engines_Container_i(const std::string& pyContainerClsName,
                               CORBA::ORB_ptr orb,
                               PortableServer::POA_ptr poa,
                               char *containerName,
                               int argc, char *argv[],
                               SALOME_NamingService_Container_Abstract *ns = nullptr,
                               bool isServantAloneInProcess = true);
  virtual ~Abstract_Engines_Container_i();

  virtual bool isSSLMode() const = 0;

 std::string getPyContainerClassName() const { return _py_container_name; }

  // --- CORBA methods

  virtual bool load_component_Library(const char *componentName, CORBA::String_out reason);

  virtual Engines::EngineComponent_ptr
  create_component_instance(const char *componentName);

  void override_environment_python( const Engines::FieldsDict& env ) override;

  void override_environment( const Engines::FieldsDict& env ) override;

  Engines::FieldsDict *get_os_environment() override;

  void set_big_obj_on_disk_threshold(CORBA::Long thresholdInByte) override;

  void set_big_obj_on_disk_directory(const char *directory) override;

  void set_number_of_retry(CORBA::Long nbRetry) override;

  void set_directory_for_replay_files(const char *directory) override;
  
  void set_startup_code(const char *codeAtStartUp) override;

  char *get_startup_code() override;

  void addLogFileNameGroup(const Engines::vectorOfString& groupOfLogFileNames) override;
    
  Engines::vectorOfVectorOfString *getAllLogFileNameGroups() override;
  
  void execute_python_code(const char *code) override;

  virtual Engines::EngineComponent_ptr
  create_component_instance_env(const char *componentName,
                                const Engines::FieldsDict &env,
                                CORBA::String_out reason);

  virtual char *
  create_python_service_instance(const char *CompName,
                                 CORBA::String_out reason);

  Engines::EngineComponent_ptr
  find_component_instance(const char *registeredName);

  Engines::EngineComponent_ptr
  load_impl(const char *nameToRegister,
            const char *componentName);

  bool is_SSL_mode() override { return this->isSSLMode(); }

  Engines::EmbeddedNamingService_ptr get_embedded_NS_if_ssl() override;

  SALOME_NamingService_Container_Abstract *getNS() const { return _NS; }

  void remove_impl(Engines::EngineComponent_ptr component_i);
  void finalize_removal();

  virtual void ping();
  CORBA::Long getNumberOfCPUCores();
  Engines::vectorOfDouble* loadOfCPUCores();
  void setPyScriptForCPULoad(const char *script);
  void resetScriptForCPULoad();
  CORBA::Long getTotalPhysicalMemory();
  CORBA::Long getTotalPhysicalMemoryInUse();
  CORBA::Long getTotalPhysicalMemoryInUseByMe();
  char *name();
  char *workingdir();
  char *logfilename() override;
  void logfilename(const char *name) override;
  char *locallogfilename() override;
  void locallogfilename(const char *name) override;
  CORBA::Long monitoringtimeresms() override;
  void monitoringtimeresms(CORBA::Long intervalInMs) override;
  void verbosity(bool& activated, CORBA::String_out level) override;
  void setVerbosity(bool activated, const char *level) override;

  void Shutdown() override;
  void ShutdownNow() override;
  char *getHostName();
  CORBA::Long getPID();
  //! Kill current container
  bool Kill_impl();

  Engines::fileRef_ptr createFileRef(const char *origFileName);
  Engines::fileTransfer_ptr getFileTransfer();

  virtual Engines::Salome_file_ptr createSalome_file(const char *origFileName);
  void copyFile(Engines::Container_ptr container, const char *remoteFile, const char *localFile);
  Engines::PyNode_ptr createPyNode(const char *nodeName, const char *code);
  Engines::PyNode_ptr getDefaultPyNode(const char *nodeName);
  Engines::PyScriptNode_ptr createPyScriptNode(const char *nodeName, const char *code);
  void removePyScriptNode(const char *nodeName) override;
  void cleanAllPyScripts() override;
  Engines::PyScriptNode_ptr getDefaultPyScriptNode(const char *nodeName);
  // --- local C++ methods

  Engines::EngineComponent_ptr
  find_or_create_instance(std::string genericRegisterName,
                          std::string componentLibraryName);

  bool load_component_CppImplementation(const char *componentName, std::string &reason);
  bool load_component_PythonImplementation(const char *componentName, std::string &reason);
  bool load_component_ExecutableImplementation(const char *componentName, std::string &reason);

  Engines::EngineComponent_ptr createPythonInstance(std::string CompName, std::string &error);
  Engines::EngineComponent_ptr createExecutableInstance(std::string CompName, const Engines::FieldsDict &env, std::string &error);
  Engines::EngineComponent_ptr createInstance(std::string genericRegisterName, void *handle, std::string &error);

  static bool isPythonContainer(const char *ContainerName);
  static void decInstanceCnt(std::string genericRegisterName);
  //??? char* machineName();

  // --- needed for parallel components, Numerical Platon

  int getArgc() { return _argc; }
  char **getArgv() { return _argv; }

  void registerTemporaryFile(const std::string &fileName);
  void unregisterTemporaryFile(const std::string &fileName);
  void clearTemporaryFiles();
  PortableServer::ObjectId *getCORBAId() const { return _id; }
public:
  static const int DFT_TIME_INTERVAL_BTW_MEASURE;
private:
  void ShutdownCommonPart();
protected:
  static std::map<std::string, int> _cntInstances_map;
  static std::map<std::string, void *> _library_map;  // library names, loaded
  static std::map<std::string, void *> _toRemove_map; // library names to remove
  static omni_mutex _numInstanceMutex;                // lib and instance protection

  bool _isSupervContainer;

  SALOME_NamingService_Container_Abstract *_NS;
  std::string _library_path;
  std::string _containerName;
  std::string _logfilename;
  std::string _localfilename;
  std::string _load_script;
  std::string _py_container_name;
  std::string _code_at_startup;
  CORBA::ORB_var _orb;
  PortableServer::POA_var _poa;
  PortableServer::ObjectId *_id;
  int _numInstance;
  std::map<std::string, Engines::EngineComponent_var> _listInstances_map;
  std::map<std::string, Engines::fileRef_var> _fileRef_map;
  std::map<std::string, Engines::Salome_file_var> _Salome_file_map;
  std::map<std::string, Engines::PyScriptNode_var> _dftPyScriptNode;
  std::map<std::string, Engines::PyNode_var> _dftPyNode;
  Utils_Mutex _mutexForDftPy;
  std::list<std::string> _tmp_files;
  Engines::fileTransfer_var _fileTransfer;
  std::vector< std::vector<std::string> > _groups_of_log_files;

  int _argc;
  char **_argv;
  long _pid;
  bool _isServantAloneInProcess;
};

constexpr char PY_CONTAINER_CLS_NAME_IN_PROCESS[] = "SALOME_Container_i";
constexpr char PY_CONTAINER_CLS_NAME_OUT_PROCESS_NO_REPLAY[] = "SALOME_Container_OutOfProcess_i";
constexpr char PY_CONTAINER_CLS_NAME_OUT_PROCESS_WITH_REPLAY[] = "SALOME_Container_OutOfProcess_Replay_i";
constexpr char PY_CONTAINER_CLS_NAME_OUT_PROCESS_NO_REPLAY_FT[] = "SALOME_Container_OutOfProcess_FT_i";
constexpr char PY_CONTAINER_CLS_NAME_OUT_PROCESS_WITH_REPLAY_FT[] = "SALOME_Container_OutOfProcess_Replay_FT_i";

class CONTAINER_EXPORT Engines_Container_i : public Abstract_Engines_Container_i
{
public:
  Engines_Container_i() { }
  Engines_Container_i(CORBA::ORB_ptr orb,
                      PortableServer::POA_ptr poa,
                      char *containerName,
                      int argc, char *argv[],
                      SALOME_NamingService_Container_Abstract *ns = nullptr,
                      bool isServantAloneInProcess = true) :
                      Abstract_Engines_Container_i(PY_CONTAINER_CLS_NAME_IN_PROCESS, orb, poa, containerName, argc, argv, ns, isServantAloneInProcess) {}
  bool isSSLMode() const override { return false; }
};

class CONTAINER_EXPORT Abstract_Engines_Container_SSL_i : public Abstract_Engines_Container_i
{
public:
  Abstract_Engines_Container_SSL_i(const std::string& pyContainerClsName,
                          CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa,
                          char *containerName,
                          int argc, char *argv[],
                          SALOME_NamingService_Container_Abstract *ns = nullptr,
                          bool isServantAloneInProcess = true) :
                          Abstract_Engines_Container_i(pyContainerClsName, orb, poa, containerName, argc, argv, ns, isServantAloneInProcess) {}
    bool isSSLMode() const override { return true; }
};

class CONTAINER_EXPORT Engines_Container_SSL_i : public Abstract_Engines_Container_SSL_i
{
public:
  Engines_Container_SSL_i(CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa,
                          char *containerName,
                          int argc, char *argv[],
                          SALOME_NamingService_Container_Abstract *ns = nullptr,
                          bool isServantAloneInProcess = true) :
                          Abstract_Engines_Container_SSL_i(PY_CONTAINER_CLS_NAME_IN_PROCESS, orb, poa, containerName, argc, argv, ns, isServantAloneInProcess) {}
};

class CONTAINER_EXPORT Engines_Container_SSL_OutOfProcess_i : public Abstract_Engines_Container_SSL_i
{
public:
  Engines_Container_SSL_OutOfProcess_i(CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa,
                          char *containerName,
                          int argc, char *argv[],
                          SALOME_NamingService_Container_Abstract *ns = nullptr,
                          bool isServantAloneInProcess = true) :
                          Abstract_Engines_Container_SSL_i(PY_CONTAINER_CLS_NAME_OUT_PROCESS_NO_REPLAY, orb, poa, containerName, argc, argv, ns, isServantAloneInProcess) {}
};

class CONTAINER_EXPORT Engines_Container_SSL_OutOfProcess_Replay_i : public Abstract_Engines_Container_SSL_i
{
public:
  Engines_Container_SSL_OutOfProcess_Replay_i(CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa,
                          char *containerName,
                          int argc, char *argv[],
                          SALOME_NamingService_Container_Abstract *ns = nullptr,
                          bool isServantAloneInProcess = true) :
                          Abstract_Engines_Container_SSL_i(PY_CONTAINER_CLS_NAME_OUT_PROCESS_WITH_REPLAY, orb, poa, containerName, argc, argv, ns, isServantAloneInProcess) {}
};

class CONTAINER_EXPORT Engines_Container_SSL_OutOfProcess_FT_i : public Abstract_Engines_Container_SSL_i
{
public:
  Engines_Container_SSL_OutOfProcess_FT_i(CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa,
                          char *containerName,
                          int argc, char *argv[],
                          SALOME_NamingService_Container_Abstract *ns = nullptr,
                          bool isServantAloneInProcess = true) :
                          Abstract_Engines_Container_SSL_i(PY_CONTAINER_CLS_NAME_OUT_PROCESS_NO_REPLAY_FT, orb, poa, containerName, argc, argv, ns, isServantAloneInProcess) {}
};

class CONTAINER_EXPORT Engines_Container_SSL_OutOfProcess_Replay_FT_i : public Abstract_Engines_Container_SSL_i
{
public:
  Engines_Container_SSL_OutOfProcess_Replay_FT_i(CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr poa,
                          char *containerName,
                          int argc, char *argv[],
                          SALOME_NamingService_Container_Abstract *ns = nullptr,
                          bool isServantAloneInProcess = true) :
                          Abstract_Engines_Container_SSL_i(PY_CONTAINER_CLS_NAME_OUT_PROCESS_WITH_REPLAY_FT, orb, poa, containerName, argc, argv, ns, isServantAloneInProcess) {}
};

/*!
 * Methods to be used in SSL mode to skip NS.
 */
namespace KERNEL
{
  CONTAINER_EXPORT Abstract_Engines_Container_SSL_i *getContainerSA();
  CONTAINER_EXPORT Engines::Container_var getContainerRefSA();
} // namespace KERNEL

#endif
