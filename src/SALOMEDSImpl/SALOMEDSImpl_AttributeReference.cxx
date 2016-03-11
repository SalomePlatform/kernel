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

//  File   : SALOMEDSImpl_AttributeReference.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeReference.hxx"

//=======================================================================
//function : GetID
//purpose  :
//=======================================================================
const std::string& SALOMEDSImpl_AttributeReference::GetID ()
{
  static std::string refID ("D913E0B3-0A9F-4ea6-9480-18A9B72D9D86");
  return refID;
} 

SALOMEDSImpl_AttributeReference* SALOMEDSImpl_AttributeReference::Set(const DF_Label& theLabel, 
                                                                      const DF_Label& theRefLabel)
{
  SALOMEDSImpl_AttributeReference* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeReference*)theLabel.FindAttribute(SALOMEDSImpl_AttributeReference::GetID()))) {
    A = new  SALOMEDSImpl_AttributeReference(); 
    theLabel.AddAttribute(A);
  }

  A->Set(theRefLabel);  
  return A;  
}

//=======================================================================
//function : Set
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeReference::Set(const DF_Label& Origin)
{
  CheckLocked();

  if(myLabel == Origin) return;

  Backup();
  myLabel = Origin;

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}  

//=======================================================================
//function : ID
//purpose  :
//=======================================================================
const std::string& SALOMEDSImpl_AttributeReference::ID () const { return GetID(); } 


std::string SALOMEDSImpl_AttributeReference::Save() 
{
  return myLabel.Entry();
}

void SALOMEDSImpl_AttributeReference::Load(const std::string& value) 
{
  myLabel = DF_Label::Label(Label(), value, true);
}

//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================
DF_Attribute* SALOMEDSImpl_AttributeReference::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeReference(); 
}

//=======================================================================
//function : Restore
//purpose  :
//=======================================================================

void SALOMEDSImpl_AttributeReference::Restore(DF_Attribute* With)
{
  myLabel = dynamic_cast<SALOMEDSImpl_AttributeReference*>(With)->Get ();
}

//=======================================================================
//function : Paste
//purpose  :
//=======================================================================

void SALOMEDSImpl_AttributeReference::Paste (DF_Attribute* Into)
{
  dynamic_cast<SALOMEDSImpl_AttributeReference*>(Into)->Set(myLabel);
} 
