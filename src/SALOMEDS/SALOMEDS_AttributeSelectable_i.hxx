// File:	SALOMEDS_AttributeSelectable_i.hxx
// Created:	Fri Jul  5 10:21:01 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header:


#ifndef SALOMEDS_AttributeSelectable_i_HeaderFile
#define SALOMEDS_AttributeSelectable_i_HeaderFile

// IDL headers

#include "SALOMEDS_SelectableAttribute.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeSelectable_i: public virtual POA_SALOMEDS::AttributeSelectable,
  public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeSelectable_i(const Handle(SALOMEDS_SelectableAttribute)& theIntAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theIntAttr;
  };  
  ~SALOMEDS_AttributeSelectable_i() {};
  CORBA::Boolean IsSelectable();
  void SetSelectable(CORBA::Boolean value);
};



#endif
