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

//  File   : SALOMEDSImpl_AttributeFlags.cxx
//  Author : Sergey LITONIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeFlags.hxx"

/*
  Class       : SALOMEDSImpl_AttributeFlags
  Description : This class is intended for storing different object attributes that
                have only two states (0 and 1).
*/

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::GetID
//purpose  : Get GUID of this attribute
//=======================================================================
const std::string& SALOMEDSImpl_AttributeFlags::GetID ()
{
  static std::string SALOMEDSImpl_AttributeFlagsID( "866EEC9F-A517-4cb4-88E6-E208DB8FC96F" );
  return SALOMEDSImpl_AttributeFlagsID;                
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::Set
//purpose  : Set value of the attribute
//=======================================================================
SALOMEDSImpl_AttributeFlags* SALOMEDSImpl_AttributeFlags::Set(const DF_Label& L,
                                                              const int value )
{
  SALOMEDSImpl_AttributeFlags* A = NULL;
  if ( !(A=(SALOMEDSImpl_AttributeFlags*)L.FindAttribute(SALOMEDSImpl_AttributeFlags::GetID())) )
  {
    A = new  SALOMEDSImpl_AttributeFlags();
    L.AddAttribute( A );
  }

  A->Set( value );
  return A;
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::SALOMEDSImpl_AttributeFlags
//purpose  : Constructor
//=======================================================================
SALOMEDSImpl_AttributeFlags::SALOMEDSImpl_AttributeFlags()
:SALOMEDSImpl_GenericAttribute("AttributeFlags")
{
  myValue = 0;
}

SALOMEDSImpl_AttributeFlags::~SALOMEDSImpl_AttributeFlags()
{
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::ID
//purpose  : Get GUID of this attribute
//=======================================================================
const std::string& SALOMEDSImpl_AttributeFlags::ID () const
{
  return GetID();
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::NewEmpty
//purpose  : Create new empty attribute
//=======================================================================

DF_Attribute* SALOMEDSImpl_AttributeFlags::NewEmpty () const
{
  return new SALOMEDSImpl_AttributeFlags();
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::Restore
//purpose  : Assign given value to the attribute
//=======================================================================
void SALOMEDSImpl_AttributeFlags::Restore( DF_Attribute* with )
{
  myValue = dynamic_cast<SALOMEDSImpl_AttributeFlags*>( with )->Get();
  return;
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::Paste
//purpose  : Assign internal value to the given attribute
//=======================================================================
void SALOMEDSImpl_AttributeFlags::Paste( DF_Attribute* into)
{
  dynamic_cast<SALOMEDSImpl_AttributeFlags*>( into )->Set( myValue );
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::Set
//purpose  : Set value
//=======================================================================
void SALOMEDSImpl_AttributeFlags::Set( const int v )
{
  Backup();
  myValue=v;
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::Get
//purpose  : GetValue
//=======================================================================
int SALOMEDSImpl_AttributeFlags::Get() const
{
  return myValue;
}




















