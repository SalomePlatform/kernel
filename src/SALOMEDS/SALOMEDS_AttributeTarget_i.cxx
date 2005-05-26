//  File   : SALOMEDS_AttributeTarget_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


using namespace std;
#include "SALOMEDS_AttributeTarget_i.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDS.hxx"

#include <TColStd_HSequenceOfTransient.hxx>

void SALOMEDS_AttributeTarget_i::Add(SALOMEDS::SObject_ptr anObject) 
{
  SALOMEDS::Locker lock; 
  TDF_Label aLabel;
  TDF_Tool::Label(_impl->Label().Data(),anObject->GetID(),aLabel,1);
  (Handle(SALOMEDSImpl_AttributeTarget)::DownCast(_impl))->Add(SALOMEDSImpl_Study::SObject(aLabel));
}

SALOMEDS::Study::ListOfSObject* SALOMEDS_AttributeTarget_i::Get() 
{
  SALOMEDS::Locker lock; 
  Handle(TColStd_HSequenceOfTransient) aSeq = (Handle(SALOMEDSImpl_AttributeTarget)::DownCast(_impl))->Get();
  SALOMEDS::Study::ListOfSObject_var aSList = new SALOMEDS::Study::ListOfSObject;
  int aLength = aSeq->Length(), i;
  if (aLength == 0) return aSList._retn();
  aSList->length(aLength);
  for(i=1; i <=aLength; i++) {
    SALOMEDS::SObject_var anSO = SALOMEDS_SObject_i::New(Handle(SALOMEDSImpl_SObject)::DownCast(aSeq->Value(i)), _orb);
    aSList[i-1] = anSO;
  }
  return aSList._retn();
}

void SALOMEDS_AttributeTarget_i::Remove(SALOMEDS::SObject_ptr anObject) 
{
  SALOMEDS::Locker lock; 
  TDF_Label aLabel;
  TDF_Tool::Label(_impl->Label().Data(),anObject->GetID(),aLabel,1);
  (Handle(SALOMEDSImpl_AttributeTarget)::DownCast(_impl))->Remove(SALOMEDSImpl_Study::SObject(aLabel));
}
