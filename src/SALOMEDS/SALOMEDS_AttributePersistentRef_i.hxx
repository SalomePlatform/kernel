// File:	SALOMEDS_AttributePersistentRef_i.hxx
// Created:	Fri Jul  5 10:22:28 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 

#ifndef SALOMEDS_AttributePersistentRef_i_HeaderFile
#define SALOMEDS_AttributePersistentRef_i_HeaderFile

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDS_PersRefAttribute.hxx"

class SALOMEDS_AttributePersistentRef_i: public virtual POA_SALOMEDS::AttributePersistentRef,
					 public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributePersistentRef_i(const Handle(SALOMEDS_PersRefAttribute)& thePersRefAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = thePersRefAttr;
  }
  ~SALOMEDS_AttributePersistentRef_i() {};

  char* Value();
  void SetValue(const char* value);
  
};



#endif
