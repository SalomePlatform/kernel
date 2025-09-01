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

#include "SALOME_LogManager.hxx"
#include "SALOME_Fake_NamingService.hxx"
#include "SALOME_ContainerManager.hxx"

#ifndef WIN32
#include <unistd.h>
#else
#include <windows.h>
#include <Basics_Utils.hxx>
#endif

#include "utilities.h"

#include <sstream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <functional>

const char SALOME_LogManager::NAME_IN_NS[]="/LogManager";

static std::vector<char> FromPyToCpp(PyObject *obj)
{
  std::vector<char> ret;
  char *buffer = nullptr;
  Py_ssize_t length = 0;
  PyBytes_AsStringAndSize(obj,&buffer,&length);
  ret.resize(length);
  for(auto i = 0 ; i < length ; ++i)
    ret[i] = buffer[i];
  return ret;
}

static SALOME::vectorOfByte *FromVectCharToCorba(const std::vector<char>& data)
{
  SALOME::vectorOfByte_var ret = new SALOME::vectorOfByte;
  auto length = data.size();
  ret->length(length);
  for(auto i = 0 ; i < length ; ++i)
    ret[i] = data[i];
  return ret._retn();
}

SALOME_ContainerScriptPerfLog::~SALOME_ContainerScriptPerfLog()
{
  DEBUG_MESSAGE("Destruction of SALOME_ContainerScriptPerfLog instance");
}

PortableServer::POA_var SALOME_ContainerScriptExecPerfLog::getPOA()
{
  return father()->getPOA();
}

void SALOME_ContainerScriptExecPerfLog::assign(const SALOME::vectorOfByte& value)
{
  auto sz = value.length();
  _data.resize( sz );
  for(auto i = 0 ; i < sz ; ++i)
    _data[i] = value[i];
  //
  AutoPyRef s( this->end( ) );
  //
  _data = FromPyToCpp(s);
}

void SALOME_ContainerScriptExecPerfLog::assignAndAppendFreestyle(const SALOME::vectorOfByte& value)
{
  auto sz = value.length();
  std::vector<char> data( sz );
  for(auto i = 0 ; i < sz ; ++i)
    data[i] = value[i];
  //
  {
    AutoGIL gstate;
    //https://docs.python.org/3/c-api/arg.html#c.Py_BuildValue
    AutoPyRef dataPyFromRemote(PyBytes_FromStringAndSize(data.data(),data.size()));
    AutoPyRef dataPyAlreadyOnSite(PyBytes_FromStringAndSize(_data.data(),_data.size()));
    AutoPyRef result(PyObject_CallMethod(pyObj(),(char*)"flushAndAppendFreestyle","OO",dataPyAlreadyOnSite.get(),dataPyFromRemote.get(),nullptr) ) ;
    if (PyErr_Occurred())
    {
      std::string error("can not assignAndAppendFreestyle");
      PyErr_Print();
      THROW_SALOME_CORBA_EXCEPTION(error.c_str(),SALOME::INTERNAL_ERROR);
    }
    _data = FromPyToCpp(result);
  }
}

SALOME::vectorOfByte *SALOME_ContainerScriptExecPerfLog::getObj()
{
  return FromVectCharToCorba(this->_data);
}

void SALOME_ContainerScriptExecPerfLog::accept(SALOME_VisitorContainerLog &visitor)
{
  visitor.visitContainerScriptExecPerfLog( *this );
}

void SALOME_ContainerScriptExecPerfLog::start()
{
  AutoGIL gstate;
  AutoPyRef result = PyObject_CallMethod(pyObj(),(char*)"start","",nullptr);
  if (PyErr_Occurred())
  {
    std::string error("can not start");
    PyErr_Print();
    THROW_SALOME_CORBA_EXCEPTION(error.c_str(),SALOME::INTERNAL_ERROR);
  }
}

AutoPyRef SALOME_ContainerScriptExecPerfLog::end()
{
  AutoGIL gstate;
  //https://docs.python.org/3/c-api/arg.html#c.Py_BuildValue
  AutoPyRef dataPy(PyBytes_FromStringAndSize(_data.data(),_data.size()));
  AutoPyRef result(PyObject_CallMethod(pyObj(),(char*)"end","O",dataPy.get(),nullptr) ) ;
  if (PyErr_Occurred())
  {
    std::string error("can not end");
    PyErr_Print();
    THROW_SALOME_CORBA_EXCEPTION(error.c_str(),SALOME::INTERNAL_ERROR);
  }
  return result;
}

/////

