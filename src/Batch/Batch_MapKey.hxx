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
 * MapKey.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Tue Oct 14 14:00:29 2003
 * Projet : Salome 2
 *
 */

#ifndef _MAPKEY_H_
#define _MAPKEY_H_

#include "Batch_Defines.hxx"


#include <string>

namespace Batch {

  // une classe privee pour les differents types
  // ces types ne peuvent pas etre redefinis
  class BATCH_EXPORT MapKey : public std::string
  {
  private:
    friend class Parametre; // seule la classe Parametre peut creer des MapKey
    MapKey() : std::string() {}
    MapKey(const MapKey & mk, size_type pos, size_type npos) : std::string(mk, pos, npos) {}
    MapKey(const char * s, size_type n) : std::string(s, n) {}
    MapKey(const char * s) : std::string(s) {}
    MapKey(size_type n, char c) : std::string(n, c) {}
#ifdef __STL_MEMBER_TEMPLATES
    template<class InputIterator>
    MapKey(InputIterator __begin, InputIterator __end) : std::string(__begin, __end) {}
#else
    MapKey(const_iterator __begin, const_iterator __end) : std::string(__begin, __end) {}
#endif

  public:
    MapKey(const MapKey & mk) : std::string(mk) {}

  };

}

#endif

// COMMENTS
