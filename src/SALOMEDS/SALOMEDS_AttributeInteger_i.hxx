// File:	SALOMEDS_AttributeInteger_i.hxx
// Created:	Fri Jul  5 10:21:01 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header:


#ifndef SALOMEDS_AttributeInteger_i_HeaderFile
#define SALOMEDS_AttributeInteger_i_HeaderFile

// IDL headers

#include <TDataStd_Integer.hxx>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeInteger_i: public virtual POA_SALOMEDS::AttributeInteger,
				   public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeInteger_i(const Handle(TDataStd_Integer)& theIntAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theIntAttr;
  };  
  ~SALOMEDS_AttributeInteger_i() {};

  CORBA::Long Value();
  void SetValue(CORBA::Long value);
};



#endif
