using namespace std;
//  File      : SALOMEDS_AttributeName_i.cxx
//  Created   : Tue Aug 13 15:05:03 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 


#include "SALOMEDS_AttributeUserID_i.hxx"
#include <TCollection_ExtendedString.hxx>
#include "SALOMEDS_SObject_i.hxx"

char* SALOMEDS_AttributeUserID_i::Value() {
  char aGUID[40];
  Handle(TDataStd_UAttribute)::DownCast(_myAttr)->ID().ToCString(aGUID);
  CORBA::String_var c_s = CORBA::string_dup(aGUID);
  return c_s._retn();
}

void SALOMEDS_AttributeUserID_i::SetValue(const char* value) {
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  Handle(TDataStd_UAttribute)::DownCast(_myAttr)->SetID(Standard_GUID(Standard_CString(Str)));
}
