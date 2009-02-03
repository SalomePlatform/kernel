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
 * PyVersatile.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Mon Oct 13 12:01:12 2003
 * Projet : Salome 2
 *
 */

#include <Python.h>
#include "Batch_TypeMismatchException.hxx"
#include "Batch_ListIsFullException.hxx"
#include "Batch_InvalidArgumentException.hxx"
#include "Batch_PyVersatile.hxx"
#include <string>

using namespace std;

namespace Batch {

  // Constructeur a partir d'un objet Versatile
  PyVersatile::PyVersatile(const Versatile & V) : Versatile(V)
  {
    // Nothing to do
  }


  // Constructeur a partir d'un PyObject
  // Les objets autorises sont les strings et les ints,
  // ainsi que les listes de strings
  PyVersatile::PyVersatile(const PyObject * PyO) throw(TypeMismatchException, ListIsFullException, InvalidArgumentException) : Versatile()
  {
    PyObject * _PyO = const_cast<PyObject *>(PyO);

    if (PyList_Check(_PyO)) { // c'est une liste
      _maxsize = PyList_Size(_PyO);
      for(int i=0; i<_maxsize; i++) {
	PyObject * val = PyList_GetItem(_PyO, i);
	if (PyString_Check(val)) {
	  *this += PyString_AsString(val);
					
	} else if (PyTuple_Check(val) && 
		   (PyTuple_Size(val) == 2) &&
		   PyString_Check( PyTuple_GetItem(val,0) ) && 
		   PyString_Check( PyTuple_GetItem(val,1) )   ) {
	  *this += Couple( PyString_AsString( PyTuple_GetItem(val,0) ),
			   PyString_AsString( PyTuple_GetItem(val,1) )
			   );
					
	} else {
	  PyErr_SetString(PyExc_RuntimeWarning, "PyVersatile::PyVersatile(const PyObject * PyO) : invalid PyObject");
	}
      }

    } else if (PyString_Check(_PyO)) { // c'est une string
      const char * s = PyString_AsString(_PyO);
      Versatile V = string(s);
      *this = V;
      
    } else if (PyInt_Check(_PyO)) { // c'est un int
      *this = PyInt_AsLong(_PyO);

    } else { // erreur
      PyErr_SetString(PyExc_RuntimeWarning, "PyVersatile::PyVersatile(const PyObject * PyO) : invalid PyObject");
    }
  }



  // Conversion de type vers un PyObject
  PyVersatile::operator PyObject *() const
  {
    PyObject * obj;

    if (_maxsize != 1) { // une liste
      obj = PyList_New(0);
      for(Versatile::const_iterator it=begin(); it!=end(); it++) {
//	char ch[2] = {0, 0};
	string st;
	Couple cp;
// 	PyObject * tuple;
	switch (_discriminator) {
	  // 	case BOOL:
	  // 	  PyList_Append(obj, PyInt_FromLong(* static_cast<BoolType *>(*it)));
	  // 	  break;

	  // 	case CHAR:
	  // 	  *ch = * static_cast<CharType *>(*it);
	  // 	  PyList_Append(obj, PyString_FromString(ch));
	  // 	  break;

	  // 	case INT:
	  // 	  PyList_Append(obj, PyInt_FromLong(* static_cast<IntType *>(*it)));
	  // 	  break;

	case LONG:
	  PyList_Append(obj, PyInt_FromLong(* static_cast<LongType *>(*it)));
	  break;

	case STRING:
	  st = * static_cast<StringType *>(*it);
	  PyList_Append(obj, PyString_FromString(st.c_str()));
	  break;

	case COUPLE:
	  cp = * static_cast<CoupleType *>(*it);
// 	  tuple = PyTuple_New(2);
// 	  PyTuple_SetItem(tuple, 0, PyString_FromString( cp.getLocal().c_str()  ) );
// 	  PyTuple_SetItem(tuple, 1, PyString_FromString( cp.getRemote().c_str() ) );
// 	  PyList_Append(obj, tuple);
	  PyList_Append(obj, Py_BuildValue("(ss)", cp.getLocal().c_str(), cp.getRemote().c_str() ));
	  break;

	case UNDEFINED:
	  PyList_Append(obj, Py_None);
	  break;
	}

      }

    } else { // un scalaire
//      char ch[2] = {0, 0};
      string st;
      Couple cp;
//       PyObject * tuple;
      switch (_discriminator) {
	//       case BOOL:
	// 	obj = PyInt_FromLong(* static_cast<BoolType *>(front()));
	// 	break;

	//       case CHAR:
	// 	*ch = * static_cast<CharType *>(front());
	// 	obj = PyString_FromString(ch);
	// 	break;

	//       case INT:
	// 	obj = PyInt_FromLong(* static_cast<IntType *>(front()));
	// 	break;

      case LONG:
	obj = PyInt_FromLong(* static_cast<LongType *>(front()));
	break;

      case STRING:
	st = * static_cast<StringType *>(front());
	obj = PyString_FromString(st.c_str());
	break;

      case COUPLE:
	cp = * static_cast<CoupleType *>(front());
// 	tuple = PyTuple_New(2);
// 	PyTuple_SetItem(tuple, 0, PyString_FromString( cp.getLocal().c_str()  ) );
// 	PyTuple_SetItem(tuple, 1, PyString_FromString( cp.getRemote().c_str() ) );
// 	obj = PyList_New(0);
// 	PyList_Append(obj, tuple);
	obj = Py_BuildValue("[(ss)]", cp.getLocal().c_str(), cp.getRemote().c_str() );
	break;

      case UNDEFINED:
	obj = Py_None;
	break;
      }
    }

    return obj;
  }


  // Operateur d'affectation a partir d'un objet Versatile
  PyVersatile & PyVersatile::operator =(const Versatile & V)
  {
    Versatile * me = this;
    *me = V;
    return *this;
  }

}


// COMMENTS
