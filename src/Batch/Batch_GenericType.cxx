/*
 * GenericType.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */


#include <iostream>
#include <string>
#include "Batch_GenericType.hxx"
using namespace std;

namespace Batch {

	// nombre total d'objet GenericType et al.
	int GenericType::_nb = 0;

	// Operateur pour l'affichage sur un stream
  ostream & operator << (ostream & os, const GenericType & obj)
  {
    return os << obj.affiche();
  }

	// Conversion en chaine
  string GenericType::affiche() const
  {
    return string("(GenericType : si ce message apparait, vous devez avoir un probleme)");
  }

	// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
	// qu'il faudra detruire ensuite manuellement
  GenericType * GenericType::clone() const
  {
    GenericType * pG = new GenericType;
    assert(pG != 0);
    return pG;
  }

}

