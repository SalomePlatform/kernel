/*
 * APIInternalFailureException.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Thu Nov 20 15:15:41 2003
 * Projet : Salome 2
 *
 */

#ifndef _APIINTERNALFAILUREEXCEPTION_H_
#define _APIINTERNALFAILUREEXCEPTION_H_


#include "Batch_GenericException.hxx"
#include "Batch_GenericException.hxx"

namespace Batch {

  class APIInternalFailureException : public GenericException
  {
  public:
		// Constructeur
    APIInternalFailureException(std::string ch = "undefined") : GenericException("APIInternalFailureException", ch) {}
  };

}

#endif
