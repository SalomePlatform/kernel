#include <Launchers.hxx>

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
   PyThreadState *_save; 
   if( !Py_IsInitialized() ) {
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

