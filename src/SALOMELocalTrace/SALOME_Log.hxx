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
//  File   : SALOME_Log.hxx
//  Author : Vasily Rusyaev (OCN), Paul RASCLE (EDF)
//  Module : KERNEL
//  $Header$

#ifndef _SALOME_LOG_HXX_
#define _SALOME_LOG_HXX_

#include <iostream>
#include <strstream>

//these declarations for files don't using OCC includes (for example HDF)
# ifdef WNT

#  ifndef Standard_EXPORT
#   define Standard_EXPORT __declspec( dllexport )
// For global variables :
#   define Standard_EXPORTEXTERN __declspec( dllexport ) extern
#   define Standard_EXPORTEXTERNC extern "C" __declspec( dllexport )
#  endif  /* Standard_EXPORT */

#  ifndef Standard_IMPORT
#   define Standard_IMPORT __declspec( dllimport ) extern
#   define Standard_IMPORTC extern "C" __declspec( dllimport )
#  endif  /* Standard_IMPORT */

# else  /* WNT */

#  ifndef Standard_EXPORT
#   define Standard_EXPORT
// For global variables :
#   define Standard_EXPORTEXTERN extern
#   define Standard_EXPORTEXTERNC extern "C"
#  endif  /* Standard_EXPORT */

#  ifndef Standard_IMPORT
#   define Standard_IMPORT extern
#   define Standard_IMPORTC extern "C"
#  endif  /* Standard_IMPORT */

# endif  /* WNT */


class SALOME_Log : public std::ostrstream
{
public:
  virtual ~SALOME_Log();
  static Standard_EXPORT SALOME_Log& Instance();
  Standard_EXPORT void putMessage(std::ostream& msg);

protected:
  //disable creation of instances: force use static SALOME_Log& Instance()
  SALOME_Log();
};


#define SLog SALOME_Log::Instance()

#endif
