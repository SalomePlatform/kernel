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
 * CoupleType.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Tue Dec  9 14:51:53 2003
 * Projet : Salome 2
 *
 */

#include <assert.h>
#include "Batch_CoupleType.hxx"
using namespace std;

namespace Batch {

	// Conversion en chaine
  string CoupleType::affiche() const
  {
    return _data.str();
  }

	// Operateur d'affectation
  CoupleType & CoupleType::operator =(const Couple & C)
  {
    _data = C;
    return *this;
  }

	// Conversion en char
  CoupleType::operator Couple() const
  {
    return _data;
  }

	// Conversion en chaine
  CoupleType::operator string() const
  {
    return _data.str();
  }

	// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
	// qu'il faudra detruire ensuite manuellement
  GenericType * CoupleType::clone() const
  {
    CoupleType * pC = new CoupleType(this->_data);
    assert(pC != 0);
    return pC;
  }

}

