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

//  File   : SALOMEDSImpl_AttributeSequenceOfReal.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeSequenceOfReal.hxx"
#include <string.h>
#include <stdlib.h>

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeSequenceOfReal::GetID () 
{
  static std::string SALOMEDSImpl_AttributeSequenceOfRealID ("12837183-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeSequenceOfRealID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

SALOMEDSImpl_AttributeSequenceOfReal* SALOMEDSImpl_AttributeSequenceOfReal::Set (const DF_Label& L) 
{
  SALOMEDSImpl_AttributeSequenceOfReal* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeSequenceOfReal*)L.FindAttribute(SALOMEDSImpl_AttributeSequenceOfReal::GetID()))) {
    A = new  SALOMEDSImpl_AttributeSequenceOfReal(); 
    L.AddAttribute(A);
  }
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeSequenceOfReal::SALOMEDSImpl_AttributeSequenceOfReal()
:SALOMEDSImpl_GenericAttribute("AttributeSequenceOfReal")
{
  myValue.clear();
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeSequenceOfReal::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

DF_Attribute* SALOMEDSImpl_AttributeSequenceOfReal::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeSequenceOfReal(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeSequenceOfReal::Restore(DF_Attribute* with) 
{
  SALOMEDSImpl_AttributeSequenceOfReal* anSeq = dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(with);
  myValue.clear();
  for(int i = 0, len = anSeq->Length(); i<len; i++)
    myValue.push_back(anSeq->myValue[i]);    
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeSequenceOfReal::Paste (DF_Attribute* into)
{
    dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(into)->Assign(myValue);
}

void SALOMEDSImpl_AttributeSequenceOfReal::Assign(const std::vector<double>& other) 
{
  CheckLocked();  
  Backup();
  myValue = other;

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeSequenceOfReal::ChangeValue(const int Index,const double& Value) 
{
  CheckLocked();  
  Backup();

  if(Index <= 0 || Index > myValue.size()) throw DFexception("Out of range");

  myValue[Index-1] = Value;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeSequenceOfReal::Add(const double& Value) 
{
  CheckLocked();  
  Backup();
  myValue.push_back(Value);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeSequenceOfReal::Remove(const int Index) 
{
  CheckLocked();  
  Backup();

  if(Index <= 0 || Index > myValue.size()) throw DFexception("Out of range");

  typedef std::vector<double>::iterator VI;
  int i = 1;    
  for(VI p = myValue.begin(); p!=myValue.end(); p++, i++) {
    if(i == Index) {
      myValue.erase(p);
      break;
    }     
  }

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved   
}

int SALOMEDSImpl_AttributeSequenceOfReal::Length() 
{
  return myValue.size();
}

double SALOMEDSImpl_AttributeSequenceOfReal::Value(const int Index) 
{
  if(Index <= 0 || Index > myValue.size()) throw DFexception("Out of range");
  return myValue[Index-1];
}


std::string SALOMEDSImpl_AttributeSequenceOfReal::Save()
{
  int aLength = Length();
  char* aResult = new char[aLength * 127];
  aResult[0] = 0;
  int aPosition = 0;
  for (int i = 1; i <= aLength; i++) {
    sprintf(aResult + aPosition , "%.64e ", Value(i));
    aPosition += strlen(aResult + aPosition);
  }
  std::string ret(aResult);
  delete aResult;
                          
  return ret;
}
                            
void SALOMEDSImpl_AttributeSequenceOfReal::Load(const std::string& value)
{
                              
  char* aCopy = (char*)value.c_str();
  char* adr = strtok(aCopy, " ");
  char *err = NULL; 
  while (adr) {
    double r =  strtod(adr, &err); 
    Add(r);
    adr = strtok(NULL, " ");
  }
}    
