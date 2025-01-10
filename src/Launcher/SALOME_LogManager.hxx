// Copyright (C) 2024-2025  CEA, EDF
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

#pragma once

#include "SALOME_Launcher_defs.hxx"

#include "Python.h"

#include "PythonCppUtils.hxx"
#include "Utils_CorbaException.hxx"
#include "SALOMEconfig.h"

#include CORBA_SERVER_HEADER(SALOME_LogManager)

#include <vector>
#include <string>
#include <memory>

class SALOME_NamingService_Abstract;
class SALOME_LogManager;
class SALOME_ContainerPerfLog;
class SALOME_ContainerScriptPerfLog;
class SALOME_ContainerScriptExecPerfLog;

namespace SALOME
{
  auto ServantDeleter = [](PortableServer::ServantBase *serv) { if(serv) serv->_remove_ref(); };
  using AutoServantDeleter = std::unique_ptr<PortableServer::ServantBase, decltype(ServantDeleter)>;

  /**
   * Class in charge to manage life cycle of servant instance and its associated reference.
   * It allows to deal cleanly with management of servant life even in case of ORB_destoy call
   * at exit.
   * 
   * As orb_destroy automaticaly desactivates all objects activated by all POAs in this
   * it's impossible to deal cleanly maangement of servant lifecyle using only _ptr or _var.
   * 
   */
  template<class TVar, class TServ>
  class RefAndServant
  {
  public:
    RefAndServant(TVar var, TServ *serv):_var(var),_serv(serv,ServantDeleter) { }
    RefAndServant(const RefAndServant& other):_var( other._var ),_serv(nullptr,ServantDeleter)
    {
      other.getServ()->_add_ref();
      _serv.reset( other.getServ() );
    }
    ~RefAndServant() = default;
    RefAndServant& operator=(const RefAndServant& other)
    {
      _var = other._var;
      other.getServ()->_add_ref();
      _serv.reset( other.getServ() );
      return *this;
    }
    TVar getVar() const { return _var; }
    TServ *getServ() const { return dynamic_cast<TServ *>( _serv.get() ); }
    void desactivateObjectFromPOA( )
    {
      PortableServer::ObjectId_var oid = getServ()->getPOA()->reference_to_id(_var);
      getServ()->getPOA()->deactivate_object(oid);
    }
  private:
    TVar _var;
    SALOME::AutoServantDeleter _serv;
  };
}

class SALOMELAUNCHER_EXPORT SALOME_VisitorContainerLog
{
public:
  virtual void enterLogManager(SALOME_LogManager& inst) = 0;
  virtual void leaveLogManager(SALOME_LogManager& inst) = 0;
  virtual void enterContainerPerfLog(SALOME_ContainerPerfLog& inst) = 0;
  virtual void leaveContainerPerfLog(SALOME_ContainerPerfLog& inst) = 0;
  virtual void enterContainerScriptPerfLog(SALOME_ContainerScriptPerfLog& inst) = 0;
  virtual void leaveContainerScriptPerfLog(SALOME_ContainerScriptPerfLog& inst) = 0;
  virtual void visitContainerScriptExecPerfLog(SALOME_ContainerScriptExecPerfLog& inst) = 0;
};


class SALOMELAUNCHER_EXPORT SALOME_ContainerScriptExecPerfLog : public POA_Engines::ContainerScriptExecPerfLog
{
public:
  SALOME_ContainerScriptExecPerfLog(SALOME_ContainerScriptPerfLog *father):_father(father) { }
  SALOME_ContainerScriptPerfLog *father() const { return _father; }
  void setPyObj(PyObject *obj) { _pyExecutionLog.set(obj); }
  PyObject *pyObj() { return _pyExecutionLog.get(); }
  PortableServer::POA_var getPOA();
  void assign(const SALOME::vectorOfByte& value) override;
  void assignAndAppendFreestyle(const SALOME::vectorOfByte& value) override;
  SALOME::vectorOfByte *getObj() override;
  const std::vector<char>& data() const { return _data; }
  void setData(std::vector<char>&& data) { _data = std::move(data); }
public:
  void accept(SALOME_VisitorContainerLog &visitor);
public:
  void start();
  AutoPyRef end();
  void clear() { _data.clear(); }
private:
  AutoPyRefGilSafe _pyExecutionLog;
  SALOME_ContainerScriptPerfLog *_father = nullptr;
  std::vector<char> _data;
};

using ContainerScriptExecPerfLogPair = SALOME::RefAndServant< Engines::ContainerScriptExecPerfLog_var, SALOME_ContainerScriptExecPerfLog >;

class SALOMELAUNCHER_EXPORT SALOME_ContainerScriptPerfLog : public POA_Engines::ContainerScriptPerfLog
{
public:
  SALOME_ContainerScriptPerfLog(SALOME_ContainerPerfLog *father, const std::string& name, const std::string& code):_father(father),_name(name),_code(code) { }
  ~SALOME_ContainerScriptPerfLog();
  SALOME_ContainerPerfLog *father() const { return _father; }
  void setPyObj(PyObject *obj) { _pyScriptLog.set(obj); }
  PyObject *pyObj() { return _pyScriptLog.get(); }
  PortableServer::POA_var getPOA();
  Engines::ContainerScriptExecPerfLog_ptr addExecutionSession() override;
  Engines::ListOfContainerScriptExecPerfLog *listOfExecs() override;
  char *getCode() override;
  char *getName() override;
  const std::string& name() const { return _name; }
  const std::string& code() const { return _code; }
  void setName(const std::string& name) { _name = name; }
  void setCode(const std::string& code) { _code = code; }
  std::size_t getNumberOfSessions() const { return _sessions.size(); }
public:
  void accept(SALOME_VisitorContainerLog &visitor);
private:
  AutoPyRefGilSafe _pyScriptLog;
  SALOME_ContainerPerfLog *_father = nullptr;
  std::string _name;
  std::string _code;
  std::vector< ContainerScriptExecPerfLogPair > _sessions;
};

