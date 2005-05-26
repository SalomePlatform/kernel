//  File   : SALOMEDS_AttributeSequenceOfSequenceOfReal_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeSequenceOfSequenceOfReal_i_HeaderFile
#define SALOMEDS_AttributeSequenceOfSequenceOfReal_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeSequenceOfReal.hxx"

class SALOMEDS_AttributeSequenceOfReal_i: public virtual POA_SALOMEDS::AttributeSequenceOfReal,
					  public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeSequenceOfReal_i(const Handle(SALOMEDSImpl_AttributeSequenceOfReal)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributeSequenceOfReal_i() {};

  void Assign(const SALOMEDS::DoubleSeq& other);
  SALOMEDS::DoubleSeq* CorbaSequence();
  void Add(CORBA::Double value);
  void Remove(CORBA::Long index);
  void ChangeValue(CORBA::Long index, CORBA::Double value);
  CORBA::Double Value(CORBA::Short index);
  CORBA::Long Length();

};



#endif
