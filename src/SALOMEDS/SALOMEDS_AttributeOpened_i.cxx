using namespace std;
//  File      : SALOMEDS_AttributeOpened_i.cxx
//  Created   : Fri Jul 05 14:25:13 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#include "SALOMEDS_AttributeOpened_i.hxx"
#include "SALOMEDS_SObject_i.hxx"

CORBA::Boolean SALOMEDS_AttributeOpened_i::IsOpened() {
  return (Handle(SALOMEDS_OpenedAttribute)::DownCast(_myAttr)->Get() == 1);
}
                                                           
void SALOMEDS_AttributeOpened_i::SetOpened(CORBA::Boolean value) {
  Standard_Integer val = 0;
  if (value != 0) val = 1;
  Handle(TDataStd_Integer)::DownCast(_myAttr)->Set(val);
}
