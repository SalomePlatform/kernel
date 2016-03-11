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

//  File   : SALOMEDSImpl_AttributeString.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeString.hxx"

//=======================================================================
//function : GetID
//purpose  :
//=======================================================================
const std::string& SALOMEDSImpl_AttributeString::GetID ()
{
  static std::string CommentID ("1808A12F-AD0E-4a6a-B58A-395DCF469FE9");
  return CommentID;
}   

SALOMEDSImpl_AttributeString* SALOMEDSImpl_AttributeString::Set (const DF_Label& L, 
                                                                 const std::string& Val) 
{
  SALOMEDSImpl_AttributeString* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeString*)L.FindAttribute(SALOMEDSImpl_AttributeString::GetID()))) {
    A = new  SALOMEDSImpl_AttributeString(); 
    L.AddAttribute(A);
  }

  A->SetValue(Val);     
    
  return A;
}

//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeString::SetValue (const std::string& S)
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
const std::string& SALOMEDSImpl_AttributeString::ID () const { return GetID(); } 

//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================
DF_Attribute* SALOMEDSImpl_AttributeString::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeString(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeString::Restore(DF_Attribute* with) 
{
  myString = dynamic_cast<SALOMEDSImpl_AttributeString*>(with)->Value ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeString::Paste (DF_Attribute* into)
{
  dynamic_cast<SALOMEDSImpl_AttributeString*>(into)->SetValue(myString);
}
