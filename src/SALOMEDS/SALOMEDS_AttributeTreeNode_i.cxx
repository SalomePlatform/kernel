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
//  File   : SALOMEDS_AttributeTreeNode_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDS_AttributeTreeNode_i.hxx"
#include "utilities.h"
#include "SALOMEDS.hxx"
#include <TDocStd_Document.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_Tool.hxx>
#include <TDF_Data.hxx>

using namespace std;

static Handle(SALOMEDSImpl_AttributeTreeNode) GetNode(SALOMEDS::AttributeTreeNode_ptr value,
						      const Handle(SALOMEDSImpl_AttributeTreeNode)& aNode) 
{
  Handle(SALOMEDSImpl_AttributeTreeNode) aResult = new SALOMEDSImpl_AttributeTreeNode;
  TCollection_AsciiString anEntry = value->Label();
  TDF_Label aLabel;
  TDF_Tool::Label(aNode->Label().Data(), anEntry, aLabel);
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

void SALOMEDS_AttributeTreeNode_i::SetFather(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  aNode->SetFather(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasFather() 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl)->HasFather();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetFather() 
{
  SALOMEDS::Locker lock;
  SALOMEDS_AttributeTreeNode_i* aFather;
  aFather = new SALOMEDS_AttributeTreeNode_i(Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl)->GetFather(), _orb);
#ifndef WNT
  return aFather->POA_SALOMEDS::AttributeTreeNode::_this();
#else
  return aFather->AttributeTreeNode::_this();
#endif
}

void SALOMEDS_AttributeTreeNode_i::SetPrevious(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  aNode->SetPrevious(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasPrevious() 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl)->HasPrevious();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetPrevious() 
{
  SALOMEDS::Locker lock;
  SALOMEDS_AttributeTreeNode_i* aPrevious;
  aPrevious=new SALOMEDS_AttributeTreeNode_i(Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl)->GetPrevious(), _orb);
#ifndef WNT
  return aPrevious->POA_SALOMEDS::AttributeTreeNode::_this();
#else
  return aPrevious->AttributeTreeNode::_this();
#endif
}

void SALOMEDS_AttributeTreeNode_i::SetNext(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  aNode->SetNext(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasNext() 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl)->HasNext();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetNext() 
{
  SALOMEDS::Locker lock;
  SALOMEDS_AttributeTreeNode_i* aNext;
  aNext = new SALOMEDS_AttributeTreeNode_i(Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl)->GetNext(), _orb);
#ifndef WNT
  return aNext->POA_SALOMEDS::AttributeTreeNode::_this();
#else
  return aNext->AttributeTreeNode::_this();
#endif
}

void SALOMEDS_AttributeTreeNode_i::SetFirst(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  aNode->SetFirst(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::HasFirst() 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl)->HasFirst();
}

SALOMEDS::AttributeTreeNode_ptr SALOMEDS_AttributeTreeNode_i::GetFirst() 
{
  SALOMEDS::Locker lock;
  SALOMEDS_AttributeTreeNode_i* aFirst;
  aFirst = new SALOMEDS_AttributeTreeNode_i(Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl)->GetFirst(), _orb);
#ifndef WNT
  return aFirst->POA_SALOMEDS::AttributeTreeNode::_this();
#else
  return aFirst->AttributeTreeNode::_this();
#endif
}

void SALOMEDS_AttributeTreeNode_i::SetTreeID(const char* value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  aNode->SetTreeID(Standard_GUID(aNode->ID()));
}

char* SALOMEDS_AttributeTreeNode_i::GetTreeID() 
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  char aGUID[40];
  aNode->ID().ToCString(aGUID);
  return CORBA::String_var(CORBA::string_dup(aGUID))._retn();
}

void SALOMEDS_AttributeTreeNode_i::Append(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  aNode->Append(GetNode(value, aNode));
}

void SALOMEDS_AttributeTreeNode_i::Prepend(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  aNode->Prepend(GetNode(value, aNode));
}

void SALOMEDS_AttributeTreeNode_i::InsertBefore(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  aNode->InsertBefore(GetNode(value, aNode));
}

void SALOMEDS_AttributeTreeNode_i::InsertAfter(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  aNode->InsertAfter(GetNode(value, aNode));
}

void SALOMEDS_AttributeTreeNode_i::Remove() 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  aNode->Remove();
}

CORBA::Long SALOMEDS_AttributeTreeNode_i::Depth() 
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  return aNode->Depth();
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsRoot() 
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  return aNode->IsRoot();
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsDescendant(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  return aNode->IsDescendant(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsFather(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  return aNode->IsFather(GetNode(value, aNode));
}

CORBA::Boolean SALOMEDS_AttributeTreeNode_i::IsChild(SALOMEDS::AttributeTreeNode_ptr value) 
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_impl);
  return aNode->IsChild(GetNode(value, aNode));
}

char* SALOMEDS_AttributeTreeNode_i::Label() 
{
  SALOMEDS::Locker lock;
  TCollection_AsciiString aLabelName;
  TDF_Tool::Entry(_impl->Label(),aLabelName);
  return CORBA::String_var(CORBA::string_dup(aLabelName.ToCString()))._retn();
}

