/*
 * InvalidKeyException.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Wed Oct 15 10:39:51 2003
 * Projet : Salome 2
 *
 */

#ifndef _INVALIDKEYEXCEPTION_H_
#define _INVALIDKEYEXCEPTION_H_


#include "Batch_GenericException.hxx"
#include "Batch_GenericException.hxx"

namespace Batch {

  class InvalidKeyException : public GenericException
  {
  public:
		// Constructeur
    InvalidKeyException(string ch = "undefined") : GenericException("InvalidKeyException", ch) {}

  protected:

  private:

  };

}

#endif

