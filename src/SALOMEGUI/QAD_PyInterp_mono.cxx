//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : QAD_PyInterp_mono.cxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#include "QAD_PyInterp_mono.h"
#include "utilities.h"
using namespace std;

/*!
 * constructor : only one Python interpreter, shared within SALOME studies.
 * calls initialize method defined in base class, which calls virtual methods
 * initstate & initcontext redefined here.
 */
QAD_PyInterp_mono::QAD_PyInterp_mono(): PyInterp_base()
{
  initialize();
}

QAD_PyInterp_mono::~QAD_PyInterp_mono()
{
}

/*!
 * EDF-CCAR
 * When SALOME uses mono  Python interpreter feature,
 * every study has its own context (dictionnary) but shares builtins
 * and all the modules (sys, ...) with other studies.
 * A module imported in a study is seen in another study (pros ans cons !).
 */

void QAD_PyInterp_mono::initState()
{
  salomeAcquireLock(); //acquire python global lock (one for all interpreters)
  _tstate = PyThreadState_Get();
  PySys_SetArgv(PyInterp_base::_argc,PyInterp_base::_argv);      // initialize sys.argv
}

void QAD_PyInterp_mono::initContext()
{
  _g = PyDict_New();          // create interpreter dictionnary context
  PyObject *bimod = PyImport_ImportModule("__builtin__");
  PyDict_SetItemString(_g, "__builtins__", bimod);
  Py_DECREF(bimod);
}

