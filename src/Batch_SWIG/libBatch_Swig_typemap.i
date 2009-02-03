//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
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
#if PY_VERSION_HEX < 0x02050000 && !defined(PY_SSIZE_T_MIN)
typedef int Py_ssize_t;
#define PY_SSIZE_T_MAX INT_MAX
#define PY_SSIZE_T_MIN INT_MIN
#endif
%}

# // supprime toutes les definitions par defaut => sert au debug
# %typemap(in) SWIGTYPE ;


# // construction d'un dictionnaire Python a partir d'un objet BatchManagerCatalog C++
%typemap(out) std::map<std::string, Batch::FactBatchManager *> *
{
  $result = PyDict_New();

  // on itere sur toutes les clefs de la map
  for(std::map<std::string, Batch::FactBatchManager *>::const_iterator it=(* $1).begin(); it!=(* $1).end(); it++) {
    std::string key = (*it).first;
    PyObject * obj  = SWIG_NewPointerObj((void *) (*it).second, SWIGTYPE_p_Batch__FactBatchManager, 0);
    PyDict_SetItem($result, PyString_FromString(key.c_str()), obj);
  }
}

%typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) Batch::Parametre
{
  $1 = PyDict_Check($input)? 1 : 0;
}

# // construction d'un dictionnaire Python a partir d'un objet Parametre C++
%typemap(out) Batch::Parametre
{
  $result = PyDict_New();

  // on itere sur toutes les clefs de la map, et on passe par la classe PyVersatile
	// qui convertit un Versatile en PyObject et vice versa
  for(Batch::Parametre::const_iterator it=$1.begin(); it!=$1.end(); it++) {
    std::string key = (*it).first;
    Batch::PyVersatile PyV = (*it).second;
    PyDict_SetItem($result, PyString_FromString(key.c_str()), PyV);
  }
}


# // construction d'un objet Parametre C++ a partir d'un dictionnaire Python
%typemap(in) Batch::Parametre & (Batch::Parametre PM)
{
  if (!PyDict_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a dictionnary");
    return NULL;
  }

  try {	
  // on itere sur toutes les clefs du dictionnaire, et on passe par la classe PyVersatile
	// qui convertit un Versatile en PyObject et vice versa
	PyObject *key, *value;
	Py_ssize_t pos = 0;
	while (PyDict_Next($input, &pos, &key, &value)) {
		std::string mk = PyString_AsString(key);
		Batch::PyVersatile PyV = value;
		PyV.setName(mk);
		PM[mk] = PyV;
	}

  $1 = &PM; // $1 est une reference donc on lui passe une adresse
  }
  catch (Batch::GenericException & ex) {
      std::string msg = ex.type + " : " + ex.message;
      PyErr_SetString(PyExc_RuntimeWarning, msg.c_str());
      return NULL;
  }
  catch (...) {
      PyErr_SetString(PyExc_RuntimeWarning, "unknown exception");
      return NULL;
  }
}


# // construction d'un objet Parametre C++ a partir d'un dictionnaire Python
%typemap(in) Batch::Parametre (Batch::Parametre PM)
{
  if (!PyDict_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a dictionnary");
    return NULL;
  }

  try {
  // on itere sur toutes les clefs du dictionnaire, et on passe par la classe PyVersatile
	// qui convertit un Versatile en PyObject et vice versa
	PyObject *key, *value;
	Py_ssize_t pos = 0;
	while (PyDict_Next($input, &pos, &key, &value)) {
		std::string mk = PyString_AsString(key);
		Batch::PyVersatile PyV = value;
		PyV.setName(mk);
		PM[mk] = PyV;
	}

  $1 = PM;
  }
  catch (Batch::GenericException & ex) {
      std::string msg = ex.type + " : " + ex.message;
      PyErr_SetString(PyExc_RuntimeWarning, msg.c_str());
      return NULL;
  }
  catch (...) {
      PyErr_SetString(PyExc_RuntimeWarning, "unknown exception");
      return NULL;
  }
}

%typemap(typecheck,precedence=SWIG_TYPECHECK_POINTER) Batch::Environnement
{
  $1 = PyDict_Check($input)? 1 : 0;
}

# // construction d'un dictionnaire Python a partir d'un objet Environnement C++
%typemap(out) Batch::Environnement
{
  $result = PyDict_New();

  // on itere sur toutes les clefs de la map
  for(Batch::Environnement::const_iterator it=$1.begin(); it!=$1.end(); it++) {
    std::string key = (*it).first;
    std::string val = (*it).second;
    PyDict_SetItem($result, 
		   PyString_FromString(key.c_str()),
		   PyString_FromString(val.c_str()));
  }
}


# // construction d'un objet Environnement C++ a partir d'un dictionnaire Python
%typemap(in) Batch::Environnement & (Batch::Environnement E)
{
  if (!PyDict_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a dictionnary");
    return NULL;
  }

	// on itere sur toutes les clefs du dictionnaire
	PyObject *key, *value;
	Py_ssize_t pos = 0;
	while (PyDict_Next($input, &pos, &key, &value)) {
		std::string mk  = PyString_AsString(key);
		std::string val = PyString_AsString(value);
		E[mk] = val;
	}
  
  $1 = &E; // $1 est une reference donc on lui passe une adresse
}



# // construction d'un objet Environnement C++ a partir d'un dictionnaire Python
%typemap(in) Batch::Environnement (Batch::Environnement E)
{
  if (!PyDict_Check($input)) {
    PyErr_SetString(PyExc_ValueError,"Expected a dictionnary");
    return NULL;
  }

	// on itere sur toutes les clefs du dictionnaire
	PyObject *key, *value;
	Py_ssize_t pos = 0;
	while (PyDict_Next($input, &pos, &key, &value)) {
		std::string mk  = PyString_AsString(key);
		std::string val = PyString_AsString(value);
		E[mk] = val;
	}
  
  $1 = E;
}



# // construction d'une string Python a partir d'une string STL
%typemap(python,out) std::string
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
