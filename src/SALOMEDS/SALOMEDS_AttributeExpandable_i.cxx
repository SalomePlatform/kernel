using namespace std;
//  File      : SALOMEDS_AttributeExpandable_i.cxx
//  Created   : Fri Jul 05 14:25:13 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#include "SALOMEDS_AttributeExpandable_i.hxx"
#include "SALOMEDS_SObject_i.hxx"

CORBA::Boolean SALOMEDS_AttributeExpandable_i::IsExpandable() {
  return (Handle(SALOMEDS_ExpandableAttribute)::DownCast(_myAttr)->Get() == 1);
}
                                                           
void SALOMEDS_AttributeExpandable_i::SetExpandable(CORBA::Boolean value) {
  Standard_Integer val = 0;
  if (value != 0) val = 1;
  Handle(TDataStd_Integer)::DownCast(_myAttr)->Set(val);
}
