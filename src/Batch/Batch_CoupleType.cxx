/*
 * CoupleType.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Tue Dec  9 14:51:53 2003
 * Projet : Salome 2
 *
 */

#include "Batch_CoupleType.hxx"

namespace Batch {

	// Conversion en chaine
  string CoupleType::affiche() const
  {
    return _data.str();
  }

	// Operateur d'affectation
  CoupleType & CoupleType::operator =(const Couple & C)
  {
    _data = C;
    return *this;
  }

	// Conversion en char
  CoupleType::operator Couple() const
  {
    return _data;
  }

	// Conversion en chaine
  CoupleType::operator string() const
  {
    return _data.str();
  }

	// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
	// qu'il faudra detruire ensuite manuellement
  GenericType * CoupleType::clone() const
  {
    CoupleType * pC = new CoupleType(this->_data);
    assert(pC != 0);
    return pC;
  }

}

