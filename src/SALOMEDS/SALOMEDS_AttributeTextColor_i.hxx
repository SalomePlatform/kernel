// File:	SALOMEDS_AttributeTextColor_i.hxx
// Created:	Fri Jul  5 10:21:15 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 


#ifndef SALOMEDS_AttributeTextColor_i_HeaderFile
#define SALOMEDS_AttributeTextColor_i_HeaderFile

// IDL headers
#include "SALOMEDS_TextColorAttribute.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeTextColor_i: public virtual POA_SALOMEDS::AttributeTextColor,
				public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeTextColor_i(const Handle(SALOMEDS_TextColorAttribute)& theTextColorAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theTextColorAttr;
  };
  ~SALOMEDS_AttributeTextColor_i() {};

  SALOMEDS::Color TextColor();
  void SetTextColor(const SALOMEDS::Color& value);
};


#endif
