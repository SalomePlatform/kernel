/*
 * StringType.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#include <string>
#include "Batch_StringType.hxx"

namespace Batch {

	// Conversion en chaine
  string StringType::affiche() const
  {
    return _data;
  }

 	// Operateur d'affectation
	StringType & StringType::operator =(string s)
  {
    _data = s;
    return *this;
  }

	// Conversion en chaine
  StringType::operator string() const
  {
    return this->_data;
  }

	// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
	// qu'il faudra detruire ensuite manuellement
  GenericType * StringType::clone() const
  {
    StringType * pS = new StringType(this->_data);
    assert(pS != 0);
    return pS;
  }

}
