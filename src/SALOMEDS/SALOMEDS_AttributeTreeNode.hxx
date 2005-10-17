//  File   : SALOMEDS_AttributeTreeNode.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeTreeNode_HeaderFile
#define SALOMEDS_AttributeTreeNode_HeaderFile

#include "SALOMEDSClient_AttributeTreeNode.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeTreeNode.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeTreeNode: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeTreeNode
{
public:  
  SALOMEDS_AttributeTreeNode(const Handle(SALOMEDSImpl_AttributeTreeNode)& theAttr);
  SALOMEDS_AttributeTreeNode(SALOMEDS::AttributeTreeNode_ptr theAttr);
  ~SALOMEDS_AttributeTreeNode();

  virtual void SetFather(const _PTR(AttributeTreeNode)& value);
  virtual bool HasFather();
  virtual _PTR(AttributeTreeNode) GetFather();
  virtual void SetPrevious(const _PTR(AttributeTreeNode)& value);
  virtual bool HasPrevious();
  virtual _PTR(AttributeTreeNode) GetPrevious();
  virtual void SetNext(const _PTR(AttributeTreeNode)& value);
  virtual bool HasNext();
  virtual _PTR(AttributeTreeNode) GetNext();
  virtual void SetFirst(const _PTR(AttributeTreeNode)& value);
  virtual bool HasFirst();
  virtual _PTR(AttributeTreeNode) GetFirst();
  virtual void SetTreeID(const std::string& value);
  virtual std::string GetTreeID();
  virtual void Append(const _PTR(AttributeTreeNode)& value);
  virtual void Prepend(const _PTR(AttributeTreeNode)& value);
  virtual void InsertBefore(const _PTR(AttributeTreeNode)& value);
  virtual void InsertAfter(const _PTR(AttributeTreeNode)& value);
  virtual void Remove();
  virtual int Depth();
  virtual bool IsRoot();
  virtual bool IsDescendant(const _PTR(AttributeTreeNode)& value);
  virtual bool IsFather(const _PTR(AttributeTreeNode)& value);
  virtual bool IsChild(const _PTR(AttributeTreeNode)& value);
  virtual std::string Label();                                        
};

#endif
