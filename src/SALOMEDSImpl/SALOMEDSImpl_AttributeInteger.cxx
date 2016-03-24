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

//  File   : SALOMEDSImpl_AttributeInteger.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeInteger.hxx"

#include <stdlib.h>

//=======================================================================
//function : GetID
//purpose  :
//=======================================================================
const std::string& SALOMEDSImpl_AttributeInteger::GetID ()
{
  static std::string IntegerID ("8CC3E213-C9B4-47e4-8496-DD5E62E22018");
  return IntegerID;
}   

SALOMEDSImpl_AttributeInteger* SALOMEDSImpl_AttributeInteger::Set (const DF_Label& L, int Val) 
{
  SALOMEDSImpl_AttributeInteger* A = NULL;
  if (!(A = (SALOMEDSImpl_AttributeInteger*)L.FindAttribute(SALOMEDSImpl_AttributeInteger::GetID()))) {
    A = new  SALOMEDSImpl_AttributeInteger(); 
    L.AddAttribute(A);
  }

  A->SetValue(Val); 
  return A;
}

//=======================================================================
//function : Value
//purpose  :
//=======================================================================
int SALOMEDSImpl_AttributeInteger::Value() const
{
    return myValue;
}

//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeInteger::SetValue(const int v)
{
  if(myValue == v) return;

  Backup();
  myValue = v;

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//function : ID
//purpose  :
//=======================================================================
const std::string& SALOMEDSImpl_AttributeInteger::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  :
//=======================================================================
DF_Attribute* SALOMEDSImpl_AttributeInteger::NewEmpty () const
{
  return new SALOMEDSImpl_AttributeInteger();
}

//=======================================================================
//function : Restore
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeInteger::Restore(DF_Attribute* With)
{
  myValue = dynamic_cast<SALOMEDSImpl_AttributeInteger*>(With)->Value();
}

//=======================================================================
//function : Paste
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeInteger::Paste (DF_Attribute* Into)
{
  dynamic_cast<SALOMEDSImpl_AttributeInteger*>(Into)->SetValue(myValue);
}             

//=======================================================================
//function : Save
//purpose  :
//=======================================================================
std::string SALOMEDSImpl_AttributeInteger::Save() 
{ 
  char buffer[128]; 
  sprintf(buffer, "%d", myValue);
  return std::string(buffer); 
}

//=======================================================================
//function : Load
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeInteger::Load(const std::string& theValue)
{
  myValue = atoi(theValue.c_str());  
}
