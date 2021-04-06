// Copyright (C) 2019-2021  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include "SALOME_CPythonHelper.hxx"

#if PY_VERSION_HEX < 0x03050000
static char*
Py_EncodeLocale(const wchar_t *arg, size_t *size)
{
  return _Py_wchar2char(arg, size);
}
static wchar_t*
Py_DecodeLocale(const char *arg, size_t *size)
{
  return _Py_char2wchar(arg, size);
}
#endif

void SALOME_CPythonHelper::initializePython(int argc, char *argv[])
{
  Py_Initialize();
#if PY_VERSION_HEX < 0x03070000
  PyEval_InitThreads();
#endif
  wchar_t **changed_argv = new wchar_t*[argc]; // Setting arguments
  for (int i = 0; i < argc; i++)
    changed_argv[i] = Py_DecodeLocale(argv[i], NULL);
  PySys_SetArgv(argc, changed_argv);
  PyObject *mainmod(PyImport_AddModule("__main__"));
  _globals=PyModule_GetDict(mainmod);
  if(PyDict_GetItemString(_globals, "__builtins__") == NULL)
    {
      PyObject *bimod(PyImport_ImportModule("__builtin__"));
      if (bimod == NULL || PyDict_SetItemString(_globals, "__builtins__", bimod) != 0)
        Py_FatalError("can't add __builtins__ to __main__");
      Py_XDECREF(bimod);
    }
  _locals=PyDict_New();
  PyObject *tmp(PyList_New(0));
  _pickler=PyImport_ImportModuleLevel(const_cast<char *>("pickle"),_globals,_locals,tmp,0);
  _subprocess=PyImport_ImportModuleLevel(const_cast<char *>("subprocess"),_globals,_locals,tmp,0);
  PyObject *socket(PyImport_ImportModuleLevel(const_cast<char *>("socket"),_globals,_locals,tmp,0));
  PyDict_SetItemString(_globals,"sp",_subprocess);
  PyDict_SetItemString(_globals,"socket",socket);
}

void SALOME_CPythonHelper::registerToSalomePiDict(const std::string& processName, long pid) const
{
  PyObject *mod(PyImport_ImportModule("addToKillList"));//new value
  if(!mod)
    return;
  PyObject *meth(PyObject_GetAttrString(mod,"addToKillList"));//new value
  if(!meth)
    { Py_XDECREF(mod); return ; }
  PyObject *args(PyTuple_New(2));
  PyTuple_SetItem(args,0,PyLong_FromLong(pid));
  PyTuple_SetItem(args,1,PyUnicode_FromString(processName.c_str()));
  PyObject *res(PyObject_CallObject(meth,args));
  Py_XDECREF(args);
  Py_XDECREF(res);
  Py_XDECREF(meth);
  Py_XDECREF(mod);
}

std::vector<long> SALOME_CPythonHelper::evalVL(const std::string& pyCode) const
{
  PyObject* code(Py_CompileString(pyCode.c_str(),"evalVL.py", Py_eval_input));
  PyObject *res(PyEval_EvalCode( code, _globals, _locals));
  Py_DECREF(code);
  Py_ssize_t n(PyList_Size(res));
  std::vector<long> ret(n);
  for(auto i = 0; i<n; ++i)
    {
      PyObject *elt(PyList_GetItem(res,i));//borrowed
      ret[i]=PyLong_AsLong(elt);
    }
  Py_DECREF(res);
  return ret;
}

std::string SALOME_CPythonHelper::evalS(const std::string& pyCode) const
{
  PyObject* code(Py_CompileString(pyCode.c_str(),"evalS.py", Py_eval_input));
  PyObject *res(PyEval_EvalCode( code, _globals, _locals));
  Py_DECREF(code);
  std::string ret(PyUnicode_AsUTF8(res));
  Py_DECREF(res);
  return ret;
}

SALOME_CPythonHelper::~SALOME_CPythonHelper()
{
  // _globals is borrowed ref -> do nothing
  Py_XDECREF(_locals);
  Py_XDECREF(_pickler);
}
