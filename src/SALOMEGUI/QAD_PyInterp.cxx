using namespace std;
using namespace std;
//=============================================================================
//  File      : QAD_PyInterp.cxx
//  Created   : Thu Jun 14 14:03:58 2001
//  Author    : Nicolas REJNERI
//  Project   : SALOME
//  Module    : SALOMEGUI
//  Copyright : Open CASCADE
//  $Header$
//=============================================================================

//    TODO: 
//    control & limit history length 

#include "QAD_PyInterp.h"
#include "utilities.h"

/*!
 * constructor : multi Python interpreter, one per SALOME study.
 * calls initialize method defined in base class, which calls virtual methods
 * initstate & initcontext redefined here.
 */
QAD_PyInterp::QAD_PyInterp(): PyInterp_base()
{
  initialize();
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
  MESSAGE("QAD_PyInterp::initState");
  salomeAcquireLock();           //acquire python global lock (one for all interpreters)
  _tstate = Py_NewInterpreter(); //create an interpreter and save current state
  SCRUTE(_tstate);
  SCRUTE(PyInterp_base::_argc);
  SCRUTE(PyInterp_base::_argv[0]);
  PySys_SetArgv(PyInterp_base::_argc,PyInterp_base::_argv);    // initialize sys.argv

  if(builtinmodule == NULL)return;
  /*
   * If builtinmodule has been initialized all the sub interpreters
   * will have the same __builtin__ module
   */
  PyObject *m=PyImport_GetModuleDict();
  PyDict_SetItemString(m, "__builtin__", builtinmodule);
  SCRUTE(builtinmodule->ob_refcnt);                            // builtinmodule reference counter
  _tstate->interp->builtins = PyModule_GetDict(builtinmodule);
  Py_INCREF(_tstate->interp->builtins);
}

void QAD_PyInterp::initContext()
{
  MESSAGE("QAD_PyInterp::initContext");
  PyObject *m;
  m=PyImport_AddModule("__main__");  // interpreter main module (module context)
  if(m == NULL)
    {
      MESSAGE("problem...");
      PyErr_Print();
      ASSERT(0);
      salomeReleaseLock(); 
      return;
    }  
  _g = PyModule_GetDict(m);          // get interpreter dictionnary context
  SCRUTE(_g);

  if(builtinmodule)
    {
      PyDict_SetItemString(_g, "__builtins__", builtinmodule); // assign singleton __builtin__ module
    }
// Debut modif CCAR
  /*
   * Import special module to change the import mechanism
   */
  m =PyImport_ImportModule("import_hook");
  if(m == NULL){
      MESSAGE("initContext: problem with import_hook import");
      PyErr_Print();
      PyErr_Clear();
      ASSERT(0);
  }
  /*
   * Call init_shared_modules to initialize the shared import mechanism for modules 
   * that must not be imported twice
   */
  if(m != NULL){
      m= PyObject_CallMethod(m,
			 "init_shared_modules","O",salome_shared_modules_module);
      if (m == NULL){
          MESSAGE("initContext: problem with init_shared_modules call");
          PyErr_Print();
          PyErr_Clear();
          ASSERT(0);
      }
  }
// Fin   modif CCAR

}
