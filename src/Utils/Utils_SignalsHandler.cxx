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


#include <stdexcept>
#include <stdio.h>
#include <signal.h>

#include "Utils_SignalsHandler.h"
using namespace std;


//============================================================================
//function : Handler 
//purpose  : univisal handler for signals
//============================================================================
static void Handler(int theSigId)
{
  char aMessage[256] = "";
  sprintf(aMessage,"Signal with ID = %d was cautch!",theSigId);
  throw std::runtime_error(aMessage);
}


//=======================================================================
//function : SetSigHandler
//purpose  : Redefine signal handlers. If the handler of the signal is
//           set as SIG_IGN. That's why the shells often ignore some 
//           signal when starting child processes. We keep it.
//=======================================================================
static void SetSignalHandler(Utils_SignalsHandler::TSigHandlerCont& theSigHandlerCont,
				      int theSigId)
{
  TSigHandler anOldHandler = signal(theSigId,&Handler);
  if(anOldHandler == SIG_IGN)
    signal(theSigId,SIG_IGN);  
  theSigHandlerCont[theSigId] = anOldHandler;
}

static TSigHandler StoreSignalHandler(Utils_SignalsHandler::TSigHandlerCont& theSigHandlerCont,
				      int theSigId)
{
  TSigHandler anOldHandler = signal(theSigId,&Handler);
  signal(theSigId,anOldHandler);
  if(anOldHandler == SIG_IGN)
    signal(theSigId,SIG_IGN);  
  theSigHandlerCont[theSigId] = anOldHandler;
  return anOldHandler;
}

static void RestoreSigHandler(TSigHandler theSigHandler,
			      int theSigId)
{
  signal(theSigId,theSigHandler);
}


//=======================================================================
//function : Utils_SignalsHandler
//purpose  : Constructor
//=======================================================================
Utils_SignalsHandler::Utils_SignalsHandler()
{
  StoreSignalHandler(mySigHandlerCont,SIGHUP); // floating point exception
  StoreSignalHandler(mySigHandlerCont,SIGFPE); // floating point exception
  
  StoreSignalHandler(mySigHandlerCont,SIGINT); // interrupt
  StoreSignalHandler(mySigHandlerCont,SIGQUIT); // quit
  StoreSignalHandler(mySigHandlerCont,SIGBUS); // bus error
  StoreSignalHandler(mySigHandlerCont,SIGILL); // illegal instruction
  StoreSignalHandler(mySigHandlerCont,SIGTERM); // termination
  StoreSignalHandler(mySigHandlerCont,SIGSEGV); // segmentation 
  //StoreSignalHandler(mySigHandlerCont,SIGABRT); // abort (ANSI).  
  // portage CCRT
  //  StoreSignalHandler(mySigHandlerCont,SIGSTKFLT); // stack fault.  
}


//=======================================================================
//function : Utils_SignalsHandler
//purpose  : destructor
//=======================================================================
Utils_SignalsHandler::~Utils_SignalsHandler() 
{
  TSigHandlerCont::iterator anIter = mySigHandlerCont.begin();
  TSigHandlerCont::iterator anIterEnd = mySigHandlerCont.end();
  for(; anIter != anIterEnd; anIter++)
    RestoreSigHandler(anIter->second,anIter->first);
}


//=======================================================================
//function : SetSigHandler
//purpose  : sets new handler for pointed signal
//=======================================================================
TSigHandler Utils_SignalsHandler::SetSigHandler(int theSigId, 
						TSigHandler theSigHandler)
{
  TSigHandler anOldHandler = signal(theSigId,theSigHandler);
  if(anOldHandler == SIG_IGN)
    signal(theSigId,SIG_IGN);  
  mySigHandlerCont[theSigId] = anOldHandler;
  return anOldHandler;
}
