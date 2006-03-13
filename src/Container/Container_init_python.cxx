//  SALOME Container : implementation of container and engine for Kernel
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : Container_init_python.cxx
//  Author : Paul RASCLE, EDF
//  Module : KERNEL
//  $Header$

#include <time.h>
#include <sys/time.h>

#include "utilities.h"

#include "Container_init_python.hxx"

using namespace std;

PyThreadState *KERNEL_PYTHON::_gtstate = 0;
PyObject *KERNEL_PYTHON::salome_shared_modules_module = NULL;
PyInterpreterState *KERNEL_PYTHON::_interp = NULL;

void KERNEL_PYTHON::init_python(int argc, char **argv)
{
  if (Py_IsInitialized())
    {
      MESSAGE("Python already initialized");
      SCRUTE(KERNEL_PYTHON::_gtstate);
      return;
    }
  MESSAGE("=================================================================");
  MESSAGE("Python Initialization...");
  MESSAGE("=================================================================");
  Py_SetProgramName(argv[0]);
  Py_Initialize(); // Initialize the interpreter
  PySys_SetArgv(argc, argv);
  KERNEL_PYTHON::_interp = PyThreadState_Get()->interp;
  PyEval_InitThreads(); // Create (and acquire) the interpreter lock
  ASSERT(!KERNEL_PYTHON::_gtstate);
  KERNEL_PYTHON::_gtstate = PyEval_SaveThread(); // Release global thread state
  SCRUTE(KERNEL_PYTHON::_gtstate);
}

