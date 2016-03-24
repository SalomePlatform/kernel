// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

%define DOCSTRING
"CALCIUM python wrapping : Superv_Component class
"
%enddef

%module(docstring=DOCSTRING) calcium

%feature("autodoc", "1");

%include cstring.i

%{
//C++ Includes 
#include <SALOMEconfig.h>
#include <Calcium.hxx>
#include <calcium.h>
#include <Superv_Component_i.hxx>
#include <Salome_file_i.hxx>
#include <omniORB4/CORBA.h>

//--- from omniORBpy.h (not present on Debian Sarge packages)

struct omniORBpyAPI {

  PyObject* (*cxxObjRefToPyObjRef)(const CORBA::Object_ptr cxx_obj,
           CORBA::Boolean hold_lock);
  // Convert a C++ object reference to a Python object reference.
  // If <hold_lock> is true, caller holds the Python interpreter lock.

  CORBA::Object_ptr (*pyObjRefToCxxObjRef)(PyObject* py_obj,
             CORBA::Boolean hold_lock);
  // Convert a Python object reference to a C++ object reference.
  // Raises BAD_PARAM if the Python object is not an object reference.
  // If <hold_lock> is true, caller holds the Python interpreter lock.

  PyObject* (*handleCxxSystemException)(const CORBA::SystemException& ex);
  // Sets the Python exception state to reflect the given C++ system
  // exception. Always returns NULL. The caller must hold the Python
  // interpreter lock.
};

  omniORBpyAPI* api;
  PyObject* dsc ;


%}

%init
%{
  // init section

#ifdef WITH_NUMPY
  import_array()
#endif

  PyObject* omnipy = PyImport_ImportModule((char*)"_omnipy");
  if (!omnipy)
  {
    PyErr_SetString(PyExc_ImportError,
        (char*)"Cannot import _omnipy");
    return;
  }
  PyObject* pyapi = PyObject_GetAttrString(omnipy, (char*)"API");
  api = (omniORBpyAPI*)PyCObject_AsVoidPtr(pyapi);
  Py_DECREF(pyapi);

  PyObject* engines = PyImport_ImportModule("Engines");
  dsc = PyObject_GetAttrString(engines, "DSC");
%}

%include <exception.i>



/*
 * Most of this code is borrowed from numpy distribution
 * The following code originally appeared in enthought/kiva/agg/src/numeric.i,
 * author unknown.  It was translated from C++ to C by John Hunter.  Bill
 * Spotz has modified it slightly to fix some minor bugs, add some comments
 * and some functionality.
 */

