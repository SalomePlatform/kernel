// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//

#include <time.h>
#include <sys/time.h>

#include "utilities.h"

#include <Launchers.hxx>

using namespace std;

static int Launchers_IsLoaded = 0;
static PyObject * Launchers_module = 0;
static char * Launchers_name="Launchers";

PyThreadState *mainThreadState=0;

PyThreadState * getMainThreadState(){
  PyInterpreterState *interp;
  PyThreadState *p;
  interp=PyInterpreterState_Head();
  for (interp = PyInterpreterState_Head(); interp != NULL; ){
    if(interp->next == NULL)break;
    interp=interp->next;
  }
  for (p = interp->tstate_head; p != NULL; ){
    if(p->next == NULL)break;
    p=p->next;
  }
  return p;
}
PyThreadState *acquireMainThread(){
    PyEval_AcquireLock();
    return  PyThreadState_Swap(mainThreadState);
}
void releaseMainThread(PyThreadState *tstate){
    PyThreadState_Swap(tstate);
    PyEval_ReleaseLock();
}

void Launchers_assertInitialized() {
  MESSAGE("===========================================================");
  MESSAGE("Launchers_assertInitialized");
  MESSAGE("===========================================================");
   PyThreadState *_save; 
   if( !Py_IsInitialized() ) {
     MESSAGE("===========================================================");
     MESSAGE("Py_Initialize()");
     MESSAGE("===========================================================");
         Py_Initialize();
         PyEval_InitThreads();
         PyEval_SaveThread();
   }
   if( !Launchers_IsLoaded ) {
      mainThreadState=getMainThreadState();
      _save=acquireMainThread();
      Launchers_module=PyImport_ImportModule(Launchers_name);
      if(!Launchers_module){
        PyErr_Print();
        return;
      }
      Py_INCREF(Launchers_module);
      Launchers_IsLoaded = 1;
      releaseMainThread(_save);
   }
}

PyObject * getLauncher(char* arg0) {
    PyObject *v;
    PyThreadState *_save; 
    Launchers_assertInitialized();
    _save=acquireMainThread();
    v=PyObject_CallMethod(Launchers_module,"getLauncher","s",arg0);
    if(!v){
      PyErr_Print();
      return NULL;
    }
    releaseMainThread(_save);
    Py_INCREF(v);
    return v;
}

void Launcher_Slaunch(PyObject *self, char* arg1, char* arg2) {
    PyObject *v;
    PyThreadState *_save; 
    Launchers_assertInitialized();
    _save=acquireMainThread();
    v=PyObject_CallMethod(self,"Slaunch","ss",arg1,arg2);
    if(!v){
      PyErr_Print();
    }
    releaseMainThread(_save);
}

