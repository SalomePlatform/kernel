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
#include "CASCatch_CatchSignals.hxx"

#include "CASCatch_Failure.hxx"  
#include "CASCatch_ErrorHandler.hxx"
#include <TCollection_AsciiString.hxx>

#define MAX_HANDLER_NUMBER 6


//================================================================================
/*! Public -
 * \brief creates a CASCatch_CatchSignals
 */
//================================================================================ 
CASCatch_CatchSignals::CASCatch_CatchSignals() 
     :myIsActivated(Standard_False)
{

  Standard_Integer i = 0;
  for(; i<=MAX_HANDLER_NUMBER; i++)
    mySigStates[i] = NULL;
}

#ifndef WNT

//================================ UNIX part ==================================================

#include <OSD.hxx>
#include <OSD_WhoAmI.hxx>
#include <OSD_SIGHUP.hxx>
#include <OSD_SIGINT.hxx>
#include <OSD_SIGQUIT.hxx>
#include <OSD_SIGILL.hxx>
#include <OSD_SIGKILL.hxx>
#include <OSD_SIGBUS.hxx>
#include <OSD_SIGSEGV.hxx>
#include <OSD_SIGSYS.hxx>
#ifndef LIN
#include <exception.h>
#endif

//==============================
typedef void (ACT_SIGIO_HANDLER)(void) ;

ACT_SIGIO_HANDLER *ADR_ACT_SIGIO_HANDLER = NULL ;

typedef void (* SIG_PFV) (int);

#ifdef SUN
# include <floatingpoint.h>
#endif

#ifdef SOLARIS
# include <floatingpoint.h>
# include <sys/machsig.h>
# include <stdlib.h>
# include <stdio.h>
#endif

#include <signal.h>
#include <sys/signal.h>

#ifdef LIN
# include <stdlib.h>
# include <stdio.h>
#else
# ifdef SA_SIGINFO 
#   ifndef AIX
# include <sys/siginfo.h>
#    endif
# endif
#endif


#ifdef IRIX
# include <sigfpe.h>
# include <sys/siginfo.h>
#endif 


//================================================================================
/*! Private -
 * \brief universal handler for signals
 */
//================================================================================ 
static void Handler(const OSD_Signals theSig, const OSD_Signals)
{
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, theSig);
  sigprocmask(SIG_UNBLOCK, &set, NULL) ;

  TCollection_AsciiString aMessage(theSig);  
  aMessage+=" signal detected";
 
  CASCatch_Failure::Raise(aMessage.ToCString());
}


#ifdef SA_SIGINFO
//================================================================================
/*! Private -
 * \brief  handler for SIGSEGV signal
 */
//================================================================================ 
static void SegvHandler(const OSD_Signals, const Standard_Address, const Standard_Address)
{
  sigset_t set;
  sigemptyset(&set);
  sigaddset(&set, SIGSEGV);
  sigprocmask (SIG_UNBLOCK, &set, NULL); 

  CASCatch_Failure::Raise("SIGSEGV detected");
}
#endif


//================================================================================
/*! Public -
 * \brief activates a signals handling
 */
//================================================================================ 
void CASCatch_CatchSignals::Activate() 
{  
  if(myIsActivated) return;

  struct sigaction act;

  Standard_Integer i = 0;
  for(; i<=MAX_HANDLER_NUMBER; i++)  
    mySigStates[i] = new struct sigaction(); //Initialize structures

  int stat;
  act.sa_handler =  (SIG_PFV) &Handler ;
  sigemptyset(&act.sa_mask) ;


  stat = sigaction(SIGHUP,&act,(struct sigaction*)mySigStates[0]);    // ...... hangup
  stat = sigaction(SIGFPE,&act,(struct sigaction*) mySigStates[1]);   // ...... floating point exception
  stat = sigaction(SIGINT,&act,(struct sigaction*)mySigStates[2]);   // ...... interrupt
  stat = sigaction(SIGQUIT,&act,(struct sigaction*)mySigStates[3]);  // ...... quit
  stat = sigaction(SIGBUS,&act,(struct sigaction*)mySigStates[4]);   // ...... bus error
  stat = sigaction(SIGILL,&act,(struct sigaction*)mySigStates[5]);   // ...... illegal instruction

#ifdef SA_RESTART
  act.sa_flags   = SA_RESTART ;
#else
  act.sa_flags   = 0 ;
#endif
  act.sa_handler = (SIG_PFV) &SegvHandler ;

#ifdef SA_SIGINFO	// OSF,SOLARIS,IRIX
  act.sa_flags = act.sa_flags | SA_SIGINFO ;
# ifdef SOLARIS
  act.sa_sigaction = (void(*)(int, siginfo_t *, void*)) &SegvHandler ;
# endif
#endif

  stat = sigaction( SIGSEGV , &act , (struct sigaction*)mySigStates[6]);    // ...... segmentation violation

  myIsActivated = Standard_True;
}


//================================================================================
/*! Public -
 * \brief deactivates a signals handling
 */
