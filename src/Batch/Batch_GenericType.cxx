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
 * GenericType.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */


#include <iostream>
#include <string>
#include <assert.h>
#include "Batch_GenericType.hxx"
using namespace std;

namespace Batch {

	// nombre total d'objet GenericType et al.
	int GenericType::_nb = 0;

	// Operateur pour l'affichage sur un stream
  ostream & operator << (ostream & os, const GenericType & obj)
  {
    return os << obj.affiche();
  }

	// Conversion en chaine
  string GenericType::affiche() const
  {
    return string("(GenericType : si ce message apparait, vous devez avoir un probleme)");
  }

	// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
	// qu'il faudra detruire ensuite manuellement
  GenericType * GenericType::clone() const
  {
    GenericType * pG = new GenericType;
    assert(pG != 0);
    return pG;
  }

}

