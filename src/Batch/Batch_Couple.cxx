/*
 * Couple.cxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Tue Dec  9 15:00:35 2003
 * Projet : Salome 2
 *
 */

#include "Batch_Couple.hxx"

namespace Batch {

	// Operateur d'affectation
	Couple & Couple::operator =(const Couple & C)
  {
		_local  = C._local;
		_remote = C._remote;
    return *this;
  }

	// Conversion en chaine
  string Couple::str()  const
  {
		string res;
		res  = "Couple(local : ";
		res += _local;
		res += ", remote : ";
		res += _remote;
		res += ")";
    return res;
  }

	// Operateur pour l'affichage sur un stream
  ostream & operator << (ostream & os, const Couple & cp)
  {
    return os << cp.str();
  }

}
