//  SALOME OCCViewer : build OCC Viewer into Salome desktop
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
//  File   : OCCViewer_Selector.cxx
//  Author : Nicolas REJNERI
//  Module : SALOME
//  $Header$

/*!
  \class OCCViewer_Selector OCCViewer_Selector.h
  \brief Base class for object selection in Open CASCADE Viewer.
*/

#include "OCCViewer_Selector.h"
using namespace std;

/*!
    Constructor
*/
OCCViewer_Selector::OCCViewer_Selector() :
  myNumSelected (0),
  myEnableSelection( true ),
  myEnableMultipleSelection( true )
{
}

/*!
    Destructor
*/
OCCViewer_Selector::~OCCViewer_Selector()
{
}

/*!
    Enables/disables selection
*/
void OCCViewer_Selector::enableSelection( bool bEnable )
{
  myEnableSelection = bEnable;
}

/*!
    Enables/disables multiple selection i.e
    selection of several objects at the same time.
    If enabled, non-multiple selection is enabled as well.
*/
void OCCViewer_Selector::enableMultipleSelection( bool bEnable )
{
  myEnableMultipleSelection = bEnable;
  if ( bEnable ) myEnableSelection = bEnable;
}
