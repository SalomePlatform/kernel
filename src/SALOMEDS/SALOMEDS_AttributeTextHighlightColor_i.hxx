// File:	SALOMEDS_AttributeTextHighlightColor_i.hxx
// Created:	Fri Jul  5 10:21:15 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 


#ifndef SALOMEDS_AttributeTextHighlightColor_i_HeaderFile
#define SALOMEDS_AttributeTextHighlightColor_i_HeaderFile

// IDL headers
#include "SALOMEDS_TextHighlightColorAttribute.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeTextHighlightColor_i: public virtual POA_SALOMEDS::AttributeTextHighlightColor,
				public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeTextHighlightColor_i(const Handle(SALOMEDS_TextHighlightColorAttribute)& theTextHighlightColorAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theTextHighlightColorAttr;
  };
  ~SALOMEDS_AttributeTextHighlightColor_i() {};

  SALOMEDS::Color TextHighlightColor();
  void SetTextHighlightColor(const SALOMEDS::Color& value);
};


#endif
