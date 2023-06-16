// Copyright (C) 2019-2023  CEA/DEN, EDF R&D, OPEN CASCADE
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
// Author : Anthony GEAY (EDF R&D)

#pragma once

#include "SALOME_Container.hxx"

#include "Python.h"

#include <string>
#include <vector>

class CONTAINER_EXPORT SALOME_CPythonHelper
{
 public:
  void initializePython(int argc, char *argv[]);
  void allowPythonCallsFromDifferentThread() const;
  void registerToSalomePiDict(const std::string& processName, long pid) const;
  std::vector<long> evalVL(const std::string& pyCode) const;
  std::string evalS(const std::string& pyCode) const;
  virtual ~SALOME_CPythonHelper();
  PyObject *getGlobals() const { return _globals; }
  PyObject *getLocals() const { return _locals; }
  PyObject *getPickler() const  { return _pickler; }
  static SALOME_CPythonHelper *Singleton();
  static void KillSingleton();
 private:
  static SALOME_CPythonHelper *_CPYTHONHELPER_INSTANCE;
 private:
  PyObject *_globals = nullptr;
  PyObject *_locals = nullptr;
  PyObject *_pickler = nullptr;
  PyObject *_subprocess = nullptr;
};
