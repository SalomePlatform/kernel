// Copyright (C) 2007-2023  CEA/DEN, EDF R&D, OPEN CASCADE
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

#include <Python.h>
#include <structmember.h>
#ifndef WIN32
  #include <sys/time.h>
#endif
#include <string>

#include "utilities.h"
#include "Container_init_python.hxx"

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

/*
  The following functions are used to hook the Python
  interpreter output.
*/

static void ContainerPyStdOut_dealloc(ContainerPyStdOut *self)
{
  PyObject_Del(self);
}

static PyObject*
ContainerPyStdOut_write(ContainerPyStdOut *self, PyObject *args)
{
  char *c;
  if (!PyArg_ParseTuple(args, "s",&c))
    return NULL;
  if(self->_cb==NULL) {
    if ( self->_iscerr )
      std::cerr << c ;
    else
      std::cout << c ;
  }
  else {
    self->_cb(self->_data,c);
  }
  Py_INCREF(Py_None);
  return Py_None;
}

static PyObject*
ContainerPyStdOut_flush(ContainerPyStdOut */*self*/, PyObject */*args*/)
{
  Py_INCREF(Py_None);
  return Py_None;
}

static PyMethodDef ContainerPyStdOut_methods[] = {
  {"write",  (PyCFunction)ContainerPyStdOut_write,  METH_VARARGS, PyDoc_STR("write(string) -> None")},
  {"flush",  (PyCFunction)ContainerPyStdOut_flush,  METH_NOARGS,  PyDoc_STR("flush() -> None")},
  {0, 0, 0, 0} /* sentinel */
};

static PyMemberDef ContainerPyStdOut_memberlist[] = {
  {(char*)"softspace", T_INT,  offsetof(ContainerPyStdOut, softspace), 0, (char*)"flag indicating that a space needs to be printed; used by print"},
  {0, 0, 0, 0, 0} /* sentinel */
};

static PyTypeObject ContainerPyStdOut_Type = {
  /* The ob_type field must be initialized in the module init function
   * to be portable to Windows without using C++. */
  PyVarObject_HEAD_INIT(NULL, 0)
  /*0,*/                                  /*ob_size*/
  "PyOut",                                /*tp_name*/
  sizeof(ContainerPyStdOut),              /*tp_basicsize*/
  0,                                      /*tp_itemsize*/
  /* methods */
  (destructor)ContainerPyStdOut_dealloc,  /*tp_dealloc*/
  0,                                      /*tp_print*/
  0,                                      /*tp_getattr*/
  0,                                      /*tp_setattr*/
  0,                                      /*tp_compare*/
  0,                                      /*tp_repr*/
  0,                                      /*tp_as_number*/
  0,                                      /*tp_as_sequence*/
  0,                                      /*tp_as_mapping*/
  0,                                      /*tp_hash*/
  0,                                      /*tp_call*/
  0,                                      /*tp_str*/
  PyObject_GenericGetAttr,                /*tp_getattro*/
 /* softspace is writable:  we must supply tp_setattro */
  PyObject_GenericSetAttr,                /* tp_setattro */
  0,                                      /*tp_as_buffer*/
  Py_TPFLAGS_DEFAULT,                     /*tp_flags*/
  0,                                      /*tp_doc*/
  0,                                      /*tp_traverse*/
  0,                                      /*tp_clear*/
  0,                                      /*tp_richcompare*/
  0,                                      /*tp_weaklistoffset*/
  0,                                      /*tp_iter*/
  0,                                      /*tp_iternext*/
  ContainerPyStdOut_methods,              /*tp_methods*/
  ContainerPyStdOut_memberlist,           /*tp_members*/
  0,                                      /*tp_getset*/
  0,                                      /*tp_base*/
  0,                                      /*tp_dict*/
  0,                                      /*tp_descr_get*/
  0,                                      /*tp_descr_set*/
  0,                                      /*tp_dictoffset*/
  0,                                      /*tp_init*/
  0,                                      /*tp_alloc*/
  0,                                      /*tp_new*/
  0,                                      /*tp_free*/
  0,                                      /*tp_is_gc*/
  0,                                      /*tp_bases*/
  0,                                      /*tp_mro*/
  0,                                      /*tp_cache*/
  0,                                      /*tp_subclasses*/
  0,                                      /*tp_weaklist*/
  0,                                      /*tp_del*/
  0,                                      /*tp_version_tag*/
  0,                                      /*tp_finalize*/
};

static ContainerPyStdOut* ContainerNewPyStdOut( bool iscerr )
{
  ContainerPyStdOut *self;
  self = PyObject_New(ContainerPyStdOut, &ContainerPyStdOut_Type);
  if (self == NULL)
    return NULL;
  self->softspace = 0;
  self->_cb = NULL;
  self->_iscerr = iscerr;
  return self;
}

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
  char* env_python=getenv("SALOME_PYTHON");
  if(env_python != 0)
    {
      wchar_t* salome_python = Py_DecodeLocale(env_python, NULL);
      Py_SetProgramName(salome_python);
    }
  Py_Initialize(); // Initialize the interpreter
  if (Py_IsInitialized())
    {
      MESSAGE("Python initialized eh eh eh");
    }
  wchar_t **changed_argv = new wchar_t*[argc]; // Setting arguments
  for (int i = 0; i < argc; i++)
  {
    changed_argv[i] = Py_DecodeLocale(argv[i], NULL);
  }
  PySys_SetArgv(argc, changed_argv);

  PyRun_SimpleString("import threading\n");
  // VSR (22/09/2016): This is a workaround to prevent invoking qFatal() from PyQt5
  // causing application aborting
  std::string script;
  script += "def _custom_except_hook(exc_type, exc_value, exc_traceback):\n";
  script += "  import sys\n";
  script += "  sys.__excepthook__(exc_type, exc_value, exc_traceback)\n";
  script += "  pass\n";
  script += "\n";
  script += "import sys\n";
  script += "sys.excepthook = _custom_except_hook\n";
  script += "del _custom_except_hook, sys\n";
  PyRun_SimpleString(script.c_str());
  // VSR (22/09/2016): end of workaround
#if PY_VERSION_HEX < 0x03070000
  PyEval_InitThreads(); // Create (and acquire) the interpreter lock
#endif
  // Create python objects to capture stdout and stderr
  PyObject* _vout=(PyObject*)ContainerNewPyStdOut( false ); // stdout
  PyObject* _verr=(PyObject*)ContainerNewPyStdOut( true );  // stderr
  PySys_SetObject((char*)"stderr",_verr);
  PySys_SetObject((char*)"stdout",_vout);

  PyThreadState *pts = PyGILState_GetThisThreadState(); 
  PyEval_ReleaseThread(pts);
  //delete[] changed_argv;
}
