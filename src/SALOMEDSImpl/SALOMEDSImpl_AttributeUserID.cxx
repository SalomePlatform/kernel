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

//  File   : SALOMEDSImpl_AttributeUAttribute.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeUserID.hxx"
#include "Basics_Utils.hxx"

std::string SALOMEDSImpl_AttributeUserID::DefaultID()
{
  return Kernel_Utils::GetGUID(Kernel_Utils::DefUserID);
}   

SALOMEDSImpl_AttributeUserID* SALOMEDSImpl_AttributeUserID::Set (const DF_Label& L, const std::string& ID) 
{
  SALOMEDSImpl_AttributeUserID* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeUserID*)L.FindAttribute(ID))) {
    A = new  SALOMEDSImpl_AttributeUserID(); 
    A->SetValue(ID);
    L.AddAttribute(A);
  }
  return A;
}

//=======================================================================
//function : ID
//purpose  :
//=======================================================================
const std::string& SALOMEDSImpl_AttributeUserID::ID() const
{ return myID; }


//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeUserID::SetValue( const std::string&  guid)
{
  CheckLocked();
  if(myID == guid) return;

  Backup();
  myID = guid;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}        

//=======================================================================
//function : NewEmpty
//purpose  :
//=======================================================================
DF_Attribute* SALOMEDSImpl_AttributeUserID::NewEmpty () const
{
  SALOMEDSImpl_AttributeUserID* A = new SALOMEDSImpl_AttributeUserID();
  A->SetValue(myID);
  return A;
}

//=======================================================================
//function : Restore
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeUserID::Restore(DF_Attribute* with)
{
  SALOMEDSImpl_AttributeUserID* A = dynamic_cast<SALOMEDSImpl_AttributeUserID*>(with);
  SetValue( A->ID() );
}

//=======================================================================
//function : Paste
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeUserID::Paste (DF_Attribute* into)
{
  SALOMEDSImpl_AttributeUserID* A = dynamic_cast<SALOMEDSImpl_AttributeUserID*>(into);
  A->SetValue( myID );
}    

std::string SALOMEDSImpl_AttributeUserID::Type() 
{

  char* aUAttrName = new char[127];
  sprintf(aUAttrName, "AttributeUserID_%s",ID().c_str());

  std::string ret(aUAttrName);
  delete aUAttrName;

  return ret;
}
