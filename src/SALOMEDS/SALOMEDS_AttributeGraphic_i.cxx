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

//  File   : SALOMEDS_AttributeGraphic_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeGraphic_i.hxx"
#include "SALOMEDS.hxx" 

/*
  Class       : SALOMEDS_AttributeGraphic_i
  Description : This class is intended for storing information about
                graphic representation of objects in dirrent views
*/


//=======================================================================
// function : SALOMEDS_AttributeGraphic_i::~SetVisibility
// purpose  : Set visibility of object in given view
//=======================================================================
void SALOMEDS_AttributeGraphic_i::SetVisibility( CORBA::Long    theViewId,
                                                 CORBA::Boolean theValue )
{
  SALOMEDS::Locker lock;    
  if ( _impl )
    dynamic_cast<SALOMEDSImpl_AttributeGraphic*>(_impl)->SetVisibility( theViewId, theValue );
}

//=======================================================================
// function : SALOMEDS_AttributeGraphic_i::~SALOMEDS_AttributeGraphic_i
// purpose  : Get visibility of object in given view
//=======================================================================                                     
CORBA::Boolean SALOMEDS_AttributeGraphic_i::GetVisibility( CORBA::Long theViewId )
{
  SALOMEDS::Locker lock;
  return ( _impl ) ? dynamic_cast<SALOMEDSImpl_AttributeGraphic*>(_impl)->GetVisibility( theViewId ) : false;
}


















