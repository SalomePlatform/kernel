//  KERNEL SALOME_Event : Define event posting mechanism
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
//  File   : SALOME_Event.cxx
//  Author : Sergey ANIKIN
//  Module : KERNEL
//  $Header$

#include "SALOME_Event.hxx"

#include "utilities.h"

#include <qsemaphore.h>
#include <qapplication.h>
#include <qthread.h>
#include <pthread.h>

#ifdef _DEBUG_
static int MYDEBUG = 0;
#else
static int MYDEBUG = 0;
#endif


static pthread_t myThread;

void SALOME_Event::GetSessionThread(){
  myThread = pthread_self();
}

bool SALOME_Event::IsSessionThread(){
  bool aResult = myThread == pthread_self();
  if(MYDEBUG) INFOS("IsSessionThread() - "<<aResult);
  return aResult;
}


//===========================================================
/*!
 *  SALOME_Event::SALOME_Event
 *  Constructor
 */
//===========================================================
SALOME_Event::SALOME_Event(){
  if(MYDEBUG) MESSAGE( "SALOME_Event::SALOME_Event(): this = "<<this );
  // Prepare the semaphore 
  mySemaphore = new QSemaphore( 2 );
  *mySemaphore += 2;
}

//===========================================================
/*!
 *  SALOME_Event::~SALOME_Event
 *  Destructor
 */
//===========================================================
SALOME_Event::~SALOME_Event(){
  if(MYDEBUG) MESSAGE( "SALOME_Event::~SALOME_Event(): this = "<<this );
  if ( mySemaphore->available() < mySemaphore->total() )
    *mySemaphore -= mySemaphore->total() - mySemaphore->available();
  delete mySemaphore;
}

//===========================================================
/*!
 *  SALOME_Event::process
 *  Posts the event and optionally waits for its completion
 */
//===========================================================
void SALOME_Event::process()
{
  QThread::postEvent( qApp, new QCustomEvent( SALOME_EVENT, (void*)this ) );
  if(MYDEBUG) MESSAGE( "SALOME_Event::process(): this = "<<this<<", *mySemaphore += 1 " );
  *mySemaphore += 1;
  if(MYDEBUG) MESSAGE( "SALOME_Event::process(): this = "<<this<<" - COMPLETED" );
}

//===========================================================
/*!
 *  SALOME_Event::processed
 *  Signals that this event has been processed
 */
//===========================================================
void SALOME_Event::processed()
{
  if(MYDEBUG) MESSAGE( "SALOME_Event::processed(): this = "<<this );
  // process() takes control over mySemaphore after the next line is executed
  *mySemaphore -= 1;
}
