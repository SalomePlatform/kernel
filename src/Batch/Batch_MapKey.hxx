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


using namespace std;
#include <string>

namespace Batch {

  // une classe privee pour les differents types
  // ces types ne peuvent pas etre redefinis
  class MapKey : public string
  {
  private:
    friend class Parametre; // seule la classe Parametre peut creer des MapKey
    MapKey() : string() {}
    MapKey(const MapKey & mk, size_type pos, size_type npos) : string(mk, pos, npos) {}
    MapKey(const char * s, size_type n) : string(s, n) {}
    MapKey(const char * s) : string(s) {}
    MapKey(size_type n, char c) : string(n, c) {}
#ifdef __STL_MEMBER_TEMPLATES
    template<class InputIterator>
    MapKey(InputIterator __begin, InputIterator __end) : string(__begin, __end) {}
#else
    MapKey(const_iterator __begin, const_iterator __end) : string(__begin, __end) {}
#endif

  public:
    MapKey(const MapKey & mk) : string(mk) {}

  };

}

#endif

// COMMENTS
