using namespace std;
//=============================================================================
//  File      : SalomePy.cxx
//  Created   : ven nov 16 17:29:20 CET 2001
//  Author    : Paul RASCLE, EDF
//  Project   : SALOME
//  Copyright : EDF 2001
//  $Header$
//=============================================================================

#include "SALOMEGUI_Swig.hxx"
#include <Python.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPythonUtil.h>
#include "utilities.h"

extern "C"
{ 
  static PyObject * libSalomePy_getRenderer(PyObject *self, PyObject *args);
}

static PyObject *libSalomePy_getRenderer(PyObject *self, PyObject *args)
{
  if (!PyArg_ParseTuple(args, ":getRenderer"))
    return NULL;
  // exemple retournant systematiquement  Py_None
  Py_INCREF(Py_None);
  return Py_None;
}

static PyMethodDef Module_Methods[] = 
  {
    {"getRenderer",         libSalomePy_getRenderer,         METH_VARARGS},
    {NULL, NULL}
  };

extern "C" { void initlibSalomePy();}

void initlibSalomePy()
{
  PyObject *m, *d, *c, *md, *obj, *vtkclass;

  static char modulename[] = "libSalomePy";
  MESSAGE("---");
  m = Py_InitModule(modulename, Module_Methods);
  MESSAGE("---");
  d = PyModule_GetDict(m);
  MESSAGE("---");
  // DANGEROUS : if (!d) Py_FatalError("can't get dictionary for module SalomePy!");
  if (PyErr_Occurred())
    {
      MESSAGE("---");
      return;
    }

  m=PyImport_ImportModule("libVTKGraphicsPython"); // import module if not already imported (new ref)
  MESSAGE("---");

  if (PyErr_Occurred())
    {
      PyErr_Print();
      MESSAGE("---");
      return;
    }

  if ( m ) {
    md = PyModule_GetDict(m);                        // dict of libVTKGraphicsPython (borrowed ref ; not decref)
    MESSAGE("---");

    vtkclass=PyDict_GetItemString(md,"vtkRenderer"); // (borrowed ref ; not decref)
    Py_DECREF(m);                                    // no more need of m
    MESSAGE("---");
  }
//NRI
  vtkRenderer *renderer = SALOMEGUI_Swig::getRenderer();

  MESSAGE("---");
  obj = PyVTKObject_New(vtkclass,renderer);        // (new ref)
  MESSAGE( "Nombre de references sur obj : " << obj->ob_refcnt ); // sys.getrefcount(o) gives ref count + 1 in Python interp
  PyDict_SetItemString(d,"renderer",obj);                              // (add a ref to obj ; has to be  decref)
  MESSAGE( "Nombre de references sur obj : " << obj->ob_refcnt ); // sys.getrefcount(o) gives ref count + 1 in Python interp
  Py_DECREF(obj);                                                      // only one ref is sufficient
  MESSAGE( "Nombre de references sur obj : " << obj->ob_refcnt ); // sys.getrefcount(o) gives ref count + 1 in Python interp
//NRI

//   vtkclass=PyDict_GetItemString(md,"vtkRenderWindowInteractor"); // (borrowed ref ; not decref)
//   Py_DECREF(m);                                    // no more need of m
//   MESSAGE("---");
//   vtkRenderWindowInteractor *RWInteractor = SALOMEGUI_Swig::getRWInteractor();
//   MESSAGE("---");
//   obj = PyVTKObject_New(vtkclass,RWInteractor);        // (new ref)
//   MESSAGE( "Nombre de references sur obj : " << obj->ob_refcnt ); // sys.getrefcount(o) gives ref count + 1 in Python interp
//   PyDict_SetItemString(d,"interactor",obj);                              // (add a ref to obj ; has to be  decref)
//   MESSAGE( "Nombre de references sur obj : " << obj->ob_refcnt ); // sys.getrefcount(o) gives ref count + 1 in Python interp
//   Py_DECREF(obj);                                                      // only one ref is sufficient
//   MESSAGE( "Nombre de references sur obj : " << obj->ob_refcnt ); // sys.getrefcount(o) gives ref count + 1 in Python interp
}


