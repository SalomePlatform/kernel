/*
 * RunTimeException.hxx : 
 *
 * Auteur : %author% - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Tue Nov 25 14:04:13 2003
 * Projet : Salome 2
 *
 */

#ifndef _RUNTIMEEXCEPTION_H_
#define _RUNTIMEEXCEPTION_H_


#include "Batch_GenericException.hxx"

namespace Batch {

  class RunTimeException : public GenericException
  {
  public:
		// Constructeur
    RunTimeException(string ch = "undefined") : GenericException("RunTimeException", ch) {}
  };

}

#endif
