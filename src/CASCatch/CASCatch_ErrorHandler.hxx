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

#ifndef _CASCatch_ErrorHandler_HeaderFile
#define _CASCatch_ErrorHandler_HeaderFile

#include "CASCatch_Failure.hxx"

#include <stdlib.h>
#include <setjmp.h>


extern int K_SETJMP_CASCatch ;

/*!
 * \class CASCatch_ErrorHandler
 * \brief This class is an exception handler, private
 *
 */ 
class CASCatch_ErrorHandler 
{
  friend class  CASCatch_Failure;     // To execute the raise exception.

 public:
 
 Standard_EXPORT CASCatch_ErrorHandler();
 Standard_EXPORT ~CASCatch_ErrorHandler();
 Standard_EXPORT Standard_Boolean Catches (const Handle(Standard_Type)&);

 private:
  Standard_EXPORT static  void Abort();
  Standard_EXPORT static  void Error(const Handle(CASCatch_Failure)&);
  Standard_EXPORT static  Handle(CASCatch_Failure)  LastCaughtError();

 //==== The fields ===========================================================
 private:
           CASCatch_ErrorHandler*   Previous;
           Handle(CASCatch_Failure) CaughtError;

 public:
  jmp_buf      Label;

};

#undef CASCatch_TRY
#define CASCatch_TRY try

#undef CASCatch_CATCH
#define CASCatch_CATCH catch


#ifdef NO_CXX_EXCEPTION
# undef CASCatch_TRY
# undef CASCatch_CATCH

# if defined(DO_ABORT)
  
#  define DoesNotAbort_CASCatch(aHandler) !(K_SETJMP_CASCatch = setjmp(aHandler.Label))

#  define CASCatch_TRY        CASCatch_ErrorHandler _Function; \
                                K_SETJMP_CASCatch = 1 ; \
                                if(DoesNotAbort_CASCatch(_Function))

# else  //If DO_ABORT is not defined
#  define DoesNotAbort_CASCatch(aHandler) !setjmp(aHandler.Label)

#  define CASCatch_TRY      CASCatch_ErrorHandler _Function; \
                              if(DoesNotAbort_CASCatch(_Function))
# endif //DO_ABORT


# define CASCatch_CATCH(Error)   else if(_Function.Catches(STANDARD_TYPE(Error)))
#endif //NO_CXX_EXCEPTION

#endif //_CASCatch_ErrorHandler_HeaderFile
