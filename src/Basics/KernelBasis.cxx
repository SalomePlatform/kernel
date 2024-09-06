// Copyright (C) 2021-2024  CEA, EDF
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

#include "KernelBasis.hxx"

#include <memory>
#include <sstream>
#include <stdexcept>

static bool DEFAULT_SSL_MODE = true;
static bool GUI_MODE = false;

// IOR of SALOME_Embedded_NamingService servant
static std::string IOR_OF_EMBEDDED_NS;

bool getSSLMode()
{
  return DEFAULT_SSL_MODE;
}

void setSSLMode(bool sslMode)
{
  DEFAULT_SSL_MODE = sslMode;
}

bool getGUIMode()
{
  return GUI_MODE;
}

void setGUIMode(bool guiMode)
{
  GUI_MODE = guiMode;
}

std::string getIOROfEmbeddedNS()
{
  return IOR_OF_EMBEDDED_NS;
}

void setIOROfEmbeddedNS(const std::string& ior)
{
  IOR_OF_EMBEDDED_NS = ior;
}

#include <iostream>

/*!
 * Callable from Python in case if sys.stdout is not connected to tty
 */
void WriteInStdout(const std::string& msg)
{
  std::cout << msg << std::endl << std::flush;
}

/*!
 * Callable from Python in case if sys.stdout is not connected to tty
 */
void WriteInStderr(const std::string& msg)
{
  std::cerr << msg << std::endl << std::flush;
}

namespace SALOME
{
  static constexpr char IN_PROCESS_VALUE = 0;
  static constexpr char IN_PROCESS_VALUE_STR[] = "InProcess";
  static constexpr char OUT_OF_PROCESS_NO_REPLAY_VALUE = 1;
  static constexpr char OUT_OF_PROCESS_NO_REPLAY_VALUE_STR[] = "OutOfProcessNoReplay";
  static constexpr char OUT_OF_PROCESS_WITH_REPLAY_VALUE = 2;
  static constexpr char OUT_OF_PROCESS_WITH_REPLAY_VALUE_STR[] = "OutOfProcessWithReplay";
  static constexpr char OUT_OF_PROCESS_NO_REPLAY_FT_VALUE = 3;
  static constexpr char OUT_OF_PROCESS_NO_REPLAY_FT_VALUE_STR[] = "OutOfProcessNoReplayFT";
  static constexpr char OUT_OF_PROCESS_WITH_REPLAY_FT_VALUE = 4;
  static constexpr char OUT_OF_PROCESS_WITH_REPLAY_FT_VALUE_STR[] = "OutOfProcessWithReplayFT";

  static PyExecutionMode FromIntToPyExecutionMode(char value)
  {
    switch(value)
    {
      case IN_PROCESS_VALUE:
        return PyExecutionMode::InProcess;
      case OUT_OF_PROCESS_NO_REPLAY_VALUE:
        return PyExecutionMode::OutOfProcessNoReplay;
      case OUT_OF_PROCESS_WITH_REPLAY_VALUE:
        return PyExecutionMode::OutOfProcessWithReplay;
      case OUT_OF_PROCESS_NO_REPLAY_FT_VALUE:
        return PyExecutionMode::OutOfProcessNoReplayFT;
      case OUT_OF_PROCESS_WITH_REPLAY_FT_VALUE:
        return PyExecutionMode::OutOfProcessWithReplayFT;
    }
    throw std::range_error("FromIntToPyExecutionMode : Invalid value for Py Execution Mode ! Must be in 0 (InProcess), 1 (OutOfProcessNoReplay) or 2 (OutOfProcessWithReplay) !");
  }
  
  static PyExecutionMode FromStrToPyExecutionMode(const std::string& value)
  {
    if(value == IN_PROCESS_VALUE_STR)
      return PyExecutionMode::InProcess;
    if(value == OUT_OF_PROCESS_NO_REPLAY_VALUE_STR)
      return PyExecutionMode::OutOfProcessNoReplay;
    if(value == OUT_OF_PROCESS_WITH_REPLAY_VALUE_STR)
      return PyExecutionMode::OutOfProcessWithReplay;
    if(value == OUT_OF_PROCESS_NO_REPLAY_FT_VALUE_STR)
      return PyExecutionMode::OutOfProcessNoReplayFT;
    if(value == OUT_OF_PROCESS_WITH_REPLAY_FT_VALUE_STR)
      return PyExecutionMode::OutOfProcessWithReplayFT;
    throw std::range_error("FromStrToPyExecutionMode : Invalid str value for py execution mode !");
  }

