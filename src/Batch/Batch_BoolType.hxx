/*
 * BoolType.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#ifndef _BOOLTYPE_H_
#define _BOOLTYPE_H_

#include <string>
#include "Batch_GenericType.hxx"

namespace Batch {

  class BoolType : public GenericType
  {
  public:
		// Constructeur
    BoolType(const bool b=false) : _data(b) {}

		// Conversion en chaine
    virtual std::string affiche() const;

		// Operateur d'affectation
    virtual BoolType & operator =(bool);

		// Conversion en bool
    virtual operator bool() const;

		// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
		// qu'il faudra detruire ensuite manuellement
    virtual GenericType * clone() const;

  protected:
    bool _data;

  private:

  };

}

#endif
