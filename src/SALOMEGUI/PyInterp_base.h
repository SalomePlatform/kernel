//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : PyInterp_base.h
//  Author : Christian CAREMOLI, Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

#ifndef _PYINTERP_BASE_H_
#define _PYINTERP_BASE_H_

#include <list>
#include <string>
#include <iostream>

#include <Python.h>

class QSemaphore;
class QMutex;

extern "C" PyObject * PyEval_EvalCode(PyObject *co, PyObject *g, PyObject *l);

#define TOP_HISTORY_PY "--- top of history ---"
#define BEGIN_HISTORY_PY "--- begin of history ---"


class SemaphoreLock{
  QSemaphore* mySemaphore;
  std::string myComment;
 public:
  SemaphoreLock(QSemaphore* theSemaphore, const char* theComment = "");
  ~SemaphoreLock();
};


class PyLockWrapper{
  PyThreadState* myThreadState;
  PyThreadState* mySaveThreadState;
 public:
  PyLockWrapper(PyThreadState* theThreadState);
  ~PyLockWrapper();
};


class ThreadLock{
  QMutex* myMutex;
  std::string myComment;
 public:
  ThreadLock(QMutex* theMutex, const char* theComment = "");
  ~ThreadLock();
};


bool IsPyLocked();

ThreadLock GetPyThreadLock(const char* theComment = "");


class PyInterp_base{
 public:
  static PyThreadState *_gtstate;
  static int _argc;
  static char* _argv[];
  static PyObject *builtinmodule;
  static PyObject *salome_shared_modules_module;
  
  PyInterp_base();
  ~PyInterp_base();
  
  virtual void initialize();

  int run(const char *command); 

  PyLockWrapper GetLockWrapper();

  std::string getbanner(); 
  std::string getverr();
  std::string getvout();  

  const char * getPrevious();
  const char * getNext();    

 protected:
  PyThreadState * _tstate;
  PyObject * _vout;
  PyObject * _verr;
  PyObject * _g;
  std::list<std::string> _history;
  std::list<std::string>::iterator _ith;
  bool _atFirst;

  int simpleRun(const char* command);
  int initRun();

  virtual void initState() = 0;
  virtual void initContext() = 0;  
};


class PyObjWrapper{
  PyObject* myObject;
public:
  PyObjWrapper(PyObject* theObject): myObject(theObject) {}
  PyObjWrapper(): myObject(0) {}
  operator PyObject*(){
    return myObject;
  }
  PyObject* operator->(){
    return myObject;
  }
  PyObject* get(){
    return myObject;
  }
  bool operator!(){
    return !myObject;
  }
  bool operator==(PyObject* theObject){
    return myObject == theObject;
  }
  PyObject** operator&(){
    return &myObject;
  }
  PyObjWrapper& operator=(PyObjWrapper* theObjWrapper){
    Py_XDECREF(myObject);
    myObject = theObjWrapper->myObject;
    return *this;
  }
  virtual ~PyObjWrapper(){ 
    Py_XDECREF(myObject);
  }
};


#endif