//================================================================================
void CASCatch_CatchSignals::Deactivate() 
{
  if(!myIsActivated) return;

  struct sigaction oact;
  int stat;

  stat = sigaction(SIGHUP,(struct sigaction*)mySigStates[0],&oact);   // ...... hangup
  stat = sigaction(SIGFPE,(struct sigaction*)mySigStates[1],&oact);   // ...... floating point exception
  stat = sigaction(SIGINT,(struct sigaction*)mySigStates[2],&oact);   // ...... interrupt
  stat = sigaction(SIGQUIT,(struct sigaction*)mySigStates[3],&oact);  // ...... quit
  stat = sigaction(SIGBUS,(struct sigaction*)mySigStates[4],&oact);   // ...... bus error
  stat = sigaction(SIGILL,(struct sigaction*)mySigStates[5],&oact);   // ...... illegal instruction
  stat = sigaction(SIGSEGV,(struct sigaction*)mySigStates[6],&oact);  // ...... segmentation violation


  Standard_Integer i = 0;
  for(; i<=MAX_HANDLER_NUMBER; i++)
    delete (struct sigaction*)mySigStates[i];

  myIsActivated = Standard_False;
}



#else
//====================================== WNT part ====================================================
#include <OSD_WNT_1.hxx>

#include <process.h>
#include <signal.h>
#include <float.h>

#define _OSD_FPX ( _EM_DENORMAL | _EM_INEXACT | _EM_UNDERFLOW | _EM_ZERODIVIDE | _EM_OVERFLOW) //Mask these exceptions

//================================================================================
/*! Private -
 * \brief handler for unexpected exceptions
 */
//================================================================================
static Standard_Integer WntHandler(const Standard_Address theExceptionInfo) 
{
  LPEXCEPTION_POINTERS lpXP = ( LPEXCEPTION_POINTERS )theExceptionInfo;
  DWORD                dwExceptionCode = lpXP -> ExceptionRecord -> ExceptionCode;

  TCollection_AsciiString aMessage((Standard_Integer)dwExceptionCode);  
  aMessage+=" Exception code - unexpected exception";

  CASCatch_Failure::Raise(aMessage.ToCString());

  return EXCEPTION_EXECUTE_HANDLER;
}

void SIGWntHandler(int , int ) ;
static void (*SIGWNTHANDLER)(int) = ( void (*)(int) ) ( &SIGWntHandler ) ;


//================================================================================
/*! Private -
 * \brief handler for signals
 */
//================================================================================
static void SIGWntHandler(const int signum , const int theCode)
{

  void (*OLDSIGWNTHANDLER)(int) ;  
  switch( signum ) { 
  case SIGFPE : 
    _fpreset() ;
    _clearfp() ; 
    _controlfp ( _OSD_FPX, _MCW_EM ); 
    OLDSIGWNTHANDLER = signal( signum , SIGWNTHANDLER ); 

    if(theCode == _FPE_UNDERFLOW || theCode == _FPE_INEXACT) return;
    CASCatch_Failure::Raise ("Floating point error"); 
    break;
  case SIGSEGV : 
    OLDSIGWNTHANDLER = signal( signum , SIGWNTHANDLER );
    CASCatch_Failure::Raise("Access violation"); 
    break; 
  case SIGILL : 
    OLDSIGWNTHANDLER = signal( signum , SIGWNTHANDLER );
    CASCatch_Failure::Raise("Illegal instruction" ); 
    break; 
  }
}


//================================================================================
/*! Public -
 * \brief activates a signals handling
 */
//================================================================================ 
void CASCatch_CatchSignals::Activate() 
{
  if(myIsActivated) return;

  mySigStates[0] = SetUnhandledExceptionFilter (( LPTOP_LEVEL_EXCEPTION_FILTER )&WntHandler);

  myFloatOpWord = _controlfp(0, 0);
  _controlfp ( _OSD_FPX, _MCW_EM );  //Enable floating point exceptions

  mySigStates[1] = signal( SIGSEGV , SIGWNTHANDLER );
  mySigStates[2] = signal( SIGFPE , SIGWNTHANDLER );
  mySigStates[3] = signal( SIGILL , SIGWNTHANDLER );

  myIsActivated = Standard_True;
}

//================================================================================
/*! Public -
 * \brief deactivates a signals handling
 */
//================================================================================
void CASCatch_CatchSignals::Deactivate() 
{
  if(!myIsActivated) return;

  SetUnhandledExceptionFilter (( LPTOP_LEVEL_EXCEPTION_FILTER )mySigStates[0]);

  _controlfp ( myFloatOpWord, _MCW_EM );

  signal( SIGSEGV ,  ( void (*)(int) )mySigStates[1]);
  signal( SIGFPE , ( void (*)(int) )mySigStates[2]);
  signal( SIGILL , ( void (*)(int) )mySigStates[3]);

  Standard_Integer i = 0;
  for(; i<=MAX_HANDLER_NUMBER; i++)
    mySigStates[i] = NULL;
  
  myIsActivated = Standard_False;
}

#endif

//================================================================================
/*! Private -
 * \brief deactivates a signals handling
 */
//================================================================================
void CASCatch_CatchSignals::Destroy() 
{
  if(myIsActivated) Deactivate();
}

