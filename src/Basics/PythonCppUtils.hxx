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

#include <Python.h>

class AutoGIL
{
public:
  AutoGIL():_gstate(PyGILState_Ensure()) { }
  ~AutoGIL() { PyGILState_Release(_gstate); }
private:
  PyGILState_STATE _gstate;
};

class AutoPyRef
{
public:
  AutoPyRef(PyObject *pyobj=nullptr):_pyobj(pyobj) { }
  ~AutoPyRef() { release(); }
  AutoPyRef(const AutoPyRef& other):_pyobj(other._pyobj) { if(_pyobj) Py_XINCREF(_pyobj); }
  AutoPyRef(AutoPyRef&& other) = default;
  AutoPyRef& operator=(const AutoPyRef& other) { if(_pyobj==other._pyobj) return *this; release(); _pyobj=other._pyobj; Py_XINCREF(_pyobj); return *this; }
  operator PyObject *() { return _pyobj; }
  void set(PyObject *pyobj) { if(pyobj==_pyobj) return ; release(); _pyobj=pyobj; }
  PyObject *get() { return _pyobj; }
  bool isNull() const { return _pyobj==0; }
  PyObject *retn() { if(_pyobj) Py_XINCREF(_pyobj); return _pyobj; }
protected:
  void release() { if(_pyobj) Py_XDECREF(_pyobj); _pyobj=nullptr; }
private:
  PyObject *_pyobj = nullptr;
};

class AutoPyRefGilSafe : public AutoPyRef
{
public:
  AutoPyRefGilSafe(PyObject *pyobj=nullptr):AutoPyRef(pyobj) { }
  ~AutoPyRefGilSafe() { AutoGIL agil; release(); }
};

class AutoPyYielder
{
private:
  PyThreadState *_save = nullptr;
public:
  AutoPyYielder() { _save = PyEval_SaveThread(); }
  ~AutoPyYielder() { PyEval_RestoreThread(_save); }
};
