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
//  File   : Utils_Timer.hxx
//  Module : SALOME

#include <stdlib.h>
#include <time.h>

# include <sys/times.h>
# include <sys/time.h>
# include <unistd.h>

class Utils_Timer {
 public:
  Utils_Timer();
  virtual ~Utils_Timer();
  void Start();
  void Stop();
  void Reset();
  void Show();
  void ShowAbsolute();
 protected:
  double Cumul_user;
  double Cumul_sys;
  bool Stopped;
  tms *RefToCurrentTMS, *RefToInitialTMS;
  timeval *RefToCurrentTimeB, *RefToInitialTimeB;
};
