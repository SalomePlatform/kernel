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


#include <string>

namespace Batch {

  // une classe privee pour les differents types
  // ces types ne peuvent pas etre redefinis
  class MapKey : public std::string
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