%{

#ifdef WITH_NUMPY
/* With Numpy */
#ifdef HAVE_ISINF
#undef HAVE_ISINF
#endif
#include <numpy/arrayobject.h>

typedef PyArrayObject ArrayObject;

/* Macros to extract array attributes.
 */
#define is_array(a)            ((a) && PyArray_Check((PyArrayObject *)a))
#define array_type(a)          (int)(PyArray_TYPE(a))
#define array_dimensions(a)    (((PyArrayObject *)a)->nd)
#define array_size(a,i)        (((PyArrayObject *)a)->dimensions[i])
#define array_is_contiguous(a) (PyArray_ISCONTIGUOUS(a))

const char* pytype_string(PyObject*);
const char* typecode_string(int);
int type_match(int, int);
int require_size(PyArrayObject*, int*, int);
int require_dimensions_n(PyArrayObject*, int*, int);
int require_dimensions(PyArrayObject*, int);
int require_contiguous(PyArrayObject*);
PyArrayObject* make_contiguous(PyArrayObject*, int*, int, int);
PyArrayObject* obj_to_array_no_conversion(PyObject*, int);
PyArrayObject* obj_to_array_allow_conversion(PyObject*, int, int*);
PyArrayObject* obj_to_array_contiguous_allow_conversion(PyObject*, int, int*);
PyArrayObject* obj_to_array_contiguous_allow_conversion(PyObject*, int, int*);

/* Given a PyObject, return a string describing its type.
 */
const char* pytype_string(PyObject* py_obj) {
  if (py_obj == NULL          ) return "C NULL value";
  if (PyCallable_Check(py_obj)) return "callable"    ;
  if (PyString_Check(  py_obj)) return "string"      ;
  if (PyInt_Check(     py_obj)) return "int"         ;
  if (PyFloat_Check(   py_obj)) return "float"       ;
  if (PyDict_Check(    py_obj)) return "dict"        ;
  if (PyList_Check(    py_obj)) return "list"        ;
  if (PyTuple_Check(   py_obj)) return "tuple"       ;
  if (PyFile_Check(    py_obj)) return "file"        ;
  if (PyModule_Check(  py_obj)) return "module"      ;
  if (PyInstance_Check(py_obj)) return "instance"    ;

  return "unknown type";
}

/*
For documentation only : numpy typecodes

enum NPY_TYPECHAR { NPY_BOOLLTR = '?',
                        NPY_BYTELTR = 'b',
                        NPY_UBYTELTR = 'B',
                        NPY_SHORTLTR = 'h',
                        NPY_USHORTLTR = 'H',
                        NPY_INTLTR = 'i',
                        NPY_UINTLTR = 'I',
                        NPY_LONGLTR = 'l',
                        NPY_ULONGLTR = 'L',
                        NPY_LONGLONGLTR = 'q',
                        NPY_ULONGLONGLTR = 'Q',
                        NPY_FLOATLTR = 'f',
                        NPY_DOUBLELTR = 'd',
                        NPY_LONGDOUBLELTR = 'g',
                        NPY_CFLOATLTR = 'F',
                        NPY_CDOUBLELTR = 'D',
                        NPY_CLONGDOUBLELTR = 'G',
                        NPY_OBJECTLTR = 'O',
                        NPY_STRINGLTR = 'S',
                        NPY_STRINGLTR2 = 'a',
                        NPY_UNICODELTR = 'U',
                        NPY_VOIDLTR = 'V',
                        NPY_CHARLTR = 'c',

                        NPY_INTPLTR = 'p',
                        NPY_UINTPLTR = 'P',

                        NPY_GENBOOLLTR ='b',
                        NPY_SIGNEDLTR = 'i',
                        NPY_UNSIGNEDLTR = 'u',
                        NPY_FLOATINGLTR = 'f',
                        NPY_COMPLEXLTR = 'c'
};
*/

/* Given a Numeric typecode, return a string describing the type.
 */
const char* typecode_string(int typecode) {
  const char* type_names[] = {"bool","byte","unsigned byte","short",
        "unsigned short","int","unsigned int","long","unsigned long",
        "longlong","unsigned longlong",
        "float","double","long double","complex float","complex double","complex long double",
        "object","string","unicode","void","ntypes","notype","char","unkown"};
  return type_names[typecode];
}

/* Make sure input has correct numeric type.  Allow character and byte
 * to match.  Also allow int and long to match.
 */
int type_match(int actual_type, int desired_type) {
  return PyArray_EquivTypenums(actual_type, desired_type);
}

/* Given a PyObject pointer, cast it to a PyArrayObject pointer if
 * legal.  If not, set the python error string appropriately and
 * return NULL./
 */
PyArrayObject* obj_to_array_no_conversion(PyObject* input, int typecode) {
  PyArrayObject* ary = NULL;
  if (is_array(input) && (typecode == PyArray_NOTYPE ||
        PyArray_EquivTypenums(array_type(input),
            typecode))) {
        ary = (PyArrayObject*) input;
    }
    else if is_array(input) {
      const char* desired_type = typecode_string(typecode);
      const char* actual_type = typecode_string(array_type(input));
      PyErr_Format(PyExc_TypeError,
       "Array of type '%s' required.  Array of type '%s' given",
       desired_type, actual_type);
      ary = NULL;
    }
    else {
      const char * desired_type = typecode_string(typecode);
      const char * actual_type = pytype_string(input);
      PyErr_Format(PyExc_TypeError,
       "Array of type '%s' required.  A %s was given",
       desired_type, actual_type);
      ary = NULL;
    }
  return ary;
}

/* Convert the given PyObject to a Numeric array with the given
 * typecode.  On Success, return a valid PyArrayObject* with the
 * correct type.  On failure, the python error string will be set and
 * the routine returns NULL.
 */
PyArrayObject* obj_to_array_allow_conversion(PyObject* input, int typecode,
                                             int* is_new_object)
{
  PyArrayObject* ary = NULL;
  PyObject* py_obj;
  if (is_array(input) && (typecode == PyArray_NOTYPE || type_match(array_type(input),typecode))) {
    ary = (PyArrayObject*) input;
    *is_new_object = 0;
  }
  else {
    py_obj = PyArray_FromObject(input, typecode, 0, 0);
    /* If NULL, PyArray_FromObject will have set python error value.*/
    ary = (PyArrayObject*) py_obj;
    *is_new_object = 1;
  }
  return ary;
}

/* Given a PyArrayObject, check to see if it is contiguous.  If so,
 * return the input pointer and flag it as not a new object.  If it is
 * not contiguous, create a new PyArrayObject using the original data,
 * flag it as a new object and return the pointer.
 */
PyArrayObject* make_contiguous(PyArrayObject* ary, int* is_new_object,
                               int min_dims, int max_dims)
{
  PyArrayObject* result;
  if (array_is_contiguous(ary)) {
    result = ary;
    *is_new_object = 0;
  }
  else {
    result = (PyArrayObject*) PyArray_ContiguousFromObject((PyObject*)ary,
                 array_type(ary),
                 min_dims,
                 max_dims);
    *is_new_object = 1;
  }
  return result;
}

/* Convert a given PyObject to a contiguous PyArrayObject of the
 * specified type.  If the input object is not a contiguous
 * PyArrayObject, a new one will be created and the new object flag
 * will be set.
 */
PyArrayObject* obj_to_array_contiguous_allow_conversion(PyObject* input,
                                                        int typecode,
                                                        int* is_new_object) {
  int is_new1 = 0;
  int is_new2 = 0;
  PyArrayObject* ary2;
  PyArrayObject* ary1 = obj_to_array_allow_conversion(input, typecode,
                  &is_new1);
  if (ary1) {
    ary2 = make_contiguous(ary1, &is_new2, 0, 0);
    if ( is_new1 && is_new2) {
      Py_DECREF(ary1);
    }
    ary1 = ary2;
  }
  *is_new_object = is_new1 || is_new2;
  return ary1;
}

/* Test whether a python object is contiguous.  If array is
 * contiguous, return 1.  Otherwise, set the python error string and
 * return 0.
 */
int require_contiguous(PyArrayObject* ary) {
  int contiguous = 1;
  if (!array_is_contiguous(ary)) {
    PyErr_SetString(PyExc_TypeError, "Array must be contiguous.  A discontiguous array was given");
    contiguous = 0;
  }
  return contiguous;
}

/* Require the given PyArrayObject to have a specified number of
 * dimensions.  If the array has the specified number of dimensions,
 * return 1.  Otherwise, set the python error string and return 0.
 */
int require_dimensions(PyArrayObject* ary, int exact_dimensions) {
  int success = 1;
  if (array_dimensions(ary) != exact_dimensions) {
    PyErr_Format(PyExc_TypeError,
     "Array must have %d dimensions.  Given array has %d dimensions",
     exact_dimensions, array_dimensions(ary));
    success = 0;
  }
  return success;
}

/* Require the given PyArrayObject to have one of a list of specified
 * number of dimensions.  If the array has one of the specified number
 * of dimensions, return 1.  Otherwise, set the python error string
 * and return 0.
 */
int require_dimensions_n(PyArrayObject* ary, int* exact_dimensions, int n) {
  int success = 0;
  int i;
  char dims_str[255] = "";
  char s[255];
  for (i = 0; i < n && !success; i++) {
    if (array_dimensions(ary) == exact_dimensions[i]) {
      success = 1;
    }
  }
  if (!success) {
    for (i = 0; i < n-1; i++) {
      sprintf(s, "%d, ", exact_dimensions[i]);
      strcat(dims_str,s);
    }
    sprintf(s, " or %d", exact_dimensions[n-1]);
    strcat(dims_str,s);
    PyErr_Format(PyExc_TypeError,
     "Array must have %s dimensions.  Given array has %d dimensions",
     dims_str, array_dimensions(ary));
  }
  return success;
}

/* Require the given PyArrayObject to have a specified shape.  If the
 * array has the specified shape, return 1.  Otherwise, set the python
 * error string and return 0.
 */
int require_size(PyArrayObject* ary, int* size, int n) {
  int i;
  int success = 1;
  int len;
  char desired_dims[255] = "[";
  char s[255];
  char actual_dims[255] = "[";
  for(i=0; i < n;i++) {
    if (size[i] != -1 &&  size[i] != array_size(ary,i)) {
      success = 0;
    }
  }
  if (!success) {
    for (i = 0; i < n; i++) {
      if (size[i] == -1) {
        sprintf(s, "*,");
      }
      else
      {
        sprintf(s, "%d,", size[i]);
      }
      strcat(desired_dims,s);
    }
    len = strlen(desired_dims);
    desired_dims[len-1] = ']';
    for (i = 0; i < n; i++) {
      sprintf(s, "%d,", array_size(ary,i));
      strcat(actual_dims,s);
    }
    len = strlen(actual_dims);
    actual_dims[len-1] = ']';
    PyErr_Format(PyExc_TypeError,
     "Array must have shape of %s.  Given array has shape of %s",
     desired_dims, actual_dims);
  }
  return success;
}

#else
/* Without Numpy */
typedef PyObject ArrayObject;

#endif
%}

