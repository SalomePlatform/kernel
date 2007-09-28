%define DOCSTRING
"CALCIUM python wrapping : Superv_Component class
"
%enddef

%module(docstring=DOCSTRING) calcium

%feature("autodoc", "0");

%{
//C++ Includes 
#include <Calcium.hxx>
#include <calcium.h>
#include <Superv_Component_i.hxx>
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


  omniORBpyAPI();
  // Constructor for the singleton. Sets up the function pointers.
};

  omniORBpyAPI* api;

%}

%init
%{
  // init section

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
%}

%include "carrays.i" 
%include "cpointer.i"

/* Wrap a class interface around an "int *" */
%pointer_class(int, intp);
/* Wrap a class interface around an "float *" */
%pointer_class(float, floatp);
/* Wrap a class interface around an "double *" */
%pointer_class(double, doublep);

%array_class(int, intArray);
%array_class(float, floatArray);
%array_class(double, doubleArray);

%typemap(python,in) CORBA::Boolean
{
  $1=(CORBA::Boolean)PyInt_AsLong($input);
}

%typemap(python,in) CORBA::ORB_ptr 
{
  try {
     CORBA::Object_ptr obj = api->pyObjRefToCxxObjRef($input,1);
     $1 = CORBA::ORB::_narrow(obj);
  }
  catch (...) {
     PyErr_SetString(PyExc_RuntimeError, "not a valid CORBA object ptr");
  }
}
%typemap(python,in) PortableServer::POA_ptr
{
  try {
     CORBA::Object_ptr obj = api->pyObjRefToCxxObjRef($input,1);
     $1 = PortableServer::POA::_narrow(obj);
  }
  catch (...) {
     PyErr_SetString(PyExc_RuntimeError, "not a valid CORBA object ptr");
  }
}

%typemap(python,in) Engines::Container_ptr
{
  try {
     CORBA::Object_ptr obj = api->pyObjRefToCxxObjRef($input,1);
     $1 = Engines::Container::_narrow(obj);
  }
  catch (...) {
     PyErr_SetString(PyExc_RuntimeError, "not a valid CORBA object ptr");
  }
}
%typemap(python,in) Ports::Port_ptr
{
  try {
     CORBA::Object_ptr obj = api->pyObjRefToCxxObjRef($input,1);
     $1 = Ports::Port::_narrow(obj);
  }
  catch (...) {
     PyErr_SetString(PyExc_RuntimeError, "not a valid CORBA object ptr");
  }
}

%typemap(python,out) Ports::Port_ptr , Ports::PortProperties_ptr
{
  $result = api->cxxObjRefToPyObjRef($1, 1);
}

/*
 * Exception section
 */
// a general exception handler
%exception {
   try {
      $action
   } catch(Engines::DSC::PortNotDefined& _e) {
      PyErr_SetString(PyExc_ValueError,"Port not defined");
      return NULL;
   } catch(Engines::DSC::PortNotConnected& _e) {
      PyErr_SetString(PyExc_ValueError,"Port not connected");
      return NULL;
   } catch(Engines::DSC::BadPortType& _e) {
      PyErr_SetString(PyExc_ValueError,"Bad port type");
      return NULL;
   } catch(...) {
      PyErr_SetString(PyExc_ValueError,"Unknown exception");
      return NULL;
   }
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

    %extend
      {
       //To get the address of the component
        long ptr()
        {
          return (long)self;
        }
      }
};

extern "C" void create_calcium_port(Superv_Component_i* compo,char* name,char* type,char *mode,char* depend);

#define   CP_TEMPS    40
#define   CP_ITERATION    41
#define   CP_SEQUENTIEL   42
#define   CP_CONT    20
#define   CP_ARRET   21

int cp_een(void *component,int dep,float t,int n,char *nom,int nval,int *val);
int cp_edb(void *component,int dep,double t,int n,char *nom,int nval,double *val);

int cp_len(void *component,int dep,float *INOUT,float *INPUT,int *INOUT,char *nom,int nmax,int *OUTPUT,int *val);
int cp_ldb(void *component,int dep,double *INOUT,double *INPUT,int *INOUT,char *nom,int nmax,int *OUTPUT,double *val);
int cp_fin(void *component,int cp_end);

