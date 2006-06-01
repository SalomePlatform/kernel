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
//  File   : SALOMEDS_GraphicAttribute.cxx
//  Author : Sergey LITONIN
//  Module : SALOME

#include "SALOMEDS_GraphicAttribute.hxx"
#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>
#include <TDF_Label.hxx>
#include <TColStd_DataMapIteratorOfDataMapOfIntegerInteger.hxx>

/*
  Class       : SALOMEDS_GraphicAttribute
  Description : This class is intended for storing information about
                graphic representation of objects in dirrent views
*/

IMPLEMENT_STANDARD_HANDLE( SALOMEDS_GraphicAttribute, TDF_Attribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDS_GraphicAttribute, TDF_Attribute )

//=======================================================================
//function : GetID
//purpose  : Get GUID of this attribute
//=======================================================================
const Standard_GUID& SALOMEDS_GraphicAttribute::GetID()
{
  static Standard_GUID SALOMEDS_GraphicAttributeID( "F17AE8F0-E354-4d6f-8E42-38385C36E67E" );
  return SALOMEDS_GraphicAttributeID;
}

//=======================================================================
//function : SALOMEDS_GraphicAttribute
//purpose  : Empty Constructor
//=======================================================================
SALOMEDS_GraphicAttribute::SALOMEDS_GraphicAttribute()
{
}

//=======================================================================
//function : ~SALOMEDS_GraphicAttribute
//purpose  : Destructor
//=======================================================================
SALOMEDS_GraphicAttribute::~SALOMEDS_GraphicAttribute()
{
}

//=======================================================================
//function : SetVisibility
//purpose  : Set visibility of object in given view
//=======================================================================
void SALOMEDS_GraphicAttribute::SetVisibility( const Standard_Integer theViewId,
                                               const Standard_Boolean theValue )
{
  if ( myVisibility.IsBound( theViewId ) && myVisibility( theViewId ) == theValue )
    return;

  Backup();
  if ( myVisibility.IsBound( theViewId ) )
    myVisibility.ChangeFind( theViewId ) = theValue ? 1 : 0;
  else
    myVisibility.Bind( theViewId, theValue ? 1 : 0 );
}


//=======================================================================
//function : Get
//purpose  : Get visibility of object in given view
//=======================================================================
Standard_Boolean SALOMEDS_GraphicAttribute::GetVisibility(
  const Standard_Integer theViewId ) const
{
  return myVisibility.IsBound( theViewId ) ? myVisibility( theViewId ) : false;
}


//=======================================================================
//function : ID
//purpose  : Get GUID of this attribute
//=======================================================================
const Standard_GUID& SALOMEDS_GraphicAttribute::ID () const
{
  return GetID();
}


//=======================================================================
//function : NewEmpty
//purpose  : Create new empty attribute
//=======================================================================
Handle(TDF_Attribute) SALOMEDS_GraphicAttribute::NewEmpty () const
{
  return new SALOMEDS_GraphicAttribute ();
}

//=======================================================================
//function : SetVisibility
//purpose  : Set visibility of object in all views
//=======================================================================
void SALOMEDS_GraphicAttribute::SetVisibility( const TColStd_DataMapOfIntegerInteger& theMap )
{
  myVisibility = theMap;
}

//=======================================================================
//function : SetVisibility
//purpose  : Get visibility of object in all views
//=======================================================================
const TColStd_DataMapOfIntegerInteger& SALOMEDS_GraphicAttribute::GetVisibility()
{
  return myVisibility;
}

//=======================================================================
//function : Restore
//purpose  : Restore value of attribute with value of theWith one
//=======================================================================
void SALOMEDS_GraphicAttribute::Restore( const Handle(TDF_Attribute)& theWith )
{
  Handle(SALOMEDS_GraphicAttribute) anAttr =
    Handle(SALOMEDS_GraphicAttribute)::DownCast( theWith );

  if ( !anAttr.IsNull() )
    SetVisibility( anAttr->GetVisibility() );
}

//=======================================================================
//function : Paste
//purpose  : Paste value of current attribute to the value of entry one
//=======================================================================
void SALOMEDS_GraphicAttribute::Paste( const Handle(TDF_Attribute)& theInto,
                                       const Handle(TDF_RelocationTable)& ) const
{
  Handle(SALOMEDS_GraphicAttribute) anAttr =
    Handle(SALOMEDS_GraphicAttribute)::DownCast( theInto );

  if ( !anAttr.IsNull() )
    anAttr->SetVisibility( myVisibility );
}

//=======================================================================
//function : Dump
//purpose  : Dump
//=======================================================================
Standard_OStream& SALOMEDS_GraphicAttribute::Dump( Standard_OStream& anOS ) const
{
  anOS << "Visibility of object:" << endl;
  TColStd_DataMapIteratorOfDataMapOfIntegerInteger anIter( myVisibility );
  for ( ; anIter.More(); anIter.Next() )
  {
    char str[ 100 ];
    
    if ( GetVisibility( anIter.Key() ) )
      sprintf( str, "Viewer ID = 0x%X State = VISIBLE\n", anIter.Key() );
    else
      sprintf( str, "Viewer ID = 0x%X State = INVISIBLE\n", anIter.Key() );
      
    anOS << str;
  }
  
  anOS << "Integer";
  return anOS;
}

