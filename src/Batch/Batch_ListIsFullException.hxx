/*
 * ListIsFullException.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#ifndef _LISTISFULLEXCEPTION_H_
#define _LISTISFULLEXCEPTION_H_

#include "Batch_GenericException.hxx"

namespace Batch {

  class ListIsFullException : public GenericException
  {
  public:
		// Constructeur
    ListIsFullException(string ch = "undefined") : GenericException("ListIsFullException", ch) {}
  };

}

#endif

