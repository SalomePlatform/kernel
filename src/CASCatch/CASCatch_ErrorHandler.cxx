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

#ifdef NO_CXX_EXCEPTION

#include "CASCatch_ErrorHandler.hxx"

// During setjmp()/longjmp() K_SETJMP_CASCatch is non zero (try)
// So if there is an abort request and if K_SETJMP_CASCatch is non zero, the abort
// request will be ignored. If the abort request do a raise during a setjmp
// or a longjmp, there will be a "terminating SEGV" impossible to handle.


Standard_EXPORT int K_SETJMP_CASCatch = 0 ; 

static Handle(CASCatch_Failure) GlbError;  //Last caught Error, Null if there is no error

static CASCatch_ErrorHandler*   Top = 0;   //The top of the Errors Stack

//=======================================================================
//function : CASCatch_ErrorHandler
//purpose  : Constructor
//=======================================================================
CASCatch_ErrorHandler::CASCatch_ErrorHandler ()
{
  Previous = Top;
  Top      = this;
  CaughtError.Nullify();
  GlbError.Nullify();
}

//=======================================================================
//function : ~CASCatch_ErrorHandler
//purpose  : Destructor : Delete the ErrorHandler and Abort if there is a 'Error'.
//=======================================================================
CASCatch_ErrorHandler::~CASCatch_ErrorHandler()
{
  Top = Top->Previous;
  if( !GlbError.IsNull() ) Abort();
}

//=======================================================================
//function : Abort: make a longjmp to the saved Context.
//purpose  : Abort if there is a non null 'Error'
//=======================================================================
void CASCatch_ErrorHandler::Abort ()
{
  //==== Check if can do the "longjmp" =======================================
  if(Top == NULL || Top->Label == NULL) {
    cout << "*** Abort *** an exception was raised, but no catch was found." << endl;
    cout << "\t... The exception is:" << GlbError;
    exit(1);
  }

#ifdef DO_ABORT
  if ( K_SETJMP_CASCatch )
    cout << "Recursive abort ===> Terminating SEGV ..." << endl ;
  K_SETJMP_CASCatch = 1 ;
#endif

  longjmp(Top->Label, Standard_True);
}

//=======================================================================
//function : Catches
//purpose  : If there is a 'Error', and it is in good type 
//           returns True and clean 'Error', else returns False.
//=======================================================================
Standard_Boolean CASCatch_ErrorHandler::Catches 
  (const Handle(Standard_Type)& AType) 
{
#ifdef DO_ABORT
  K_SETJMP_CASCatch = 0 ;
#endif
  if(GlbError.IsNull())
    return Standard_False;

  if(GlbError->IsKind(AType)){
    CaughtError = GlbError;
    GlbError.Nullify();
    return Standard_True;
  } else {
    return Standard_False;
  }
}

//=======================================================================
//function : LastCaughtError
//purpose  : 
//=======================================================================
Handle(CASCatch_Failure) CASCatch_ErrorHandler::LastCaughtError()
{
  return Top->CaughtError;
}

//=======================================================================
//function : Error
//purpose  : 
//=======================================================================
void CASCatch_ErrorHandler::Error(const Handle(CASCatch_Failure)& aError)
{
  GlbError = aError;
}

#endif
