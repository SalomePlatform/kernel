/*
 * IntType.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#ifndef _INTTYPE_H_
#define _INTTYPE_H_

#include <string>
#include "Batch_GenericType.hxx"

namespace Batch {

  class IntType : public GenericType
  {
  public:
		// Constructeur
    IntType(const int i=0) : _data(i) {}

		// Conversion en chaine
    virtual std::string affiche() const;

		// Operateur d'affectation
    virtual IntType & operator =(int);

		// Conversion en int
    virtual operator int() const;

		// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
		// qu'il faudra detruire ensuite manuellement
    virtual GenericType * clone() const;

  protected:
    int _data;

  private:

  };

}

#endif
