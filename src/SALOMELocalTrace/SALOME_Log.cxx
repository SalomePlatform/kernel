//  SALOME_LocalTrace : log on local machine
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
//  File   : SALOME_Log.hxx
//  Author : Vasily Rusyaev, Paul RASCLE, EDF
//  Module : KERNEL
//  $Header$

#include <string>
#include <iostream>

#include "SALOME_Log.hxx"
using namespace std;

SALOME_Log* SALOME_Log::_singleton = 0;

// log line size: if too short, log line is truncated, without crash...
char SALOME_LogStr[1024]; 

SALOME_Log::SALOME_Log(): ostrstream(SALOME_LogStr,sizeof(SALOME_LogStr))
{
}

SALOME_Log::~SALOME_Log()
{
}

SALOME_Log* SALOME_Log::Instance()
{
  if (_singleton == 0) _singleton = new SALOME_Log();
  return _singleton;
}

void SALOME_Log::putMessage(std::ostream& msg)
{
  cout << std::flush;
  cerr <<str() << std::flush;
  rdbuf()->freeze(false);
  seekp(0);
}


