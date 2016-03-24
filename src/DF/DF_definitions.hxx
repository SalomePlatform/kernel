// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

#ifndef  DF_DEF_HXX
#define  DF_DEF_HXX

#ifdef WIN32
# if defined DF_EXPORTS
#  define DF_EXPORT __declspec( dllexport )
# else
#  define DF_EXPORT __declspec( dllimport )
# endif
#else
# define DF_EXPORT
#endif

#include <iostream> 
#include <stdio.h>

# if defined(WIN32) && !defined(HAVE_NO_DLL)

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

# else  /* WIN32 */

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

# endif  /* WIN32 */

# ifndef __Standard_API
//#  ifdef WIN32
#   if !defined(WIN32) 
#    define __Standard_API Standard_EXPORT
#    define __Standard_APIEXTERN Standard_EXPORTEXTERN
#   else
#    define __Standard_API Standard_IMPORT
#    define __Standard_APIEXTERN Standard_IMPORT
#   endif  // __Standard_DLL
//#  else
//#   define __Standard_API
//#  endif  // WIN32
# endif  // __Standard_API

#include <iostream>
class Standard_EXPORT DFexception
{
public :
  DFexception(const char *message) {
    std::cerr << message << std::endl;
  }
}; 


#endif
