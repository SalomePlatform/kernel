/*
 * StringType.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#ifndef _STRINGTYPE_H_
#define _STRINGTYPE_H_

#include <string>
#include "Batch_GenericType.hxx"

namespace Batch {

  class StringType : public GenericType
  {
  public:
		// Constructeur
    StringType(const std::string & s="")  : _data(s) {}

		// Conversion en chaine
    virtual std::string affiche() const;
    virtual operator std::string() const;

		// Operateur d'affectation
    virtual StringType & operator =(std::string);

		// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
		// qu'il faudra detruire ensuite manuellement
    virtual GenericType * clone() const;

  protected:
    std::string _data;

  private:

  };

}

#endif
