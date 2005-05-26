//  File   : SALOMEDS_AttributeTreeNode_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeTreeNode_i_HeaderFile
#define SALOMEDS_AttributeTreeNode_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeTreeNode.hxx"

class SALOMEDS_AttributeTreeNode_i: public virtual POA_SALOMEDS::AttributeTreeNode,
                                    public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeTreeNode_i(const Handle(SALOMEDSImpl_AttributeTreeNode)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 
 
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
