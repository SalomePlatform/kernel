// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

%module libSALOME_Comm

%feature("autodoc", "1");

%{
  #include "ReceiverFactory.hxx"
  #include "MatrixClient.hxx"
  #include "SALOME_Comm_i.hxx"
  #include "SALOMEMultiComm.hxx"
  #include "SenderFactory.hxx"
%}

%typemap(in) SALOME::SenderDouble_ptr
{
  PyObject* pdict = PyDict_New();
  PyDict_SetItemString(pdict, "__builtins__", PyEval_GetBuiltins());
  PyRun_String("import CORBA", Py_single_input, pdict, pdict);
 
  PyRun_String("o = CORBA.ORB_init([''], CORBA.ORB_ID);", Py_single_input,
                   pdict, pdict);
 
  PyObject* orb = PyDict_GetItemString(pdict, "o");

  // Ask omniORBpy to transform SUPPORT (python Corba) ptr to IOR string

  PyObject* iorSupport = PyObject_CallMethod(orb, (char*)"object_to_string", (char*)"O", $input);
 
  if (iorSupport == Py_None)
    return NULL;
  char * s = PyString_AsString(PyObject_Str(iorSupport));
 
  // Ask omniORB to convert IOR string to SALOME::SenderDouble_ptr

  int argc = 0;
  char *xargv = (char*)"";
  char **argv = &xargv;
  CORBA::ORB_var ORB = CORBA::ORB_init(argc, argv);
  CORBA::Object_var O =  ORB->string_to_object(s);
  SALOME::SenderDouble_ptr t = SALOME::SenderDouble::_narrow(O);
  $1 = t;
}

%typemap(in) SALOME::SenderInt_ptr
{
  PyObject* pdict = PyDict_New();
  PyDict_SetItemString(pdict, "__builtins__", PyEval_GetBuiltins());
  PyRun_String("import CORBA", Py_single_input, pdict, pdict);
 
  PyRun_String("o = CORBA.ORB_init([''], CORBA.ORB_ID);", Py_single_input,
                   pdict, pdict);
 
  PyObject* orb = PyDict_GetItemString(pdict, "o");

  // Ask omniORBpy to transform SUPPORT (python Corba) ptr to IOR string

  PyObject* iorSupport = PyObject_CallMethod(orb, (char*)"object_to_string", (char*)"O", $input);
 
  if (iorSupport == Py_None)
    return NULL;
  char * s = PyString_AsString(PyObject_Str(iorSupport));
 
  // Ask omniORB to convert IOR string to SALOME::SenderInt_ptr

  int argc = 0;
  char *xargv = (char*)"";
  char **argv = &xargv;
  CORBA::ORB_var ORB = CORBA::ORB_init(argc, argv);
  CORBA::Object_var O =  ORB->string_to_object(s);
  SALOME::SenderInt_ptr t = SALOME::SenderInt::_narrow(O);
  $1 = t;
}

%typemap(out) SALOME::SenderDouble_ptr
{  
   PyObject* pdict = PyDict_New();
   PyDict_SetItemString(pdict, "__builtins__", PyEval_GetBuiltins());
   PyRun_String("import CORBA", Py_single_input, pdict, pdict);
   PyRun_String("o = CORBA.ORB_init([''], CORBA.ORB_ID);", Py_single_input,
                   pdict, pdict);
   PyObject* orb = PyDict_GetItemString(pdict, "o");
   // Get the orb Corba C++
   int argc = 0;
   char *xargv = (char*)"";
   char **argv = &xargv;
   CORBA::ORB_var ORB = CORBA::ORB_init(argc, argv);
   std::string s =  ORB->object_to_string($1);
   PyObject * tmp = PyString_FromString(s.c_str());
   $result = PyObject_CallMethod(orb, (char*)"string_to_object", (char*)"O", tmp);
}

%typemap(out) SALOME::SenderInt_ptr
{  
   PyObject* pdict = PyDict_New();
   PyDict_SetItemString(pdict, "__builtins__", PyEval_GetBuiltins());
   PyRun_String("import CORBA", Py_single_input, pdict, pdict);
   PyRun_String("o = CORBA.ORB_init([''], CORBA.ORB_ID);", Py_single_input,
                   pdict, pdict);
   PyObject* orb = PyDict_GetItemString(pdict, "o");
   // Get the orb Corba C++
   int argc = 0;
   char *xargv = (char*)"";
   char **argv = &xargv;
   CORBA::ORB_var ORB = CORBA::ORB_init(argc, argv);
   std::string s =  ORB->object_to_string($1);
   PyObject * tmp = PyString_FromString(s.c_str());
   $result = PyObject_CallMethod(orb, (char*)"string_to_object", (char*)"O", tmp);
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
                const char * message = "Error in SUPPORT::getTypes";
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
                const char * message = "Error in SUPPORT::getTypes";
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

PyObject * getValueForMatrix(SALOME::Matrix_ptr matrix);
%{
PyObject * getValueForMatrix(SALOME::Matrix_ptr matrix)
{
  PyObject *py_list;
  int column,row;
  double *ret=MatrixClient::getValue(matrix,column,row);
  py_list = PyList_New(row);
  for(int i=0;i<row;i++)
    {
       PyObject *tmpRow=PyList_New(column);
       for(int j=0;j<column;j++)
         {
           int err = PyList_SetItem(tmpRow, j, Py_BuildValue("d", (double) ret[i*column+j]));
            if(err)
              {
                const char * message = "PyList_SetItem matrix sent may be invalid";
                PyErr_SetString(PyExc_RuntimeError, message);
                return NULL;
              }
         }
       PyList_SetItem(py_list,i,tmpRow);
       Py_DECREF(tmpRow);
    }
  delete [] ret;
  Py_DECREF(py_list);
  return py_list;
}
%}

SALOME::SenderDouble_ptr buildSenderDoubleFromList(PyObject *pylist);
%{
SALOME::SenderDouble_ptr buildSenderDoubleFromList(PyObject *pylist)
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

SALOME::SenderInt_ptr buildSenderIntFromList(PyObject *pylist);
%{
SALOME::SenderInt_ptr buildSenderIntFromList(PyObject *pylist)
{
  if (PyList_Check(pylist)) 
  {
    int listLgth = PyList_Size(pylist);
    int *tab=new int[listLgth];
    for (int i=0;i<listLgth;i++)
	{
	  tab[i]=PyInt_AsLong(PyList_GetItem(pylist,i));
	}
    SALOMEMultiComm communicator;
    return SenderFactory::buildSender(communicator,tab,listLgth,true);
  }
  else
  { 
    PyErr_SetString(PyExc_TypeError,"not a list");
    return SALOME::SenderInt::_nil();
  }
}
%}
