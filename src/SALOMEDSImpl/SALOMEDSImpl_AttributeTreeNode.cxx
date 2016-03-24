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

//  File   : SALOMEDSImpl_AttributeTreeNode.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeTreeNode.hxx"
#include <string.h>

const std::string&  SALOMEDSImpl_AttributeTreeNode::GetDefaultTreeID()
{
  static std::string TreeNodeID ("0E1C36E6-379B-4d90-AC37-17A14310E648");
  return TreeNodeID;
}    


SALOMEDSImpl_AttributeTreeNode::SALOMEDSImpl_AttributeTreeNode() 
:SALOMEDSImpl_GenericAttribute("AttributeTreeNode"), myFather(NULL),  myPrevious(NULL), myNext(NULL), myFirst(NULL) 
{}


SALOMEDSImpl_AttributeTreeNode* SALOMEDSImpl_AttributeTreeNode::Set (const DF_Label& L, const std::string& ID) 
{
  SALOMEDSImpl_AttributeTreeNode* TN = NULL;

  if (!(TN=(SALOMEDSImpl_AttributeTreeNode*)L.FindAttribute(ID))) {
    TN = new SALOMEDSImpl_AttributeTreeNode ();
    TN->SetTreeID(ID);
    L.AddAttribute(TN);
  }

  return TN;    
}

//=======================================================================
//TreeNode : ID
//purpose  : Returns GUID of the TreeNode
//=======================================================================
const std::string& SALOMEDSImpl_AttributeTreeNode::ID() const
{
  return myTreeID;
}  

//=======================================================================
//function : Append
//purpose  : Add <TN> as last child of me
//=======================================================================
bool SALOMEDSImpl_AttributeTreeNode::Append (SALOMEDSImpl_AttributeTreeNode* TN)
{
  CheckLocked();

  if (!(TN->ID() == myTreeID)) throw DFexception("SALOMEDSImpl_AttributeTreeNode::Append : uncompatible GUID");

  if(TN->Label() == Label()) throw DFexception("Attempt of self linking");

  TN->SetNext(NULL); // Deconnects from next.

  // Find the last
  if (!HasFirst()) {
    SetFirst(TN);
    TN->SetPrevious(NULL); // Deconnects from previous.
  }
  else {
    SALOMEDSImpl_AttributeTreeNode* Last = GetFirst();
    while (Last && Last->HasNext()) {
      Last = Last->GetNext();
    }
    Last->SetNext(TN);
    TN->SetPrevious(Last);
  }
  // Set Father
  TN->SetFather(this);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
  
  return (TN);
}

//=======================================================================
//function : Prepend
//purpose  : Add <TN> as first child of me
//=======================================================================
bool SALOMEDSImpl_AttributeTreeNode::Prepend (SALOMEDSImpl_AttributeTreeNode* TN)
{
  CheckLocked();

  if (!(TN->ID() == myTreeID) ) throw DFexception("SALOMEDSImpl_AttributeTreeNode::Prepend : uncompatible GUID");

  if(TN->Label() == Label()) throw DFexception("Attempt of self linking");

  TN->SetPrevious(NULL);
  if (HasFirst()) {
    TN->SetNext(GetFirst());
    GetFirst()->SetPrevious(TN);
  }
  else {
    TN->SetNext(NULL);
  }
  TN->SetFather(this);
  SetFirst(TN);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
  
  return (TN);
}                     


//=======================================================================
//function : InsertBefore
//purpose  : Inserts the TreeNode  <TN> before me
//=======================================================================
bool SALOMEDSImpl_AttributeTreeNode::InsertBefore (SALOMEDSImpl_AttributeTreeNode* TN)
{
  CheckLocked();

  if (!(TN->ID() == myTreeID) ) throw DFexception("SALOMEDSImpl_AttributeTreeNode::InsertBefore : uncompatible GUID");

  if(TN->Label() == Label()) throw DFexception("Attempt of self linking");

  TN->SetFather(GetFather());
  TN->SetPrevious(GetPrevious());
  TN->SetNext(this);

  if (!HasPrevious())
    GetFather()->SetFirst(TN);
  else
    GetPrevious()->SetNext(TN);

  SetPrevious(TN);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
  
  return (TN);
}

//=======================================================================
//function : InsertAfter
//purpose  : Inserts the TreeNode  <TN> after me
//=======================================================================
bool SALOMEDSImpl_AttributeTreeNode::InsertAfter (SALOMEDSImpl_AttributeTreeNode* TN)
{
  CheckLocked();

  if(TN->Label() == Label()) throw DFexception("Attempt of self linking");

  if (!(TN->ID() == myTreeID) ) throw DFexception("SALOMEDSImpl_AttributeTreeNode::InsertAfter : uncompatible GUID");

  TN->SetFather(GetFather());
  TN->SetPrevious(this);
  TN->SetNext(GetNext());

  if (HasNext()) GetNext()->SetPrevious(TN);

  SetNext(TN);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
  
  return (TN);
}         

