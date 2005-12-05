// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
//  File   : SALOMEDSImpl_AttributeTreeNode.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDSImpl_AttributeTreeNode.hxx"
#include <Standard_DomainError.hxx>
#include <TDF_Tool.hxx>
#include <TDF_Data.hxx>
#include <TDF_DataSet.hxx>
#include <TDF_RelocationTable.hxx>
#include <TCollection_AsciiString.hxx> 

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeTreeNode, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeTreeNode, SALOMEDSImpl_GenericAttribute )

static char* Entry(const TDF_Label& theLabel) 
{
  TCollection_AsciiString anEntry;
  TDF_Tool::Entry(theLabel, anEntry);
  return anEntry.ToCString();
}  

const Standard_GUID&  SALOMEDSImpl_AttributeTreeNode::GetDefaultTreeID()
{
  static Standard_GUID TreeNodeID ("0E1C36E6-379B-4d90-AC37-17A14310E648");
  return TreeNodeID;
}    


SALOMEDSImpl_AttributeTreeNode::SALOMEDSImpl_AttributeTreeNode() 
:SALOMEDSImpl_GenericAttribute("AttributeTreeNode"), myFather(NULL),  myPrevious(NULL), myNext(NULL), myFirst(NULL) 
{}


Handle(SALOMEDSImpl_AttributeTreeNode) SALOMEDSImpl_AttributeTreeNode::Set (const TDF_Label& L, const Standard_GUID& ID) 
{
  Handle(SALOMEDSImpl_AttributeTreeNode) TN;

  if (!L.FindAttribute(ID,TN)) {
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
const Standard_GUID& SALOMEDSImpl_AttributeTreeNode::ID() const
{
  return myTreeID;
}  

//=======================================================================
//function : Append
//purpose  : Add <TN> as last child of me
//=======================================================================
Standard_Boolean SALOMEDSImpl_AttributeTreeNode::Append (const Handle(SALOMEDSImpl_AttributeTreeNode)& TN)
{
  CheckLocked();

  if (!(TN->ID() == myTreeID) )
    Standard_DomainError::Raise("SALOMEDSImpl_AttributeTreeNode::Append : uncompatible GUID");

  Handle(SALOMEDSImpl_AttributeTreeNode) bid;
  TN->SetNext(bid); // Deconnects from next.

  // Find the last
  if (!HasFirst()) {
    SetFirst(TN);
    TN->SetPrevious(bid); // Deconnects from previous.
  }
  else {
    Handle(SALOMEDSImpl_AttributeTreeNode) Last = GetFirst();
    while (Last->HasNext()) {
      Last = Last->GetNext();
    }
    Last->SetNext(TN);
    TN->SetPrevious(Last);
  }
  // Set Father
  TN->SetFather(this);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
  
  return !TN.IsNull();
}

//=======================================================================
//function : Prepend
//purpose  : Add <TN> as first child of me
//=======================================================================
Standard_Boolean SALOMEDSImpl_AttributeTreeNode::Prepend (const Handle(SALOMEDSImpl_AttributeTreeNode)& TN)
{
  CheckLocked();

  if (!(TN->ID() == myTreeID) )
    Standard_DomainError::Raise("SALOMEDSImpl_AttributeTreeNode::Prepend : uncompatible GUID");

  Handle(SALOMEDSImpl_AttributeTreeNode) bid;
  TN->SetPrevious(bid);
  if (HasFirst()) {
    TN->SetNext(GetFirst());
    GetFirst()->SetPrevious(TN);
  }
  else {
    TN->SetNext(bid);
  }
  TN->SetFather(this);
  SetFirst(TN);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
  
  return !TN.IsNull();
}                     


//=======================================================================
//function : InsertBefore
//purpose  : Inserts the TreeNode  <TN> before me
//=======================================================================
Standard_Boolean SALOMEDSImpl_AttributeTreeNode::InsertBefore (const Handle(SALOMEDSImpl_AttributeTreeNode)& TN)
{
  CheckLocked();

  if (!(TN->ID() == myTreeID) )
    Standard_DomainError::Raise("SALOMEDSImpl_AttributeTreeNode::InsertBefore : uncompatible GUID");

  TN->SetFather(GetFather());
  TN->SetPrevious(GetPrevious());
  TN->SetNext(this);

  if (!HasPrevious())
    GetFather()->SetFirst(TN);
  else
    GetPrevious()->SetNext(TN);

  SetPrevious(TN);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
  
  return !TN.IsNull();
}

//=======================================================================
//function : InsertAfter
//purpose  : Inserts the TreeNode  <TN> after me
//=======================================================================
Standard_Boolean SALOMEDSImpl_AttributeTreeNode::InsertAfter (const Handle(SALOMEDSImpl_AttributeTreeNode)& TN)
{
  CheckLocked();

  if (!(TN->ID() == myTreeID) )
    Standard_DomainError::Raise("SALOMEDSImpl_AttributeTreeNode::InsertAfter : uncompatible GUID");

  TN->SetFather(GetFather());
  TN->SetPrevious(this);
  TN->SetNext(GetNext());

  if (HasNext()) GetNext()->SetPrevious(TN);

  SetNext(TN);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
  
  return !TN.IsNull();
}         

//=======================================================================
//function : Remove
//purpose  : Removees the function from the function tree
//=======================================================================
Standard_Boolean SALOMEDSImpl_AttributeTreeNode::Remove ()
{
  CheckLocked();

  if (IsRoot()) return Standard_True;

  Handle(SALOMEDSImpl_AttributeTreeNode) bid;
  if (!HasPrevious())
    GetFather()->SetFirst(GetNext());
  else
    GetPrevious()->SetNext(GetNext());

  if (HasNext()) {
    if (HasPrevious()) GetNext()->SetPrevious(GetPrevious());
    else GetNext()->SetPrevious(bid);
  }
  else {
    if (HasPrevious()) GetPrevious()->SetNext(bid);
  }

  if (GetFather()->HasFirst()) {
    if (Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(this) == GetFather()->GetFirst()) {
      if (HasNext()) {
        GetFather()->SetFirst(GetNext());
      }
      else GetFather()->SetFirst(bid);
    }
  }

  SetFather(bid);
  SetNext(bid);
  SetPrevious(bid);

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 

  return Standard_True;
}         

//=======================================================================
//function : Depth
//purpose  :
//=======================================================================
Standard_Integer SALOMEDSImpl_AttributeTreeNode::Depth () const
{
  Standard_Integer depth = 0;
  Handle(SALOMEDSImpl_AttributeTreeNode) current = this;
  while (current->HasFather()) {
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
void SALOMEDSImpl_AttributeTreeNode::SetTreeID (const Standard_GUID& explicitID)
{
  myTreeID = explicitID;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}


//=======================================================================
//function : IsAscendant
//purpose  :
//=======================================================================
Standard_Boolean SALOMEDSImpl_AttributeTreeNode::IsAscendant (const Handle(SALOMEDSImpl_AttributeTreeNode)& ofTN) const
{
  return ofTN->IsDescendant(this);
}                

//=======================================================================
//function : IsDescendant
//purpose  :
//=======================================================================

Standard_Boolean SALOMEDSImpl_AttributeTreeNode::IsDescendant (const Handle(SALOMEDSImpl_AttributeTreeNode)& ofTN) const
{
  Handle(SALOMEDSImpl_AttributeTreeNode) current = this;
  while (current->HasFather()) {
    if (current->GetFather() == ofTN) return Standard_True;
    current = current->GetFather();
  }
  return Standard_False;
}

//=======================================================================
//function : IsFather
//purpose  :
//=======================================================================

Standard_Boolean SALOMEDSImpl_AttributeTreeNode::IsFather (const Handle(SALOMEDSImpl_AttributeTreeNode)& ofTN) const
{
  return (ofTN->GetFather() == this);
}


//=======================================================================
//function : IsChild
//purpose  :
//=======================================================================

Standard_Boolean SALOMEDSImpl_AttributeTreeNode::IsChild (const Handle(SALOMEDSImpl_AttributeTreeNode)& ofTN) const
{
  return (myFather == ofTN);
}

//=======================================================================
//TreeNode : IsRoot
//purpose  : Returns Standard_True if the TreeNode is not attached to a
//           TreeNode tree or hasn't an Father.
//=======================================================================
Standard_Boolean SALOMEDSImpl_AttributeTreeNode::IsRoot() const
{
  if (myFather.IsNull() &&
      myPrevious.IsNull() &&
      myNext.IsNull())
    return Standard_True;
  return Standard_False;
}

//=======================================================================
//TreeNode : Root
//purpose  : Returns the TreeNode which has no Father
//=======================================================================
Handle(SALOMEDSImpl_AttributeTreeNode) SALOMEDSImpl_AttributeTreeNode::Root() const
{
  Handle(SALOMEDSImpl_AttributeTreeNode) O = this;
  while (O->HasFather())
    O = O->GetFather();
  return O;
}       

//=======================================================================
//TreeNode : SetFather
//purpose  : Sets the TreeNode F as Father of me
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::SetFather(const Handle(SALOMEDSImpl_AttributeTreeNode)& F)
{
  CheckLocked();
  Backup();
  myFather = F;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//TreeNode : SetNext
//purpose  : Sets the TreeNode F next to me
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::SetNext(const Handle(SALOMEDSImpl_AttributeTreeNode)& F)
{
  CheckLocked();
  Backup();
  myNext = F;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}


//=======================================================================
//TreeNode : SetPrevious
//purpose  : Sets the TreeNode F previous to me
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::SetPrevious(const Handle(SALOMEDSImpl_AttributeTreeNode)& F)
{
  CheckLocked();
  Backup();
  myPrevious = F;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//TreeNode : SetFirst
//purpose  : Sets the TreeNode F as first in the TreeNode tree
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::SetFirst(const Handle(SALOMEDSImpl_AttributeTreeNode)& F)
{
  CheckLocked();
  Backup();
  myFirst = F;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}         

//=======================================================================
//TreeNode : AfterAddition
//purpose  : Connects the TreeNode to the tree.
//           Backuped attribute must stay disconnected
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::AfterAddition() 
{
  if (!IsBackuped()) {
    if (!myPrevious.IsNull()) {
      myPrevious->SetNext(this);
    }
    else if (!myFather.IsNull()) {
      myFather->SetFirst(this);
    }
    if (!myNext.IsNull())
      myNext->SetPrevious(this);
  }
}

//=======================================================================
//TreeNode : BeforeForget
//purpose  : Disconnect the TreeNode from the tree.
//           Backuped attribute is normaly not concerned by such an operation
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::BeforeForget() 
{
  if (!IsBackuped()) {
    Remove();
    while (HasFirst()) GetFirst()->Remove();
  }
}

//=======================================================================
//TreeNode : AfterResume
//purpose  : Connects the TreeNode to the tree
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::AfterResume() 
{
  AfterAddition();
}

//=======================================================================
//TreeNode : BeforeUndo
//purpose  : Disconnect the TreeNode from the tree.
//=======================================================================
Standard_Boolean SALOMEDSImpl_AttributeTreeNode::BeforeUndo(const Handle(TDF_AttributeDelta)& anAttDelta,
							    const Standard_Boolean forceIt)
{
  if (anAttDelta->IsKind(STANDARD_TYPE(TDF_DeltaOnAddition))) BeforeForget(); // Disconnect.
  return Standard_True;
}           

//=======================================================================
//TreeNode : AfterUndo
//purpose  : Connect the TreeNode from the tree.
//=======================================================================
Standard_Boolean SALOMEDSImpl_AttributeTreeNode::AfterUndo(const Handle(TDF_AttributeDelta)& anAttDelta,
							   const Standard_Boolean forceIt)
{
  if (anAttDelta->IsKind(STANDARD_TYPE(TDF_DeltaOnRemoval))) AfterAddition(); // Reconnect.
  return Standard_True;
}

//=======================================================================
//TreeNode : Restore
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::Restore(const Handle(TDF_Attribute)& other) 
{
  Handle(SALOMEDSImpl_AttributeTreeNode) F =  Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(other);
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

void SALOMEDSImpl_AttributeTreeNode::Paste(const Handle(TDF_Attribute)& into,
					   const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeTreeNode) intof = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(into);
  Handle(SALOMEDSImpl_AttributeTreeNode) func;
  if (!RT->HasRelocation(myFather, func) && RT->AfterRelocate()) {
    func.Nullify();
  }
  intof->SetFather(func);
  if (!RT->HasRelocation(myNext, func) && RT->AfterRelocate()) {
    func.Nullify();
  }
  intof->SetNext(func);
  if (!RT->HasRelocation(myPrevious, func) && RT->AfterRelocate()) {
    func.Nullify();
  }
  intof->SetPrevious(func);
  if (!RT->HasRelocation(myFirst, func) && RT->AfterRelocate()) {
    func.Nullify();
  }

  intof->SetFirst(func);
  intof->SetTreeID(myTreeID);
}

//=======================================================================
//TreeNode : NewEmpty
//purpose  : Returns new empty TreeNode attribute
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeTreeNode::NewEmpty() const
{
  Handle(SALOMEDSImpl_AttributeTreeNode) T = new SALOMEDSImpl_AttributeTreeNode();
  T->SetTreeID(myTreeID);
  return T;
}

//=======================================================================
//TreeNode : References
//purpose  : Collects the references
//=======================================================================
void SALOMEDSImpl_AttributeTreeNode::References(const Handle(TDF_DataSet)& aDataSet) const
{
  Handle(SALOMEDSImpl_AttributeTreeNode) fct = myFirst;
  while (!fct.IsNull()) {
    aDataSet->AddAttribute(fct);
    fct = fct->myNext;
  }
}          

TCollection_AsciiString SALOMEDSImpl_AttributeTreeNode::Type()
{
   char* aNodeName = new char[60];
   char aGUID[40];
   ID().ToCString(aGUID);
   sprintf(aNodeName, "AttributeTreeNodeGUID%s",aGUID);
   TCollection_AsciiString ret(aNodeName); 
   delete aNodeName;
   
   return ret;                               
}

TCollection_AsciiString SALOMEDSImpl_AttributeTreeNode::Save() 
{
  TCollection_AsciiString aFather, aPrevious, aNext, aFirst;

  if (HasFather()) aFather = Entry(GetFather()->Label()); else aFather = "!";
  if (HasPrevious()) aPrevious = Entry(GetPrevious()->Label()); else aPrevious = "!";
  if (HasNext()) aNext = Entry(GetNext()->Label()); else aNext = "!";
  if (HasFirst()) aFirst = Entry(GetFirst()->Label()); else aFirst = "!";

  int aLength = 4;
  aLength += aFather.Length() + aPrevious.Length() + aNext.Length() + aFirst.Length();
  char* aResult = new char[aLength];
  sprintf(aResult, "%s %s %s %s", aFather.ToCString(), aPrevious.ToCString(), aNext.ToCString(), aFirst.ToCString());
  TCollection_AsciiString ret(aResult);
  delete aResult;
  return ret;
}

void SALOMEDSImpl_AttributeTreeNode::Load(const TCollection_AsciiString& value) 
{
  Handle(TDF_Data) DF = Label().Data();
  
  char* aCopy = (char*)value.ToCString();
  char* adr = strtok(aCopy, " ");
  
  TDF_Label aLabel;
  Handle(SALOMEDSImpl_AttributeTreeNode) aDepNode;

  if (adr && adr[0] != '!') {
    TDF_Tool::Label(DF, adr, aLabel, 1);
    if (!aLabel.FindAttribute(ID(), aDepNode)) 
      aDepNode =  SALOMEDSImpl_AttributeTreeNode::Set(aLabel, ID());

    SetFather(aDepNode);
  }

  adr = strtok(NULL, " ");
  if (adr && adr[0] != '!') {
    TDF_Tool::Label(DF, adr, aLabel, 1);
    if (!aLabel.FindAttribute(ID(), aDepNode)) 
      aDepNode = SALOMEDSImpl_AttributeTreeNode::Set(aLabel, ID());
    SetPrevious(aDepNode);
  }

  adr = strtok(NULL, " ");
  if (adr && adr[0] != '!') {
    TDF_Tool::Label(DF, adr, aLabel, 1);
    if (!aLabel.FindAttribute(ID(), aDepNode)) 
      aDepNode = SALOMEDSImpl_AttributeTreeNode::Set(aLabel, ID());
    SetNext(aDepNode);
  }

  adr = strtok(NULL, " ");
  if (adr && adr[0] != '!') {
    TDF_Tool::Label(DF, adr, aLabel, 1);
    if (!aLabel.FindAttribute(ID(), aDepNode)) 
      aDepNode = SALOMEDSImpl_AttributeTreeNode::Set(aLabel, ID());
    SetFirst(aDepNode);
  }
}


