// File:	SALOMEDS_AttributeDrawable_i.hxx
// Created:	Fri Jul  5 10:21:01 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header:


#ifndef SALOMEDS_AttributeDrawable_i_HeaderFile
#define SALOMEDS_AttributeDrawable_i_HeaderFile

// IDL headers

#include "SALOMEDS_DrawableAttribute.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeDrawable_i: public virtual POA_SALOMEDS::AttributeDrawable,
  public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeDrawable_i(const Handle(SALOMEDS_DrawableAttribute)& theIntAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theIntAttr;
  };  
  ~SALOMEDS_AttributeDrawable_i() {};
  CORBA::Boolean IsDrawable();
  void SetDrawable(CORBA::Boolean value);
};



#endif