  static std::string FromExecutionModeToStr(PyExecutionMode execMode)
  {
    switch(execMode)
    {
      case PyExecutionMode::InProcess:
        return IN_PROCESS_VALUE_STR;
      case PyExecutionMode::OutOfProcessNoReplay:
        return OUT_OF_PROCESS_NO_REPLAY_VALUE_STR;
      case PyExecutionMode::OutOfProcessWithReplay:
        return OUT_OF_PROCESS_WITH_REPLAY_VALUE_STR;
      case PyExecutionMode::OutOfProcessNoReplayFT:
        return OUT_OF_PROCESS_NO_REPLAY_FT_VALUE_STR;
      case PyExecutionMode::OutOfProcessWithReplayFT:
        return OUT_OF_PROCESS_WITH_REPLAY_FT_VALUE_STR;
      default:
        throw std::range_error("FromExecutionModeToStr : Invalid str value for py execution mode !");
    }
  }
}

constexpr int SALOME_BIG_OBJ_ON_DISK_THRES_DFT = 50000000;

static int SALOME_BIG_OBJ_ON_DISK_THRES = SALOME_BIG_OBJ_ON_DISK_THRES_DFT;

std::string SALOME_REPLAY_FILES_DIR;

int SALOME::GetBigObjOnDiskThreshold()
{
  return SALOME_BIG_OBJ_ON_DISK_THRES;
}

void SALOME::SetBigObjOnDiskThreshold(int newThresholdInByte)
{
  SALOME_BIG_OBJ_ON_DISK_THRES = newThresholdInByte;
}

constexpr char SALOME_FILE_BIG_OBJ_DIR_SEP = '@';

static std::string SALOME_FILE_BIG_OBJ_DIR;

constexpr int DFT_SALOME_NB_RETRY = 1;

static int SALOME_NB_RETRY = DFT_SALOME_NB_RETRY;

constexpr int DFT_SALOME_PY_EXEC_TIMEOUT = -1;

static int SALOME_PY_EXEC_TIMEOUT = DFT_SALOME_PY_EXEC_TIMEOUT;

static bool SALOME_FW_CWD_STATUS = false;

SALOME::BigObjTransferProtocol SALOME::FromIntToBigObjOnDiskProtocol(char protocol)
{
  switch( protocol )
  {
    case SALOME::SHARED_FILE_SYSTEM_PROTOCOL:
      return SALOME::BigObjTransferProtocol::SharedFileSystem;
    case SALOME::SSD_COPY_FILE_SYSTEM_PROTOCOL:
      return SALOME::BigObjTransferProtocol::SSDCopyFileSystem;
    default:
      throw std::runtime_error("FromIntToBigObjOnDiskProtocol unrecognized protocol ! should be in [0,1] !");
  }
}

SALOME::BigObjTransferProtocol SALOME::BigObjOnDiskProtocolFromStr(const std::string& protocol)
{
  if( protocol == SALOME::SHARED_FILE_SYSTEM_PROTOCOL_STR )
    return SALOME::BigObjTransferProtocol::SharedFileSystem;
  if( protocol == SALOME::SSD_COPY_FILE_SYSTEM_PROTOCOL_STR )
    return SALOME::BigObjTransferProtocol::SSDCopyFileSystem;
  throw std::runtime_error("BigObjOnDiskProtocolFromStr unrecognized protocol !");
}

std::string SALOME::BigObjOnDiskProtocolToStr(BigObjTransferProtocol protocol)
{
  switch( protocol )
  {
    case SALOME::BigObjTransferProtocol::SharedFileSystem:
      return SALOME::SHARED_FILE_SYSTEM_PROTOCOL_STR;
    case SALOME::BigObjTransferProtocol::SSDCopyFileSystem:
      return SALOME::SSD_COPY_FILE_SYSTEM_PROTOCOL_STR;
    default:
      throw std::runtime_error("BigObjOnDiskProtocolToStr unrecognized protocol ! should be in [0,1] !");
  }
}

/*!
 * This method returns the protocol of proxy transfert and the directory
 */
