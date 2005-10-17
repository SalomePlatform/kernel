//  File   : SALOMEDS_AttributeSequenceOfInteger.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeSequenceOfInteger_HeaderFile
#define SALOMEDS_AttributeSequenceOfInteger_HeaderFile

#include "SALOMEDSClient_AttributeSequenceOfInteger.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeSequenceOfInteger.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include <vector>

class SALOMEDS_AttributeSequenceOfInteger: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeSequenceOfInteger
{
public:  
  SALOMEDS_AttributeSequenceOfInteger(const Handle(SALOMEDSImpl_AttributeSequenceOfInteger)& theAttr);
  SALOMEDS_AttributeSequenceOfInteger(SALOMEDS::AttributeSequenceOfInteger_ptr theAttr);
  ~SALOMEDS_AttributeSequenceOfInteger();

  virtual void Assign(const std::vector<int>& other);
  virtual std::vector<int> CorbaSequence();
  virtual void Add(int value);
  virtual void Remove(int index);
  virtual void ChangeValue(int index, int value);
  virtual int Value(int index);
  virtual int Length();

};

#endif
