/*
 * BoolType.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#include <string>
#include "Batch_BoolType.hxx"

namespace Batch {

	// Conversion en chaine
  string BoolType::affiche() const
  {
		return _data ? string("true") : string("false");
  }

	// Operateur d'affectation
  BoolType & BoolType::operator =(bool b)
  {
    _data = b;
    return *this;
  }

	// Conversion en bool
  BoolType::operator bool() const
  {
    return this->_data;
  }

	// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
	// qu'il faudra detruire ensuite manuellement
  GenericType * BoolType::clone() const
  {
    BoolType * pB = new BoolType(this->_data);
    assert(pB != 0);
    return pB;
  }

}
