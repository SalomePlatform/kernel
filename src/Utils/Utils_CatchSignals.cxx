//  KERNEL Utils : common utils for KERNEL
//  Copyright (C) 2003  CEA
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
//  See http://www.salome-platform.org or email : webmaster.salome@opencascade.org
//
//
//
//  File   : Utils_CatchSignals.cxx
//  Author : Oksana Tchebanova
//  Module : KERNEL
//  $Header:


#include "Utils_CatchSignals.h"
#include "utilities.h"

#include <stdexcept>

#define RESERVE_SIZE 100000

static void* theCallBack = NULL;
static int *myReserve = NULL;
using namespace std;
//=======================================================================
//function : Utils_CatchSignals
//purpose  : Constructor
//=======================================================================
Utils_CatchSignals::Utils_CatchSignals() 
     :myIsActivated(false)
{
  
  for(int i = 0; i<SIG_MAX_NUM; i++)
    mySigStates[i] = NULL;

  theCallBack = NULL; 
}

#ifndef WNT

//================================ UNIX part ==================================================


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
# include <bits/siginfo.h>
#    endif
# endif
#endif


#ifdef IRIX
# include <sigfpe.h>
# include <sys/siginfo.h>
#endif 

#include <unistd.h>
//============================================================================
//function : Handler 
//purpose  : univisal handler for signals
//============================================================================
static void Handler(const int theSig )
{
  if (myReserve) {
    delete myReserve; 
    myReserve = NULL;
  }
  char aMessage[1000];

#if defined __GNUC__
  #if __GNUC__ == 2
    #define __GNUC_2__
  #endif
#endif
#if defined __GNUC_2__
  const char* message = "La fonction strsignal n'est pas supportée par gcc2.95.4";
  sprintf (aMessage, "%d signal cautch : %s", theSig, message);
#else
  sprintf (aMessage, "%d signal cautch : %s", theSig, strsignal(theSig));
#endif


  MESSAGE("Utils_CatchSignal  Handler :: " << aMessage );
  if(theCallBack == NULL) {
    MESSAGE("Utils_CatchSignal  Handler : throw std::runtime_error()");
    throw (std::runtime_error(aMessage));
  }
  else ((void (*)())theCallBack)();  
}
//=======================================================================
//function : TryHandlerReset
//purpose  : Redefine signal handlers. If the handler of the signal is
//           set as SIG_IGN. That's why the shells often ignore some 
//           signal when starting child processes. We keep it.
//=======================================================================
static SIG_PFV TryHandlerReset( int theSigNum )
{
  if (signal(theSigNum, (SIG_PFV) &Handler) == SIG_IGN)
    {
      signal(theSigNum, SIG_IGN);  
      MESSAGE("signal "<<theSigNum <<" is kept as ignored");
      return SIG_IGN;
    } 
  else return signal(theSigNum, (SIG_PFV) &Handler);
}
//=======================================================================
//function : Activate
//purpose  : 
//=======================================================================

void Utils_CatchSignals::Activate() 
{  
  if(myIsActivated) return;

  if (myReserve) delete myReserve;
  myReserve = new int[RESERVE_SIZE];

    
  mySigStates[0] = TryHandlerReset(SIGHUP);   // ...... floating point exception
  mySigStates[1] = TryHandlerReset(SIGFPE);   // ...... floating point exception
  
  mySigStates[2] = TryHandlerReset(SIGINT);   // ...... interrupt
  mySigStates[3] = TryHandlerReset(SIGQUIT);  // ...... quit
  mySigStates[4] = TryHandlerReset(SIGBUS);   // ...... bus error
  mySigStates[5] = TryHandlerReset(SIGILL);   // ...... illegal instruction
  mySigStates[6] = TryHandlerReset(SIGTERM);  // ...... termination
  mySigStates[7] = TryHandlerReset(SIGSEGV);  // ...... segmentation 
  //mySigStates[8] = TryHandlerReset (SIGABRT); // ...... abort (ANSI).  
  mySigStates[9] = TryHandlerReset(SIGSTKFLT);// ...... stack fault.  

  myIsActivated = true;
}

//=======================================================================
//function : Deactivate
//purpose  : 
//=======================================================================
void Utils_CatchSignals::Deactivate() 
{
  if(!myIsActivated) return;

  signal(SIGHUP, (SIG_PFV)mySigStates[0]);   // ...... hangup
  signal(SIGFPE, (SIG_PFV)mySigStates[1]);   // ...... floating point exception
  signal(SIGINT, (SIG_PFV)mySigStates[2]);   // ...... interrupt
  signal(SIGQUIT,(SIG_PFV)mySigStates[3]);   // ...... quit
  signal(SIGBUS, (SIG_PFV)mySigStates[4]);   // ...... bus error
  signal(SIGILL, (SIG_PFV)mySigStates[5]);   // ...... illegal instruction
  signal(SIGTERM,(SIG_PFV)mySigStates[6]);   // ...... illegal instruction
  signal(SIGSEGV,(SIG_PFV)mySigStates[7]);   // ...... segmentation violation
  //signal(SIGABRT,(SIG_PFV)mySigStates[8] );  // ...... abort (ANSI).     
  signal(SIGSTKFLT,(SIG_PFV)mySigStates[9]); // ...... stack fault.  

  myIsActivated = false;
  delete myReserve;
  myReserve = NULL;
}
#endif // ifndef WNT

//=======================================================================
//function : SetCallBack
//purpose  : Sets pointer to (void (*)()
//=======================================================================
void Utils_CatchSignals::SetCallBack(const int theAddressHiPart, 
					 const int theAddressLowPart) 
{
  theCallBack = (void *)((theAddressHiPart << 32 ) + theAddressLowPart); 
}

//=======================================================================
//function : UnsetCallBack
//purpose  : 
//=======================================================================
void Utils_CatchSignals::UnsetCallBack()
{
  theCallBack = NULL;
}

//=======================================================================
//function : Destroy
//purpose  : 
//=======================================================================
void Utils_CatchSignals::Destroy() 
{
  if(myIsActivated) Deactivate();
}

bool Utils_CatchSignals::ReserveMemory()
{
  if (!myReserve) myReserve = new int [RESERVE_SIZE];
  return true;
}

void Utils_CatchSignals::FreeReserved()
{
  if (myReserve) 
    {delete myReserve ; myReserve = NULL;}
}
