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

using namespace std;
#include <string>
#include "Batch_GenericType.hxx"

namespace Batch {

  class StringType : public GenericType
  {
  public:
		// Constructeur
    StringType(const string & s="")  : _data(s) {}

		// Conversion en chaine
    virtual string affiche() const;
    virtual operator string() const;

		// Operateur d'affectation
    virtual StringType & operator =(string);

		// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
		// qu'il faudra detruire ensuite manuellement
    virtual GenericType * clone() const;

  protected:
    string _data;

  private:

  };

}

#endif
