/*
 * CharType.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#ifndef _CHARTYPE_H_
#define _CHARTYPE_H_

#include <string>
#include "Batch_GenericType.hxx"

namespace Batch {

  class CharType : public GenericType
  {
  public:
		// Constructeur
    CharType(const char c=0) : _data(c) {}

		// Conversion en chaine
    virtual std::string affiche() const;

		// Operateur d'affectation
    virtual CharType & operator =(char);

		// Conversion en char
    virtual operator char() const;

		// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
		// qu'il faudra detruire ensuite manuellement
    virtual GenericType * clone() const;

  protected:
    char _data;

  private:

  };

}

#endif

