/*
 * NotYetImplementedException.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Mail   : mailto:ivan.dutka-malen@der.edf.fr
 * Date   : Tue Nov 25 11:35:07 2003
 * Projet : Salome 2
 *
 */

#ifndef _NOTYETIMPLEMENTEDEXCEPTION_H_
#define _NOTYETIMPLEMENTEDEXCEPTION_H_


#include "Batch_GenericException.hxx"

namespace Batch {

  class NotYetImplementedException : public GenericException
  {
  public:
		// Constructeur
    NotYetImplementedException(string ch = "undefined") : GenericException("NotYetImplementedException", ch) {}
  };

}

#endif

