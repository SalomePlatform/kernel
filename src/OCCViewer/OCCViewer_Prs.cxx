//  SALOME OCCViewer : build OCC Viewer into Salome desktop
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
//  File   : OCCViewer_Prs.cxx
//  Author : Sergey ANIKIN
//  Module : SALOME
//  $Header$

#include "OCCViewer_Prs.h"
using namespace std;

//==========================================================
/*!
 *  OCCViewer_Prs::OCCViewer_Prs
 *  Default constructor
 */
//==========================================================
OCCViewer_Prs::OCCViewer_Prs() 
{
  myToActivate = true;
}

//==========================================================
/*!
 *  OCCViewer_Prs::OCCViewer_Prs
 *  Standard constructor
 */
//==========================================================
OCCViewer_Prs::OCCViewer_Prs( const Handle(AIS_InteractiveObject)& obj ) 
{  
  AddObject( obj ); 
}

//==========================================================
/*!
 *  OCCViewer_Prs::~OCCViewer_Prs
 *  Destructor
 */
//==========================================================
OCCViewer_Prs::~OCCViewer_Prs()
{ 
  myObjects.Clear(); 
}

//==========================================================
/*!
 *  OCCViewer_Prs::GetObjects
 *  Get interactive objects list
 */
//==========================================================
void OCCViewer_Prs::GetObjects( AIS_ListOfInteractive& list ) const 
{ 
  list = myObjects; 
}

//==========================================================
/*!
 *  OCCViewer_Prs::AddObject
 *  Add interactive object
 */
//==========================================================
void OCCViewer_Prs::AddObject( const Handle(AIS_InteractiveObject)& obj ) 
{ 
  myObjects.Append( obj ); 
}

//==========================================================
/*!
 *  OCCViewer_Prs::IsNull
 *  Return 0 if list of the interactive objects is empty
 *  [ Reimplemented from SALOME_Prs ]
 */
//==========================================================
bool OCCViewer_Prs::IsNull() const 
{ 
  return myObjects.IsEmpty(); 
}

//=================================================================
/*!
 *  GEOM_Displayer::SetToActivate
 *  This method is used for activisation/deactivisation of
 *  objects in the moment of displaying
*/   
//=================================================================
void OCCViewer_Prs::SetToActivate( const bool toActivate )
{
  myToActivate = toActivate;
}
bool OCCViewer_Prs::ToActivate() const
{
  return myToActivate;
}
