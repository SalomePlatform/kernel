//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : PyInterp_base.cxx
//  Author : Christian CAREMOLI, Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#include "PyInterp_base.h"
#include "utilities.h"
#include <string>
#include <vector>
#include <cStringIO.h>

using namespace std;

extern "C" PyObject * PyEval_EvalCode(PyObject *co, PyObject *g, PyObject *l);

static PyThreadState *savedThreadState = NULL;

/*!
 * We have our own routines which are identical to the SIP routines
 * to not depend from SIP software evolutions
 */

extern "C" void salomeAcquireLock()
{
  MESSAGE("salomeAcquireLock");
  PyEval_RestoreThread(savedThreadState);
  savedThreadState = NULL;
}

extern "C" void salomeReleaseLock()
{
  MESSAGE("salomeReleaseLock");
  savedThreadState = PyEval_SaveThread();
}

extern "C" int salomeCondAcquireLock()
{
  MESSAGE("salomeCondAcquireLock");
  if(savedThreadState != NULL)
    {
      /*
       * If savedThreadState is not NULL, Python global lock is not already acquired
       * We acquire it
       * and return 1 to the caller
       */
      salomeAcquireLock();
      //MESSAGE("We got it (the lock)");
      return 1;
    }
  /*
   * If savedThreadState is NULL, Python global lock is already acquired
   * We don't acquire or release it
   * We return 0 to the caller
   * CAUTION : it's only true when there is no event programming running (Tkinter, PyQt)
   */
  return 0;
}

extern "C" void salomeCondReleaseLock(int rellock)
{
  MESSAGE("salomeCondReleaseLock");
  if(rellock )
    salomeReleaseLock();
}

// main python interpreter

PyThreadState *PyInterp_base::_gtstate=0; // force 0 before execution
int PyInterp_base::_argc=1;
char* PyInterp_base::_argv[] = {""};

PyObject *PyInterp_base::builtinmodule=NULL;
PyObject *PyInterp_base::salome_shared_modules_module=NULL;

void init_python()
{
  SCRUTE(PyInterp_base::_gtstate);
  if (PyInterp_base::_gtstate) return;
  Py_Initialize();               // Initialize the interpreter
  PyEval_InitThreads();          // Create (and acquire) the interpreter lock
  PySys_SetArgv(PyInterp_base::_argc,PyInterp_base::_argv);      // initialize sys.argv
  PyInterp_base::_gtstate = PyThreadState_Get();
  SCRUTE(PyInterp_base::_gtstate);

  //   /*
  //    * Import __builtin__ module and store it to use it with all sub-interpreters
  //    * This hack could be used with event programming (PyQt) to avoid errors 
  //    * encountered with incoherent builtins
  //    */

  //   PyInterp_base::builtinmodule =PyImport_ImportModule("__builtin__");
  //   SCRUTE(PyInterp_base::builtinmodule->ob_refcnt);

  /*
   * Import salome_shared_modules module and store it to use it with all sub-interpreters
   */

  PyInterp_base::salome_shared_modules_module =PyImport_ImportModule("salome_shared_modules");
  if(PyInterp_base::salome_shared_modules_module == NULL){
      MESSAGE("init_python: problem with salome_shared_modules import");
      PyErr_Print();
      PyErr_Clear();
      salomeReleaseLock();
      return;
  }
  SCRUTE(PyInterp_base::salome_shared_modules_module->ob_refcnt);
  salomeReleaseLock();
}

/*!
 * This function compiles a string (command) and then evaluates it in the dictionnary
 * context if possible.
 * Returns :
 * -1 : fatal error 
 *  1 : incomplete text
 *  0 : complete text executed with success
 */

