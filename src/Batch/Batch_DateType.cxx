/*
 * DateType.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Wed Nov 26 11:15:34 2003
 * Projet : Salome 2
 *
 */
extern "C" {
#include <time.h>
}
#include "Batch_DateType.hxx"

namespace Batch {

	// Conversion en chaine
  string DateType::affiche() const
  {
    return _data.str();
  }

	// Operateur d'affectation
  DateType & DateType::operator =(const Date & d)
  {
    _data = d;
    return *this;
  }

	// Conversion en Date
  DateType::operator Date() const
  {
    return _data;
  }

	// Conversion en long
  DateType::operator long() const
  {
    return _data.epoch();
  }

	// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
	// qu'il faudra detruire ensuite manuellement
  GenericType * DateType::clone() const
  {
    DateType * pD = new DateType(this->_data);
    assert(pD != 0);
    return pD;
  }

}
