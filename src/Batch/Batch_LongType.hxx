/*
 * LongType.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#ifndef _LONGTYPE_H_
#define _LONGTYPE_H_

#include <string>
#include "Batch_GenericType.hxx"

namespace Batch {

  class LongType : public GenericType
  {
  public:
		// Constructeur
    LongType(const long l=0L) : _data(l) {}

		// Conversion en chaine
    virtual std::string affiche() const;

		// Operateur d'affectation
    virtual LongType & operator =(long);

		// Conversion en long
    virtual operator long() const;

		// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
		// qu'il faudra detruire ensuite manuellement
    virtual GenericType * clone() const;

  protected:
    long _data;

  private:

  };

}

#endif
