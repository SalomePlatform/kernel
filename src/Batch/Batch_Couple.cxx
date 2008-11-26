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
 * Couple.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Tue Dec  9 15:00:35 2003
 * Projet : Salome 2
 *
 */

#include "Batch_Couple.hxx"
using namespace std;

namespace Batch {

	// Operateur d'affectation
	Couple & Couple::operator =(const Couple & C)
  {
		_local  = C._local;
		_remote = C._remote;
    return *this;
  }

	// Conversion en chaine
  string Couple::str()  const
  {
		string res;
		res  = "Couple(local : ";
		res += _local;
		res += ", remote : ";
		res += _remote;
		res += ")";
    return res;
  }

	// Operateur pour l'affichage sur un stream
  ostream & operator << (ostream & os, const Couple & cp)
  {
    return os << cp.str();
  }

}