int compile_command(const char *command,PyObject *context)
{
  SCRUTE(command);
  PyObject *m,*v,*r;

  m=PyImport_AddModule("codeop");
  if(m == NULL)
    {
      /*
       * Fatal error. No way to go on.
       */
      PyErr_Print();
      return -1;
    }
  v= PyObject_CallMethod(m,"compile_command","s",command);
  if (v == NULL)
    {
      /*
       * Error encountered. It should be SyntaxError
       * so we don't write out traceback
       */
      PyObject *exception,*value,*tb;
      PyErr_Fetch(&exception, &value, &tb);
      PyErr_NormalizeException(&exception, &value, &tb);
      PyErr_Display(exception, value, NULL);
      Py_XDECREF(exception);
      Py_XDECREF(value);
      Py_XDECREF(tb);

      return -1;
    }
  else if (v == Py_None)
    {
      /*
       *  Incomplete text we return 1 : we need a complete text to execute
       */
      return 1;
    }
  else
    {
      /*
       * Complete and correct text. We evaluate it.
       */
      r  = PyEval_EvalCode(v,context,context);
      Py_DECREF(v);
      if (r==NULL)
	{
	  /*
	   * Execution error. We return -1
	   */
	  PyErr_Print();
	  return -1;
	}
      Py_DECREF(r);
      /*
       * The command has been successfully executed. Return 0
       */
      return 0;
    }
}

/*!
 * basic constructor here : herited classes constructors must call initalize() method
 * defined here.
 */

PyInterp_base::PyInterp_base():_tstate(0),_vout(0),_verr(0),_g(0),_atFirst(true)
{
  MESSAGE("PyInterp_base::PyInterp_base()");
}

PyInterp_base::~PyInterp_base()
{
  MESSAGE("PyInterp_base::~PyInterp_base()");
  salomeAcquireLock();
  PyThreadState_Swap(_tstate);
  Py_EndInterpreter(_tstate);
  salomeReleaseLock();
}

/*!
 * Must be called by herited classes constructors. initialize() calls virtuals methods
 * initstate & initcontext, not defined here in base class. initstate & initcontext methods
 * must be implemented in herited classes, following the Python interpreter policy
 * (mono or multi interpreter...).
 */
void PyInterp_base::initialize()
{
  MESSAGE("PyInterp_base::initialize()");
  PyObject *m,*v,*c;

  _history.clear();       // start a new list of user's commands 
  _ith = _history.begin();

  init_python();

  initState();

  initContext();

  m=PyImport_ImportModule("codeop"); // used to interpret & compile commands
  if(m == NULL)
    {
      MESSAGE("Problem...");
      PyErr_Print();
      ASSERT(0);
      salomeReleaseLock(); 
      return;
    }   
  Py_DECREF(m);   
  
  /*
   *  Create cStringIO to capture stdout and stderr
   */
  //PycString_IMPORT;
  PycStringIO=(PycStringIO_CAPI *)xxxPyCObject_Import("cStringIO", "cStringIO_CAPI");
  _vout=PycStringIO->NewOutput(128);
  _verr=PycStringIO->NewOutput(128);
  
  // All the initRun outputs are redirected to the standard output (console)
  this->initRun();

  // We go out of Python world to enter the C++ world. Release the Python global lock 
  salomeReleaseLock();
  SCRUTE(_tstate);
  SCRUTE(this);
}

string PyInterp_base::getbanner()
{
  MESSAGE("PyInterp_base::getbanner()");
  string banner = "Python ";
  banner = banner + Py_GetVersion() + " on " + Py_GetPlatform() ;
  banner = banner + "\ntype help to get general information on environment\n";
  return banner.c_str();
}

int PyInterp_base::initRun()
{
  MESSAGE("PyInterp_base::initRun()");
  PySys_SetObject("stderr",_verr);
  PySys_SetObject("stdout",_vout);

  PyObject *v = PyObject_CallMethod(_verr,"reset","");
  Py_XDECREF(v);
  v = PyObject_CallMethod(_vout,"reset","");
  Py_XDECREF(v);


  PyObject *m;
  m = PyImport_GetModuleDict();
  
  PySys_SetObject("stdout",PySys_GetObject("__stdout__"));
  PySys_SetObject("stderr",PySys_GetObject("__stderr__"));

  MESSAGE(this->getvout());
  MESSAGE(this->getverr());
    
  return 0;
}