//=======================================================================
//function : Remove
//purpose  : Removees the function from the function tree
//=======================================================================
bool SALOMEDSImpl_AttributeTreeNode::Remove ()
{
  CheckLocked();

  if (IsRoot()) return true;

  if (!HasPrevious())
    GetFather()->SetFirst(GetNext());
  else
    GetPrevious()->SetNext(GetNext());

  if (HasNext()) {
    if (HasPrevious()) GetNext()->SetPrevious(GetPrevious());
    else GetNext()->SetPrevious(NULL);
  }
  else {
    if (HasPrevious()) GetPrevious()->SetNext(NULL);
  }

  if (GetFather()->HasFirst()) {
    if (this == GetFather()->GetFirst()) {
      if (HasNext()) {
        GetFather()->SetFirst(GetNext());
      }
      else GetFather()->SetFirst(NULL);
    }
  }

  SetFather(NULL);
  SetNext(NULL);
  SetPrevious(NULL);

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 

  return true;
}         

//=======================================================================
//function : Depth
//purpose  :
//=======================================================================
int SALOMEDSImpl_AttributeTreeNode::Depth () const
{
  int depth = 0;
  SALOMEDSImpl_AttributeTreeNode* current = (SALOMEDSImpl_AttributeTreeNode*)this;
  while (current) {
    depth++;
    current = current->GetFather();
  }
  return depth;
}

//=======================================================================
//function : SetTreeID
//purpose  : Finds or creates a TreeNode  attribute with explicit ID
//         : a driver for it
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::SetTreeID (const std::string& explicitID)
{
  myTreeID = explicitID;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}


//=======================================================================
//function : IsAscendant
//purpose  :
//=======================================================================
bool SALOMEDSImpl_AttributeTreeNode::IsAscendant (const SALOMEDSImpl_AttributeTreeNode* ofTN) const
{
  return ofTN->IsDescendant(this);
}                

//=======================================================================
//function : IsDescendant
//purpose  :
//=======================================================================

bool SALOMEDSImpl_AttributeTreeNode::IsDescendant (const SALOMEDSImpl_AttributeTreeNode* ofTN) const
{
  SALOMEDSImpl_AttributeTreeNode* current = (SALOMEDSImpl_AttributeTreeNode*)this;
  while (current) {
    if (current->GetFather() == ofTN) return true;
    current = current->GetFather();
  }
  return false;
}

//=======================================================================
//function : IsFather
//purpose  :
//=======================================================================

bool SALOMEDSImpl_AttributeTreeNode::IsFather (const SALOMEDSImpl_AttributeTreeNode* ofTN) const
{
  return (ofTN->GetFather() == this);
}


//=======================================================================
//function : IsChild
//purpose  :
//=======================================================================

bool SALOMEDSImpl_AttributeTreeNode::IsChild (const SALOMEDSImpl_AttributeTreeNode* ofTN) const
{
  return (myFather == ofTN);
}

//=======================================================================
//TreeNode : IsRoot
//purpose  : Returns Standard_True if the TreeNode is not attached to a
//           TreeNode tree or hasn't an Father.
//=======================================================================
bool SALOMEDSImpl_AttributeTreeNode::IsRoot() const
{
  if (!myFather && !myPrevious && !myNext)
    return true;
  return false;
}

//=======================================================================
//TreeNode : Root
//purpose  : Returns the TreeNode which has no Father
//=======================================================================
SALOMEDSImpl_AttributeTreeNode* SALOMEDSImpl_AttributeTreeNode::Root() const
{
  SALOMEDSImpl_AttributeTreeNode* O = (SALOMEDSImpl_AttributeTreeNode*)this;
  while (O && O->HasFather())
    O = O->GetFather();
  return O;
}       

