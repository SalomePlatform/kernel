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
 * DateType.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Wed Nov 26 11:15:34 2003
 * Projet : Salome 2
 *
 */
extern "C" {
#include <time.h>
}
#include <assert.h>
#include "Batch_DateType.hxx"
using namespace std;

namespace Batch {

	// Conversion en chaine
  string DateType::affiche() const
  {
    return _data.str();
  }

	// Operateur d'affectation
  DateType & DateType::operator =(const Date & d)
  {
    _data = d;
    return *this;
  }

	// Conversion en Date
  DateType::operator Date() const
  {
    return _data;
  }

	// Conversion en long
  DateType::operator long() const
  {
    return _data.epoch();
  }

	// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
	// qu'il faudra detruire ensuite manuellement
  GenericType * DateType::clone() const
  {
    DateType * pD = new DateType(this->_data);
    assert(pD != 0);
    return pD;
  }

}
