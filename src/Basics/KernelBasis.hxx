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

#pragma once

#include "SALOME_Basics.hxx"

#include <string>
#include <vector>

bool BASICS_EXPORT getSSLMode();
void BASICS_EXPORT setSSLMode(bool sslMode);
bool BASICS_EXPORT getGUIMode();
void BASICS_EXPORT setGUIMode(bool guiMode);

std::string BASICS_EXPORT getIOROfEmbeddedNS();
void BASICS_EXPORT setIOROfEmbeddedNS(const std::string& ior);

void BASICS_EXPORT WriteInStdout(const std::string& msg);
void BASICS_EXPORT WriteInStderr(const std::string& msg);

namespace SALOME
{
  enum class PyExecutionMode { NotSet, InProcess, OutOfProcessNoReplay, OutOfProcessWithReplay, OutOfProcessNoReplayFT, OutOfProcessWithReplayFT };
  void BASICS_EXPORT SetPyExecutionMode(PyExecutionMode mode);
  void BASICS_EXPORT SetPyExecutionModeStr(const std::string& mode);
  std::vector<std::string> BASICS_EXPORT GetAllPyExecutionModes();
  std::string BASICS_EXPORT GetPyExecutionModeStr();
  PyExecutionMode BASICS_EXPORT GetPyExecutionMode();
  int BASICS_EXPORT GetBigObjOnDiskThreshold();
  void BASICS_EXPORT SetBigObjOnDiskThreshold(int newThresholdInByte);
  std::string BASICS_EXPORT GetBigObjOnDiskDirectory();
  void BASICS_EXPORT SetBigObjOnDiskDirectory(const std::string& directory);
  bool BASICS_EXPORT BigObjOnDiskDirectoryDefined();
  void BASICS_EXPORT SetNumberOfRetry(int nbRetry);
  int BASICS_EXPORT GetNumberOfRetry();
}
