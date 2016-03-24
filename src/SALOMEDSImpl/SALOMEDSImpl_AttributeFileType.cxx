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

//  File   : SALOMEDSImpl_AttributeFileType.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeFileType.hxx"

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeFileType::GetID () 
{
  static std::string SALOMEDSImpl_AttributeFileTypeID ("0181B525-3F15-4ab2-9DE3-5E2F54B5F340");
  return SALOMEDSImpl_AttributeFileTypeID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

SALOMEDSImpl_AttributeFileType* SALOMEDSImpl_AttributeFileType::Set (const DF_Label& L,
                                                                     const std::string& S) 
{

  SALOMEDSImpl_AttributeFileType* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeFileType*)L.FindAttribute(SALOMEDSImpl_AttributeFileType::GetID()))) {
    A = new  SALOMEDSImpl_AttributeFileType(); 
    L.AddAttribute(A);
  }
  
  A->SetValue (S); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeFileType::SALOMEDSImpl_AttributeFileType()
:SALOMEDSImpl_GenericAttribute("AttributeFileType")
{
}

//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeFileType::SetValue (const std::string& S)
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

const std::string& SALOMEDSImpl_AttributeFileType::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

DF_Attribute* SALOMEDSImpl_AttributeFileType::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeFileType(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeFileType::Restore(DF_Attribute* with) 
{
  myString = dynamic_cast<SALOMEDSImpl_AttributeFileType*>(with)->Value ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeFileType::Paste (DF_Attribute* into)
{
  dynamic_cast<SALOMEDSImpl_AttributeFileType*>(into)->SetValue (myString);
}

