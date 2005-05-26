//  File   : SALOMEDS_AttributeSequenceOfReal_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeSequenceOfReal_i.hxx"
#include "SALOMEDS.hxx" 
#include <TColStd_HSequenceOfReal.hxx>

void SALOMEDS_AttributeSequenceOfReal_i::Assign(const SALOMEDS::DoubleSeq& other) 
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  Handle(TColStd_HSequenceOfReal) CasCadeSeq = new TColStd_HSequenceOfReal;
  for (int i = 0; i < other.length(); i++) {
    CasCadeSeq->Append(other[i]);
  }
  Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(_impl)->Assign(CasCadeSeq);
}
 
SALOMEDS::DoubleSeq* SALOMEDS_AttributeSequenceOfReal_i::CorbaSequence()
{
  SALOMEDS::Locker lock; 
  SALOMEDS::DoubleSeq_var CorbaSeq = new SALOMEDS::DoubleSeq;
  Handle(SALOMEDSImpl_AttributeSequenceOfReal) CasCadeSeq = Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(_impl);
  CorbaSeq->length(CasCadeSeq->Length());
  for (int i = 0; i < CasCadeSeq->Length(); i++) {
    CorbaSeq[i] = CasCadeSeq->Value(i+1);;
  }
  return CorbaSeq._retn();
}
 
void SALOMEDS_AttributeSequenceOfReal_i::Add(CORBA::Double value) 
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(_impl)->Add(value);
}

void SALOMEDS_AttributeSequenceOfReal_i::Remove(CORBA::Long index) 
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(_impl)->Remove(index);
}
 
void SALOMEDS_AttributeSequenceOfReal_i::ChangeValue(CORBA::Long index, CORBA::Double value)
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(_impl)->ChangeValue(index, value);
}
 
CORBA::Double SALOMEDS_AttributeSequenceOfReal_i::Value(CORBA::Short index) 
{
  SALOMEDS::Locker lock; 
  return Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(_impl)->Value(index);
}

CORBA::Long SALOMEDS_AttributeSequenceOfReal_i::Length() 
{
  SALOMEDS::Locker lock; 
  return Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(_impl)->Length();
}

