//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDSImpl_AttributeTarget.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeTarget.hxx"
#include "SALOMEDSImpl_AttributeReference.hxx"
#include "SALOMEDSImpl_Study.hxx"

using namespace std;


//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const std::string& SALOMEDSImpl_AttributeTarget::GetID () 
{
  static std::string SALOMEDSImpl_AttributeTargetID ("12837197-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeTargetID;
}


//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

SALOMEDSImpl_AttributeTarget* SALOMEDSImpl_AttributeTarget::Set (const DF_Label& L) 
{
  SALOMEDSImpl_AttributeTarget* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeTarget*)L.FindAttribute(SALOMEDSImpl_AttributeTarget::GetID()))) {
    A = new  SALOMEDSImpl_AttributeTarget(); 
    L.AddAttribute(A);
  }
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeTarget::SALOMEDSImpl_AttributeTarget()
:SALOMEDSImpl_GenericAttribute("AttributeTarget")
{
}

void SALOMEDSImpl_AttributeTarget::SetRelation(const std::string& theRelation)
{
  CheckLocked();
  if(myRelation == theRelation) return;

  Backup();
  myRelation = theRelation; 
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//function : Add
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeTarget::Add(const SALOMEDSImpl_SObject& theSO) 
{
  Backup();
  DF_Label aRefLabel = theSO.GetLabel();
  SALOMEDSImpl_AttributeReference* aReference;
  if ((aReference=(SALOMEDSImpl_AttributeReference*)aRefLabel.FindAttribute(SALOMEDSImpl_AttributeReference::GetID()))) {
    for(int i = 0, len = myVariables.size(); i<len; i++) if(myVariables[i]->Label() == aRefLabel) return; //BugID: PAL6192    
    myVariables.push_back(aReference);
  } 
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//function : Get
//purpose  : 
//=======================================================================
vector<SALOMEDSImpl_SObject> SALOMEDSImpl_AttributeTarget::Get() 
{
  vector<SALOMEDSImpl_SObject> aSeq;
  
  for(int i = 0, len = myVariables.size(); i<len; i++) 
    aSeq.push_back( SALOMEDSImpl_Study::SObject(myVariables[i]->Label()));
  
  return aSeq;
}

//=======================================================================
//function : Remove
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeTarget::Remove(const SALOMEDSImpl_SObject& theSO) 
{
  Backup();
  DF_Label aRefLabel = theSO.GetLabel();

  vector<DF_Attribute*> va;
  for(int i = 0, len = myVariables.size(); i<len; i++) {
    DF_Label L = myVariables[i]->Label();
    if(myVariables[i]->Label() == aRefLabel) continue;
    va.push_back(myVariables[i]);       
  }

  myVariables.clear();
  myVariables = va;    
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================
const std::string& SALOMEDSImpl_AttributeTarget::ID () const { return GetID(); }

//=======================================================================
//function : Restore
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeTarget::Restore(DF_Attribute* With)
{
  SALOMEDSImpl_AttributeTarget* REL = dynamic_cast<SALOMEDSImpl_AttributeTarget*>(With);
  myRelation = REL->GetRelation();
  myVariables.clear();
  for (int i = 0, len = REL->myVariables.size(); i<len; i++) {
    myVariables.push_back(REL->myVariables[i]);
  }
}

//=======================================================================
//function : NewEmpty
//purpose  :
//=======================================================================
DF_Attribute* SALOMEDSImpl_AttributeTarget::NewEmpty() const
{
  return new SALOMEDSImpl_AttributeTarget();
}

//=======================================================================
//function : Paste
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeTarget::Paste(DF_Attribute* into)
{
  SALOMEDSImpl_AttributeTarget* REL = dynamic_cast<SALOMEDSImpl_AttributeTarget*>(into);
  REL->SetRelation(myRelation);
  REL->myVariables.clear();
  for (int i = 0, len = myVariables.size(); i<len; i++) {
    REL->myVariables.push_back(myVariables[i]);
  }  
}   
