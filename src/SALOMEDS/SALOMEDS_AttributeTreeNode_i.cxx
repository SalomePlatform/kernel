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

//  File   : SALOMEDS_AttributeTreeNode_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeTreeNode_i.hxx"
#include "utilities.h"
#include "SALOMEDS.hxx"

static SALOMEDSImpl_AttributeTreeNode* GetNode(SALOMEDS::AttributeTreeNode_ptr value,
                                               SALOMEDSImpl_AttributeTreeNode* aNode) 
{
  SALOMEDSImpl_AttributeTreeNode* aResult = NULL;
  DF_Label aLabel = DF_Label::Label(aNode->Label(), value->Label());
  if (aLabel.IsNull()) {
    MESSAGE("SALOMEDS_AttributeTreeNode_i class: no such label")
    MESSAGE("SALOMEDS_AttributeTreeNode_i class: no such label")
    return aResult;
  }
  if (!(aResult=(SALOMEDSImpl_AttributeTreeNode*)aLabel.FindAttribute(aNode->ID()))) {
    MESSAGE("SALOMEDS_AttributeTreeNode_i class: no such attribute")
    MESSAGE("SALOMEDS_AttributeTreeNode_i class: no such attribute")
  }
  return aResult;
}

void SALOMEDS_AttributeTreeNode_i::SetFather(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  aNode->SetFather(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasFather() 
{
  SALOMEDS::Locker lock;
  return dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl)->HasFather();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetFather() 
{
  SALOMEDS::Locker lock;
  SALOMEDS_AttributeTreeNode_i* aFather;
  aFather = new SALOMEDS_AttributeTreeNode_i(dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl)->GetFather(), _orb);
#ifndef WIN32
  return aFather->POA_SALOMEDS::AttributeTreeNode::_this();
#else
  return aFather->AttributeTreeNode::_this();
#endif
}

void SALOMEDS_AttributeTreeNode_i::SetPrevious(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  aNode->SetPrevious(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasPrevious() 
{
  SALOMEDS::Locker lock;
  return dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl)->HasPrevious();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetPrevious() 
{
  SALOMEDS::Locker lock;
  SALOMEDS_AttributeTreeNode_i* aPrevious;
  aPrevious=new SALOMEDS_AttributeTreeNode_i(dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl)->GetPrevious(), _orb);
#ifndef WIN32
  return aPrevious->POA_SALOMEDS::AttributeTreeNode::_this();
#else
  return aPrevious->AttributeTreeNode::_this();
#endif
}

void SALOMEDS_AttributeTreeNode_i::SetNext(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  aNode->SetNext(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasNext() 
{
  SALOMEDS::Locker lock;
  return dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl)->HasNext();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetNext() 
{
  SALOMEDS::Locker lock;
  SALOMEDS_AttributeTreeNode_i* aNext;
  aNext = new SALOMEDS_AttributeTreeNode_i(dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl)->GetNext(), _orb);
#ifndef WIN32
  return aNext->POA_SALOMEDS::AttributeTreeNode::_this();
#else
  return aNext->AttributeTreeNode::_this();
#endif
}

void SALOMEDS_AttributeTreeNode_i::SetFirst(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  aNode->SetFirst(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasFirst() 
{
  SALOMEDS::Locker lock;
  return dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl)->HasFirst();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetFirst() 
{
  SALOMEDS::Locker lock;
  SALOMEDS_AttributeTreeNode_i* aFirst;
  aFirst = new SALOMEDS_AttributeTreeNode_i(dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl)->GetFirst(), _orb);
#ifndef WIN32
  return aFirst->POA_SALOMEDS::AttributeTreeNode::_this();
#else
  return aFirst->AttributeTreeNode::_this();
#endif
}

void SALOMEDS_AttributeTreeNode_i::SetTreeID(const char* value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  aNode->SetTreeID(value);
}

char* SALOMEDS_AttributeTreeNode_i::GetTreeID() 
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  return CORBA::String_var(CORBA::string_dup(aNode->ID().c_str()))._retn();
}

void SALOMEDS_AttributeTreeNode_i::Append(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  aNode->Append(GetNode(value, aNode));
}

void SALOMEDS_AttributeTreeNode_i::Prepend(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  aNode->Prepend(GetNode(value, aNode));
}

void SALOMEDS_AttributeTreeNode_i::InsertBefore(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  aNode->InsertBefore(GetNode(value, aNode));
}

void SALOMEDS_AttributeTreeNode_i::InsertAfter(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  aNode->InsertAfter(GetNode(value, aNode));
}

void SALOMEDS_AttributeTreeNode_i::Remove() 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  aNode->Remove();
}

CORBA::Long SALOMEDS_AttributeTreeNode_i::Depth() 
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  return aNode->Depth();
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsRoot() 
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  return aNode->IsRoot();
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsDescendant(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  return aNode->IsDescendant(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsFather(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  return aNode->IsFather(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsChild(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_AttributeTreeNode* aNode = dynamic_cast<SALOMEDSImpl_AttributeTreeNode*>(_impl);
  return aNode->IsChild(GetNode(value, aNode));
}

char* SALOMEDS_AttributeTreeNode_i::Label() 
{
  SALOMEDS::Locker lock;
  return CORBA::String_var(CORBA::string_dup(_impl->Label().Entry().c_str()))._retn();
}

