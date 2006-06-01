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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : SALOMEDS_FlagsAttribute.cxx
//  Author : Sergey LITONIN
//  Module : SALOME

#include "SALOMEDS_FlagsAttribute.hxx"
#include <TDataStd_Integer.hxx>
#include <Standard_GUID.hxx>
#include <TDF_Label.hxx>

using namespace std;


/*
  Class       : SALOMEDS_FlagsAttribute
  Description : This class is intended for storing different object attributes that
                have only two states (0 and 1).
*/

IMPLEMENT_STANDARD_HANDLE( SALOMEDS_FlagsAttribute, TDataStd_Integer )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDS_FlagsAttribute, TDataStd_Integer )

//=======================================================================
//function : SALOMEDS_FlagsAttribute::GetID
//purpose  : Get GUID of this attribute
//=======================================================================
const Standard_GUID& SALOMEDS_FlagsAttribute::GetID ()
{
  static Standard_GUID SALOMEDS_FlagsAttributeID( "866EEC9F-A517-4cb4-88E6-E208DB8FC96F" );
  return SALOMEDS_FlagsAttributeID;                
}

//=======================================================================
//function : SALOMEDS_FlagsAttribute::Set
//purpose  : Set value of the attribute
//=======================================================================
Handle(SALOMEDS_FlagsAttribute) SALOMEDS_FlagsAttribute::Set( const TDF_Label& L,
                                                              const Standard_Integer value )
{
  Handle(SALOMEDS_FlagsAttribute) A;
  if ( !L.FindAttribute(SALOMEDS_FlagsAttribute::GetID(),A ) )
  {
    A = new  SALOMEDS_FlagsAttribute();
    L.AddAttribute( A );
  }

  ( Handle(TDataStd_Integer)::DownCast( A ) )->Set( value );
  return A;
}

//=======================================================================
//function : SALOMEDS_FlagsAttribute::SALOMEDS_FlagsAttribute
//purpose  : Constructor
//=======================================================================
SALOMEDS_FlagsAttribute::SALOMEDS_FlagsAttribute()
{
  TDataStd_Integer::Set( 0 );
}

SALOMEDS_FlagsAttribute::~SALOMEDS_FlagsAttribute()
{
}

//=======================================================================
//function : SALOMEDS_FlagsAttribute::ID
//purpose  : Get GUID of this attribute
//=======================================================================
const Standard_GUID& SALOMEDS_FlagsAttribute::ID () const
{
  return GetID();
}

//=======================================================================
//function : SALOMEDS_FlagsAttribute::NewEmpty
//purpose  : Create new empty attribute
//=======================================================================

Handle(TDF_Attribute) SALOMEDS_FlagsAttribute::NewEmpty () const
{
  return new SALOMEDS_FlagsAttribute();
}

//=======================================================================
//function : SALOMEDS_FlagsAttribute::Restore
//purpose  : Assign given value to the attribute
//=======================================================================
void SALOMEDS_FlagsAttribute::Restore( const Handle(TDF_Attribute)& with )
{
  Standard_Integer s = Handle(TDataStd_Integer)::DownCast( with )->Get();
  TDataStd_Integer::Set( s );
  return;
}

//=======================================================================
//function : SALOMEDS_FlagsAttribute::Paste
//purpose  : Assign internal value to the given attribute
//=======================================================================
void SALOMEDS_FlagsAttribute::Paste( const Handle(TDF_Attribute)& into,
                                     const Handle(TDF_RelocationTable)& RT ) const
{
  Handle(TDataStd_Integer)::DownCast( into )->Set( Get() );
}

//=======================================================================
//function : SALOMEDS_FlagsAttribute::Set
//purpose  : Set value
//=======================================================================
void SALOMEDS_FlagsAttribute::Set( const Standard_Integer v )
{
  TDataStd_Integer::Set( v );
}

//=======================================================================
//function : SALOMEDS_FlagsAttribute::Get
//purpose  : GetValue
//=======================================================================
Standard_Integer SALOMEDS_FlagsAttribute::Get() const
{
  return TDataStd_Integer::Get();
}




















