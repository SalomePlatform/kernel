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
//  File   : Utils_Timer.cxx
//  Module : SALOME
//
# include "Utils_Timer.hxx"

# include <iostream>

#include "utilities.h"

#ifndef WIN32
static struct timezone *tz=(struct timezone*) malloc(sizeof(struct timezone));
#else
//timezone *tz=_timezone;
#endif

#ifndef CLK_TCK
# define CLK_TCK      CLOCKS_PER_SEC
#endif

Utils_Timer::Utils_Timer() {
#ifndef WIN32
  RefToInitialTMS = new tms;
  RefToCurrentTMS = new tms;

  RefToInitialTimeB = new timeval;
  RefToCurrentTimeB = new timeval;
#else
  RefToInitialTMS = new FILETIME;
  RefToCurrentTMS = new FILETIME;

  RefToInitialTimeB = new time_t;
  RefToCurrentTimeB = new time_t;
#endif

  Cumul_user      = Cumul_sys = 0.;
  Stopped         = 1;
}

Utils_Timer::~Utils_Timer() {
  delete RefToInitialTMS ;
  delete RefToCurrentTMS ;

  delete RefToInitialTimeB ;
  delete RefToCurrentTimeB ;
}

void Utils_Timer::Start() {
  if (Stopped) {
    Stopped = 0;
#ifndef WIN32
    times(RefToInitialTMS);
    gettimeofday(RefToInitialTimeB,tz);
#else
    SYSTEMTIME st;
    GetSystemTime(&st);
    SystemTimeToFileTime(&st, RefToInitialTMS);
          time(RefToCurrentTimeB);
#endif
  }
}

void Utils_Timer::Stop() {
  if (!Stopped) {
#ifndef WIN32
    times(RefToCurrentTMS);
    int diffr_user = RefToCurrentTMS->tms_utime - RefToInitialTMS->tms_utime;
    int diffr_sys  = RefToCurrentTMS->tms_stime - RefToInitialTMS->tms_stime;
    gettimeofday(RefToCurrentTimeB,tz);

    Cumul_user += (double) diffr_user / CLK_TCK ;
    Cumul_sys  += (double) diffr_sys  / CLK_TCK ;
#else
    SYSTEMTIME st;
    GetSystemTime(&st);
    SystemTimeToFileTime(&st, RefToCurrentTMS);
    Cumul_user += (int)(((ULARGE_INTEGER*)(RefToCurrentTMS))->QuadPart - ((ULARGE_INTEGER*)(RefToInitialTMS))->QuadPart) / 10000000;
          Cumul_sys = Cumul_user;
          time(RefToCurrentTimeB);
#endif
   Stopped = 1;
  }
}

void Utils_Timer::Show() {
  bool StopSav = Stopped;
  if (!StopSav) Stop();
  MESSAGE("CPU user time: "   << Cumul_user  << " seconds ");
  MESSAGE("CPU system time: " << Cumul_sys   << " seconds ");
  if (!StopSav) Start();
}

void Utils_Timer::Reset() {
  Stopped     = 1;
  Cumul_user  = Cumul_sys = 0. ;
}

void Utils_Timer::ShowAbsolute(){
#if defined(_DEBUG_) || defined(_DEBUG)
#ifndef WIN32
    unsigned long Absolute_user = (unsigned long) ((timeval*)RefToCurrentTimeB)->tv_sec ;
#else
    unsigned long Absolute_user = (unsigned long) *RefToCurrentTimeB;
#endif
    MESSAGE("Absolute time: "   << Absolute_user  << " seconds ");
#endif
}
