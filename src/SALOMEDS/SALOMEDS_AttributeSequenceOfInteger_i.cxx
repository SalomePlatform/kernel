//  File   : SALOMEDS_AttributeSequenceOfInteger_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeSequenceOfInteger_i.hxx"
#include "SALOMEDS.hxx"
#include <TColStd_HSequenceOfInteger.hxx>


void SALOMEDS_AttributeSequenceOfInteger_i::Assign(const SALOMEDS::LongSeq& other) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(TColStd_HSequenceOfInteger) CasCadeSeq = new TColStd_HSequenceOfInteger;
  for (int i = 0; i < other.length(); i++) {
    CasCadeSeq->Append(other[i]);
  }
  Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_impl)->Assign(CasCadeSeq);
}
 
SALOMEDS::LongSeq* SALOMEDS_AttributeSequenceOfInteger_i::CorbaSequence()
{
  SALOMEDS::Locker lock;
  SALOMEDS::LongSeq_var CorbaSeq = new SALOMEDS::LongSeq;
  Handle(SALOMEDSImpl_AttributeSequenceOfInteger) CasCadeSeq;
  CasCadeSeq = Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_impl);
  CorbaSeq->length(CasCadeSeq->Length());
  for (int i = 0; i < CasCadeSeq->Length(); i++) {
    CorbaSeq[i] = CasCadeSeq->Value(i+1);;
  }
  return CorbaSeq._retn();
}
 
void SALOMEDS_AttributeSequenceOfInteger_i::Add(CORBA::Long value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_impl)->Add(value);
}

void SALOMEDS_AttributeSequenceOfInteger_i::Remove(CORBA::Long index) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_impl)->Remove(index);
}
 
void SALOMEDS_AttributeSequenceOfInteger_i::ChangeValue(CORBA::Long index, CORBA::Long value)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_impl)->ChangeValue(index, value);
}
 
CORBA::Long SALOMEDS_AttributeSequenceOfInteger_i::Value(CORBA::Short index) 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_impl)->Value(index);
}

CORBA::Long SALOMEDS_AttributeSequenceOfInteger_i::Length() 
{  
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_impl)->Length();
}

