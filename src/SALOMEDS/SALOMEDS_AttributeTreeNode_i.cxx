//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
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
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOMEDS_AttributeTreeNode_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header: 

#include "SALOMEDS_AttributeTreeNode_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include "utilities.h"
using namespace std;

static Handle(TDataStd_TreeNode) GetNode(SALOMEDS::AttributeTreeNode_ptr value,
					 const Handle(TDataStd_TreeNode)& aNode) {
  Handle(TDataStd_TreeNode) aResult = new TDataStd_TreeNode;
  CORBA::String_var aString = CORBA::string_dup(value->Label());
  TDF_Label aLabel;
  TDF_Tool::Label(aNode->Label().Data(), TCollection_AsciiString(aString), aLabel, 0);
  if (aLabel.IsNull()) {
    MESSAGE("SALOMEDS_AttributeTreeNode_i class: no such label")
    MESSAGE("SALOMEDS_AttributeTreeNode_i class: no such label")
    return aResult;
  }
  if (!aLabel.FindAttribute(aNode->ID(), aResult)) {
    MESSAGE("SALOMEDS_AttributeTreeNode_i class: no such attribute")
    MESSAGE("SALOMEDS_AttributeTreeNode_i class: no such attribute")
  }
  return aResult;
}

void SALOMEDS_AttributeTreeNode_i::SetFather(SALOMEDS::AttributeTreeNode_ptr value) {
  CheckLocked();
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  aNode->SetFather(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasFather() {
  return Handle(TDataStd_TreeNode)::DownCast(_myAttr)->HasFather();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetFather() {
  SALOMEDS_AttributeTreeNode_i* aFather = new SALOMEDS_AttributeTreeNode_i(Handle(TDataStd_TreeNode)::DownCast(_myAttr)->Father(), _myOrb);
  return aFather->POA_SALOMEDS::AttributeTreeNode::_this();
}

void SALOMEDS_AttributeTreeNode_i::SetPrevious(SALOMEDS::AttributeTreeNode_ptr value) {
  CheckLocked();
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  aNode->SetPrevious(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasPrevious() {
  return Handle(TDataStd_TreeNode)::DownCast(_myAttr)->HasPrevious();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetPrevious() {
  SALOMEDS_AttributeTreeNode_i* aPrevious = new SALOMEDS_AttributeTreeNode_i(Handle(TDataStd_TreeNode)::DownCast(_myAttr)->Previous(), _myOrb);
  return aPrevious->POA_SALOMEDS::AttributeTreeNode::_this();
}

void SALOMEDS_AttributeTreeNode_i::SetNext(SALOMEDS::AttributeTreeNode_ptr value) {
  CheckLocked();
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  aNode->SetNext(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasNext() {
  return Handle(TDataStd_TreeNode)::DownCast(_myAttr)->HasNext();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetNext() {
  SALOMEDS_AttributeTreeNode_i* aNext = new SALOMEDS_AttributeTreeNode_i(Handle(TDataStd_TreeNode)::DownCast(_myAttr)->Next(), _myOrb);
  return aNext->POA_SALOMEDS::AttributeTreeNode::_this();
}

void SALOMEDS_AttributeTreeNode_i::SetFirst(SALOMEDS::AttributeTreeNode_ptr value) {
  CheckLocked();
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  aNode->SetFirst(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasFirst() {
  return Handle(TDataStd_TreeNode)::DownCast(_myAttr)->HasFirst();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetFirst() {
  SALOMEDS_AttributeTreeNode_i* aFirst = new SALOMEDS_AttributeTreeNode_i(Handle(TDataStd_TreeNode)::DownCast(_myAttr)->First(), _myOrb);
  return aFirst->POA_SALOMEDS::AttributeTreeNode::_this();
}

void SALOMEDS_AttributeTreeNode_i::SetTreeID(const char* value) {
  CheckLocked();
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  aNode->SetTreeID(Standard_GUID(aNode->ID()));
}

char* SALOMEDS_AttributeTreeNode_i::GetTreeID() {
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  char aGUID[40];
  aNode->ID().ToCString(aGUID);
  return CORBA::String_var(CORBA::string_dup(aGUID))._retn();
}

void SALOMEDS_AttributeTreeNode_i::Append(SALOMEDS::AttributeTreeNode_ptr value) {
  CheckLocked();
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  aNode->Append(GetNode(value, aNode));
}

void SALOMEDS_AttributeTreeNode_i::Prepend(SALOMEDS::AttributeTreeNode_ptr value) {
  CheckLocked();
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  aNode->Prepend(GetNode(value, aNode));
}

void SALOMEDS_AttributeTreeNode_i::InsertBefore(SALOMEDS::AttributeTreeNode_ptr value) {
  CheckLocked();
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  aNode->InsertBefore(GetNode(value, aNode));
}

void SALOMEDS_AttributeTreeNode_i::InsertAfter(SALOMEDS::AttributeTreeNode_ptr value) {
  CheckLocked();
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  aNode->InsertAfter(GetNode(value, aNode));
}

void SALOMEDS_AttributeTreeNode_i::Remove() {
  CheckLocked();
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  aNode->Remove();
}

CORBA::Long SALOMEDS_AttributeTreeNode_i::Depth() {
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  return aNode->Depth();
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsRoot() {
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  return aNode->IsRoot();
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsDescendant(SALOMEDS::AttributeTreeNode_ptr value) {
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  return aNode->IsDescendant(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsFather(SALOMEDS::AttributeTreeNode_ptr value) {
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  return aNode->IsFather(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsChild(SALOMEDS::AttributeTreeNode_ptr value) {
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  return aNode->IsChild(GetNode(value, aNode));
}

char* SALOMEDS_AttributeTreeNode_i::Label() {
  TCollection_AsciiString aLabelName;
  TDF_Tool::Entry(_myAttr->Label(),aLabelName);
  return CORBA::String_var(CORBA::string_dup(aLabelName.ToCString()))._retn();
}
