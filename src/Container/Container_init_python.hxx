// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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
//  File   : Container_init_python.hxx
//  Author : Paul RASCLE, EDF
//  Module : KERNEL
//  $Header$
//
#ifndef _CONTAINER_INIT_PYTHON_HXX_
#define _CONTAINER_INIT_PYTHON_HXX_

#include "SALOME_Container.hxx"
#include <SALOMEconfig.h>

#include <pthread.h>  // must be before Python.h !

#ifdef _XOPEN_SOURCE
#undef _XOPEN_SOURCE
#endif
#include <Python.h>


// next two MACRO must be used together only once inside a block
// -------------------------------------------------------------
// protect a sequence of Python calls:
// - Python lock must be acquired for these calls
// - new Python thread state allows multi thread use of the sequence:
//    - Python may release the lock within the sequence, so multiple
//      thread execution of the sequence may occur.
//    - For that case, each sequence call must use a specific Python
//      thread state.
//    - There is no need of C Lock protection of the sequence.


#define Py_ACQUIRE_NEW_THREAD \
  PyGILState_STATE gil_state = PyGILState_Ensure();

#define Py_RELEASE_NEW_THREAD \
  PyGILState_Release(gil_state);

struct CONTAINER_EXPORT KERNEL_PYTHON
{
  static void init_python(int argc, char **argv);
};

#endif
