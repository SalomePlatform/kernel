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

//  File   : SALOMEDSImpl_AttributeExternalFileDef.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeExternalFileDef.hxx"

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================
const std::string& SALOMEDSImpl_AttributeExternalFileDef::GetID () 
{
  static std::string SALOMEDSImpl_AttributeExternalFileDefID ("7123AD4C-ACDB-4e3a-8FDC-70EA164D2CBE");
  return SALOMEDSImpl_AttributeExternalFileDefID;
}

//=======================================================================
//function : Set
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeExternalFileDef*
SALOMEDSImpl_AttributeExternalFileDef::Set (const DF_Label& L, const std::string& S) 
{

  SALOMEDSImpl_AttributeExternalFileDef* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeExternalFileDef*)L.FindAttribute(SALOMEDSImpl_AttributeExternalFileDef::GetID()))) {
    A = new  SALOMEDSImpl_AttributeExternalFileDef(); 
    L.AddAttribute(A);
  }
  
  A->SetValue (S); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeExternalFileDef::SALOMEDSImpl_AttributeExternalFileDef()
:SALOMEDSImpl_GenericAttribute("AttributeExternalFileDef")
{
}

//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeExternalFileDef::SetValue (const std::string& S)
{
  CheckLocked();

  if(myString == S) return;

  Backup();

  myString = S;

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}
           


//=======================================================================
//function : ID
//purpose  : 
//=======================================================================
const std::string& SALOMEDSImpl_AttributeExternalFileDef::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================
DF_Attribute* SALOMEDSImpl_AttributeExternalFileDef::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeExternalFileDef(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeExternalFileDef::Restore(DF_Attribute* with) 
{
  myString = dynamic_cast<SALOMEDSImpl_AttributeExternalFileDef*>(with)->Value ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeExternalFileDef::Paste (DF_Attribute* into)
{
  SALOMEDSImpl_AttributeExternalFileDef* anAttr = dynamic_cast<SALOMEDSImpl_AttributeExternalFileDef*>(into); 
  anAttr->SetValue(myString);
}

