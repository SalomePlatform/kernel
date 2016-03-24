// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
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

//  SALOME Utils : general SALOME's definitions and tools
//  File:       Utils_Mutex.cxx
//  Author:     Sergey ANIKIN
//  Module :    SALOME
//  $Header$
//
#include "Utils_Mutex.hxx"

Utils_Mutex::Utils_Mutex() 
: myCount( 0 )
{
  pthread_mutex_init( &myMutex, 0 );
  pthread_mutex_init( &myHelperMutex, 0 );
}

Utils_Mutex::~Utils_Mutex()
{
  pthread_mutex_destroy( &myHelperMutex );
  pthread_mutex_destroy( &myMutex );
}

void Utils_Mutex::lock()
{
  pthread_mutex_lock( &myHelperMutex );

#ifndef WIN32 
  if ( myCount > 0 && myThread == pthread_self() ) {
#else
  if ( myCount > 0 && myThread.p == pthread_self().p ) {
#endif
    myCount++;
  }
  else {
    pthread_mutex_unlock( &myHelperMutex );
    pthread_mutex_lock( &myMutex );
    pthread_mutex_lock( &myHelperMutex );
    myCount = 1;
    myThread = pthread_self();
  }
  
  pthread_mutex_unlock( &myHelperMutex );
}

void Utils_Mutex::unlock()
{
  pthread_mutex_lock( &myHelperMutex );

#ifndef WIN32  
  if ( myThread == pthread_self() ) {
#else
  if ( myThread.p == pthread_self().p ) {
#endif
    if ( myCount && (--myCount) < 1 ) {
      myCount = 0;
      pthread_mutex_unlock( &myMutex );   
    }
  }
  
  pthread_mutex_unlock( &myHelperMutex );
}

Utils_Locker::Utils_Locker( Utils_Mutex* mutex )
: myMutex( mutex ) 
{ 
  if ( myMutex ) myMutex->lock(); 
}

Utils_Locker::~Utils_Locker() 
{
  if ( myMutex ) myMutex->unlock(); 
}
