//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
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
//  File   : SALOMEDS_AttributeGraphic_i.cxx
//  Author : Sergey LITONIN
//  Module : SALOME
//  $Header$

#include "SALOMEDS_AttributeGraphic_i.hxx"

using namespace std;

//=======================================================================
// function : SALOMEDS_AttributeGraphic_i::~SetVisibility
// purpose  : Set visibility of object in given view
//=======================================================================
void SALOMEDS_AttributeGraphic_i::SetVisibility( CORBA::Long    theViewId,
                                                 CORBA::Boolean theValue )
{
  Handle(SALOMEDS_GraphicAttribute) anAttr =
    Handle(SALOMEDS_GraphicAttribute)::DownCast( _myAttr );
  if ( !anAttr.IsNull() )
    anAttr->SetVisibility( theViewId, theValue );
}

//=======================================================================
// function : SALOMEDS_AttributeGraphic_i::~SALOMEDS_AttributeGraphic_i
// purpose  : Get visibility of object in given view
//=======================================================================                                     
CORBA::Boolean SALOMEDS_AttributeGraphic_i::GetVisibility( CORBA::Long theViewId )
{
  Handle(SALOMEDS_GraphicAttribute) anAttr =
    Handle(SALOMEDS_GraphicAttribute)::DownCast( _myAttr );
  return !anAttr.IsNull() ? anAttr->GetVisibility( theViewId ) : false;
}


















