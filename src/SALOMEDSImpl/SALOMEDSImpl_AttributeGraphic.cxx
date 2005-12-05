// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
//  File   : SALOMEDSImpl_AttributeGraphic.cxx
//  Author : Sergey LITONIN
//  Module : SALOME

#include "SALOMEDSImpl_AttributeGraphic.hxx"
#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>
#include <TColStd_DataMapIteratorOfDataMapOfIntegerInteger.hxx>

/*
  Class       : SALOMEDSImpl_AttributeGraphic
  Description : This class is intended for storing information about
                graphic representation of objects in dirrent views
*/

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeGraphic, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeGraphic, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : Get GUID of this attribute
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeGraphic::GetID()
{
  static Standard_GUID SALOMEDSImpl_AttributeGraphicID( "F17AE8F0-E354-4d6f-8E42-38385C36E67E" );
  return SALOMEDSImpl_AttributeGraphicID;
}

//=======================================================================
//function : SALOMEDSImpl_AttributeGraphic
//purpose  : Empty Constructor
//=======================================================================
SALOMEDSImpl_AttributeGraphic::SALOMEDSImpl_AttributeGraphic()
:SALOMEDSImpl_GenericAttribute("AttributeGraphic")
{
}

//=======================================================================
//function : ~SALOMEDSImpl_AttributeGraphic
//purpose  : Destructor
//=======================================================================
SALOMEDSImpl_AttributeGraphic::~SALOMEDSImpl_AttributeGraphic()
{
}

//=======================================================================
//function : SetVisibility
//purpose  : Set visibility of object in given view
//=======================================================================
void SALOMEDSImpl_AttributeGraphic::SetVisibility(const Standard_Integer theViewId,
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
Standard_Boolean SALOMEDSImpl_AttributeGraphic::GetVisibility(const Standard_Integer theViewId ) const
{
  return myVisibility.IsBound( theViewId ) ? myVisibility( theViewId ) : false;
}


//=======================================================================
//function : ID
//purpose  : Get GUID of this attribute
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeGraphic::ID () const
{
  return GetID();
}


//=======================================================================
//function : NewEmpty
//purpose  : Create new empty attribute
//=======================================================================
Handle(TDF_Attribute) SALOMEDSImpl_AttributeGraphic::NewEmpty () const
{
  return new SALOMEDSImpl_AttributeGraphic ();
}

//=======================================================================
//function : SetVisibility
//purpose  : Set visibility of object in all views
//=======================================================================
void SALOMEDSImpl_AttributeGraphic::SetVisibility( const TColStd_DataMapOfIntegerInteger& theMap )
{
  myVisibility = theMap;
}

//=======================================================================
//function : SetVisibility
//purpose  : Get visibility of object in all views
//=======================================================================
const TColStd_DataMapOfIntegerInteger& SALOMEDSImpl_AttributeGraphic::GetVisibility()
{
  return myVisibility;
}

//=======================================================================
//function : Restore
//purpose  : Restore value of attribute with value of theWith one
//=======================================================================
void SALOMEDSImpl_AttributeGraphic::Restore( const Handle(TDF_Attribute)& theWith )
{
  Handle(SALOMEDSImpl_AttributeGraphic) anAttr =
    Handle(SALOMEDSImpl_AttributeGraphic)::DownCast( theWith );

  if ( !anAttr.IsNull() )
    SetVisibility( anAttr->GetVisibility() );
}

//=======================================================================
//function : Paste
//purpose  : Paste value of current attribute to the value of entry one
//=======================================================================
void SALOMEDSImpl_AttributeGraphic::Paste( const Handle(TDF_Attribute)& theInto,
                                       const Handle(TDF_RelocationTable)& ) const
{
  Handle(SALOMEDSImpl_AttributeGraphic) anAttr =
    Handle(SALOMEDSImpl_AttributeGraphic)::DownCast( theInto );

  if ( !anAttr.IsNull() )
    anAttr->SetVisibility( myVisibility );
}

//=======================================================================
//function : Dump
//purpose  : Dump
//=======================================================================
Standard_OStream& SALOMEDSImpl_AttributeGraphic::Dump( Standard_OStream& anOS ) const
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

