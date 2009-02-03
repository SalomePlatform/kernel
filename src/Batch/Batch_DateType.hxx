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
 * DateType.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Wed Nov 26 11:15:34 2003
 * Projet : Salome 2
 *
 */

#ifndef _DATETYPE_H_
#define _DATETYPE_H_

#include "Batch_Defines.hxx"

#include <string>
#include "Batch_GenericType.hxx"
#include "Batch_Date.hxx"

namespace Batch {

  class BATCH_EXPORT DateType : public GenericType
  {
  public:
		// Constructeur
    DateType(const Date & d)  : _data(d) {};

		// Conversion en chaine
    virtual std::string affiche() const;

		// Operateur d'affectation
    virtual DateType & operator =(const Date &);

		// Conversion en Date
    virtual operator Date() const;

		// Conversion en long
    virtual operator long() const;

		// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
		// qu'il faudra detruire ensuite manuellement
    virtual GenericType * clone() const;

  protected:
    Date _data;

  private:
  };

}

#endif