%include "carrays.i" 

%array_class(int,    intArray);
%array_class(long,   longArray);
%array_class(float,  floatArray);
%array_class(double, doubleArray);

/* special struct to handle string arrays */
%inline %{
struct stringArray
{
  stringArray(int nelements,int size=0) {
    nelem=nelements;
    size=size;
    data= new char*[nelements];
    for(int i=0;i<nelements;i++)
    {
      data[i]=(char *)malloc((size+1)*sizeof(char));
      data[i][size+1]='\0';
    }
  }
  ~stringArray() 
  {
    for(int i=0;i<nelem;i++)
      free(data[i]);
    delete [] data;
  }
  char* __getitem__(int index) {
    return data[index];
  }
  void __setitem__(int index, char* value) {
    free(data[index]);
    data[index] = strdup(value);
  }
  char** data;
  int nelem;
  int size;
};
%}
/* End of special struct to handle string arrays */

/* input typemap 
   This typemap can be used for input array objects only.
   It accepts swig carray objects or numpy contiguous or non contiguous objects.
   In case of non-contiguous numpy object, it is converted (new object) into a contiguous numpy object
   This new object is deleted after the call.
*/
%define TYPEMAP_IN3(type,typecode)
%typemap(in) type* IN_ARRAY3
             (ArrayObject* array=NULL, int is_new_object) {
  if ((SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor,0)) == -1)
  {
%#ifdef WITH_NUMPY
    int size[1] = {-1};
    array = obj_to_array_contiguous_allow_conversion($input, typecode, &is_new_object);
    if (!array || !require_dimensions(array,1) || !require_size(array,size,1)) SWIG_fail;
    $1 = (type*) array->data;
%#else
    SWIG_exception(SWIG_TypeError, "type* expected");
%#endif
  }
}
%typemap(freearg) type* IN_ARRAY3 {
%#ifdef WITH_NUMPY
  if (is_new_object$argnum && array$argnum) 
    {
      Py_DECREF(array$argnum);
    }
%#endif
}
%enddef

