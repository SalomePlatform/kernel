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
%rename (GetBigObjOnDiskDirectory) GetBigObjOnDiskDirectorySwig;
%rename (SetBigObjOnDiskDirectory) SetBigObjOnDiskDirectorySwig;
%rename (BigObjOnDiskDirectoryDefined) BigObjOnDiskDirectoryDefinedSwig;

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

std::string GetBigObjOnDiskDirectorySwig()
{
  return SALOME::GetBigObjOnDiskDirectory();
}

void SetBigObjOnDiskDirectorySwig(const std::string& directory)
{
  return SALOME::SetBigObjOnDiskDirectory(directory);
}

bool BigObjOnDiskDirectoryDefinedSwig()
{
  return SALOME::BigObjOnDiskDirectoryDefined();
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