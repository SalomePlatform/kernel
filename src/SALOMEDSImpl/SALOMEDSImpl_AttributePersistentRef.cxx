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

//  File   : SALOMEDSImpl_AttributePersistentRef.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributePersistentRef.hxx"

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributePersistentRef::GetID () 
{
  static std::string SALOMEDSImpl_AttributePersistentRefID ("92888E06-7074-11d5-A690-0800369C8A03");
  return SALOMEDSImpl_AttributePersistentRefID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

SALOMEDSImpl_AttributePersistentRef* SALOMEDSImpl_AttributePersistentRef::Set (const DF_Label& L,
                                                                               const std::string& S)
{
  SALOMEDSImpl_AttributePersistentRef* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributePersistentRef*)L.FindAttribute(SALOMEDSImpl_AttributePersistentRef::GetID()))) {
    A = new  SALOMEDSImpl_AttributePersistentRef(); 
    L.AddAttribute(A);
  }
  
  A->SetValue (S); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributePersistentRef::SALOMEDSImpl_AttributePersistentRef()
:SALOMEDSImpl_GenericAttribute("AttributePersistentRef")
{
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributePersistentRef::ID () const { return GetID(); }


//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributePersistentRef::SetValue (const std::string& S)
{
  CheckLocked();

  if(myString == S) return;

  Backup();

  myString = S;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

DF_Attribute* SALOMEDSImpl_AttributePersistentRef::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributePersistentRef(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributePersistentRef::Restore(DF_Attribute* with) 
{
  myString = dynamic_cast<SALOMEDSImpl_AttributePersistentRef*>(with)->Value ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributePersistentRef::Paste (DF_Attribute* into)
{
  dynamic_cast<SALOMEDSImpl_AttributePersistentRef*>(into)->SetValue(myString);
}

