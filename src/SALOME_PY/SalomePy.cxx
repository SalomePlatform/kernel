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
#include <vtkPythonUtil.h>

#include <vtkVersion.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

#include "QAD_Application.h"
#include "QAD_Desktop.h"
#include "QAD_Study.h"
#include "QAD_RightFrame.h"

#include "VTKViewer_ViewFrame.h"
#include "VTKViewer_RenderWindow.h"
#include "VTKViewer_RenderWindowInteractor.h"

#include "utilities.h"

using namespace std;


PyObject* GetPyClass(const char* theClassName){
  static PyObject *aVTKModule = NULL;
  if(!aVTKModule){
    if (VTK_MAJOR_VERSION > 3)
      aVTKModule = PyImport_ImportModule("libvtkRenderingPython"); 
    else
      aVTKModule = PyImport_ImportModule("libVTKGraphicsPython"); 
    if(PyErr_Occurred()){
      PyErr_Print();
      return NULL;
    }
  }
  PyObject* aVTKDict = PyModule_GetDict(aVTKModule);
  char* aClassName = const_cast<char*>(theClassName);
  PyObject* aPyClass = PyDict_GetItemString(aVTKDict,aClassName);
  //Py_DECREF(aVTKModule);
  return aPyClass;
}


VTKViewer_ViewFrame* GetVTKViewFrame(){
  QAD_Study* aStudy = QAD_Application::getDesktop()->getActiveStudy();
  QAD_StudyFrame* aStudyFrame = aStudy->getActiveStudyFrame();
  QAD_ViewFrame* aViewFrame = aStudyFrame->getRightFrame()->getViewFrame();
  return dynamic_cast<VTKViewer_ViewFrame*>(aViewFrame);
}


extern "C" PyObject *libSalomePy_getRenderer(PyObject *self, PyObject *args)
{
  if(VTKViewer_ViewFrame* aVTKViewFrame = GetVTKViewFrame()){
    PyObject* aPyClass = GetPyClass("vtkRenderer");
    vtkRenderer* aVTKObject = aVTKViewFrame->getRenderer();
    return PyVTKObject_New(aPyClass,aVTKObject);
  }
  return NULL;
}


extern "C" PyObject *libSalomePy_getRenderWindow(PyObject *self, PyObject *args)
{
  if(VTKViewer_ViewFrame* aVTKViewFrame = GetVTKViewFrame()){
    PyObject* aPyClass = GetPyClass("vtkRenderWindow");
    vtkRenderWindow* aVTKObject = aVTKViewFrame->getRW()->getRenderWindow();
    return PyVTKObject_New(aPyClass,aVTKObject);
  }
  return NULL;
}


extern "C" PyObject *libSalomePy_getRenderWindowInteractor(PyObject *self, PyObject *args)
{
  if(VTKViewer_ViewFrame* aVTKViewFrame = GetVTKViewFrame()){
    PyObject* aPyClass = GetPyClass("vtkRenderWindowInteractor");
    vtkRenderWindowInteractor* aVTKObject = aVTKViewFrame->getRWInteractor();
    return PyVTKObject_New(aPyClass,aVTKObject);
  }
  return NULL;
}


static PyMethodDef Module_Methods[] = 
  {
    {"getRenderer",libSalomePy_getRenderer,METH_NOARGS},
    {"getRenderWindow",libSalomePy_getRenderWindow,METH_NOARGS},
    {"getRenderWindowInteractor",libSalomePy_getRenderWindow,METH_NOARGS},
    {NULL, NULL}
  };


extern "C" void initlibSalomePy()
{
  static char modulename[] = "libSalomePy";
  PyObject* aModule = Py_InitModule(modulename, Module_Methods);
  PyObject* aDict = PyModule_GetDict(aModule);
  if(PyErr_Occurred()){
    PyErr_Print();
    return;
  }
  PyObject* anObj = libSalomePy_getRenderer(NULL,NULL);
  PyDict_SetItemString(aDict,"renderer",anObj);
  Py_DECREF(anObj);
}
