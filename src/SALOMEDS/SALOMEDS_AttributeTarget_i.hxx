// File:	SALOMEDS_AttributeTarget_i.hxx
//  Created   : Fri Aug 16 10:20:05 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header:


#ifndef SALOMEDS_AttributeTarget_i_HeaderFile
#define SALOMEDS_AttributeTarget_i_HeaderFile

// IDL headers

#include "SALOMEDS_TargetAttribute.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeTarget_i: public virtual POA_SALOMEDS::AttributeTarget,
  public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeTarget_i(const Handle(SALOMEDS_TargetAttribute)& theIntAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theIntAttr; 
  };  
  ~SALOMEDS_AttributeTarget_i() {};

  virtual void Add(SALOMEDS::SObject_ptr anObject) ;
  virtual SALOMEDS::Study::ListOfSObject* Get();
  virtual void Remove(SALOMEDS::SObject_ptr anObject);
};



#endif
