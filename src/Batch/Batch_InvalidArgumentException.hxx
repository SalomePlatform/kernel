/*
 * InvalidArgumentException.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Thu Oct 16 16:18:00 2003
 * Projet : Salome 2
 *
 */

#ifndef _INVALIDARGUMENTEXCEPTION_H_
#define _INVALIDARGUMENTEXCEPTION_H_


#include "Batch_GenericException.hxx"
#include "Batch_GenericException.hxx"

namespace Batch {

  class InvalidArgumentException : public GenericException
  {
  public:
		// Constructeur
    InvalidArgumentException(string ch = "undefined") : GenericException("InvalidArgumentException", ch) {}
  };

}

#endif

