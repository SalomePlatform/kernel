/*
 * GenericException.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Wed Nov 26 10:15:56 2003
 * Projet : Salome 2
 *
 */

#ifndef _GENERICEXCEPTION_H_
#define _GENERICEXCEPTION_H_


#include <string>

namespace Batch {

  class GenericException
  {
  public:
    const std::string type;    // la nature de l'exception
    const std::string message; // la raison de l'exception

		// Constructeur
    GenericException(const std::string tp = "GenericException", const std::string ch = "undefined") : type(tp), message(ch) {}
  };

}

#endif
