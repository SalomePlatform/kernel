//  VTKViewer_Filter : Filter for VTK viewer
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
//  File   : VTKViewer_Filter.cxx
//  Author : Sergey LITONIN
//  Module : SALOME

#include "VTKViewer_Filter.h"

IMPLEMENT_STANDARD_HANDLE(VTKViewer_Filter, MMgt_TShared)
IMPLEMENT_STANDARD_RTTIEXT(VTKViewer_Filter, MMgt_TShared)

/*
  Class       : VTKViewer_Filter
  Description : Base class of filters of for VTK viewer. Method IsValid 
                should be redefined in derived classes
*/

VTKViewer_Filter::VTKViewer_Filter()
{
  myActor = 0;
}

VTKViewer_Filter::~VTKViewer_Filter()
{
}

bool VTKViewer_Filter::IsValid( SALOME_Actor* theActor, const int theCellId )
{
  SetActor( theActor );
  return IsValid( theCellId );
}

void VTKViewer_Filter::SetActor( SALOME_Actor* theActor )
{
  myActor = theActor;
}