PortableServer::POA_var SALOME_ContainerScriptPerfLog::getPOA()
{
  return father()->getPOA();
}

char *SALOME_ContainerScriptPerfLog::getCode()
{
  return CORBA::string_dup( _code.c_str() );
}

char *SALOME_ContainerScriptPerfLog::getName()
{
  return CORBA::string_dup( _name.c_str() );
}

void SALOME_ContainerScriptPerfLog::accept(SALOME_VisitorContainerLog &visitor)
{
  visitor.enterContainerScriptPerfLog( *this );
  for(auto& session : _sessions)
  {
    visitor.visitContainerScriptExecPerfLog( *session.getServ() );
  }
  visitor.leaveContainerScriptPerfLog( *this );
}


Engines::ContainerScriptExecPerfLog_ptr SALOME_ContainerScriptPerfLog::addExecutionSession()
{
  SALOME_ContainerScriptExecPerfLog *execution = new SALOME_ContainerScriptExecPerfLog(this);
  PortableServer::ObjectId_var id(getPOA()->activate_object(execution));
  CORBA::Object_var executionPtr(getPOA()->id_to_reference(id));
  Engines::ContainerScriptExecPerfLog_var executionPtr2 = Engines::ContainerScriptExecPerfLog::_narrow(executionPtr);
  _sessions.push_back( { executionPtr2,execution } );
  {
    AutoGIL gstate;
    AutoPyRef result = PyObject_CallMethod(pyObj(),(char*)"addExecution","",nullptr);
    if (PyErr_Occurred())
    {
      std::string error("can not addExecution");
      PyErr_Print();
      THROW_SALOME_CORBA_EXCEPTION(error.c_str(),SALOME::INTERNAL_ERROR);
    }
    execution->setPyObj( result.retn() );//ownership of result is transfered to execution
  }
  execution->start();
  return executionPtr2._retn();
}

Engines::ListOfContainerScriptExecPerfLog *SALOME_ContainerScriptPerfLog::listOfExecs()
{
  Engines::ListOfContainerScriptExecPerfLog_var ret = new Engines::ListOfContainerScriptExecPerfLog;
  auto sz = this->_sessions.size();
  ret->length(sz);
  for(auto i = 0 ; i < sz ; ++i)
    ret[i] = this->_sessions[i].getVar();
  return ret._retn();
}

/////

SALOME_ContainerPerfLog::~SALOME_ContainerPerfLog()
{
}

PortableServer::POA_var SALOME_ContainerPerfLog::getPOA()
{
  return father()->getPOA();
  }

Engines::ContainerScriptPerfLog_ptr SALOME_ContainerPerfLog::addScript(const char *name, const char *code)
{
  SALOME_ContainerScriptPerfLog *script = new SALOME_ContainerScriptPerfLog(this,name,code);
  PortableServer::ObjectId_var id(getPOA()->activate_object(script));
  CORBA::Object_var scriptPtr(getPOA()->id_to_reference(id));
  Engines::ContainerScriptPerfLog_var scriptPtr2 = Engines::ContainerScriptPerfLog::_narrow(scriptPtr);
  _scripts.push_back( { scriptPtr2,script } );
  {
    AutoGIL gstate;
    PyObject *result = PyObject_CallMethod(pyObj(),(char*)"addScript","",nullptr);
    if (PyErr_Occurred())
    {
      std::string error("can not addScript");
      PyErr_Print();
      THROW_SALOME_CORBA_EXCEPTION(error.c_str(),SALOME::INTERNAL_ERROR);
    }
    script->setPyObj( result );
  }
  return scriptPtr2._retn();
}

Engines::ListOfContainerScriptPerfLog *SALOME_ContainerPerfLog::listOfScripts()
{
  Engines::ListOfContainerScriptPerfLog_var ret = new Engines::ListOfContainerScriptPerfLog;
  std::size_t len( this->_scripts.size() );
  ret->length( len );
  for(std::size_t i = 0 ; i < len ; ++i)
    ret[i] = this->_scripts[i].getVar();
  return ret._retn();
}

void SALOME_ContainerPerfLog::destroy()
{
  this->destroyInternal();
}

void SALOME_ContainerPerfLog::destroyInternal()
{
  _father->removeEntryBeforeDying( this );
  for(auto& script : _scripts)
  {
    script.desactivateObjectFromPOA( );
  }
  _scripts.clear();
}

void SALOME_ContainerPerfLog::accept(SALOME_VisitorContainerLog &visitor)
{
  visitor.enterContainerPerfLog( *this );
  for(auto& script : _scripts)
  {
    script.getServ()->accept(visitor);
  }
  visitor.leaveContainerPerfLog( *this );
}

