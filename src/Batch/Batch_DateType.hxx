/*
 * DateType.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Wed Nov 26 11:15:34 2003
 * Projet : Salome 2
 *
 */

#ifndef _DATETYPE_H_
#define _DATETYPE_H_

using namespace std;
#include <string>
#include "Batch_GenericType.hxx"
#include "Batch_Date.hxx"

namespace Batch {

  class DateType : public GenericType
  {
  public:
		// Constructeur
    DateType(const Date & d)  : _data(d) {};

		// Conversion en chaine
    virtual string affiche() const;

		// Operateur d'affectation
    virtual DateType & operator =(const Date &);

		// Conversion en Date
    virtual operator Date() const;

		// Conversion en long
    virtual operator long() const;

		// Clone duplique l'objet et en fabrique un nouveau a l'aide de new
		// qu'il faudra detruire ensuite manuellement
    virtual GenericType * clone() const;

  protected:
    Date _data;

  private:
  };

}

#endif

