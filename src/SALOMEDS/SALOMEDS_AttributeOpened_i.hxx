// File:	SALOMEDS_AttributeOpened_i.hxx
// Created:	Fri Jul  5 10:21:01 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header:


#ifndef SALOMEDS_AttributeOpened_i_HeaderFile
#define SALOMEDS_AttributeOpened_i_HeaderFile

// IDL headers

#include "SALOMEDS_OpenedAttribute.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeOpened_i: public virtual POA_SALOMEDS::AttributeOpened,
  public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeOpened_i(const Handle(SALOMEDS_OpenedAttribute)& theIntAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theIntAttr; 
  };  
  ~SALOMEDS_AttributeOpened_i() {};
  CORBA::Boolean IsOpened();
  void SetOpened(CORBA::Boolean value);
};



#endif
