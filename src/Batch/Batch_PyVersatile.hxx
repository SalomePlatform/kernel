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


#include "Batch_Defines.hxx"

#include <Python.h>
#include "Batch_Versatile.hxx"
#include "Batch_TypeMismatchException.hxx"
#include "Batch_ListIsFullException.hxx"
#include "Batch_InvalidArgumentException.hxx"

namespace Batch {

  class BATCH_EXPORT PyVersatile : public Versatile
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
