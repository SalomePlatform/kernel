using namespace std;
//  File      : SALOMEDS_AttributeLocalID_i.cxx
//  Created   : Tue Aug 13 14:05:03 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 


#include "SALOMEDS_AttributeLocalID_i.hxx"
#include "SALOMEDS_SObject_i.hxx"

CORBA::Long SALOMEDS_AttributeLocalID_i::Value() {
  return Handle(SALOMEDS_LocalIDAttribute)::DownCast(_myAttr)->Get();
}
                                                           
void SALOMEDS_AttributeLocalID_i::SetValue(CORBA::Long value) {
  CheckLocked();
  Handle(TDataStd_Integer)::DownCast(_myAttr)->Set(value);
}