char *SALOME_ContainerPerfLog::getLogFile()
{
  return CORBA::string_dup( this->_log_file.c_str() );
}

char *SALOME_ContainerPerfLog::getContainerEntryInNS()
{
  return CORBA::string_dup( this->_name_in_ns.c_str() );
}

/////

std::string SALOME_SafeLoggerFileHolder::getLastVersionOfFileNameLogger()
{
  switch( _version_activated )
  {
  case SafeLoggerActiveVersionType::VersionA_Activated:
    return _logger_file_a;
  case SafeLoggerActiveVersionType::VersionB_Activated:
    return _logger_file_b;
  case SafeLoggerActiveVersionType::NoVersion_Activated:
    return std::string();
  default:
    throw std::runtime_error("getLastVersionOfFileNameLogger : Situation not managed");
  }
}

/////

SALOME_LogManager::SALOME_LogManager(CORBA::ORB_ptr orb, PortableServer::POA_var poa,SALOME_NamingService_Abstract *ns):_poa(poa)
{
  _NS.reset(ns);
  PortableServer::ObjectId_var id(_poa->activate_object(this));
  CORBA::Object_var obj(_poa->id_to_reference(id));
  Engines::LogManager_var refPtr(Engines::LogManager::_narrow(obj));
  _NS->Register(refPtr,NAME_IN_NS);
  {
    AutoGIL gstate;
    std::ostringstream myCommand;
    myCommand << "mylogmanager = SALOME_LogManager.SALOME_LogManagerHelper()";
    PyRun_SimpleString("from salome.kernel import SALOME_LogManager");
    PyRun_SimpleString((char*)myCommand.str().c_str());
    PyObject *mainmod = PyImport_AddModule("__main__");
    PyObject *globals = PyModule_GetDict(mainmod);
    _pyLogManager = PyDict_GetItemString(globals, "mylogmanager");
  }
}

SALOME_LogManager::~SALOME_LogManager()
{
}

void SALOME_LogManager::clear()
{
  for(auto& cont : _containers)
  {
    cont.desactivateObjectFromPOA( );
  }
  _containers.clear();
}

Engines::ContainerPerfLog_ptr SALOME_LogManager::declareContainer(const char *contInNS, const char *logfile)
{
  SALOME_ContainerPerfLog *cont = new SALOME_ContainerPerfLog(this,contInNS,logfile);
  PortableServer::ObjectId_var id(_poa->activate_object(cont));
  CORBA::Object_var contPtr(_poa->id_to_reference(id));
  Engines::ContainerPerfLog_var contPtr2 = Engines::ContainerPerfLog::_narrow(contPtr);
  _containers.push_back( { contPtr2, cont } );
  {
    AutoGIL gstate;
    PyObject *result = PyObject_CallMethod(_pyLogManager,(char*)"declareContainer","ss",contInNS,logfile,nullptr);
    if (PyErr_Occurred())
    {
      std::string error("can not declareContainer");
      PyErr_Print();
      THROW_SALOME_CORBA_EXCEPTION(error.c_str(),SALOME::INTERNAL_ERROR);
    }
    cont->setPyObj( result );
  }
  return contPtr2._retn();
}

Engines::ListOfContainerPerfLog *SALOME_LogManager::listOfContainerLogs()
{
  Engines::ListOfContainerPerfLog_var ret = new Engines::ListOfContainerPerfLog;
  std::size_t len( this->_containers.size() );
  ret->length( len );
  for(std::size_t i = 0 ; i < len ; ++i)
  {
    ret[i] = this->_containers[i].getVar();
  }
  return ret._retn();
}

void SALOME_LogManager::accept(SALOME_VisitorContainerLog &visitor)
{
  visitor.enterLogManager( *this );
  for(auto& container : _containers)
  {
    container.getServ()->accept(visitor);
  }
  visitor.leaveLogManager( *this );
}

/*!
  \param [in] unloadMemory - specify if big part of struct data (SALOME_ContainerScriptExecPerfLog) is cleared after retrieving data
 */
SALOME::vectorOfByte *SALOME_LogManager::getAllStruct(bool clearMemory)
{
  std::vector<char> data = this->dumpCppInternalFrmt(clearMemory);
  return FromVectCharToCorba(data);
}

