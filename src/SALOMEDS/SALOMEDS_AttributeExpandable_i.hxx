// File:	SALOMEDS_AttributeExpandable_i.hxx
// Created:	Fri Jul  5 10:21:01 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header:


#ifndef SALOMEDS_AttributeExpandable_i_HeaderFile
#define SALOMEDS_AttributeExpandable_i_HeaderFile

// IDL headers

#include "SALOMEDS_ExpandableAttribute.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeExpandable_i: public virtual POA_SALOMEDS::AttributeExpandable,
  public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeExpandable_i(const Handle(SALOMEDS_ExpandableAttribute)& theIntAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theIntAttr;
  };  
  ~SALOMEDS_AttributeExpandable_i() {};
  CORBA::Boolean IsExpandable();
  void SetExpandable(CORBA::Boolean value);
};



#endif
