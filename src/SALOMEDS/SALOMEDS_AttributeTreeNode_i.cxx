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
//  $Header$

#include <TDocStd_Document.hxx>
#include <TDF_Tool.hxx>

#include "SALOMEDS_AttributeTreeNode_i.hxx"

#include "utilities.h"

using namespace std;

static Handle(TDataStd_TreeNode) GetNode(SALOMEDS::AttributeTreeNode_ptr value,
					 const Handle(TDataStd_TreeNode)& aNode) 
{
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

void SALOMEDS_AttributeTreeNode_i::SetFather(SALOMEDS::AttributeTreeNode_ptr theValue) {
  CheckLocked();
  _myAttr->SetFather(GetNode(theValue,_myAttr));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasFather() {
  return _myAttr->HasFather();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetFather() {
  SALOMEDS_AttributeTreeNode_i* aTreeNode = 
    new SALOMEDS_AttributeTreeNode_i(_myAttr->Father(),_mySObject);
  return aTreeNode->_this();
}

void SALOMEDS_AttributeTreeNode_i::SetPrevious(SALOMEDS::AttributeTreeNode_ptr theValue) {
  CheckLocked();
  _myAttr->SetPrevious(GetNode(theValue,_myAttr));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasPrevious() {
  return _myAttr->HasPrevious();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetPrevious() {
  SALOMEDS_AttributeTreeNode_i* aTreeNode = 
    new SALOMEDS_AttributeTreeNode_i(_myAttr->Previous(),_mySObject);
  return aTreeNode->_this();
}

void SALOMEDS_AttributeTreeNode_i::SetNext(SALOMEDS::AttributeTreeNode_ptr theValue) {
  CheckLocked();
  _myAttr->SetNext(GetNode(theValue,_myAttr));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasNext() {
  return _myAttr->HasNext();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetNext() {
  SALOMEDS_AttributeTreeNode_i* aTreeNode = 
    new SALOMEDS_AttributeTreeNode_i(_myAttr->Next(),_mySObject);
  return aTreeNode->_this();
}

void SALOMEDS_AttributeTreeNode_i::SetFirst(SALOMEDS::AttributeTreeNode_ptr theValue) {
  CheckLocked();
  _myAttr->SetFirst(GetNode(theValue,_myAttr));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasFirst() {
  return _myAttr->HasFirst();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetFirst() {
  SALOMEDS_AttributeTreeNode_i* aTreeNode = 
    new SALOMEDS_AttributeTreeNode_i(_myAttr->First(),_mySObject);
  return aTreeNode->_this();
}

void SALOMEDS_AttributeTreeNode_i::SetTreeID(const char* value) {
  CheckLocked();
  _myAttr->SetTreeID(Standard_GUID(_myAttr->ID()));
}

char* SALOMEDS_AttributeTreeNode_i::GetTreeID() {
  char aGUID[40];
  _myAttr->ID().ToCString(aGUID);
  return CORBA::string_dup(aGUID);
}

void SALOMEDS_AttributeTreeNode_i::Append(SALOMEDS::AttributeTreeNode_ptr theValue) {
  CheckLocked();
  _myAttr->Append(GetNode(theValue,_myAttr));
}

void SALOMEDS_AttributeTreeNode_i::Prepend(SALOMEDS::AttributeTreeNode_ptr theValue) {
  CheckLocked();
  _myAttr->Prepend(GetNode(theValue,_myAttr));
}

void SALOMEDS_AttributeTreeNode_i::InsertBefore(SALOMEDS::AttributeTreeNode_ptr theValue) {
  CheckLocked();
  _myAttr->InsertBefore(GetNode(theValue,_myAttr));
}

void SALOMEDS_AttributeTreeNode_i::InsertAfter(SALOMEDS::AttributeTreeNode_ptr theValue) {
  CheckLocked();
  _myAttr->InsertAfter(GetNode(theValue,_myAttr));
}

void SALOMEDS_AttributeTreeNode_i::Remove() {
  CheckLocked();
  _myAttr->Remove();
}

CORBA::Long SALOMEDS_AttributeTreeNode_i::Depth() {
  return _myAttr->Depth();
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsRoot() {
  return _myAttr->IsRoot();
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsDescendant(SALOMEDS::AttributeTreeNode_ptr theValue) {
  return _myAttr->IsDescendant(GetNode(theValue,_myAttr));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsFather(SALOMEDS::AttributeTreeNode_ptr theValue) {
  return _myAttr->IsFather(GetNode(theValue,_myAttr));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsChild(SALOMEDS::AttributeTreeNode_ptr theValue) {
  return _myAttr->IsChild(GetNode(theValue,_myAttr));
}

char* SALOMEDS_AttributeTreeNode_i::Label() {
  TCollection_AsciiString aLabelName;
  TDF_Tool::Entry(_myAttr->Label(),aLabelName);
  return CORBA::string_dup(aLabelName.ToCString());
}

char* SALOMEDS_AttributeTreeNode_i::Store() {
  char* aStr[4];

  if (HasFather()) aStr[0] = GetFather()->Label(); else aStr[0] = "!";
  if (HasPrevious()) aStr[1] = GetPrevious()->Label(); else aStr[1] = "!";
  if (HasNext()) aStr[2] = GetNext()->Label(); else aStr[2] = "!";
  if (HasFirst()) aStr[3] = GetFirst()->Label(); else aStr[3] = "!";

  int aLength = 4;
  aLength += strlen(aStr[0]) + strlen(aStr[1]) + strlen(aStr[2]) + strlen(aStr[3]);
  CORBA::String_var aResult = new char[aLength];
  sprintf(aResult, "%s %s %s %s", aStr[0], aStr[1], aStr[2], aStr[3]);
  return aResult._retn();
}

void SALOMEDS_AttributeTreeNode_i::Restore(const char* value) {
  Handle(TDataStd_TreeNode) aNode = Handle(TDataStd_TreeNode)::DownCast(_myAttr);
  Handle(TDF_Data) DF = TDocStd_Document::Get(_myAttr->Label())->GetData();
  
  char* aCopy = strdup(value);
  char* adr = strtok(aCopy, " ");
  
  TDF_Label aLabel;
  Handle(TDataStd_TreeNode) aDepNode;

  if (adr && adr[0] != '!') {
    TDF_Tool::Label(DF, adr, aLabel, 1);
    if (!aLabel.FindAttribute(aNode->ID(), aDepNode)) aDepNode = TDataStd_TreeNode::Set(aLabel, aNode->ID());
    aNode->SetFather(aDepNode);
  }

  adr = strtok(NULL, " ");
  if (adr && adr[0] != '!') {
    TDF_Tool::Label(DF, adr, aLabel, 1);
    if (!aLabel.FindAttribute(aNode->ID(), aDepNode)) aDepNode = TDataStd_TreeNode::Set(aLabel, aNode->ID());
    aNode->SetPrevious(aDepNode);
  }

  adr = strtok(NULL, " ");
  if (adr && adr[0] != '!') {
    TDF_Tool::Label(DF, adr, aLabel, 1);
    if (!aLabel.FindAttribute(aNode->ID(), aDepNode)) aDepNode = TDataStd_TreeNode::Set(aLabel, aNode->ID());
    aNode->SetNext(aDepNode);
  }

  adr = strtok(NULL, " ");
  if (adr && adr[0] != '!') {
    TDF_Tool::Label(DF, adr, aLabel, 1);
    if (!aLabel.FindAttribute(aNode->ID(), aDepNode)) aDepNode = TDataStd_TreeNode::Set(aLabel, aNode->ID());
    aNode->SetFirst(aDepNode);
  }
}
