using namespace std;
//  File      : SALOMEDS_AttributeTarget_i.cxx
//  Created   : Fri Aug 16 10:20:05 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 


#include "SALOMEDS_AttributeTarget_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include <TDF_LabelList.hxx>
#include <TDF_ListIteratorOfLabelList.hxx>

void SALOMEDS_AttributeTarget_i::Add(SALOMEDS::SObject_ptr anObject) {
  TDF_Label aLabel;
  TDF_Tool::Label(_myAttr->Label().Data(),anObject->GetID(),aLabel,1);
  (Handle(SALOMEDS_TargetAttribute)::DownCast(_myAttr))->Append(aLabel);
}

SALOMEDS::Study::ListOfSObject* SALOMEDS_AttributeTarget_i::Get() {
  TDF_LabelList aLList;
  SALOMEDS::Study::ListOfSObject_var aSList = new SALOMEDS::Study::ListOfSObject;
  (Handle(SALOMEDS_TargetAttribute)::DownCast(_myAttr))->Get(aLList);
  aSList->length(aLList.Extent());
  TDF_ListIteratorOfLabelList anIter(aLList);
  int index;
  for(index=0;anIter.More();anIter.Next(),index++) {
    SALOMEDS_SObject_i* anSO = new SALOMEDS_SObject_i(anIter.Value(),_myOrb);
    aSList[index] = anSO->_this();
  }
  return aSList._retn();
}

void SALOMEDS_AttributeTarget_i::Remove(SALOMEDS::SObject_ptr anObject) {
  TDF_Label aLabel;
  TDF_Tool::Label(_myAttr->Label().Data(),anObject->GetID(),aLabel,1);
  (Handle(SALOMEDS_TargetAttribute)::DownCast(_myAttr))->Remove(aLabel);
}
