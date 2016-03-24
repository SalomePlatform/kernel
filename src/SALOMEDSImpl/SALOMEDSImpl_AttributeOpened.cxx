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

//  File   : SALOMEDSImpl_AttributeOpened.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeOpened.hxx"

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeOpened::GetID () 
{
  static std::string SALOMEDSImpl_AttributeOpenedID ("12837186-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeOpenedID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

SALOMEDSImpl_AttributeOpened* SALOMEDSImpl_AttributeOpened::Set (const DF_Label& L,
                                                                 const int value) 
{
  SALOMEDSImpl_AttributeOpened* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeOpened*)L.FindAttribute(SALOMEDSImpl_AttributeOpened::GetID()))) {
    A = new  SALOMEDSImpl_AttributeOpened(); 
    L.AddAttribute(A);
  }
  
  A->SetOpened (value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeOpened::SALOMEDSImpl_AttributeOpened()
:SALOMEDSImpl_GenericAttribute("AttributeOpened")
{
  myValue = 0; 
}

//=======================================================================
//function : SetOpened
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeOpened::SetOpened(const int theValue)
{
  Backup();

  (theValue!=0)?myValue=1:myValue=0;
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeOpened::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

DF_Attribute* SALOMEDSImpl_AttributeOpened::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeOpened(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeOpened::Restore(DF_Attribute* with) 
{
  myValue = dynamic_cast<SALOMEDSImpl_AttributeOpened*>(with)->IsOpened ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeOpened::Paste (DF_Attribute* into)
{
  dynamic_cast<SALOMEDSImpl_AttributeOpened*>(into)->SetOpened (myValue);
}