TYPEMAP_IN3(int,     PyArray_INT)
TYPEMAP_IN3(long,    PyArray_LONG)
TYPEMAP_IN3(float,   PyArray_FLOAT )
TYPEMAP_IN3(double,  PyArray_DOUBLE)

#undef TYPEMAP_IN3

%apply int*    IN_ARRAY3 {int    *eval};
%apply long*   IN_ARRAY3 {long   *eval};
%apply float*  IN_ARRAY3 {float  *eval};
%apply double* IN_ARRAY3 {double *eval};

/*  Specific typemap for complex */
%typemap(in) float*  ecpval
             (ArrayObject* array=NULL, int is_new_object) {
  if ((SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor,0)) == -1)
  {
%#ifdef WITH_NUMPY
    int size[1] = {-1};
    array = obj_to_array_contiguous_allow_conversion($input, PyArray_CFLOAT, &is_new_object);
    if (!array || !require_dimensions(array,1) || !require_size(array,size,1)) SWIG_fail;
    $1 = (float*) array->data;
%#else
    SWIG_exception(SWIG_TypeError, "complex array expected");
%#endif
  }
}
%typemap(freearg) float* ecpval {
%#ifdef WITH_NUMPY
  if (is_new_object$argnum && array$argnum) 
    {
      Py_DECREF(array$argnum);
    }
%#endif
}
/* End of  Specific typemap for complex */

