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
 * CharType.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#include <string>
#include <assert.h>
#include "Batch_CharType.hxx"
using namespace std;

namespace Batch {

	// Conversion en chaine
  string CharType::affiche() const
  {
    return string(1, _data);
  }

	// Operateur d'affectation
  CharType & CharType::operator =(char c)
  {
    _data = c;
    return *this;
  }

	// Conversion en char
  CharType::operator char() const
  {
    return this->_data;
  }

	// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
	// qu'il faudra detruire ensuite manuellement
  GenericType * CharType::clone() const
  {
    CharType * pC = new CharType(this->_data);
    assert(pC != 0);
    return pC;
  }

}
