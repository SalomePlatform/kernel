using namespace std;
//  File      : SALOMEDS_AttributeDrawable_i.cxx
//  Created   : Fri Jul 05 14:25:13 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#include "SALOMEDS_AttributeDrawable_i.hxx"
#include "SALOMEDS_SObject_i.hxx"

CORBA::Boolean SALOMEDS_AttributeDrawable_i::IsDrawable() {
  return (Handle(SALOMEDS_DrawableAttribute)::DownCast(_myAttr)->Get() == 1);
}
                                                           
void SALOMEDS_AttributeDrawable_i::SetDrawable(CORBA::Boolean value) {
  CheckLocked();
  Standard_Integer val = 0;
  if (value != 0) val = 1;
  Handle(TDataStd_Integer)::DownCast(_myAttr)->Set(val);
}
