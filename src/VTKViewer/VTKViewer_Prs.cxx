//  SALOME VTKViewer : build VTK viewer into Salome desktop
//
//  Copyright (C) 2004  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.salome-platform.org or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : VTKViewer_Prs.cxx
//  Author : Sergey ANIKIN
//  Module : SALOME
//  $Header$

#include "VTKViewer_Prs.h"
using namespace std;

//==========================================================
/*!
 *  VTKViewer_Prs::VTKViewer_Prs
 *  Default constructor
 */
//==========================================================
VTKViewer_Prs::VTKViewer_Prs() : myObjects( 0 )
{
}
 
//==========================================================
/*!
 *  VTKViewer_Prs::VTKViewer_Prs
 *  tandard constructora
 */
//==========================================================
VTKViewer_Prs::VTKViewer_Prs( const vtkActor* obj ) 
{ 
  AddObject( obj ); 
}

//==========================================================
/*!
 *  VTKViewer_Prs::~VTKViewer_Prs
 *  Destructor
 */
//==========================================================
VTKViewer_Prs:: ~VTKViewer_Prs() 
{ 
  if ( myObjects ) myObjects->Delete(); 
}

//==========================================================
/*!
 *  VTKViewer_Prs::GetObjects
 *  Get actors list
 */
//==========================================================
vtkActorCollection* VTKViewer_Prs::GetObjects() const 
{ 
  return myObjects; 
}

//==========================================================
/*!
 *  VTKViewer_Prs::AddObject
 *  Add actor
 */
//==========================================================
void VTKViewer_Prs::AddObject( const vtkActor* obj ) 
{ 
  if ( !myObjects) 
    myObjects = vtkActorCollection::New(); 
  myObjects->AddItem( (vtkActor*)obj ); 
}
  
//==========================================================
/*!
 *  VTKViewer_Prs::IsNull
 *  Return 0 if list of the actors is empty
 *  [ Reimplemented from SALOME_Prs ]
 */
//==========================================================
bool VTKViewer_Prs::IsNull() const 
{ 
  return !myObjects || myObjects->GetNumberOfItems() <= 0; 
}
