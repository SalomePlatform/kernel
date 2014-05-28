// Copyright (C) 2007-2014  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

//  SALOME Container : implementation of container and engine for Kernel
//  File   : Container_init_python.cxx
//  Author : Paul RASCLE, EDF
//  Module : KERNEL
//  $Header$
//
#include <time.h>
#ifndef WIN32
  #include <sys/time.h>
#endif

#include "utilities.h"

#include "Container_init_python.hxx"

void KERNEL_PYTHON::init_python(int argc, char **argv)
{
  if (Py_IsInitialized())
    {
      MESSAGE("Python already initialized");
      return;
    }
  MESSAGE("=================================================================");
  MESSAGE("Python Initialization...");
  MESSAGE("=================================================================");
  // set stdout to line buffering (aka C++ std::cout)
  setvbuf(stdout, (char *)NULL, _IOLBF, BUFSIZ);
  char* salome_python=getenv("SALOME_PYTHON");
  if(salome_python != 0)
    Py_SetProgramName(salome_python);
  Py_Initialize(); // Initialize the interpreter
  PySys_SetArgv(argc, argv);
  PyRun_SimpleString("import threading\n");
  PyEval_InitThreads(); // Create (and acquire) the interpreter lock
  PyEval_ReleaseLock();  // Py_InitThreads acquires the GIL
}

