// File:	SALOMEDS_AttributeTreeNode_i.hxx
// Created:	Fri Aug  2 10:21:01 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header:


#ifndef SALOMEDS_AttributeTreeNode_i_HeaderFile
#define SALOMEDS_AttributeTreeNode_i_HeaderFile

// IDL headers
#include <TDataStd_TreeNode.hxx>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeTreeNode_i: public virtual POA_SALOMEDS::AttributeTreeNode,
  public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeTreeNode_i(const Handle(TDataStd_TreeNode)& theAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theAttr; 
  };  
  ~SALOMEDS_AttributeTreeNode_i() {};

  void SetFather(SALOMEDS::AttributeTreeNode_ptr value);
  CORBA::Boolean HasFather();
  SALOMEDS::AttributeTreeNode_ptr GetFather();
  void SetPrevious(SALOMEDS::AttributeTreeNode_ptr value);
  CORBA::Boolean HasPrevious();
  SALOMEDS::AttributeTreeNode_ptr GetPrevious();
  void SetNext(SALOMEDS::AttributeTreeNode_ptr value);
  CORBA::Boolean HasNext();
  SALOMEDS::AttributeTreeNode_ptr GetNext();
  void SetFirst(SALOMEDS::AttributeTreeNode_ptr value);
  CORBA::Boolean HasFirst();
  SALOMEDS::AttributeTreeNode_ptr GetFirst();
  void SetTreeID(const char* value);
  char* GetTreeID();
  void Append(SALOMEDS::AttributeTreeNode_ptr value);
  void Prepend(SALOMEDS::AttributeTreeNode_ptr value);
  void InsertBefore(SALOMEDS::AttributeTreeNode_ptr value);
  void InsertAfter(SALOMEDS::AttributeTreeNode_ptr value);
  void Remove();
  CORBA::Long Depth();
  CORBA::Boolean IsRoot();
  CORBA::Boolean IsDescendant(SALOMEDS::AttributeTreeNode_ptr value);
  CORBA::Boolean IsFather(SALOMEDS::AttributeTreeNode_ptr value);
  CORBA::Boolean IsChild(SALOMEDS::AttributeTreeNode_ptr value);
  char* Label();
};

#endif
