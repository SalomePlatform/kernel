/*
 * CoupleType.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Tue Dec  9 14:51:53 2003
 * Projet : Salome 2
 *
 */

#ifndef _COUPLETYPE_H_
#define _COUPLETYPE_H_

using namespace std;
#include <string>
#include "Batch_GenericType.hxx"
#include "Batch_Couple.hxx"

namespace Batch {

  class CoupleType : public GenericType
  {
  public:
		// Constructeur
    CoupleType(const Couple & C) : _data(C) {}

		// Conversion en chaine
    virtual string affiche() const;
    virtual operator string() const;

		// Operateur d'affectation
    virtual CoupleType & operator =(const Couple & C);

		// Conversion en char
    virtual operator Couple() const;

		// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
		// qu'il faudra detruire ensuite manuellement
    virtual GenericType * clone() const;

  protected:
		Couple _data;

  private:

  };

}

#endif
