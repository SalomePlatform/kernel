/*
 * GenericType.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#ifndef _GENERICTYPE_H_
#define _GENERICTYPE_H_


#include <iostream>
#include <string>

namespace Batch {

  class GenericType
  {
  public:
		// Constructeur et destructeur
    GenericType() { _nb++; }
    virtual ~GenericType() { _nb--; }

		// Operateur pour l'affichage sur un stream
    friend std::ostream & operator << (std::ostream & os, const GenericType & obj);

		// Conversion en chaine
    virtual std::string affiche() const;

		// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
		// qu'il faudra detruire ensuite manuellement
    virtual GenericType * clone() const;

		// Retourne le nombre d'objets GenericType et al.
		static int getNb() { return _nb; }

  protected:

  private:
		static int _nb; // nombre total d'objets GenericType et al.
  
  };

}

#endif

