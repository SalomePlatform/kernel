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

//  File   : SALOMEDSImpl_StudyHandle.cxx
//  Author : Sergey LITONIN
//  Module : SALOME
//
#include "SALOMEDSImpl_StudyHandle.hxx"

/*
  Class       : SALOMEDSImpl_StudyHandle
  Description : This class is intended for storing information about
                graphic representation of objects in dirrent views
*/


//=======================================================================
//function : GetID
//purpose  : Get GUID of this attribute
//=======================================================================
const std::string& SALOMEDSImpl_StudyHandle::GetID()
{
  static std::string SALOMEDSImpl_StudyHandleID( "050C9555-4BA8-49bf-8F1C-086F0469A40B" );
  return SALOMEDSImpl_StudyHandleID;
}

//=======================================================================
//function : SALOMEDSImpl_StudyHandle
//purpose  : Empty Constructor
//=======================================================================
SALOMEDSImpl_StudyHandle::SALOMEDSImpl_StudyHandle()
{
  myHandle = NULL;
}

//=======================================================================
//function : Set
//purpose  : 
//=======================================================================
SALOMEDSImpl_StudyHandle* SALOMEDSImpl_StudyHandle::Set(const DF_Label& theLabel, 
                                                        SALOMEDSImpl_Study* theStudy)
{
  SALOMEDSImpl_StudyHandle* A = NULL;
  if (!(A=(SALOMEDSImpl_StudyHandle*)theLabel.FindAttribute(GetID()))) {
    A = new SALOMEDSImpl_StudyHandle; 
    theLabel.AddAttribute(A);
  }

  A->Set(theStudy);
  return A;  
}


//=======================================================================
//function : ID
//purpose  : Get GUID of this attribute
//=======================================================================
const std::string& SALOMEDSImpl_StudyHandle::ID () const
{
  return GetID();
}


//=======================================================================
//function : NewEmpty
//purpose  : Create new empty attribute
//=======================================================================
DF_Attribute* SALOMEDSImpl_StudyHandle::NewEmpty () const
{
  return new SALOMEDSImpl_StudyHandle ();
}


//=======================================================================
//function : Restore
//purpose  : Restore value of attribute with value of theWith one
//=======================================================================
void SALOMEDSImpl_StudyHandle::Restore( DF_Attribute* theWith )
{
  SALOMEDSImpl_StudyHandle* anAttr = dynamic_cast<SALOMEDSImpl_StudyHandle*>( theWith );
  if ( anAttr ) Set ( anAttr->Get() );
}

//=======================================================================
//function : Paste
//purpose  : Paste value of current attribute to the value of entry one
//=======================================================================
void SALOMEDSImpl_StudyHandle::Paste( DF_Attribute* theInto)
{
  SALOMEDSImpl_StudyHandle* anAttr =  dynamic_cast<SALOMEDSImpl_StudyHandle*>( theInto );
  if ( anAttr ) anAttr->Set ( myHandle );
}