/* array of strings on input */
%typemap(in) (char** eval,int strSize)
         (ArrayObject* array=NULL, int is_new_object) {
  stringArray* sarray;
  if ((SWIG_ConvertPtr($input, (void **) &sarray, $descriptor(stringArray *),0)) == -1)
  {
%#ifdef WITH_NUMPY
    int size[1] = {-1};
    array = obj_to_array_contiguous_allow_conversion($input, PyArray_STRING, &is_new_object);
    if (!array || !require_dimensions(array,1) || !require_size(array,size,1)) SWIG_fail;
    $1 = (char**) malloc(array_size(array,0)*sizeof(char*));
    $2 = array->strides[0];
    for(int i=0;i<array_size(array,0);i++)
      {
        $1[i]=(char*) malloc(sizeof(char)*(array->strides[0]+1));
        strncpy($1[i],(char*) array->data + i* array->strides[0],array->strides[0]);
        *($1[i]+array->strides[0])='\0';
      }
%#else
    SWIG_exception(SWIG_TypeError, "string array expected");
%#endif
  }
  else
  {
    $1=sarray->data;
    $2=sarray->size;
  }
}

%typemap(freearg) (char** eval,int strSize) {
%#ifdef WITH_NUMPY
  if (array$argnum)
    {
      for(int i=0;i<array_size(array$argnum,0);i++)
        free($1[i]);
      free($1);
    }
  if (is_new_object$argnum && array$argnum) 
    {
      Py_DECREF(array$argnum);
    }
%#endif
}
/* End of array of strings on input */

/* inplace typemaps 
   This typemap can be used for input/output array objects.
   It accepts swig carray objects or numpy contiguous objects.
*/

%define TYPEMAP_INPLACE3(type,typecode)
%typemap(in) type* INPLACE_ARRAY3 (ArrayObject* temp=NULL) {
  if ((SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor,0)) == -1)
  {
%#ifdef WITH_NUMPY
    temp = obj_to_array_no_conversion($input,typecode);
    if (!temp  || !require_contiguous(temp)) SWIG_fail;
    $1 = (type*) temp->data;
%#else
    temp = NULL;
    SWIG_exception(SWIG_TypeError, "type* expected");
%#endif
  }
}
%enddef

TYPEMAP_INPLACE3(int,     PyArray_INT)
TYPEMAP_INPLACE3(long,    PyArray_LONG)
TYPEMAP_INPLACE3(float,   PyArray_FLOAT )
TYPEMAP_INPLACE3(double,  PyArray_DOUBLE)

#undef TYPEMAP_INPLACE3

%apply int*    INPLACE_ARRAY3 {int    *lval};
%apply long*   INPLACE_ARRAY3 {long   *lval};
%apply float*  INPLACE_ARRAY3 {float  *lval};
%apply double* INPLACE_ARRAY3 {double *lval};

/*  typemap for complex inout */
%typemap(in) float* lcpval
             (ArrayObject* temp=NULL) {
  if ((SWIG_ConvertPtr($input, (void **) &$1, $1_descriptor,0)) == -1)
  {
%#ifdef WITH_NUMPY
    temp = obj_to_array_no_conversion($input,PyArray_CFLOAT);
    if (!temp  || !require_contiguous(temp)) SWIG_fail;
    $1 = (float*) temp->data;
%#else
    temp = NULL;
    SWIG_exception(SWIG_TypeError, "complex array expected");
%#endif
  }
}
/*  End of typemap for complex inout */

