/*
 * _typemap.i : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

%{
#include <string>
#include <list>
#include <map>
#include "Batch_Parametre.hxx"
#include "Batch_PyVersatile.hxx"
#include "Batch_JobId.hxx"
#include "Batch_FactBatchManager.hxx"
%}

# // supprime toutes les definitions par defaut => sert au debug
# %typemap(in) SWIGTYPE ;


# // construction d'un dictionnaire Python a partir d'un objet BatchManagerCatalog C++
%typemap(out) map<string, Batch::FactBatchManager *> *
{
  $result = PyDict_New();

  // on itere sur toutes les clefs de la map
  for(map<string, FactBatchManager *>::const_iterator it=(* $1).begin(); it!=(* $1).end(); it++) {
    string key = (*it).first;
    PyObject * obj  = SWIG_NewPointerObj((void *) (*it).second, SWIGTYPE_p_Batch__FactBatchManager, 0);
    PyDict_SetItem($result, PyString_FromString(key.c_str()), obj);
  }
}


# // construction d'un dictionnaire Python a partir d'un objet Parametre C++
%typemap(out) Parametre
{
  $result = PyDict_New();

  // on itere sur toutes les clefs de la map, et on passe par la classe PyVersatile
	// qui convertit un Versatile en PyObject et vice versa
  for(Parametre::const_iterator it=$1.begin(); it!=$1.end(); it++) {
    string key = (*it).first;
    PyVersatile PyV = (*it).second;
    PyDict_SetItem($result, PyString_FromString(key.c_str()), PyV);
  }
}


# // construction d'un objet Parametre C++ a partir d'un dictionnaire Python
%typemap(in) Parametre & (Parametre PM)
{
  if (!PyDict_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a dictionnary");
    return NULL;
  }

  try {	
  // on itere sur toutes les clefs du dictionnaire, et on passe par la classe PyVersatile
	// qui convertit un Versatile en PyObject et vice versa
	PyObject *key, *value;
	int pos = 0;
	while (PyDict_Next($input, &pos, &key, &value)) {
		string mk = PyString_AsString(key);
		PyVersatile PyV = value;
		PyV.setName(mk);
		PM[mk] = PyV;
	}

  $1 = &PM; // $1 est une reference donc on lui passe une adresse
  }
  catch (GenericException & ex) {
      string msg = ex.type + " : " + ex.message;
      PyErr_SetString(PyExc_RuntimeWarning, msg.c_str());
      return NULL;
  }
  catch (...) {
      PyErr_SetString(PyExc_RuntimeWarning, "unknown exception");
      return NULL;
  }
}


# // construction d'un objet Parametre C++ a partir d'un dictionnaire Python
%typemap(in) Parametre (Parametre PM)
{
  if (!PyDict_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a dictionnary");
    return NULL;
  }

  try {
  // on itere sur toutes les clefs du dictionnaire, et on passe par la classe PyVersatile
	// qui convertit un Versatile en PyObject et vice versa
	PyObject *key, *value;
	int pos = 0;
	while (PyDict_Next($input, &pos, &key, &value)) {
		string mk = PyString_AsString(key);
		PyVersatile PyV = value;
		PyV.setName(mk);
		PM[mk] = PyV;
	}

  $1 = PM;
  }
  catch (GenericException & ex) {
      string msg = ex.type + " : " + ex.message;
      PyErr_SetString(PyExc_RuntimeWarning, msg.c_str());
      return NULL;
  }
  catch (...) {
      PyErr_SetString(PyExc_RuntimeWarning, "unknown exception");
      return NULL;
  }
}


# // construction d'un dictionnaire Python a partir d'un objet Environnement C++
%typemap(out) Environnement
{
  $result = PyDict_New();

  // on itere sur toutes les clefs de la map
  for(Environnement::const_iterator it=$1.begin(); it!=$1.end(); it++) {
    string key = (*it).first;
    string val = (*it).second;
    PyDict_SetItem($result, 
		   PyString_FromString(key.c_str()),
		   PyString_FromString(val.c_str()));
  }
}


# // construction d'un objet Environnement C++ a partir d'un dictionnaire Python
%typemap(in) Environnement & (Environnement E)
{
  if (!PyDict_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a dictionnary");
    return NULL;
  }

	// on itere sur toutes les clefs du dictionnaire
	PyObject *key, *value;
	int pos = 0;
	while (PyDict_Next($input, &pos, &key, &value)) {
		string mk  = PyString_AsString(key);
		string val = PyString_AsString(value);
		E[mk] = val;
	}
  
  $1 = &E; // $1 est une reference donc on lui passe une adresse
}



# // construction d'un objet Environnement C++ a partir d'un dictionnaire Python
%typemap(in) Environnement (Environnement E)
{
  if (!PyDict_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a dictionnary");
    return NULL;
  }

	// on itere sur toutes les clefs du dictionnaire
	PyObject *key, *value;
	int pos = 0;
	while (PyDict_Next($input, &pos, &key, &value)) {
		string mk  = PyString_AsString(key);
		string val = PyString_AsString(value);
		E[mk] = val;
	}
  
  $1 = E;
}



# // construction d'une string Python a partir d'une string STL
%typemap(python,out) string
{
	$result = PyString_FromString($1.c_str());
}



# // construction d'une string STL a partir d'une string Python
#%typemap(in) string & (string S)
#{
##  if (!PyString_Check($input)) {
#    PyErr_SetString(PyExc_ValueError,"Expected a string");
#    return NULL;
#  }
#
#  S = string(PyString_AsString($input));
#	$1 = &S; // $1 est une reference donc on lui passe une adresse
#}



# // construction d'une string STL a partir d'une string Python
#%typemap(in) string (string S)
#{
##  if (!PyString_Check($input)) {
#    PyErr_SetString(PyExc_ValueError,"Expected a string");
#    return NULL;
#  }
#
#  S = string(PyString_AsString($input));
#	$1 = S;
#}