//=======================================================================
//TreeNode : SetFather
//purpose  : Sets the TreeNode F as Father of me
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::SetFather(const SALOMEDSImpl_AttributeTreeNode* F)
{
  CheckLocked();
  Backup();
  myFather = (SALOMEDSImpl_AttributeTreeNode*)F;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//TreeNode : SetNext
//purpose  : Sets the TreeNode F next to me
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::SetNext(const SALOMEDSImpl_AttributeTreeNode* F)
{
  CheckLocked();
  Backup();
  myNext = (SALOMEDSImpl_AttributeTreeNode*)F;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}


//=======================================================================
//TreeNode : SetPrevious
//purpose  : Sets the TreeNode F previous to me
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::SetPrevious(const SALOMEDSImpl_AttributeTreeNode* F)
{
  CheckLocked();
  Backup();
  myPrevious = (SALOMEDSImpl_AttributeTreeNode*)F;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//TreeNode : SetFirst
//purpose  : Sets the TreeNode F as first in the TreeNode tree
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::SetFirst(const SALOMEDSImpl_AttributeTreeNode* F)
{
  CheckLocked();
  Backup();
  myFirst = (SALOMEDSImpl_AttributeTreeNode*)F;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}         

//=======================================================================
//TreeNode : AfterAddition
//purpose  : Connects the TreeNode to the tree.
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::AfterAddition() 
{
  if (myPrevious) {
    myPrevious->SetNext(this);
  }
  else if (myFather) {
    myFather->SetFirst(this);
  }
  if (myNext) {
    myNext->SetPrevious(this);
  }
}

//=======================================================================
//TreeNode : BeforeForget
//purpose  : Disconnect the TreeNode from the tree.
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::BeforeForget() 
{
    Remove();
    while (HasFirst()) GetFirst()->Remove();
}

//=======================================================================
//TreeNode : Restore
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::Restore(DF_Attribute* other) 
{
  SALOMEDSImpl_AttributeTreeNode* F =  dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(other);
  myFather     = F->myFather;
  myPrevious   = F->myPrevious;
  myNext       = F->myNext;
  myFirst      = F->myFirst;
  myTreeID     = F->myTreeID;
}       

//=======================================================================
//TreeNode : Paste
//purpose  : Method for Copy mechanism
//=======================================================================

void SALOMEDSImpl_AttributeTreeNode::Paste(DF_Attribute* into)
{
  SALOMEDSImpl_AttributeTreeNode* intof = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(into);
  intof->SetFather(myFather);
  intof->SetNext(myNext);
  intof->SetPrevious(myPrevious);
  intof->SetFirst(myFirst);
  intof->SetTreeID(myTreeID);
}

//=======================================================================
//TreeNode : NewEmpty
//purpose  : Returns new empty TreeNode attribute
//=======================================================================

DF_Attribute* SALOMEDSImpl_AttributeTreeNode::NewEmpty() const
{
  SALOMEDSImpl_AttributeTreeNode* T = new SALOMEDSImpl_AttributeTreeNode();
  T->SetTreeID(myTreeID);
  return T;
}

std::string SALOMEDSImpl_AttributeTreeNode::Type()
{
   char* aNodeName = new char[127];
   sprintf(aNodeName, "AttributeTreeNodeGUID%s", ID().c_str());
   std::string ret(aNodeName); 
   delete [] aNodeName;
   
   return ret;                               
}

std::string SALOMEDSImpl_AttributeTreeNode::Save() 
{
  std::string aFather, aPrevious, aNext, aFirst;

  if (HasFather()) aFather = GetFather()->Label().Entry(); else aFather = "!";
  if (HasPrevious()) aPrevious = GetPrevious()->Label().Entry(); else aPrevious = "!";
  if (HasNext()) aNext = GetNext()->Label().Entry(); else aNext = "!";
  if (HasFirst()) aFirst = GetFirst()->Label().Entry(); else aFirst = "!";

  int aLength = 4;
  aLength += aFather.size() + aPrevious.size() + aNext.size() + aFirst.size();
  char* aResult = new char[aLength];
  sprintf(aResult, "%s %s %s %s", aFather.c_str(), aPrevious.c_str(), aNext.c_str(), aFirst.c_str());
  std::string ret(aResult);
  delete [] aResult;
  return ret;
}

void SALOMEDSImpl_AttributeTreeNode::Load(const std::string& value) 
{
  char* aCopy = (char*)value.c_str();
  char* adr = strtok(aCopy, " ");
  
  DF_Label aLabel;
  SALOMEDSImpl_AttributeTreeNode* aDepNode = NULL;

  if (adr && adr[0] != '!') {
    aLabel = DF_Label::Label(Label(), adr, true);
    if (!(aDepNode=(SALOMEDSImpl_AttributeTreeNode*)aLabel.FindAttribute(ID()))) 
      aDepNode =  SALOMEDSImpl_AttributeTreeNode::Set(aLabel, ID());

    SetFather(aDepNode);
  }

  adr = strtok(NULL, " ");
  if (adr && adr[0] != '!') {
    aLabel = DF_Label::Label(Label(), adr, true);
    if (!(aDepNode=(SALOMEDSImpl_AttributeTreeNode*)aLabel.FindAttribute(ID()))) 
      aDepNode = SALOMEDSImpl_AttributeTreeNode::Set(aLabel, ID());
    SetPrevious(aDepNode);
  }

  adr = strtok(NULL, " ");
  if (adr && adr[0] != '!') {
    aLabel = DF_Label::Label(Label(), adr, true);
    if (!(aDepNode=(SALOMEDSImpl_AttributeTreeNode*)aLabel.FindAttribute(ID()))) 
      aDepNode = SALOMEDSImpl_AttributeTreeNode::Set(aLabel, ID());
    SetNext(aDepNode);
  }

  adr = strtok(NULL, " ");
  if (adr && adr[0] != '!') {
    aLabel = DF_Label::Label(Label(), adr, true);
    if (!(aDepNode=(SALOMEDSImpl_AttributeTreeNode*)aLabel.FindAttribute(ID()))) 
      aDepNode = SALOMEDSImpl_AttributeTreeNode::Set(aLabel, ID());
    SetFirst(aDepNode);
  }
}


