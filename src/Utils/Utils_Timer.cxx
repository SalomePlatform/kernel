//  SALOME Utils : general SALOME's definitions and tools
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
//  File   : Utils_Timer.cxx
//  Module : SALOME

# include "Utils_Timer.hxx"
# include <iostream>
using namespace std;

static struct timezone *tz=(struct timezone*) malloc(sizeof(struct timezone));

#ifndef CLK_TCK
# define CLK_TCK      CLOCKS_PER_SEC
#endif

Utils_Timer::Utils_Timer() {
  RefToInitialTMS = new tms;
  RefToCurrentTMS = new tms;

  RefToInitialTimeB = new timeval;
  RefToCurrentTimeB = new timeval;

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
    times(RefToInitialTMS);
    gettimeofday(RefToInitialTimeB,tz);
  }
}

void Utils_Timer::Stop() {
  if (!Stopped) {
    times(RefToCurrentTMS);
    int diffr_user = RefToCurrentTMS->tms_utime - RefToInitialTMS->tms_utime;
    int diffr_sys  = RefToCurrentTMS->tms_stime - RefToInitialTMS->tms_stime;
    gettimeofday(RefToCurrentTimeB,tz);
     
    Cumul_user += (double) diffr_user / CLK_TCK ;
    Cumul_sys  += (double) diffr_sys  / CLK_TCK ;
    
    Stopped = 1;
  }
}

void Utils_Timer::Show() {
  bool StopSav = Stopped;
  if (!StopSav) Stop();
  cout << "CPU user time: "   << Cumul_user  << " seconds " << endl;
  cout << "CPU system time: " << Cumul_sys   << " seconds " << endl;
  if (!StopSav) Start();
}

void Utils_Timer::Reset() {
  Stopped     = 1;
  Cumul_user  = Cumul_sys = 0. ;
}

void Utils_Timer::ShowAbsolute(){
    unsigned long Absolute_user = (unsigned long) ((timeval*)RefToCurrentTimeB)->tv_sec ;
    cout << "Absolute time: "   << Absolute_user  << " seconds " << endl;
}
