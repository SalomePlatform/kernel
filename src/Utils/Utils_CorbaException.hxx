//  SALOME Utils : general SALOME's definitions and tools
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : Utils_CorbaException.hxx
//  Author : Paul RASCLE, EDF
//  Module : SALOME
//  $Header$

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

#include <ostream.h>

//Dump the CORBA exception type.
inline std::ostream& operator<<(std::ostream& os, const CORBA::Exception& e)
{
  CORBA::Any tmp;
  tmp<<= e;
  CORBA::TypeCode_var tc = tmp.type();
  const char *p = tc->name();
  os<<"CORBA Exception was catch of the kind : ";
  if ( *p != '\0' ) {
    os<<p;
  } 
  else  { 
    os << tc->id();
  }
  
  return os;
}

#endif
