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
//  File   : SALOMEDS_AttributeFlags_i.cxx
//  Author : Sergey LITONIN
//  Module : SALOME
//  $Header$

#include "SALOMEDS_AttributeFlags_i.hxx"

using namespace std;

//=======================================================================
// function : SALOMEDS_AttributeFlags_i::GetFlags
// purpose  : Get all flags as integer value
//=======================================================================
CORBA::Long SALOMEDS_AttributeFlags_i::GetFlags()
{
  return Handle(SALOMEDS_FlagsAttribute)::DownCast( _myAttr )->Get();
}

//=======================================================================
// function : SALOMEDS_AttributeFlags_i::SetFlags
// purpose  : Set all flags as integer value
//=======================================================================
void SALOMEDS_AttributeFlags_i::SetFlags( CORBA::Long theFlags )
{
  Handle(SALOMEDS_FlagsAttribute)::DownCast( _myAttr )->Set( theFlags );
}

//=======================================================================
// function : SALOMEDS_AttributeFlags_i::Get
// purpose  : Get specified flag
//=======================================================================
CORBA::Boolean SALOMEDS_AttributeFlags_i::Get( CORBA::Long theFlag )
{
  return Handle(SALOMEDS_FlagsAttribute)::DownCast( _myAttr )->Get() & theFlag ? true : false;
}

//=======================================================================
// function : SALOMEDS_AttributeFlags_i::Set
// purpose  : Set/Unset specified flag
//=======================================================================
void SALOMEDS_AttributeFlags_i::Set( CORBA::Long theFlag, CORBA::Boolean theValue )
{
  Handle(SALOMEDS_FlagsAttribute) anAttr =
    Handle(SALOMEDS_FlagsAttribute)::DownCast( _myAttr );
  if ( theValue )
    anAttr->Set( anAttr->Get() | theFlag );
  else
    anAttr->Set( anAttr->Get() & ~theFlag );
}


















