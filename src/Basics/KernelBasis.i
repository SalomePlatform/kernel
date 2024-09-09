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

%module KernelBasis

%{
#include "KernelBasis.hxx"
#include "HeatMarcel.hxx"
#include "libSALOMELog.hxx"
#include "Monitoring.hxx"
using namespace SALOME;
%}

%include std_string.i
%include std_set.i
%include std_except.i
%include std_vector.i

%template(dvec) std::vector<double>;
%template(strvec) std::vector<std::string>;

%exception {
   try 
   {
      $action
   } 
   catch(std::exception& e) 
   {
      SWIG_exception(SWIG_SystemError, e.what() );
   } 
   catch(...) 
   {
     SWIG_exception(SWIG_UnknownError, "Unknown exception");
   }
}

%rename (HeatMarcel) HeatMarcelSwig;
%rename (GetBigObjOnDiskThreshold) GetBigObjOnDiskThresholdSwig;
%rename (SetBigObjOnDiskThreshold) SetBigObjOnDiskThresholdSwig;
%rename (GetBigObjOnDiskProtocolAndDirectory) GetBigObjOnDiskProtocolAndDirectorySwig;
%rename (BigObjOnDiskProtocolFromStr) BigObjOnDiskProtocolFromStrSwig;
%rename (BigObjOnDiskProtocolToStr) BigObjOnDiskProtocolToStrSwig;
%rename (SetBigObjOnDiskDirectory) SetBigObjOnDiskDirectorySwig;
%rename (BigObjOnDiskDirectoryDefined) BigObjOnDiskDirectoryDefinedSwig;
%rename (GetDirectoryForReplayFiles) GetDirectoryForReplayFilesSwig;
%rename (SetDirectoryForReplayFiles) SetDirectoryForReplayFilesSwig;
%rename (SetNumberOfRetry) SetNumberOfRetrySwig;
%rename (GetNumberOfRetry) GetNumberOfRetrySwig;
%rename (SetExecutionTimeOut) SetExecutionTimeOutSwig;
%rename (GetExecutionTimeOut) GetExecutionTimeOutSwig;
%rename (GetForwardCurrentDirectoryStatus) GetForwardCurrentDirectoryStatusSwig;
%rename (SetForwardCurrentDirectoryStatus) SetForwardCurrentDirectoryStatusSwig;

bool getSSLMode();
void setSSLMode(bool sslMode);

bool getGUIMode();
void setGUIMode(bool guiMode);

std::string getIOROfEmbeddedNS();
void setIOROfEmbeddedNS(const std::string& ior);

double GetTimeAdjustmentCst();

long LaunchMonitoring(const std::string& pyScriptToEvaluate);

void StopMonitoring(long pid);

bool VerbosityActivated();

void SetVerbosityActivated(bool flag);

bool IsDebugLevel();

bool IsInfoLevel();

bool IsWarningLevel();

bool IsErrorLevel();

void WriteInStdout(const std::string& msg);

void WriteInStderr(const std::string& msg);

%rename (SetVerbosityLevel) SetVerbosityLevelSwig;
%rename (VerbosityLevel) VerbosityLevelSwig;
%rename (SetPyExecutionMode) SetPyExecutionModeStrSwig;
%rename (GetPyExecutionMode) GetPyExecutionModeStrSwig;
%rename (GetAllPyExecutionModes) GetAllPyExecutionModesSwig;

%inline
{
void IncrementRefCounter( PyObject * obj )
{
  Py_XINCREF( obj );
}

PyObject *HeatMarcelSwig(double timeAjustment, unsigned int nbThreads = 0)
{
  double timeInS = 0.0;
  long double piVal = HeatMarcel(timeAjustment,timeInS,nbThreads);
  PyObject *ret(PyTuple_New(2));
  PyTuple_SetItem(ret,0,SWIG_From_double((double)piVal));
  PyTuple_SetItem(ret,1,SWIG_From_double(timeInS));
  return ret;
}

std::vector<double> ReadFloatsInFileSwig(const std::string& fileName)
{
  std::vector<double> ret;
  try
  {
    ret = SALOME::ReadFloatsInFile( fileName );
  }
  catch(std::exception& e) { }
  return ret;
}

int GetBigObjOnDiskThresholdSwig()
{
  return SALOME::GetBigObjOnDiskThreshold();
}

void SetBigObjOnDiskThresholdSwig(int newThreshold)
{
  return SALOME::SetBigObjOnDiskThreshold(newThreshold);
}

void SetBigObjOnDiskDirectorySwig(const std::string& directory)
{
  return SALOME::SetBigObjOnDiskDirectory(directory);
}

bool BigObjOnDiskDirectoryDefinedSwig()
{
  return SALOME::BigObjOnDiskDirectoryDefined();
}

void SetNumberOfRetrySwig(int nbRetry)
{
  SALOME::SetNumberOfRetry( nbRetry );
}

int GetNumberOfRetrySwig()
{
  return SALOME::GetNumberOfRetry( );
}

void SetExecutionTimeOutSwig(int timeOutInSecond)
{
  SALOME::SetExecutionTimeOut( timeOutInSecond );
}

int GetExecutionTimeOutSwig()
{
  return SALOME::GetExecutionTimeOut();
}

bool GetForwardCurrentDirectoryStatusSwig()
{
  return SALOME::GetForwardCurrentDirectoryStatus();
}

void SetForwardCurrentDirectoryStatusSwig(bool newStatus)
{
  SALOME::SetForwardCurrentDirectoryStatus(newStatus);
}

std::string GetDirectoryForReplayFilesSwig()
{
  return SALOME::GetDirectoryForReplayFiles();
}

void SetDirectoryForReplayFilesSwig(const std::string& directory)
{
  SALOME::SetDirectoryForReplayFiles(directory);
}

std::string BigObjOnDiskProtocolToStrSwig( int protocol )
{
  return SALOME::BigObjOnDiskProtocolToStr( SALOME::FromIntToBigObjOnDiskProtocol( protocol ) );
}

int BigObjOnDiskProtocolFromStrSwig(const std::string& protocol)
{
  return static_cast<char>( SALOME::BigObjOnDiskProtocolFromStr( protocol ) );
}

PyObject *GetBigObjOnDiskProtocolAndDirectorySwig()
{
  std::string directory;
  SALOME::BigObjTransferProtocol ret0 = SALOME::GetBigObjOnDiskProtocolAndDirectory(directory);
  PyObject *ret(PyTuple_New(2));
  PyTuple_SetItem(ret,0,PyInt_FromLong(static_cast<char>( ret0 ) ));
  PyTuple_SetItem(ret,1,PyUnicode_FromString(directory.c_str()));
  return ret;
}

void SetVerbosityLevelSwig(const std::string& level)
{
  SetVerbosityLevelStr(level);
}

std::string VerbosityLevelSwig()
{
  return VerbosityLevelStr();
}

void SetPyExecutionModeStrSwig(const std::string& mode)
{
  SetPyExecutionModeStr( mode );
}

std::string GetPyExecutionModeStrSwig()
{
  return GetPyExecutionModeStr();
}

std::vector<std::string> GetAllPyExecutionModesSwig()
{
  return GetAllPyExecutionModes();
}
}

%pythoncode %{
def ReadFloatsInFile( fileName ):
  ret = ReadFloatsInFileSwig( fileName )
  return ret
%}
