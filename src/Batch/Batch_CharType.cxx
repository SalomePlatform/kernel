/*
 * CharType.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#include <string>
#include "Batch_CharType.hxx"

namespace Batch {

	// Conversion en chaine
  string CharType::affiche() const
  {
    return string(1, _data);
  }

	// Operateur d'affectation
  CharType & CharType::operator =(char c)
  {
    _data = c;
    return *this;
  }

	// Conversion en char
  CharType::operator char() const
  {
    return this->_data;
  }

	// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
	// qu'il faudra detruire ensuite manuellement
  GenericType * CharType::clone() const
  {
    CharType * pC = new CharType(this->_data);
    assert(pC != 0);
    return pC;
  }

}
