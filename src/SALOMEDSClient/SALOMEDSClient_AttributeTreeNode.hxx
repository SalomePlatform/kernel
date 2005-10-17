//  File   : SALOMEDSClient_AttributeTreeNode.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributeTreeNode_HeaderFile
#define SALOMEDSClient_AttributeTreeNode_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeTreeNode
{
public:
  
  virtual void SetFather(const _PTR(AttributeTreeNode)& value) = 0;
  virtual bool HasFather() = 0;
  virtual _PTR(AttributeTreeNode) GetFather() = 0;
  virtual void SetPrevious(const _PTR(AttributeTreeNode)& value) = 0;
  virtual bool HasPrevious() = 0;
  virtual _PTR(AttributeTreeNode) GetPrevious() = 0;
  virtual void SetNext(const _PTR(AttributeTreeNode)& value) = 0;
  virtual bool HasNext() = 0;
  virtual _PTR(AttributeTreeNode) GetNext() = 0;
  virtual void SetFirst(const _PTR(AttributeTreeNode)& value) = 0;
  virtual bool HasFirst() = 0;
  virtual _PTR(AttributeTreeNode) GetFirst() = 0;
  virtual void SetTreeID(const std::string& value) = 0;
  virtual std::string GetTreeID() = 0;
  virtual void Append(const _PTR(AttributeTreeNode)& value) = 0;
  virtual void Prepend(const _PTR(AttributeTreeNode)& value) = 0;
  virtual void InsertBefore(const _PTR(AttributeTreeNode)& value) = 0;
  virtual void InsertAfter(const _PTR(AttributeTreeNode)& value) = 0;
  virtual void Remove() = 0;
  virtual int Depth() = 0;
  virtual bool IsRoot() = 0;
  virtual bool IsDescendant(const _PTR(AttributeTreeNode)& value) = 0;
  virtual bool IsFather(const _PTR(AttributeTreeNode)& value) = 0;
  virtual bool IsChild(const _PTR(AttributeTreeNode)& value) = 0;
  virtual std::string Label() = 0;

};

#endif