/* typemap for array of strings on input/output */
%typemap(in) (char** lval,int strSize)
            (ArrayObject* temp=NULL) {
  stringArray* sarray;
  if ((SWIG_ConvertPtr($input, (void **) &sarray, $descriptor(stringArray *) ,0)) == -1)
  {
%#ifdef WITH_NUMPY
    temp = obj_to_array_no_conversion($input,PyArray_STRING);
    if (!temp  || !require_contiguous(temp)) SWIG_fail;
    $1 = (char**) malloc(array_size(temp,0)*sizeof(char*));
    $2 = temp->strides[0];
    for(int i=0;i<array_size(temp,0);i++)
      {
        $1[i]=(char*) temp->data+i*temp->strides[0];
        memset($1[i],0,temp->strides[0]); //numpy strings must be completed with 0 up to elsize
      }
%#else
    temp = NULL;
    SWIG_exception(SWIG_TypeError, "string array expected");
%#endif
  }
  else
  {
    $1=sarray->data;
    $2=sarray->size;
  }
}
%typemap(freearg) (char** lval,int strSize) {
%#ifdef WITH_NUMPY
  if (temp$argnum) free($1);
%#endif
}
/* End of typemap for array of strings on input/output */

%typemap(in) CORBA::Boolean
{
  $1=(CORBA::Boolean)PyInt_AsLong($input);
}
%typemap(out) CORBA::Boolean
{
  $result=PyInt_FromLong($1 ? 1 : 0);
}

%define CORBAPTR(type)
%typemap(in) type##_ptr
{
  Py_BEGIN_ALLOW_THREADS
  try
  {
     CORBA::Object_var obj = api->pyObjRefToCxxObjRef($input,0);
     $1 = type##::_narrow(obj);
  }
  catch(...)
  {
     Py_BLOCK_THREADS
     PyErr_SetString(PyExc_RuntimeError, "not a valid CORBA object ptr");
  }
  Py_END_ALLOW_THREADS
}
%typemap(freearg) type##_ptr {
  CORBA::release($1);
}
%enddef

CORBAPTR(CORBA::ORB)
CORBAPTR(Ports::PortProperties)
CORBAPTR(Ports::Port)
CORBAPTR(Engines::Container)
CORBAPTR(PortableServer::POA)

%typemap(out) Ports::Port_ptr 
{
  $result = api->cxxObjRefToPyObjRef($1, 1);
  //All output Ports::Port_ptr variables are duplicated by security. Need to release them for python.
  CORBA::release($1);
}

%typemap(out) Ports::PortProperties_ptr, Engines::Salome_file_ptr
{
  $result = api->cxxObjRefToPyObjRef($1, 1);
  //the _ptr is duplicated by the routine called. 
  //Need to release it for Python because the call to cxxObjRefToPyObjRef has created another ref with a count of 1
  CORBA::release($1);
}

%typemap(out) Engines::DSC::uses_port *
{
   $result = PyList_New($1->length());
   for (CORBA::ULong i=0; i < $1->length() ; i++)
     PyList_SetItem($result,i,api->cxxObjRefToPyObjRef((*$1)[i], 1));
   //delete the copy (created by new) of uses port sequence
   delete $1;
}

/*
 * Exception section
 */
// a general exception handler
%exception {
   Py_BEGIN_ALLOW_THREADS
   try {
      $action
   }
   catch(Engines::DSC::PortNotDefined& _e) {
      Py_BLOCK_THREADS
      PyObject* excc = PyObject_GetAttrString(dsc, "PortNotDefined");
      PyObject* exci = PyEval_CallObject(excc, (PyObject *)NULL);
      PyErr_SetObject(excc, exci);
      Py_XDECREF(excc);
      Py_XDECREF(exci);
      return NULL;
   }
   catch(Engines::DSC::PortNotConnected& _e) {
      Py_BLOCK_THREADS
      PyObject* excc = PyObject_GetAttrString(dsc, "PortNotConnected");
      PyObject* exci = PyEval_CallObject(excc, (PyObject *)NULL);
      PyErr_SetObject(excc, exci);
      Py_XDECREF(excc);
      Py_XDECREF(exci);
      return NULL;
   }
   catch(Engines::DSC::BadPortType& _e) {
      Py_BLOCK_THREADS
      PyObject* excc = PyObject_GetAttrString(dsc, "BadPortType");
      PyObject* exci = PyEval_CallObject(excc, (PyObject *)NULL);
      PyErr_SetObject(excc, exci);
      Py_XDECREF(excc);
      Py_XDECREF(exci);
      return NULL;
   }
   catch (SALOME_Exception &e) {
      Py_BLOCK_THREADS
      PyErr_SetString(PyExc_RuntimeError,e.what());
      return NULL;
   }
   catch (SALOME::SALOME_Exception &e) {
      Py_BLOCK_THREADS
      //This one should be converted into a python corba exception
      PyErr_SetString(PyExc_RuntimeError,e.details.text);
      return NULL;
   }
   catch (const CORBA::SystemException& e) {
      Py_BLOCK_THREADS 
      return api->handleCxxSystemException(e);
   }
   catch(...) {
      Py_BLOCK_THREADS
      PyErr_SetString(PyExc_ValueError,"Unknown exception");
      return NULL;
   }
   Py_END_ALLOW_THREADS
}

