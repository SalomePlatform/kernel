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

//  File   : SALOMEDSImpl_AttributePixMap.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributePixMap.hxx"

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributePixMap::GetID () 
{
  static std::string SALOMEDSImpl_AttributePixMapID ("12837187-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributePixMapID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

SALOMEDSImpl_AttributePixMap* SALOMEDSImpl_AttributePixMap::Set (const DF_Label& L,
                                                                 const std::string& S) 
{
  SALOMEDSImpl_AttributePixMap* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributePixMap*)L.FindAttribute(SALOMEDSImpl_AttributePixMap::GetID()))) {
    A = new  SALOMEDSImpl_AttributePixMap(); 
    L.AddAttribute(A);
  }
  
  A->SetPixMap (S); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributePixMap::SALOMEDSImpl_AttributePixMap()
:SALOMEDSImpl_GenericAttribute("AttributePixMap")
{
  myString = "None";
}

//=======================================================================
//function : SetPixMap
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributePixMap::SetPixMap (const std::string& S)
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

const std::string& SALOMEDSImpl_AttributePixMap::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

DF_Attribute* SALOMEDSImpl_AttributePixMap::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributePixMap(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributePixMap::Restore(DF_Attribute* with) 
{
  myString = dynamic_cast<SALOMEDSImpl_AttributePixMap*>(with)->GetPixMap ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributePixMap::Paste (DF_Attribute* into)
{
  dynamic_cast<SALOMEDSImpl_AttributePixMap*>(into)->SetPixMap (myString);
}

