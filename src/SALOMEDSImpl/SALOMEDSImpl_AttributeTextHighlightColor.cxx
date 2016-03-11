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

//  File   : SALOMEDSImpl_AttributeTextHighlightColor.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeTextHighlightColor.hxx"

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeTextHighlightColor::GetID () 
{
  static std::string SALOMEDSImpl_AttributeTextHighlightColorID ("12837190-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeTextHighlightColorID;
}



//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeTextHighlightColor::SALOMEDSImpl_AttributeTextHighlightColor()
:SALOMEDSImpl_GenericAttribute("AttributeTextHighlightColor")
{ 
  myValue.clear();
  myValue.push_back(0.0);
  myValue.push_back(0.0);
  myValue.push_back(0.0);
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeTextHighlightColor::ID () const { return GetID(); }


//=======================================================================
//function : SetTextHighlightColor
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeTextHighlightColor::SetTextHighlightColor(const double& R, 
                                                                     const double& G, 
                                                                     const double& B)
{
   CheckLocked();
   Backup(); 

   myValue[0] = R;
   myValue[1] = G;
   myValue[2] = B;
   
   SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}
            
//=======================================================================
//function : TextHighlightColor
//purpose  :
//=======================================================================
std::vector<double> SALOMEDSImpl_AttributeTextHighlightColor::TextHighlightColor()
{
   return myValue;
}      

//=======================================================================
//function : ChangeArray
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeTextHighlightColor::ChangeArray(const std::vector<double>& newArray)
{
  Backup();

  for(int i = 0; i < 3; i++)
    myValue[i] = newArray[i];

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved  
}    

//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

DF_Attribute* SALOMEDSImpl_AttributeTextHighlightColor::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeTextHighlightColor(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeTextHighlightColor::Restore(DF_Attribute* with) 
{
  ChangeArray(dynamic_cast<SALOMEDSImpl_AttributeTextHighlightColor*>(with)->TextHighlightColor());;
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeTextHighlightColor::Paste (DF_Attribute* into)
{
  dynamic_cast<SALOMEDSImpl_AttributeTextHighlightColor*>(into)->ChangeArray (myValue);
}

std::string SALOMEDSImpl_AttributeTextHighlightColor::Save() 
{
  char *Val = new char[75];
  sprintf(Val, "%f %f %f", (float)myValue[0], 
                           (float)myValue[1], 
                           (float)myValue[2]);
  std::string ret(Val);
  delete Val;
  return ret;
}

void SALOMEDSImpl_AttributeTextHighlightColor::Load(const std::string& value) 
{
  float r, g, b;
  sscanf(value.c_str(), "%f %f %f", &r, &g, &b);
  myValue[0] = r;
  myValue[1] = g;
  myValue[2] = b;
}

