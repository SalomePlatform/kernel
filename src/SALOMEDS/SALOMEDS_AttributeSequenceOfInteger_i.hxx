//  File   : SALOMEDS_AttributeSequenceOfInteger_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeSequenceOfInteger_i_HeaderFile
#define SALOMEDS_AttributeSequenceOfInteger_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeSequenceOfInteger.hxx"

class SALOMEDS_AttributeSequenceOfInteger_i: public virtual POA_SALOMEDS::AttributeSequenceOfInteger,
                                             public virtual SALOMEDS_GenericAttribute_i 
{    
public:
  
  SALOMEDS_AttributeSequenceOfInteger_i(const Handle(SALOMEDSImpl_AttributeSequenceOfInteger)& theAttr,CORBA::ORB_ptr orb)
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

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
