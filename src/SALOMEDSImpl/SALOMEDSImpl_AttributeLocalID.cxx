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

//  File   : SALOMEDSImpl_AttributeLocalID.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeLocalID.hxx"
#include <stdlib.h>

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeLocalID::GetID () 
{
  static std::string SALOMEDSImpl_AttributeLocalID ("12837196-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeLocalID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

SALOMEDSImpl_AttributeLocalID* SALOMEDSImpl_AttributeLocalID::Set (const DF_Label& L,
                                                                   const int value) 
{
  SALOMEDSImpl_AttributeLocalID* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeLocalID*)L.FindAttribute(SALOMEDSImpl_AttributeLocalID::GetID()))) {
    A = new  SALOMEDSImpl_AttributeLocalID(); 
    L.AddAttribute(A);
  }
  
  A->SetValue(value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeLocalID::SALOMEDSImpl_AttributeLocalID()
:SALOMEDSImpl_GenericAttribute("AttributeLocalID")
{
  myValue = 0; 
}

//=======================================================================
//function : Set
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeLocalID::SetValue(const int theValue)
{
  CheckLocked();

  Backup();

  myValue = theValue;

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}
    

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeLocalID::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

DF_Attribute* SALOMEDSImpl_AttributeLocalID::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeLocalID(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeLocalID::Restore(DF_Attribute* with) 
{
  myValue = dynamic_cast<SALOMEDSImpl_AttributeLocalID*>(with)->Value();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeLocalID::Paste (DF_Attribute* into)
{
  dynamic_cast<SALOMEDSImpl_AttributeLocalID*>(into)->SetValue(myValue);
}

//=======================================================================
//function : Save
//purpose  :
//=======================================================================
std::string SALOMEDSImpl_AttributeLocalID::Save() 
{ 
  char buffer[128]; 
  sprintf(buffer, "%d", myValue);
  return std::string(buffer); 
}

//=======================================================================
//function : Load
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeLocalID::Load(const std::string& theValue)
{
  myValue = atoi(theValue.c_str());  
}
