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

//  File   : SALOMEDSImpl_AttributeReal.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeReal.hxx"

#include <stdlib.h>


//=======================================================================
//function : GetID
//purpose  :
//=======================================================================
const std::string& SALOMEDSImpl_AttributeReal::GetID ()
{
  static std::string realID ("1D1992F0-56F4-46b4-8065-CDEA68061CAB");
  return realID;
}   

SALOMEDSImpl_AttributeReal* SALOMEDSImpl_AttributeReal::Set (const DF_Label& L, const double& Val) 
{
  SALOMEDSImpl_AttributeReal* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeReal*)L.FindAttribute(SALOMEDSImpl_AttributeReal::GetID()))) {
    A = new  SALOMEDSImpl_AttributeReal(); 
    L.AddAttribute(A);
  }

  A->SetValue(Val); 
  return A;
}

//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeReal::SetValue(const double& v)
{
  CheckLocked();

  if( myValue == v) return;

  Backup();
  myValue = v;  

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//function : ID
//purpose  :
//=======================================================================
const std::string& SALOMEDSImpl_AttributeReal::ID () const 
{ 
  return GetID(); 
} 

//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================
DF_Attribute* SALOMEDSImpl_AttributeReal::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeReal(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeReal::Restore(DF_Attribute* with) 
{
  myValue = dynamic_cast<SALOMEDSImpl_AttributeReal*>(with)->Value ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeReal::Paste (DF_Attribute* into)
{
  dynamic_cast<SALOMEDSImpl_AttributeReal*>(into)->SetValue(myValue);
}

//=======================================================================
//function : Save
//purpose  :
//=======================================================================
std::string SALOMEDSImpl_AttributeReal::Save() 
{ 
  char buffer[255]; 
  sprintf(buffer, "%.64e", myValue);
  return std::string(buffer); 
}

//=======================================================================
//function : Load
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeReal::Load(const std::string& theValue)
{
  myValue = atof(theValue.c_str());  
}
