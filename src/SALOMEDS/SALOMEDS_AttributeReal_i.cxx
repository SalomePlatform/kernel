using namespace std;
//  File      : SALOMEDS_AttributeReal_i.cxx
//  Created   : Fri Jul 05 14:38:10 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#include "SALOMEDS_AttributeReal_i.hxx"
#include "SALOMEDS_SObject_i.hxx"

CORBA::Double SALOMEDS_AttributeReal_i::Value() {
  return Handle(TDataStd_Real)::DownCast(_myAttr)->Get();
}

void SALOMEDS_AttributeReal_i::SetValue(CORBA::Double value) {
  CheckLocked();
  Handle(TDataStd_Real)::DownCast(_myAttr)->Set(value);
}
