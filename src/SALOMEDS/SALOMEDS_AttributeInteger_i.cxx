using namespace std;
//  File      : SALOMEDS_AttributeInteger_i.cxx
//  Created   : Fri Jul 05 14:25:13 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#include "SALOMEDS_AttributeInteger_i.hxx"
#include "SALOMEDS_SObject_i.hxx"


CORBA::Long SALOMEDS_AttributeInteger_i::Value() {
  return Handle(TDataStd_Integer)::DownCast(_myAttr)->Get();
}

void SALOMEDS_AttributeInteger_i::SetValue(CORBA::Long value) {
  CheckLocked();
  Handle(TDataStd_Integer)::DownCast(_myAttr)->Set(value);
}
