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

//  File   : SALOMEDS_AttributeTreeNode.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeTreeNode.hxx"
#include "SALOMEDS.hxx"

#include <string>
#include <stdexcept>

#include "SALOMEDSImpl_AttributeTreeNode.hxx"
#include "SALOMEDS_AttributeTreeNode.hxx"

SALOMEDS_AttributeTreeNode::SALOMEDS_AttributeTreeNode(SALOMEDSImpl_AttributeTreeNode* theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTreeNode::SALOMEDS_AttributeTreeNode(SALOMEDS::AttributeTreeNode_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTreeNode::~SALOMEDS_AttributeTreeNode()
{}

void SALOMEDS_AttributeTreeNode::SetFather(const _PTR(AttributeTreeNode)& value)
{
  SALOMEDS_AttributeTreeNode* aTN = dynamic_cast<SALOMEDS_AttributeTreeNode*>(value.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode *aNode, *aFather;
    aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    aFather = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(aTN->_local_impl);
    aNode->SetFather(aFather);
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    SALOMEDS::AttributeTreeNode_var aFather = SALOMEDS::AttributeTreeNode::_narrow(aTN->_corba_impl);
    aNode->SetFather(aFather);
  }
}

bool SALOMEDS_AttributeTreeNode::HasFather()
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    ret = aNode->HasFather();
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    ret = aNode->HasFather();
  }
  return ret;
}

_PTR(AttributeTreeNode) SALOMEDS_AttributeTreeNode::GetFather()
{
  SALOMEDSClient_AttributeTreeNode* aTN  = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    aTN = new SALOMEDS_AttributeTreeNode(aNode->GetFather());
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    aTN = new SALOMEDS_AttributeTreeNode(aNode->GetFather());
  }
  return _PTR(AttributeTreeNode)(aTN);
}

void SALOMEDS_AttributeTreeNode::SetPrevious(const _PTR(AttributeTreeNode)& value)
{
  SALOMEDS_AttributeTreeNode* aTN = dynamic_cast<SALOMEDS_AttributeTreeNode*>(value.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode *aNode, *aPrev;
    aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    aPrev = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(aTN->_local_impl);
    aNode->SetPrevious(aPrev);
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    SALOMEDS::AttributeTreeNode_var aPrev = SALOMEDS::AttributeTreeNode::_narrow(aTN->_corba_impl);
    aNode->SetPrevious(aPrev);
  }
}

bool SALOMEDS_AttributeTreeNode::HasPrevious()
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    ret = aNode->HasPrevious();
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    ret = aNode->HasPrevious();
  }
  return ret;
}

_PTR(AttributeTreeNode) SALOMEDS_AttributeTreeNode::GetPrevious()
{
  SALOMEDSClient_AttributeTreeNode* aTN = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    aTN = new SALOMEDS_AttributeTreeNode(aNode->GetPrevious());
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    aTN = new SALOMEDS_AttributeTreeNode(aNode->GetPrevious());
  }
  return _PTR(AttributeTreeNode)(aTN);
}

void SALOMEDS_AttributeTreeNode::SetNext(const _PTR(AttributeTreeNode)& value)
{
  SALOMEDS_AttributeTreeNode* aTN = dynamic_cast<SALOMEDS_AttributeTreeNode*>(value.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode  *aNode, *aNext;
    aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    aNext = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(aTN->_local_impl);
    aNode->SetNext(aNext);
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    SALOMEDS::AttributeTreeNode_var aNext = SALOMEDS::AttributeTreeNode::_narrow(aTN->_corba_impl);
    aNode->SetNext(aNext);
  }
}

bool SALOMEDS_AttributeTreeNode::HasNext()
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    ret = aNode->HasNext();
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    ret = aNode->HasNext();
  }
  return ret;
}

_PTR(AttributeTreeNode) SALOMEDS_AttributeTreeNode::GetNext()
{
  SALOMEDSClient_AttributeTreeNode* aTN = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    aTN = new SALOMEDS_AttributeTreeNode(aNode->GetNext());
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    aTN = new SALOMEDS_AttributeTreeNode(aNode->GetNext());
  }
  return _PTR(AttributeTreeNode)(aTN);
}

void SALOMEDS_AttributeTreeNode::SetFirst(const _PTR(AttributeTreeNode)& value)
{
  SALOMEDS_AttributeTreeNode* aTN = dynamic_cast<SALOMEDS_AttributeTreeNode*>(value.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode *aNode, *aFirst;
    aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    aFirst = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(aTN->_local_impl);
    aNode->SetFirst(aFirst);
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    SALOMEDS::AttributeTreeNode_var aFirst = SALOMEDS::AttributeTreeNode::_narrow(aTN->_corba_impl);
    aNode->SetFirst(aFirst);
  }
}

bool SALOMEDS_AttributeTreeNode::HasFirst()
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    ret = aNode->HasFirst();
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    ret = aNode->HasFirst();
  }
  return ret;
}

_PTR(AttributeTreeNode) SALOMEDS_AttributeTreeNode::GetFirst()
{
  SALOMEDSClient_AttributeTreeNode* aTN = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    aTN = new SALOMEDS_AttributeTreeNode(aNode->GetFirst());
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    aTN = new SALOMEDS_AttributeTreeNode(aNode->GetFirst());
  }
  return _PTR(AttributeTreeNode)(aTN);
}

void SALOMEDS_AttributeTreeNode::SetTreeID(const std::string& value)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    aNode->SetTreeID(value);
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    aNode->SetTreeID(value.c_str());
  }
}

