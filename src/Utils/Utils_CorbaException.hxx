//=============================================================================
// File      : Utils_CorbaException.hxx
// Created   : mer déc 19 14:12:19 CET 2001
// Author    : Paul RASCLE, EDF
// Project   : SALOME
// Copyright : EDF 2001
// $Header$
//=============================================================================

#ifndef _UTILS_CORBAEXCEPTION_HXX_
#define _UTILS_CORBAEXCEPTION_HXX_

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Exception)

/*! 
 * To raise a CORBA exception from a CORBA servant.
 */
# define THROW_SALOME_CORBA_EXCEPTION(chain, typex) \
              {\
                SALOME::ExceptionStruct ExDescription; \
                ExDescription.text = CORBA::string_dup(chain); \
                ExDescription.type = typex; \
                ExDescription.sourceFile = CORBA::string_dup(__FILE__); \
                ExDescription.lineNumber = __LINE__; \
                throw SALOME::SALOME_Exception(ExDescription); \
              }

#endif
