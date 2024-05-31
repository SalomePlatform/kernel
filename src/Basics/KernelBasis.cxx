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

int SALOME::GetBigObjOnDiskThreshold()
{
  return SALOME_BIG_OBJ_ON_DISK_THRES;
}

void SALOME::SetBigObjOnDiskThreshold(int newThresholdInByte)
{
  SALOME_BIG_OBJ_ON_DISK_THRES = newThresholdInByte;
}

static std::string SALOME_FILE_BIG_OBJ_DIR;

constexpr int DFT_SALOME_NB_RETRY = 1;

static int SALOME_NB_RETRY = DFT_SALOME_NB_RETRY;

std::string SALOME::GetBigObjOnDiskDirectory()
{
  return SALOME_FILE_BIG_OBJ_DIR;
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
