// Copyright (C) 2021-2023  CEA, EDF
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
using namespace SALOME;
%}

%include "std_string.i"

%rename (HeatMarcel) HeatMarcelSwig;

bool getSSLMode();
void setSSLMode(bool sslMode);

bool getGUIMode();
void setGUIMode(bool guiMode);

std::string getIOROfEmbeddedNS();
void setIOROfEmbeddedNS(const std::string& ior);

double GetTimeAdjustmentCst();

bool VerbosityActivated();

void SetVerbosityActivated(bool flag);

void WriteInStdout(const std::string& msg);

void WriteInStderr(const std::string& msg);


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
}