void SALOME_LogManager::putStructInFileAtomic(bool clearMemory, const char *fileName)
{
  std::vector<char> data = this->dumpCppInternalFrmt(clearMemory);
  {
    AutoGIL gstate;
    AutoPyRef dataPy(PyBytes_FromStringAndSize(data.data(),data.size()));
    AutoPyRef result = PyObject_CallMethod(_pyLogManager,(char*)"putStructInFileAtomic","Os",dataPy.get(),fileName,nullptr);
  }
}

void SALOME_LogManager::setFileNamePairOfLogger(const char *loggerFileNameA, const char *loggerFileNameB)
{
  _safe_logger_file_holder.setFileNamePairOfLogger(loggerFileNameA,loggerFileNameB);
}

void SALOME_LogManager::getFileNamePairOfLogger(CORBA::String_out loggerFileNameA, CORBA::String_out loggerFileNameB)
{
  std::string loggerFileNameAS,loggerFileNameBS;
  _safe_logger_file_holder.getFileNamePairOfLogger(loggerFileNameAS,loggerFileNameBS);
  loggerFileNameA = CORBA::string_dup(loggerFileNameAS.c_str());
  loggerFileNameB = CORBA::string_dup(loggerFileNameBS.c_str());
}

char *SALOME_LogManager::getLastVersionOfFileNameLogger()
{
  return CORBA::string_dup( _safe_logger_file_holder.getLastVersionOfFileNameLogger().c_str() );
}

void SALOME_LogManager::removeEntryBeforeDying(SALOME_ContainerPerfLog *child)
{
  for(auto cont = _containers.begin() ; cont != _containers.end() ; ++cont )
  {
    if( (*cont).getServ() == child )
      {
        (*cont).desactivateObjectFromPOA();
        _containers.erase( cont );
        break;
      }
  }
}

///////////////////////
 
 #include <cstdint>

static void PushIntInVC(std::uint32_t val, std::vector<char>& data)
{
  char *valPtr = reinterpret_cast<char *>(&val);
  data.insert(data.end(),valPtr,valPtr+sizeof(std::uint32_t));
}
 
template<class T>
static void PushStringInVC(const T& str, std::vector<char>& data)
{
  std::uint32_t sz = static_cast<std::uint32_t>( str.size() );
  PushIntInVC(sz,data);
  data.insert(data.end(),str.data(),str.data()+sz);
}

class InternalFormatVisitorDump : public SALOME_VisitorContainerLog
{
public:
  InternalFormatVisitorDump(bool clearMemory, std::vector<char> *data):_clear_memory(clearMemory),_data(data) { }
  void enterLogManager(SALOME_LogManager& inst) override;
  void leaveLogManager(SALOME_LogManager& inst) override { }
  void enterContainerPerfLog(SALOME_ContainerPerfLog& inst) override;
  void leaveContainerPerfLog(SALOME_ContainerPerfLog& inst) override { }
  void enterContainerScriptPerfLog(SALOME_ContainerScriptPerfLog& inst) override;
  void leaveContainerScriptPerfLog(SALOME_ContainerScriptPerfLog& inst) override { }
  void visitContainerScriptExecPerfLog(SALOME_ContainerScriptExecPerfLog& inst) override;
private:
  bool _clear_memory = false;
  std::vector<char> *_data = nullptr;
};

void InternalFormatVisitorDump::visitContainerScriptExecPerfLog(SALOME_ContainerScriptExecPerfLog& inst)
{
  PushStringInVC<std::vector<char>>(inst.data(),*_data);
  if( _clear_memory )
    inst.clear();
}

void InternalFormatVisitorDump::enterContainerScriptPerfLog(SALOME_ContainerScriptPerfLog& inst)
{
  PushStringInVC<std::string>(inst.name(),*_data);
  PushStringInVC<std::string>(inst.code(),*_data);
  PushIntInVC((std::uint32_t)inst.getNumberOfSessions(),*_data);
}

void InternalFormatVisitorDump::enterContainerPerfLog(SALOME_ContainerPerfLog& inst)
{
  PushStringInVC<std::string>(inst.nameInNS(),*_data);
  PushStringInVC<std::string>(inst.logFile(),*_data);
  PushIntInVC((std::uint32_t)inst.getNumberOfScripts(),*_data);
}

void InternalFormatVisitorDump::enterLogManager(SALOME_LogManager& inst)
{
  PushIntInVC((std::uint32_t)inst.getNumberOfContainers(),*_data);
}

std::vector<char> SALOME_LogManager::dumpCppInternalFrmt(bool clearMemory)
{
  std::vector<char> ret;
  InternalFormatVisitorDump visitor(clearMemory,&ret);
  this->accept( visitor );
  return ret;
}
