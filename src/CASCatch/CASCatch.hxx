// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
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

#ifndef _CASCatch_HeaderFile
#define _CASCatch_HeaderFile

#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>


#ifdef try
#  undef try
#endif

#ifdef catch
#  undef catch
#endif

#ifdef NO_CXX_EXCEPTION
#  if defined(DO_ABORT)
#    define CASCatch_TRY \
       Standard_ErrorHandler _Function; \
       K_SETJMP = 1 ; \
       if(DoesNotAbort(_Function))
#   else  //If DO_ABORT is not defined
#     define CASCatch_TRY \
        Standard_ErrorHandler _Function; \
        if(DoesNotAbort(_Function))
#   endif //DO_ABORT
#   define CASCatch_CATCH(Error)   \
      else if(_Function.Catches(STANDARD_TYPE(Error)))
#else
#  define CASCatch_TRY try
#  define CASCatch_CATCH catch
#endif //NO_CXX_EXCEPTION


#endif
