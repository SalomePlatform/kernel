//  SALOME SALOME_PY : binding of VTK graphics and Python
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
//  File   : SalomePy.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#include <Python.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkPythonUtil.h>
#include <vtkVersion.h>
#include "utilities.h"
#include "QAD_Study.h"
#include "QAD_RightFrame.h"
#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "VTKViewer_ViewFrame.h"
using namespace std;

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

  if (VTK_MAJOR_VERSION > 3)
    m=PyImport_ImportModule("libvtkRenderingPython"); // import module if not already imported (new ref)
  else
    m=PyImport_ImportModule("libVTKGraphicsPython"); // import module if not already imported (new ref)
    
  MESSAGE("---");

  if (PyErr_Occurred())
    {
      PyErr_Print();
      MESSAGE("---");
      return;
    }

  if ( m ) {
    md = PyModule_GetDict(m);                        // dict of libvtkGraphicsPython (borrowed ref ; not decref)
    MESSAGE("---");

    vtkclass=PyDict_GetItemString(md,"vtkRenderer"); // (borrowed ref ; not decref)
    Py_DECREF(m);                                    // no more need of m
    MESSAGE("---");
  }
//NRI
  //san:T3.13 - move getRenderer() implementation here
  //vtkRenderer *renderer = SALOMEGUI_Swig::getRenderer();
  QAD_Study* myStudy = QAD_Application::getDesktop()->getActiveStudy();
  int nbStudyFrames = myStudy->getStudyFramesCount();
  vtkRenderer *renderer = NULL;
  int viewId = -1;
  if (viewId == -1) // find the first frame with VTK viewer & get renderer
    {
      int i=0;
      for(i=0; i<nbStudyFrames; i++)
	{
	  if ( myStudy->getStudyFrame(i)->getTypeView() == VIEW_VTK )
	    {
	      renderer = ((VTKViewer_ViewFrame*)myStudy->getStudyFrame(i)->getRightFrame()->getViewFrame())->getRenderer();
	      break;
	    }
	}
    }
  else     // get the VTK renderer of a given frame
    {
      SCRUTE(viewId);
      if ((viewId >=0) && (viewId <nbStudyFrames))
	renderer = ((VTKViewer_ViewFrame*)myStudy->getStudyFrame(viewId)->getRightFrame()->getViewFrame())->getRenderer();
    }
  if (renderer == NULL) INFOS("No VTK Renderer available !");
  //san:T3.13 - move getRenderer() implementation here

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