std::string SALOMEDS_AttributeTreeNode::GetTreeID()
{
  std::string aGUID;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    aGUID = aNode->GetTreeID();
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    aGUID = aNode->GetTreeID();
  }

  return aGUID;
}

void SALOMEDS_AttributeTreeNode::Append(const _PTR(AttributeTreeNode)& value)
{
  SALOMEDS_AttributeTreeNode* aTN = dynamic_cast<SALOMEDS_AttributeTreeNode*>(value.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode *aNode, *anOther;
    aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    anOther = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(aTN->_local_impl);
    aNode->Append(anOther);
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    SALOMEDS::AttributeTreeNode_var anOther = SALOMEDS::AttributeTreeNode::_narrow(aTN->_corba_impl);
    aNode->Append(anOther);
  }
}

void SALOMEDS_AttributeTreeNode::Prepend(const _PTR(AttributeTreeNode)& value)
{
  SALOMEDS_AttributeTreeNode* aTN = dynamic_cast<SALOMEDS_AttributeTreeNode*>(value.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode *aNode, *anOther;
    aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    anOther = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(aTN->_local_impl);
    aNode->Prepend(anOther);
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    SALOMEDS::AttributeTreeNode_var anOther = SALOMEDS::AttributeTreeNode::_narrow(aTN->_corba_impl);
    aNode->Prepend(anOther);
  }
}

void SALOMEDS_AttributeTreeNode::InsertBefore(const _PTR(AttributeTreeNode)& value)
{
  SALOMEDS_AttributeTreeNode* aTN = dynamic_cast<SALOMEDS_AttributeTreeNode*>(value.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode *aNode, *anOther;
    aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    anOther = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(aTN->_local_impl);
    aNode->InsertBefore(anOther);
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    SALOMEDS::AttributeTreeNode_var anOther = SALOMEDS::AttributeTreeNode::_narrow(aTN->_corba_impl);
    aNode->InsertBefore(anOther);
  }
}

void SALOMEDS_AttributeTreeNode::InsertAfter(const _PTR(AttributeTreeNode)& value)
{
  SALOMEDS_AttributeTreeNode* aTN = dynamic_cast<SALOMEDS_AttributeTreeNode*>(value.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode *aNode, *anOther;
    aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    anOther = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(aTN->_local_impl);
    aNode->InsertAfter(anOther);
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    SALOMEDS::AttributeTreeNode_var anOther = SALOMEDS::AttributeTreeNode::_narrow(aTN->_corba_impl);
    aNode->InsertAfter(anOther);
  }
}

void SALOMEDS_AttributeTreeNode::Remove()
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl)->Remove();
  }
  else SALOMEDS::AttributeTreeNode::_narrow(_corba_impl)->Remove();
}

int SALOMEDS_AttributeTreeNode::Depth()
{
  int aDepth;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aDepth = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl)->Depth();
  }
  else aDepth =  SALOMEDS::AttributeTreeNode::_narrow(_corba_impl)->Depth();
  return aDepth;
}

bool SALOMEDS_AttributeTreeNode::IsRoot()
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl)->IsRoot();
  }
  else ret =  SALOMEDS::AttributeTreeNode::_narrow(_corba_impl)->IsRoot();
  return ret;
}

bool SALOMEDS_AttributeTreeNode::IsDescendant(const _PTR(AttributeTreeNode)& value)
{
  bool ret;
  SALOMEDS_AttributeTreeNode* aTN = dynamic_cast<SALOMEDS_AttributeTreeNode*>(value.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode *aNode, *anOther;
    aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    anOther = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(aTN->_local_impl);
    ret = aNode->IsDescendant(anOther);
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    SALOMEDS::AttributeTreeNode_var anOther = SALOMEDS::AttributeTreeNode::_narrow(aTN->_corba_impl);
    ret = aNode->IsDescendant(anOther);
  }  
  return ret;
}

bool SALOMEDS_AttributeTreeNode::IsFather(const _PTR(AttributeTreeNode)& value)
{
  bool ret;
  SALOMEDS_AttributeTreeNode* aTN = dynamic_cast<SALOMEDS_AttributeTreeNode*>(value.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode *aNode, *anOther;
    aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    anOther = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(aTN->_local_impl);
    ret = aNode->IsFather(anOther);
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    SALOMEDS::AttributeTreeNode_var anOther = SALOMEDS::AttributeTreeNode::_narrow(aTN->_corba_impl);
    ret = aNode->IsFather(anOther);
  }  
  return ret;  
}

bool SALOMEDS_AttributeTreeNode::IsChild(const _PTR(AttributeTreeNode)& value)
{
  bool ret;
  SALOMEDS_AttributeTreeNode* aTN = dynamic_cast<SALOMEDS_AttributeTreeNode*>(value.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTreeNode *aNode, *anOther;
    aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_local_impl);
    anOther = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(aTN->_local_impl);
    ret = aNode->IsChild(anOther);
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    SALOMEDS::AttributeTreeNode_var anOther = SALOMEDS::AttributeTreeNode::_narrow(aTN->_corba_impl);
    ret = aNode->IsChild(anOther);
  }  
  return ret;  
}

std::string SALOMEDS_AttributeTreeNode::Label()
{
  std::string aLabel;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aLabel = _local_impl->Label().Entry();
  }
  else aLabel = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl)->Label();
  return aLabel;
}
