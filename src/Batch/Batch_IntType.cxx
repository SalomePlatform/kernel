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
 * IntType.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#include <string>
#include <sstream>
#include <assert.h>
//#include "MEDMEM_STRING.hxx"
#include "Batch_IntType.hxx"
using namespace std;


namespace Batch {

	// Conversion en chaine
  string IntType::affiche() const
  {
    //MEDMEM::STRING sst;
    ostringstream sst;
    sst << _data;
    return sst.str();
  }

	// Operateur d'affectation
  IntType & IntType::operator =(int i)
  {
    _data = i;
    return *this;
  }

	// Conversion en int
  IntType::operator int()  const
  {
    return this->_data;
  }

	// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
	// qu'il faudra detruire ensuite manuellement
  GenericType * IntType::clone() const
  {
    IntType * pI = new IntType(this->_data);
    assert(pI != 0);
    return pI;
  }

}