using ContainerScriptPerfLogPair = SALOME::RefAndServant< Engines::ContainerScriptPerfLog_var, SALOME_ContainerScriptPerfLog >;

class SALOMELAUNCHER_EXPORT SALOME_ContainerPerfLog : public POA_Engines::ContainerPerfLog
{
public:
  SALOME_ContainerPerfLog(SALOME_LogManager *father, const std::string& nameInNS, const std::string& logFile):_father(father),_name_in_ns(nameInNS),_log_file(logFile) { }
  ~SALOME_ContainerPerfLog();
  SALOME_LogManager *father() const { return _father; }
  void setPyObj(PyObject *obj) { _pyContLog.set(obj); }
  PyObject *pyObj() { return _pyContLog.get(); }
  PortableServer::POA_var getPOA(); 
  char *getLogFile() override;
  char *getContainerEntryInNS() override;
  Engines::ContainerScriptPerfLog_ptr addScript(const char *name, const char *code) override;
  Engines::ListOfContainerScriptPerfLog *listOfScripts() override;
  /* Call that destroy this object AND unregister from POA all referecences of this*/
  void destroy() override;
  const std::string& nameInNS() const { return _name_in_ns; }
  const std::string& logFile() const { return _log_file; }
  void setNameInNS(const std::string& name) { _name_in_ns = name; }
  void setLogFile(const std::string& logFile) { _log_file = logFile; }
  std::size_t getNumberOfScripts() const { return _scripts.size(); }
public:
  void accept(SALOME_VisitorContainerLog &visitor);
private:
  void destroyInternal();
private:
  AutoPyRefGilSafe _pyContLog;
  SALOME_LogManager *_father = nullptr;
  std::string _name_in_ns;
  std::string _log_file;
  std::vector< ContainerScriptPerfLogPair > _scripts;
};

enum class SafeLoggerActiveVersionType
{ VersionA_Activated, VersionB_Activated, NoVersion_Activated };

class SALOME_SafeLoggerFileHolder
{
public:
  void setFileNamePairOfLogger(const std::string& loggerFileNameA, const std::string& loggerFileNameB) { _logger_file_a = loggerFileNameA; _logger_file_b = loggerFileNameB; }
  void getFileNamePairOfLogger(std::string& loggerFileNameA, std::string& loggerFileNameB) { loggerFileNameA = _logger_file_a; loggerFileNameB = _logger_file_b; }
  void versionA_IsTheLatestValidVersion() { _version_activated = SafeLoggerActiveVersionType::VersionA_Activated; }
  void versionB_IsTheLatestValidVersion() { _version_activated = SafeLoggerActiveVersionType::VersionB_Activated; }
  std::string getLastVersionOfFileNameLogger();
private:
  std::string _logger_file_a;
  std::string _logger_file_b;
  SafeLoggerActiveVersionType _version_activated = SafeLoggerActiveVersionType::NoVersion_Activated;
};

using ContainerPerfLogPair = SALOME::RefAndServant< Engines::ContainerPerfLog_var, SALOME_ContainerPerfLog >;

class SALOMELAUNCHER_EXPORT SALOME_LogManager : public POA_Engines::LogManager
{
 public:
  SALOME_LogManager(CORBA::ORB_ptr orb, PortableServer::POA_var poa, SALOME_NamingService_Abstract *ns = nullptr);
  PortableServer::POA_var getPOA() { return _poa; }
  PyObject *pyHelper() const { return _pyLogManager; }
  ~SALOME_LogManager();
  void clear() override;
  Engines::ContainerPerfLog_ptr declareContainer(const char *contInNS, const char *logfile) override;
  Engines::ListOfContainerPerfLog *listOfContainerLogs() override;
  SALOME::vectorOfByte *getAllStruct(bool clearMemory) override;
  void putStructInFileAtomic(bool clearMemory, const char *fileName) override;
 public:
  void setFileNamePairOfLogger(const char *loggerFileNameA, const char *loggerFileNameB) override;
  void getFileNamePairOfLogger(CORBA::String_out loggerFileNameA, CORBA::String_out loggerFileNameB) override;
  void versionA_IsTheLatestValidVersion() override { _safe_logger_file_holder.versionA_IsTheLatestValidVersion(); }
  void versionB_IsTheLatestValidVersion() override { _safe_logger_file_holder.versionB_IsTheLatestValidVersion(); }
  char *getLastVersionOfFileNameLogger() override;
 public:
  void removeEntryBeforeDying(SALOME_ContainerPerfLog *child);
 public:
  std::size_t getNumberOfContainers() const { return _containers.size(); }
 public:
  void accept(SALOME_VisitorContainerLog &visitor);
 private:
  std::vector<char> dumpCppInternalFrmt(bool clearMemory);
 private:
  PyObject *_pyLogManager = nullptr;
  std::unique_ptr<SALOME_NamingService_Abstract> _NS;
  PortableServer::POA_var _poa;
  std::vector< ContainerPerfLogPair > _containers;
  SALOME_SafeLoggerFileHolder _safe_logger_file_holder;
 public:
  static const char NAME_IN_NS[];
};