/*
 * End of Exception section
 */
namespace Engines
{
class DSC
{
  public:
    enum Message { AddingConnection, RemovingConnection, ApplicationError };
};
}

class PySupervCompo:public Superv_Component_i
{
  public:

    PySupervCompo(CORBA::ORB_ptr orb,
         PortableServer::POA_ptr poa,
         Engines::Container_ptr contai,
         const char *instanceName,
         const char *interfaceName);

    virtual ~PySupervCompo();
    CORBA::Boolean init_service(const char * service_name){return true;};
    virtual provides_port * create_provides_data_port(const char* port_fab_type)
        throw (BadFabType);
    virtual uses_port * create_uses_data_port(const char* port_fab_type)
        throw (BadFabType);
    virtual void add_port(const char * port_fab_type,
        const char * port_type,
        const char * port_name)
        throw (PortAlreadyDefined, BadFabType, BadType, BadProperty);
    template < typename SpecificPortType >
    SpecificPortType * add_port(const char * port_fab_type,
            const char * port_type,
            const char * port_name)
          throw (PortAlreadyDefined, BadFabType, BadType, BadCast, BadProperty);
    virtual void add_port(provides_port * port,
          const char* provides_port_name)
          throw (PortAlreadyDefined, NilPort, BadProperty);
    virtual void add_port(uses_port * port,
          const char* uses_port_name)
          throw (PortAlreadyDefined, NilPort, BadProperty);
    template <typename SpecificPortType >
    SpecificPortType * get_port( const char * port_name)
          throw (PortNotDefined, PortNotConnected, BadCast, UnexpectedState);
    virtual Ports::Port_ptr get_provides_port(const char* provides_port_name,
              const CORBA::Boolean connection_error)
              throw (Engines::DSC::PortNotDefined,
                     Engines::DSC::PortNotConnected,
                     Engines::DSC::BadPortType);
    virtual void connect_uses_port(const char* uses_port_name,
                         Ports::Port_ptr provides_port_ref)
              throw (Engines::DSC::PortNotDefined,
                     Engines::DSC::BadPortType,
                     Engines::DSC::NilPort);
    virtual void connect_provides_port(const char* provides_port_name)
              throw (Engines::DSC::PortNotDefined);
    virtual void disconnect_provides_port(const char* provides_port_name,
              const Engines::DSC::Message message)
              throw (Engines::DSC::PortNotDefined,
                     Engines::DSC::PortNotConnected);

    virtual void disconnect_uses_port(const char* uses_port_name,
                  Ports::Port_ptr provides_port_ref,
                  const Engines::DSC::Message message)
              throw (Engines::DSC::PortNotDefined,
                     Engines::DSC::PortNotConnected,
                     Engines::DSC::BadPortReference);

