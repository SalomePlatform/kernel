%{
  #include "ReceiverFactory.hxx"
  #include "SALOME_Comm_i.hxx"
  #include "SALOMEMultiComm.hxx"
  #include "SenderFactory.hxx"
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

%typemap(python,out) SALOME::SenderDouble_ptr
{  
   PyObject* pdict = PyDict_New();
   PyDict_SetItemString(pdict, "__builtins__", PyEval_GetBuiltins());
   PyRun_String("import CORBA", Py_single_input, pdict, pdict);
   PyRun_String("o = CORBA.ORB_init([''], CORBA.ORB_ID);", Py_single_input,
                   pdict, pdict);
   PyObject* orb = PyDict_GetItemString(pdict, "o");
   // Get the orb Corba C++
   int argc = 0;
   char *xargv = "";
   char **argv = &xargv;
   CORBA::ORB_var ORB = CORBA::ORB_init(argc, argv);
   string s =  ORB->object_to_string($1);
   PyObject * tmp = PyString_FromString(s.c_str());
   $result = PyObject_CallMethod(orb, "string_to_object", "O", tmp);
}

PyObject * getValueForSender(SALOME::SenderDouble_ptr senderDouble);

%{
PyObject * getValueForSender(SALOME::SenderDouble_ptr senderDouble)
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

SALOME::SenderDouble_ptr buildSenderFromList(PyObject *pylist);
%{
SALOME::SenderDouble_ptr buildSenderFromList(PyObject *pylist)
{
  if (PyList_Check(pylist)) 
  {
    int listLgth = PyList_Size(pylist);
    double *tab=new double[listLgth];
    for (int i=0;i<listLgth;i++)
	{
	  tab[i]=PyFloat_AsDouble(PyList_GetItem(pylist,i));
	}
    SALOMEMultiComm communicator;
    return SenderFactory::buildSender(communicator,tab,listLgth,true);
  }
  else
  { 
    PyErr_SetString(PyExc_TypeError,"not a list");
    return SALOME::SenderDouble::_nil();
  }
}
%}
