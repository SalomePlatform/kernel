//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
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
//  File   : QAD_PyInterp.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

#include "QAD_PyInterp.h"
#include "utilities.h"

using namespace std;


#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif


/*!
 * constructor : multi Python interpreter, one per SALOME study.
 * calls initialize method defined in base class, which calls virtual methods
 * initstate & initcontext redefined here.
 */
QAD_PyInterp::QAD_PyInterp(): PyInterp_base()
{
}

QAD_PyInterp::~QAD_PyInterp()
{
}
 
/*!
 * EDF-CCAR
 * When SALOME uses multi Python interpreter feature,
 * Every study has its own interpreter and thread state (_tstate = Py_NewInterpreter())
 * This is fine because every study has its own modules (sys.modules) stdout and stderr
 * BUT some Python modules must be imported only once. In multi interpreter context Python
 * modules (*.py) are imported several times.
 * The pyqt module must be imported only once because it registers classes in a C module.
 * It's quite the same with omniorb modules (internals and generated with omniidl)
 * This problem is handled with "shared modules" defined in salome_shared_modules.py
 * These "shared modules" are imported only once and only copied in all the other interpreters
 * BUT it's not the only problem. Every interpreter has its own __builtin__ module. That's fine
 * but if we have copied some modules and imported others problems may arise with operations that
 * are not allowed in restricted execution environment. So we must impose that all interpreters
 * have identical __builtin__ module.
 * That's all, for the moment ...
 */

void QAD_PyInterp::initState()
{
  _tstate = Py_NewInterpreter(); // create an interpreter and save current state
  PySys_SetArgv(PyInterp_base::_argc,PyInterp_base::_argv); // initialize sys.argv
  if(MYDEBUG) MESSAGE("QAD_PyInterp::initState - this = "<<this<<"; _tstate = "<<_tstate);

  /*
   * If builtinmodule has been initialized all the sub interpreters
   * will have the same __builtin__ module
   */
  if(builtinmodule){ 
    PyObject *m = PyImport_GetModuleDict();
    PyDict_SetItemString(m, "__builtin__", builtinmodule);
    SCRUTE(builtinmodule->ob_refcnt); // builtinmodule reference counter
    _tstate->interp->builtins = PyModule_GetDict(builtinmodule);
    Py_INCREF(_tstate->interp->builtins);
  }
}


void QAD_PyInterp::initContext()
{
  PyObject *m = PyImport_AddModule("__main__");  // interpreter main module (module context)
  if(!m){
    if(MYDEBUG) MESSAGE("problem...");
    PyErr_Print();
    ASSERT(0);
    return;
  }  
  _g = PyModule_GetDict(m);          // get interpreter dictionnary context
  if(MYDEBUG) MESSAGE("QAD_PyInterp::initContext - this = "<<this<<"; _g = "<<_g);

  if(builtinmodule){
    PyDict_SetItemString(_g, "__builtins__", builtinmodule); // assign singleton __builtin__ module
  }

  // Debut modif CCAR
  // Import special module to change the import mechanism
  PyObjWrapper m1(PyImport_ImportModule("import_hook"));
  if(!m1){
    MESSAGE("initContext: problem with import_hook import");
    PyErr_Print();
    PyErr_Clear();
    ASSERT(0);
  }else{
    // Call init_shared_modules to initialize the shared import mechanism for modules 
    //that must not be imported twice
    PyObjWrapper m2(PyObject_CallMethod(m1,"init_shared_modules","O",salome_shared_modules_module));
    if(!m2){
      MESSAGE("initContext: problem with init_shared_modules call");
      PyErr_Print();
      PyErr_Clear();
      ASSERT(0);
    }
  }
  // Fin   modif CCAR
}
