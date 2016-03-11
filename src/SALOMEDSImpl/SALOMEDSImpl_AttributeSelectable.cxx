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

//  File   : SALOMEDSImpl_AttributeSelectable.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeSelectable.hxx"

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeSelectable::GetID () 
{
  static std::string SALOMEDSImpl_AttributeSelectableID ("12837188-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeSelectableID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

SALOMEDSImpl_AttributeSelectable* SALOMEDSImpl_AttributeSelectable::Set (const DF_Label& L,
                                                                         const int value) 
{
  SALOMEDSImpl_AttributeSelectable* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeSelectable*)L.FindAttribute(SALOMEDSImpl_AttributeSelectable::GetID()))) {
    A = new  SALOMEDSImpl_AttributeSelectable(); 
    L.AddAttribute(A);
  }
  
  A->SetSelectable (value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeSelectable::SALOMEDSImpl_AttributeSelectable()
:SALOMEDSImpl_GenericAttribute("AttributeSelectable")
{
  myValue = 1;
}

//=======================================================================
//function : SetSelectable
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeSelectable::SetSelectable(const int theValue)
{
  Backup();

  (theValue!=0)?myValue=1:myValue=0;
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeSelectable::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

DF_Attribute* SALOMEDSImpl_AttributeSelectable::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeSelectable(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeSelectable::Restore(DF_Attribute* with) 
{
  myValue = dynamic_cast<SALOMEDSImpl_AttributeSelectable*>(with)->IsSelectable ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeSelectable::Paste (DF_Attribute* into)
{
  dynamic_cast<SALOMEDSImpl_AttributeSelectable*>(into)->SetSelectable (myValue);
}

