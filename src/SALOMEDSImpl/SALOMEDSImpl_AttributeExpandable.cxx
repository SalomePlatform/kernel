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

//  File   : SALOMEDSImpl_AttributeExpandable.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeExpandable.hxx"

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================
const std::string& SALOMEDSImpl_AttributeExpandable::GetID () 
{
  static std::string SALOMEDSImpl_AttributeExpandableID ("12837185-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeExpandableID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeExpandable* SALOMEDSImpl_AttributeExpandable::Set (const DF_Label& L,
                                                                        const int value) 
{
  SALOMEDSImpl_AttributeExpandable* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeExpandable*)L.FindAttribute(SALOMEDSImpl_AttributeExpandable::GetID()))) {
    A = new  SALOMEDSImpl_AttributeExpandable(); 
    L.AddAttribute(A);
  }
  
  A->SetExpandable(value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeExpandable::SALOMEDSImpl_AttributeExpandable()
:SALOMEDSImpl_GenericAttribute("AttributeExpandable")
{
  myValue = 1;
}

//=======================================================================
//function : SetExpandable
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeExpandable::SetExpandable(const int theValue)
{
  CheckLocked();

  Backup();

  (theValue!=0)?myValue=1:myValue=0;

  SetModifyFlag(); //VSR: Mark the study as being modified, so it could be saved
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeExpandable::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

DF_Attribute* SALOMEDSImpl_AttributeExpandable::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeExpandable(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeExpandable::Restore(DF_Attribute* with) 
{
  myValue = dynamic_cast<SALOMEDSImpl_AttributeExpandable*>(with)->IsExpandable ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeExpandable::Paste (DF_Attribute* into)
{
  dynamic_cast<SALOMEDSImpl_AttributeExpandable*>(into)->SetExpandable (myValue);
}

