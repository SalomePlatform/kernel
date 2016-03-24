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

//  File   : SALOMEDSImpl_AttributeGraphic.cxx
//  Author : Sergey LITONIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeGraphic.hxx"
#include "DF_Attribute.hxx"

/*
  Class       : SALOMEDSImpl_AttributeGraphic
  Description : This class is intended for storing information about
                graphic representation of objects in dirrent views
*/

//=======================================================================
//function : GetID
//purpose  : Get GUID of this attribute
//=======================================================================
const std::string& SALOMEDSImpl_AttributeGraphic::GetID()
{
  static std::string SALOMEDSImpl_AttributeGraphicID( "F17AE8F0-E354-4d6f-8E42-38385C36E67E" );
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
void SALOMEDSImpl_AttributeGraphic::SetVisibility(const int theViewId,
                                                  const bool theValue )
{
  if ( myVisibility.find( theViewId ) != myVisibility.end() && myVisibility[theViewId] == theValue )
    return;

  Backup();
  myVisibility[ theViewId ] = theValue ? 1 : 0;
}


//=======================================================================
//function : Get
//purpose  : Get visibility of object in given view
//=======================================================================
bool SALOMEDSImpl_AttributeGraphic::GetVisibility(const int theViewId )
{
  bool isVisible = false;
  if(myVisibility.find( theViewId )!=myVisibility.end()) 
    isVisible = (bool)myVisibility[theViewId]; 
  else 
    isVisible = false;
  
  return isVisible;  
}


//=======================================================================
//function : ID
//purpose  : Get GUID of this attribute
//=======================================================================
const std::string& SALOMEDSImpl_AttributeGraphic::ID () const
{
  return GetID();
}


//=======================================================================
//function : NewEmpty
//purpose  : Create new empty attribute
//=======================================================================
DF_Attribute* SALOMEDSImpl_AttributeGraphic::NewEmpty () const
{
  return new SALOMEDSImpl_AttributeGraphic ();
}

//=======================================================================
//function : SetVisibility
//purpose  : Set visibility of object in all views
//=======================================================================
void SALOMEDSImpl_AttributeGraphic::SetVisibility( const std::map<int, int>& theMap )
{
  myVisibility = theMap;
}

//=======================================================================
//function : SetVisibility
//purpose  : Get visibility of object in all views
//=======================================================================
const std::map<int, int>& SALOMEDSImpl_AttributeGraphic::GetVisibility()
{
  return myVisibility;
}

//=======================================================================
//function : Restore
//purpose  : Restore value of attribute with value of theWith one
//=======================================================================
void SALOMEDSImpl_AttributeGraphic::Restore( DF_Attribute* theWith )
{
  SALOMEDSImpl_AttributeGraphic* anAttr =
    dynamic_cast<SALOMEDSImpl_AttributeGraphic*>( theWith );

  if ( anAttr )
    SetVisibility( anAttr->GetVisibility() );
}

//=======================================================================
//function : Paste
//purpose  : Paste value of current attribute to the value of entry one
//=======================================================================
void SALOMEDSImpl_AttributeGraphic::Paste( DF_Attribute* theInto)
{
  SALOMEDSImpl_AttributeGraphic* anAttr =
    dynamic_cast<SALOMEDSImpl_AttributeGraphic*>( theInto );

  if ( anAttr )
    anAttr->SetVisibility( myVisibility );
}