SALOME::BigObjTransferProtocol SALOME::GetBigObjOnDiskProtocolAndDirectory(std::string& directory)
{
  if(SALOME_FILE_BIG_OBJ_DIR.size() < 3)
  {
    directory = SALOME_FILE_BIG_OBJ_DIR;
    return SALOME::BigObjTransferProtocol::SharedFileSystem;
  }
  std::string protocol = SALOME_FILE_BIG_OBJ_DIR.substr(0,3);
  directory = SALOME_FILE_BIG_OBJ_DIR.substr(3);
  if( protocol[0]!=SALOME_FILE_BIG_OBJ_DIR_SEP || protocol[2]!=SALOME_FILE_BIG_OBJ_DIR_SEP)
  {
    directory = SALOME_FILE_BIG_OBJ_DIR;
    return SALOME::BigObjTransferProtocol::SharedFileSystem;
  }
  std::istringstream iss(protocol.substr(1,1)); iss.exceptions(std::istringstream::failbit | std::istringstream::badbit);
  short iproxyprot = 0;
  iss >> iproxyprot;
  return FromIntToBigObjOnDiskProtocol( iproxyprot );
}

std::string SALOME::GetBigObjOnDiskDirectoryCoarse()
{
  return SALOME_FILE_BIG_OBJ_DIR;
}

std::string SALOME::GetDirectoryForReplayFiles()
{
  return SALOME_REPLAY_FILES_DIR;
}

void SALOME::SetDirectoryForReplayFiles(const std::string& directory)
{
  SALOME_REPLAY_FILES_DIR = directory;
}

void SALOME::SetBigObjOnDiskDirectory(const std::string& directory)
{
  SALOME_FILE_BIG_OBJ_DIR = directory;
}

bool SALOME::BigObjOnDiskDirectoryDefined()
{
  return ! SALOME_FILE_BIG_OBJ_DIR.empty();
}

void SALOME::SetNumberOfRetry(int nbRetry)
{
  SALOME_NB_RETRY = nbRetry;
}

int SALOME::GetNumberOfRetry()
{
  return SALOME_NB_RETRY;
}

void SALOME::SetExecutionTimeOut(int timeOutInSecond)
{
  SALOME_PY_EXEC_TIMEOUT = timeOutInSecond;
}

int SALOME::GetExecutionTimeOut()
{
  return SALOME_PY_EXEC_TIMEOUT;
}

static SALOME::PyExecutionMode DefaultPyExecMode = SALOME::PyExecutionMode::NotSet;

void SALOME::SetPyExecutionMode(PyExecutionMode mode)
{
  DefaultPyExecMode = mode;
}

void SALOME::SetPyExecutionModeStr(const std::string& mode)
{
  SALOME::SetPyExecutionMode( SALOME::FromStrToPyExecutionMode(mode) );
}

std::vector<std::string> SALOME::GetAllPyExecutionModes()
{
  return {IN_PROCESS_VALUE_STR,OUT_OF_PROCESS_NO_REPLAY_VALUE_STR,OUT_OF_PROCESS_WITH_REPLAY_VALUE_STR};
}

std::string SALOME::GetPyExecutionModeStr()
{
  return SALOME::FromExecutionModeToStr( SALOME::GetPyExecutionMode() );
}

SALOME::PyExecutionMode SALOME::GetPyExecutionMode()
{
  auto isEnvVarSet = []() -> SALOME::PyExecutionMode
  {
    const char *envVar = std::getenv("SALOME_PY_EXECUTION_MODE");
    if (envVar && (envVar[0] != '\0'))
    {
      const int numValue = std::stoi(envVar);
      return SALOME::FromIntToPyExecutionMode( static_cast<char>(numValue) );
    }
    return SALOME::PyExecutionMode::InProcess;
  };
  if(DefaultPyExecMode == SALOME::PyExecutionMode::NotSet)
    DefaultPyExecMode = isEnvVarSet();
  return DefaultPyExecMode;
}

bool SALOME::GetForwardCurrentDirectoryStatus()
{
  return SALOME_FW_CWD_STATUS;
}

void SALOME::SetForwardCurrentDirectoryStatus(bool newStatus)
{
  SALOME_FW_CWD_STATUS = newStatus;
}

extern "C"
{
#ifndef WIN32
# include <unistd.h>
#else
# include <windows.h>
# include <direct.h>
#endif
}

#ifdef WIN32
#define getcwd _getcwd
#endif

auto CStyleDeleter = [](char *ptr) { if(ptr) free(ptr); };
using AutoCDeleter = std::unique_ptr<char, decltype(CStyleDeleter)>;

std::string SALOME::GetCurrentWorkingDirectory()
{
  AutoCDeleter cwdPtr( getcwd( nullptr,10000 ), CStyleDeleter );
  return std::string( cwdPtr.get() );
}
