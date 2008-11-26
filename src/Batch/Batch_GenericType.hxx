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
 * GenericType.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#ifndef _GENERICTYPE_H_
#define _GENERICTYPE_H_

#include "Batch_Defines.hxx"


#include <iostream>
#include <string>

namespace Batch {

  class BATCH_EXPORT GenericType
  {
  public:
		// Constructeur et destructeur
    GenericType() { _nb++; }
    virtual ~GenericType() { _nb--; }

		// Operateur pour l'affichage sur un stream
    friend std::ostream & operator << (std::ostream & os, const GenericType & obj);

		// Conversion en chaine
    virtual std::string affiche() const;

		// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
		// qu'il faudra detruire ensuite manuellement
    virtual GenericType * clone() const;

		// Retourne le nombre d'objets GenericType et al.
		static int getNb() { return _nb; }

  protected:

  private:
		static int _nb; // nombre total d'objets GenericType et al.
  
  };

}

#endif

