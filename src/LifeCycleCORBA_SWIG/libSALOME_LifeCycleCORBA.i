

%module libSALOME_LifeCycleCORBA

%{
#include "utilities.h"
#include "SALOME_LifeCycleCORBA.hxx"

  using namespace std;
%}

%typemap(python,out) Engines::Container_ptr, Engines::Component_ptr
{
  MESSAGE("typemap out on CORBA object ptr");
  SCRUTE($1);

  // --- Get the Python orb

  PyObject* pdict = PyDict_New();
  PyDict_SetItemString(pdict, "__builtins__", PyEval_GetBuiltins());
  PyRun_String("import CORBA", Py_single_input, pdict, pdict);
  PyRun_String("o = CORBA.ORB_init([''], CORBA.ORB_ID);", Py_single_input,
                   pdict, pdict);
  PyObject* orb = PyDict_GetItemString(pdict, "o");

  // --- Get the C++ orb

  int argc = 0;
  char *xargv = "";
  char **argv = &xargv;
  CORBA::ORB_var ORB = CORBA::ORB_init(argc, argv);
  string s =  ORB->object_to_string($1);
  SCRUTE(s);
  PyObject * tmp = PyString_FromString(s.c_str());
  SCRUTE(tmp);
  $result = PyObject_CallMethod(orb, "string_to_object", "O", tmp);
  SCRUTE($result);
}


%typemap(typecheck) const Engines::MachineParameters &
{
  $1 = ($input != 0);
}

%typemap(python,in) const Engines::MachineParameters &
{
  printf("typemap in on Engines::MachineParameters\n");
  //MESSAGE("typemap in on Engines::MachineParameters");
  //ASSERT (PyDict_Check($input))
  if (PyDict_Check($input) == 1)
    {
      Engines::MachineParameters *param = new Engines::MachineParameters ;
      param->container_name = CORBA::string_dup("");
      param->hostname = CORBA::string_dup("");
      param->OS = CORBA::string_dup("");
      param->mem_mb = 0;
      param->cpu_clock = 0;
      param->nb_proc_per_node = 0;
      param->nb_node = 0;
      param->isMPI = false;
      PyObject *key, *value;
      int pos = 0;
      while (PyDict_Next($input, &pos, &key, &value))
	{
	  char* keystr = PyString_AsString(key);
	  printf("key: %s\n", keystr);
	  if (strcmp(keystr,"container_name")==0)
	    {
	      param->container_name = CORBA::string_dup(PyString_AsString(value));
	    }
	  else if (strcmp(keystr,"hostname")==0)
	    {
	      param->hostname = CORBA::string_dup(PyString_AsString(value));
	    }
	  else if (strcmp(keystr,"OS")==0)
	    {
	      param->OS = CORBA::string_dup(PyString_AsString(value));
	    }
	  else if (strcmp(keystr,"mem_mb")==0)
	    {
	      param->mem_mb = PyLong_AsLong(value);
	    }
	  else if (strcmp(keystr,"cpu_clock")==0)
	    {
	      param->cpu_clock = PyLong_AsLong(value);
	    }
	  else if (strcmp(keystr,"nb_proc_per_node")==0)
	    {
	      param->nb_proc_per_node = PyLong_AsLong(value);
	    }
	  else if (strcmp(keystr,"nb_node")==0)
	    {
	      param->nb_node = PyLong_AsLong(value);
	    }
	  else if (strcmp(keystr,"isMPI")==0)
	    {
	      param->isMPI = PyLong_AsLong(value);
	    }
	}
      $1 = param;
    }
  else 
    {
       printf("pas un dico\n");
       return NULL;
    }
}

%typemap(python,freearg) const Engines::MachineParameters &
{
  MESSAGE("delete $1");
  delete $1;
}

%include "SALOME_LifeCycleCORBA.hxx"
