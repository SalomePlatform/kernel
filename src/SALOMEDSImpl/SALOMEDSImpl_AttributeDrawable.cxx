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

//  File   : SALOMEDSImpl_AttributeDrawable.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeDrawable.hxx"

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================
const std::string& SALOMEDSImpl_AttributeDrawable::GetID () 
{
  static std::string SALOMEDSImpl_AttributeDrawableID ("12837184-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeDrawableID;
}


//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

SALOMEDSImpl_AttributeDrawable* SALOMEDSImpl_AttributeDrawable::Set (const DF_Label& L,
                                                                     const int value) 
{
  SALOMEDSImpl_AttributeDrawable* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeDrawable*)L.FindAttribute(SALOMEDSImpl_AttributeDrawable::GetID()))) {
    A = new  SALOMEDSImpl_AttributeDrawable(); 
    L.AddAttribute(A);
  }
  
  A->SetDrawable (value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeDrawable::SALOMEDSImpl_AttributeDrawable()
:SALOMEDSImpl_GenericAttribute("AttributeDrawable") 
{
  myValue = 0;
}

//=======================================================================
//function : SetDrawable
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeDrawable::SetDrawable(const int theValue)
{
  CheckLocked();

  Backup();

  (theValue!=0)?myValue=1:myValue=0;
  
  SetModifyFlag();
}
           

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeDrawable::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

DF_Attribute* SALOMEDSImpl_AttributeDrawable::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeDrawable(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeDrawable::Restore(DF_Attribute* with) 
{
  myValue = dynamic_cast<SALOMEDSImpl_AttributeDrawable*>(with)->IsDrawable ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeDrawable::Paste (DF_Attribute* into)
{
  dynamic_cast<SALOMEDSImpl_AttributeDrawable*>(into)->SetDrawable (myValue);
}

