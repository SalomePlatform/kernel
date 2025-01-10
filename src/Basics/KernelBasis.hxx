// Copyright (C) 2021-2025  CEA, EDF
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
  constexpr char SHARED_FILE_SYSTEM_PROTOCOL = 0;
  constexpr char SSD_COPY_FILE_SYSTEM_PROTOCOL = 1;
  const char SHARED_FILE_SYSTEM_PROTOCOL_STR[] = "SharedFileSystem";
  const char SSD_COPY_FILE_SYSTEM_PROTOCOL_STR[] = "SSDCopyFileSystem";

  enum class PyExecutionMode { NotSet, InProcess, OutOfProcessNoReplay, OutOfProcessWithReplay, OutOfProcessNoReplayFT, OutOfProcessWithReplayFT };

  enum class BigObjTransferProtocol : char
  { SharedFileSystem = SHARED_FILE_SYSTEM_PROTOCOL, SSDCopyFileSystem = SSD_COPY_FILE_SYSTEM_PROTOCOL };

  void BASICS_EXPORT SetPyExecutionMode(PyExecutionMode mode);
  void BASICS_EXPORT SetPyExecutionModeStr(const std::string& mode);
  std::vector<std::string> BASICS_EXPORT GetAllPyExecutionModes();
  std::string BASICS_EXPORT GetPyExecutionModeStr();
  PyExecutionMode BASICS_EXPORT GetPyExecutionMode();
  std::string BASICS_EXPORT GetDirectoryForReplayFiles();
  void BASICS_EXPORT SetDirectoryForReplayFiles(const std::string& directory);
  int BASICS_EXPORT GetBigObjOnDiskThreshold();
  void BASICS_EXPORT SetBigObjOnDiskThreshold(int newThresholdInByte);
  std::string BASICS_EXPORT BigObjOnDiskProtocolToStr(BigObjTransferProtocol protocol);
  BigObjTransferProtocol BASICS_EXPORT BigObjOnDiskProtocolFromStr(const std::string& protocol);
  BigObjTransferProtocol BASICS_EXPORT FromIntToBigObjOnDiskProtocol(char protocol);
  BigObjTransferProtocol BASICS_EXPORT GetBigObjOnDiskProtocolAndDirectory(std::string& directory);
  std::string BASICS_EXPORT GetBigObjOnDiskDirectoryCoarse();
  void BASICS_EXPORT SetBigObjOnDiskDirectory(const std::string& directory);
  bool BASICS_EXPORT BigObjOnDiskDirectoryDefined();
  void BASICS_EXPORT SetNumberOfRetry(int nbRetry);
  int BASICS_EXPORT GetNumberOfRetry();
  void BASICS_EXPORT SetExecutionTimeOut(int timeOutInSecond);
  int BASICS_EXPORT GetExecutionTimeOut();
  bool BASICS_EXPORT GetForwardCurrentDirectoryStatus();
  void BASICS_EXPORT SetForwardCurrentDirectoryStatus(bool newStatus);
  std::string BASICS_EXPORT GetCurrentWorkingDirectory();
}
