/*
 * PyVersatile.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Mon Oct 13 12:01:12 2003
 * Projet : Salome 2
 *
 */

#ifndef _PYVERSATILE_H_
#define _PYVERSATILE_H_


#include <Python.h>
#include "Batch_Versatile.hxx"
#include "Batch_TypeMismatchException.hxx"
#include "Batch_ListIsFullException.hxx"
#include "Batch_InvalidArgumentException.hxx"

namespace Batch {

  class PyVersatile : public Versatile
  {
  public:
		// Constructeur a partir d'un objet Versatile
    PyVersatile(const Versatile &);

		// Constructeur a partir d'un PyObject
    PyVersatile(const PyObject *) throw(TypeMismatchException, ListIsFullException, InvalidArgumentException);

		// Conversion de type vers un PyObject
    operator PyObject *() const;

		// Operateur d'affectation a partir d'un objet Versatile
    PyVersatile & operator =(const Versatile &);

  protected:

  private:

  };

}

#endif