void PyInterp_base::enter()
{
  MESSAGE("PyInterp_base::enter()");
  salomeAcquireLock();
  PyThreadState_Swap(_tstate);
}

void PyInterp_base::quit()
{
  MESSAGE("PyInterp_base::quit()");
  salomeReleaseLock();
}

void PyInterp_base::basicRun(const char *command)
{
  SCRUTE(command);
  PyObject *code,*r;
  enter();
  code=Py_CompileString((char *)command,"PyInterp_base",Py_file_input);
  if (code == NULL)
    {
      /*
       * Caught an error : SyntaxError
       * Print it and quit
       */
      PyErr_Print();
      quit();
      return;
    }
  r = PyEval_EvalCode(code,_g,_g);
  Py_DECREF(code);
  if (r==NULL)
    {
      /*
       * Caught an error during execution
       * Print it and quit
       */
      PyErr_Print();
      quit();
      return;
    }
  Py_DECREF(r);
  quit();
}

int PyInterp_base::run(const char *command)
{
  SCRUTE(command);
  int ret = 0;
  if (_atFirst)
    {
      _atFirst = false;
      ret = this->simpleRun("from Help import *");
      MESSAGE(this->getvout())
	MESSAGE(this->getverr())
	if (ret != 0) return ret;
      ret = this->simpleRun("import salome");
      MESSAGE(this->getvout());
      MESSAGE(this->getverr())
	if (ret != 0) return ret;
    }
  ret = this->simpleRun(command);
  return ret;
}

int PyInterp_base::simpleRun(const char *command)
{
  SCRUTE(command);
  PyObject *v,*m,*r,*g;
  char *output;
  int ier=0;
  string s_com = command;

  if (s_com.size() > 0)
    {
      _history.push_back(s_com);
      _ith = _history.end();
      SCRUTE(_history.back());
    }

  //   SCRUTE(this);
  //   SCRUTE(_tstate);
  // We come from C++ to enter Python world
  // We need to acquire the Python global lock
  salomeAcquireLock();
  // Restore the sub interpreter thread state : this._tstate
  PyThreadState_Swap(_tstate);

  /*
    Reset redirected outputs before treatment
  */
  PySys_SetObject("stderr",_verr);
  PySys_SetObject("stdout",_vout);

  v = PyObject_CallMethod(_verr,"reset","");
  Py_XDECREF(v);
  v = PyObject_CallMethod(_vout,"reset","");
  Py_XDECREF(v);
  
  ier=compile_command(command,_g);

  // Outputs are redirected on standards outputs (console)
  PySys_SetObject("stdout",PySys_GetObject("__stdout__"));
  PySys_SetObject("stderr",PySys_GetObject("__stderr__"));

  // We go back to the C++ world. Release the lock.
  salomeReleaseLock();
  return ier;
}

static string vout_buffer;
static string verr_buffer;

char * PyInterp_base::getverr()
{ 
  MESSAGE("PyInterp_base::getverr"); 
  PyObject *v;
  v=PycStringIO->cgetvalue(_verr);
  verr_buffer=PyString_AsString(v);
  Py_DECREF(v);
  return (char *)verr_buffer.c_str();
}

char * PyInterp_base::getvout()
{  
  MESSAGE("PyInterp_base::getvout"); 
  PyObject *v;
  v=PycStringIO->cgetvalue(_vout);
  vout_buffer=PyString_AsString(v);
  Py_DECREF(v);
  return (char *)vout_buffer.c_str();
}
 
const char * PyInterp_base::getPrevious()
{
  MESSAGE("PyInterp_base::getPrevious"); 
  if (_ith != _history.begin())
    {
      _ith--;
      return (*_ith).c_str();
    }
  else
    return BEGIN_HISTORY_PY;
}

const char * PyInterp_base::getNext()
{
  MESSAGE("PyInterp_base::getNext"); 
  if (_ith != _history.end())
    {
      _ith++;
    }
  if (_ith == _history.end())
    return TOP_HISTORY_PY;
  else
    return (*_ith).c_str();
}
