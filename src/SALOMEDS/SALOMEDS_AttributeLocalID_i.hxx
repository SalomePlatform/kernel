// File:	SALOMEDS_AttributeLocalID_i.hxx
//  Created   : Tue Aug 13 14:05:03 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header:


#ifndef SALOMEDS_AttributeLocalID_i_HeaderFile
#define SALOMEDS_AttributeLocalID_i_HeaderFile

// IDL headers

#include "SALOMEDS_LocalIDAttribute.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeLocalID_i: public virtual POA_SALOMEDS::AttributeLocalID,
  public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeLocalID_i(const Handle(SALOMEDS_LocalIDAttribute)& theIntAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theIntAttr; 
  };  
  ~SALOMEDS_AttributeLocalID_i() {};
  CORBA::Long Value();
  void SetValue(CORBA::Long value);
};



#endif
