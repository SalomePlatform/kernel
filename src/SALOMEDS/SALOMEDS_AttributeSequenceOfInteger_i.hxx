//  File      : SALOMEDS_AttributeSequenceOfInteger_i.hxx
//  Created   : Fri Jul 05 10:57:25 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#ifndef SALOMEDS_AttributeSequenceOfInteger_i_HeaderFile
#define SALOMEDS_AttributeSequenceOfInteger_i_HeaderFile

// IDL headers
#include "SALOMEDS_SequenceOfIntegerAttribute.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeSequenceOfInteger_i: public virtual POA_SALOMEDS::AttributeSequenceOfInteger,
  public virtual SALOMEDS_GenericAttribute_i {
    
public:
  
  SALOMEDS_AttributeSequenceOfInteger_i(const Handle(SALOMEDS_SequenceOfIntegerAttribute)& theSequenceOfIntegerAttr, 
				     CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theSequenceOfIntegerAttr;
  };
  ~SALOMEDS_AttributeSequenceOfInteger_i() {};

  void Assign(const SALOMEDS::LongSeq& other);
  SALOMEDS::LongSeq* CorbaSequence();
  void Add(CORBA::Long value);
  void Remove(CORBA::Long index);
  void ChangeValue(CORBA::Long index, CORBA::Long value);
  CORBA::Long Value(CORBA::Short index);
  CORBA::Long Length();
};

#endif
