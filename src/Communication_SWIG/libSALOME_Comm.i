%module libSALOME_Comm

%{
  #include "ReceiverFactory.hxx"
  #undef SEEK_SET
  #undef SEEK_CUR
  #undef SEEK_END
  #include "SALOME_Comm_i.hxx"
%}

%typemap(python,in) SALOME::SenderDouble_ptr
{
  PyObject* pdict = PyDict_New();
  PyDict_SetItemString(pdict, "__builtins__", PyEval_GetBuiltins());
  PyRun_String("import CORBA", Py_single_input, pdict, pdict);
 
  PyRun_String("o = CORBA.ORB_init([''], CORBA.ORB_ID);", Py_single_input,
                   pdict, pdict);
 
  PyObject* orb = PyDict_GetItemString(pdict, "o");

  // Ask omniORBpy to transform SUPPORT (python Corba) ptr to IOR string

  PyObject* iorSupport = PyObject_CallMethod(orb, "object_to_string", "O", $input);
 
  if (iorSupport == Py_None)
    return NULL;
  char * s = PyString_AsString(PyObject_Str(iorSupport));
 
  // Ask omniORB to convert IOR string to SALOME::SenderDouble_ptr

  int argc = 0;
  char *xargv = "";
  char **argv = &xargv;
  CORBA::ORB_var ORB = CORBA::ORB_init(argc, argv);
  CORBA::Object_var O =  ORB->string_to_object(s);
  SALOME::SenderDouble_ptr t = SALOME::SenderDouble::_narrow(O);
  $1 = t;
}

%typemap(python,in) SALOME::SenderInt_ptr
{
  PyObject* pdict = PyDict_New();
  PyDict_SetItemString(pdict, "__builtins__", PyEval_GetBuiltins());
  PyRun_String("import CORBA", Py_single_input, pdict, pdict);
 
  PyRun_String("o = CORBA.ORB_init([''], CORBA.ORB_ID);", Py_single_input,
                   pdict, pdict);
 
  PyObject* orb = PyDict_GetItemString(pdict, "o");

  // Ask omniORBpy to transform SUPPORT (python Corba) ptr to IOR string

  PyObject* iorSupport = PyObject_CallMethod(orb, "object_to_string", "O", $input);
 
  if (iorSupport == Py_None)
    return NULL;
  char * s = PyString_AsString(PyObject_Str(iorSupport));
 
  // Ask omniORB to convert IOR string to SALOME::SenderInt_ptr

  int argc = 0;
  char *xargv = "";
  char **argv = &xargv;
  CORBA::ORB_var ORB = CORBA::ORB_init(argc, argv);
  CORBA::Object_var O =  ORB->string_to_object(s);
  SALOME::SenderInt_ptr t = SALOME::SenderInt::_narrow(O);
  $1 = t;
}

PyObject * getValueForSenderDouble(SALOME::SenderDouble_ptr senderDouble);

%{
PyObject * getValueForSenderDouble(SALOME::SenderDouble_ptr senderDouble)
{
  PyObject *py_list;
  long size;
  double *ret=ReceiverFactory::getValue(senderDouble,size);
  py_list = PyList_New(size);
  for (int i=0; i < size; i++)
    {
            int err = PyList_SetItem(py_list, i, Py_BuildValue("d", (double) ret[i]));
            if(err)
              {
                char * message = "Error in SUPPORT::getTypes";
                PyErr_SetString(PyExc_RuntimeError, message);
                return NULL;
              }
    }
  PyObject * result = Py_BuildValue("O", py_list);
  delete [] ret;
  Py_DECREF(py_list);
  return result;
}
%}


PyObject * getValueForSenderInt(SALOME::SenderInt_ptr senderInt);

%{
PyObject * getValueForSenderInt(SALOME::SenderInt_ptr senderInt)
{
  PyObject *py_list;
  long size;
  int *ret=ReceiverFactory::getValue(senderInt,size);
  py_list = PyList_New(size);
  for (int i=0; i < size; i++)
    {
            int err = PyList_SetItem(py_list, i, Py_BuildValue("i", (int) ret[i]));
            if(err)
              {
                char * message = "Error in SUPPORT::getTypes";
                PyErr_SetString(PyExc_RuntimeError, message);
                return NULL;
              }
    }
  PyObject * result = Py_BuildValue("O", py_list);
  delete [] ret;
  Py_DECREF(py_list);
  return result;
}
%}
