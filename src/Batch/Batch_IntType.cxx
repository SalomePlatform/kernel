/*
 * IntType.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#include <string>
#include <sstream>
//#include "MEDMEM_STRING.hxx"
#include "Batch_IntType.hxx"
using namespace std;


namespace Batch {

	// Conversion en chaine
  string IntType::affiche() const
  {
    //MEDMEM::STRING sst;
    ostringstream sst;
    sst << _data;
    return sst.str();
  }

	// Operateur d'affectation
  IntType & IntType::operator =(int i)
  {
    _data = i;
    return *this;
  }

	// Conversion en int
  IntType::operator int()  const
  {
    return this->_data;
  }

	// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
	// qu'il faudra detruire ensuite manuellement
  GenericType * IntType::clone() const
  {
    IntType * pI = new IntType(this->_data);
    assert(pI != 0);
    return pI;
  }

}
