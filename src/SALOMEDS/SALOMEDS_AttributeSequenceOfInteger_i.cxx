using namespace std;
//  File      : SALOMEDS_AttributeSequenceOfInteger_i.cxx
//  Created   : Fri Jul 05 18:09:47 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#include "SALOMEDS_AttributeSequenceOfInteger_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include <TColStd_HSequenceOfInteger.hxx>


void SALOMEDS_AttributeSequenceOfInteger_i::Assign(const SALOMEDS::LongSeq& other) 
{
  CheckLocked();
  Handle(TColStd_HSequenceOfInteger) CasCadeSeq = new TColStd_HSequenceOfInteger;
  for (int i = 0; i < other.length(); i++) {
    CasCadeSeq->Append(other[i]);
  }
  Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr)->Assign(CasCadeSeq);
}
 
SALOMEDS::LongSeq* SALOMEDS_AttributeSequenceOfInteger_i::CorbaSequence()
{
  SALOMEDS::LongSeq_var CorbaSeq = new SALOMEDS::LongSeq;
  Handle(SALOMEDS_SequenceOfIntegerAttribute) CasCadeSeq = Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr);
  CorbaSeq->length(CasCadeSeq->Length());
  for (int i = 0; i < CasCadeSeq->Length(); i++) {
    CorbaSeq[i] = CasCadeSeq->Value(i+1);;
  }
  return CorbaSeq._retn();
}
 
void SALOMEDS_AttributeSequenceOfInteger_i::Add(CORBA::Long value) 
{
  CheckLocked();
  Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr)->Add(value);
}

void SALOMEDS_AttributeSequenceOfInteger_i::Remove(CORBA::Long index) 
{
  CheckLocked();
  Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr)->Remove(index);
}
 
void SALOMEDS_AttributeSequenceOfInteger_i::ChangeValue(CORBA::Long index, CORBA::Long value)
{
  CheckLocked();
  Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr)->ChangeValue(index, value);
}
 
CORBA::Long SALOMEDS_AttributeSequenceOfInteger_i::Value(CORBA::Short index) 
{
  return Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr)->Value(index);
}

CORBA::Long SALOMEDS_AttributeSequenceOfInteger_i::Length() 
{
  return Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr)->Length();
}
