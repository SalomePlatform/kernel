/*
 * ConnexionFailureException.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Thu Nov 13 11:24:31 2003
 * Projet : Salome 2
 *
 */

#ifndef _CONNEXIONFAILUREEXCEPTION_H_
#define _CONNEXIONFAILUREEXCEPTION_H_


#include "Batch_GenericException.hxx"

namespace Batch {

  class ConnexionFailureException : public GenericException
  {
  public:
		// Constructeur
    ConnexionFailureException(std::string ch = "undefined") : GenericException("ConnexionFailureException", ch) {}

  };

}

#endif

