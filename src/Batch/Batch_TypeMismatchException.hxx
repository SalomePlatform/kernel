/*
 * TypeMismatchException.hxx : 
 *
 * Auteur : Ivan DUTKA-MALEN - EDF R&D
 * Date   : Septembre 2003
 * Projet : SALOME 2
 *
 */

#ifndef _TYPEMISMATCHEXCEPTION_H_
#define _TYPEMISMATCHEXCEPTION_H_


#include "Batch_GenericException.hxx"

namespace Batch {

  class TypeMismatchException : public GenericException
  {
  public:
		// Constructeur
    TypeMismatchException(string ch = "undefined") : GenericException("TypeMismatchException", ch) {}
  };

}

#endif