    virtual Ports::PortProperties_ptr get_port_properties(const char* port_name);

// Interface for Salome_file
    Engines::Salome_file_ptr getInputFileToService(const char* service_name, const char* Salome_file_name);
    void checkInputFilesToService(const char* service_name);
    Engines::Salome_file_ptr setInputFileToService(const char* service_name, const char* Salome_file_name);
    Engines::Salome_file_ptr getOutputFileToService(const char* service_name, const char* Salome_file_name);
    void checkOutputFilesToService(const char* service_name);
    Engines::Salome_file_ptr setOutputFileToService(const char* service_name, const char* Salome_file_name);
// End of Interface for Salome_file

// DSC interface for python components
  virtual void add_provides_port(Ports::Port_ptr ref, const char* provides_port_name, Ports::PortProperties_ptr port_prop);
  virtual void add_uses_port(const char* repository_id, const char* uses_port_name, Ports::PortProperties_ptr port_prop);
  virtual Engines::DSC::uses_port * get_uses_port(const char* uses_port_name);
  CORBA::Boolean is_connected(const char* port_name) throw (Engines::DSC::PortNotDefined);
// End of DSC interface for python components

   static void setTimeOut();


    %extend
      {
       //To get the address of the component
        long ptr()
        {
          return (long)self;
        }
      }
};

%apply int *OUTPUT { int *nval };
%apply float *INOUT { float  *ti };
%apply float *INPUT { float  *tf };
%apply int *INOUT { int  *niter };
%apply double *INOUT { double  *ti };
%apply double *INPUT { double  *tf };

extern "C" void create_calcium_port(Superv_Component_i* compo,char* name,char* type,char *mode,char* depend);

%ignore CPMESSAGE;
%include "calciumP.h"

%cstring_bounded_output(char *instanceName, 1024);

int  cp_cd(Superv_Component_i *component,char *instanceName);

int cp_een(Superv_Component_i *component,int dep,float  t,int n,char *nom,int nval,int    *eval);
int cp_edb(Superv_Component_i *component,int dep,double t,int n,char *nom,int nval,double *eval);
int cp_ere(Superv_Component_i *component,int dep,float  t,int n,char *nom,int nval,float  *eval);
int cp_erd(Superv_Component_i *component,int dep,float  t,int n,char *nom,int nval,float  *eval);
int cp_ecp(Superv_Component_i *component,int dep,float  t,int n,char *nom,int nval,float  *ecpval);
int cp_elo(Superv_Component_i *component,int dep,float  t,int n,char *nom,int nval,int    *eval);
int cp_ech(Superv_Component_i *component,int dep,float  t,int n,char *nom,int nval,char** eval,int strSize);
int cp_elg(Superv_Component_i *component,int dep,float  t,int n,char *nom,int nval,long   *eval);
int cp_eln(Superv_Component_i *component,int dep,float  t,int n,char *nom,int nval,long   *eval);


int cp_len(Superv_Component_i *component,int dep,float  *ti,float  *tf,int *niter,char *nom,int nmax,int *nval,int    *lval);
int cp_ldb(Superv_Component_i *component,int dep,double *ti,double *tf,int *niter,char *nom,int nmax,int *nval,double *lval);
int cp_lre(Superv_Component_i *component,int dep,float  *ti,float  *tf,int *niter,char *nom,int nmax,int *nval,float  *lval);
int cp_lrd(Superv_Component_i *component,int dep,float  *ti,float  *tf,int *niter,char *nom,int nmax,int *nval,float  *lval);
int cp_lcp(Superv_Component_i *component,int dep,float  *ti,float  *tf,int *niter,char *nom,int nmax,int *nval,float  *lcpval);
int cp_llo(Superv_Component_i *component,int dep,float  *ti,float  *tf,int *niter,char *nom,int nmax,int *nval,int    *lval);
int cp_lch(Superv_Component_i *component,int dep,float  *ti,float  *tf,int *niter,char *nom,int nmax,int *nval,char** lval,int strSize);
int cp_llg(Superv_Component_i *component,int dep,float  *ti,float  *tf,int *niter,char *nom,int nmax,int *nval,long   *lval);
int cp_lln(Superv_Component_i *component,int dep,float  *ti,float  *tf,int *niter,char *nom,int nmax,int *nval,long   *lval);

int cp_fini(Superv_Component_i *component,char *nom, int n);
int cp_fint(Superv_Component_i *component,char *nom, float t);
int cp_effi(Superv_Component_i *component,char *nom, int n);
int cp_efft(Superv_Component_i *component,char *nom, float t);

int cp_fin(Superv_Component_i *component,int cp_end);

