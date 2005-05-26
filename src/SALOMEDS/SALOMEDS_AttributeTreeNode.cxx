//  File   : SALOMEDS_AttributeTreeNode.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeTreeNode.hxx"

#include <string>
#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <Standard_GUID.hxx>
#include <TDF_Label.hxx>
#include <TDF_Tool.hxx>

#include "SALOMEDSImpl_AttributeTreeNode.hxx"
#include "SALOMEDS_AttributeTreeNode.hxx"

SALOMEDS_AttributeTreeNode::SALOMEDS_AttributeTreeNode(const Handle(SALOMEDSImpl_AttributeTreeNode)& theAttr)
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode, aFather;
    aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
    aFather = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(aTN->_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode, aPrev;
    aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
    aPrev = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(aTN->_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode, aNext;
    aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
    aNext = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(aTN->_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode, aFirst;
    aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
    aFirst = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(aTN->_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
    aNode->SetTreeID(Standard_GUID((char*)value.c_str()));
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    aNode->SetTreeID(value.c_str());
  }
}

std::string SALOMEDS_AttributeTreeNode::GetTreeID()
{
  TCollection_AsciiString aGUID;
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
    char guid[40];
    aNode->GetTreeID().ToCString(guid);
    aGUID = TCollection_AsciiString(guid);
  }
  else {
    SALOMEDS::AttributeTreeNode_var aNode = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl);
    aGUID = TCollection_AsciiString(aNode->GetTreeID());
  }

  return std::string(aGUID.ToCString());
}

void SALOMEDS_AttributeTreeNode::Append(const _PTR(AttributeTreeNode)& value)
{
  SALOMEDS_AttributeTreeNode* aTN = dynamic_cast<SALOMEDS_AttributeTreeNode*>(value.get());
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode, anOther;
    aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
    anOther = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(aTN->_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode, anOther;
    aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
    anOther = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(aTN->_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode, anOther;
    aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
    anOther = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(aTN->_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode, anOther;
    aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
    anOther = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(aTN->_local_impl);
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
  if(_isLocal) Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl)->Remove();
  else SALOMEDS::AttributeTreeNode::_narrow(_corba_impl)->Remove();
}

int SALOMEDS_AttributeTreeNode::Depth()
{
  int aDepth;
  if(_isLocal) aDepth = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl)->Depth();
  else aDepth =  SALOMEDS::AttributeTreeNode::_narrow(_corba_impl)->Depth();
  return aDepth;
}

bool SALOMEDS_AttributeTreeNode::IsRoot()
{
  bool ret;
  if(_isLocal) ret = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl)->IsRoot();
  else ret =  SALOMEDS::AttributeTreeNode::_narrow(_corba_impl)->IsRoot();
  return ret;
}

bool SALOMEDS_AttributeTreeNode::IsDescendant(const _PTR(AttributeTreeNode)& value)
{
  bool ret;
  SALOMEDS_AttributeTreeNode* aTN = dynamic_cast<SALOMEDS_AttributeTreeNode*>(value.get());
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode, anOther;
    aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
    anOther = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(aTN->_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode, anOther;
    aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
    anOther = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(aTN->_local_impl);
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
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTreeNode) aNode, anOther;
    aNode = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(_local_impl);
    anOther = Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(aTN->_local_impl);
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
  TCollection_AsciiString aLabel;
  if(_isLocal) TDF_Tool::Entry(_local_impl->Label(), aLabel);
  else aLabel = SALOMEDS::AttributeTreeNode::_narrow(_corba_impl)->Label();
  return std::string(aLabel.ToCString());
}


