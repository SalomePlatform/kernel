/*
 * LongType.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#include <string>
#include <sstream>
//#include "MEDMEM_STRING.hxx"
#include "Batch_LongType.hxx"

namespace Batch {

	// Conversion en chaine
  string LongType::affiche() const
  {
    //MEDMEM::STRING sst;
    ostringstream sst;
    sst << _data;
    return sst.str();
  }

	// Operateur d'affectation
  LongType & LongType::operator =(long l)
  {
    _data = l;
    return *this;
  }

	// Conversion en long
  LongType::operator long() const
  {
    return this->_data;
  }

	// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
	// qu'il faudra detruire ensuite manuellement
  GenericType * LongType::clone() const
  {
    LongType * pL = new LongType(this->_data);
    assert(pL != 0);
    return pL;
  }

}
