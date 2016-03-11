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

//  File   : SALOMEDSImpl_AttributeComment.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeComment.hxx"

//=======================================================================
//function : GetID
//purpose  :
//=======================================================================
const std::string& SALOMEDSImpl_AttributeComment::GetID ()
{
  static std::string CommentID ("7AF2F7CC-1CA2-4476-BE95-8ACC996BC7B9");
  return CommentID;
}   

SALOMEDSImpl_AttributeComment* SALOMEDSImpl_AttributeComment::Set (const DF_Label& L, 
                                                                   const std::string& Val) 
{
  SALOMEDSImpl_AttributeComment* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeComment*)L.FindAttribute(SALOMEDSImpl_AttributeComment::GetID()))) {
    A = new  SALOMEDSImpl_AttributeComment(); 
    L.AddAttribute(A);
  }

  A->SetValue(Val);     
    
  return A;
}

//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeComment::SetValue (const std::string& S)
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
const std::string& SALOMEDSImpl_AttributeComment::ID () const { return GetID(); } 

//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================
DF_Attribute* SALOMEDSImpl_AttributeComment::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeComment(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeComment::Restore(DF_Attribute* with) 
{
  myString = dynamic_cast<SALOMEDSImpl_AttributeComment*>(with)->Value ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeComment::Paste (DF_Attribute* into)
{
  dynamic_cast<SALOMEDSImpl_AttributeComment*>(into)->SetValue(myString);
}
