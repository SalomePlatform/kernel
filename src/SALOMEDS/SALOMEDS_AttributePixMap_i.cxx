using namespace std;
//  File      : SALOMEDS_AttributePixMap_i.cxx
//  Created   : Fri Jul 05 14:38:10 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#include "SALOMEDS_AttributePixMap_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include <TCollection_AsciiString.hxx>

CORBA::Boolean SALOMEDS_AttributePixMap_i::HasPixMap() {
  TCollection_ExtendedString S = Handle(SALOMEDS_PixMapAttribute)::DownCast(_myAttr)->Get();
  if (strcmp(TCollection_AsciiString(S).ToCString(), "None") == 0) return Standard_False;
  return Standard_True;
}

char* SALOMEDS_AttributePixMap_i::GetPixMap() {
 CORBA::String_var S = CORBA::string_dup(TCollection_AsciiString(Handle(SALOMEDS_PixMapAttribute)::DownCast(_myAttr)->Get()).ToCString());
 return S._retn();
}

void SALOMEDS_AttributePixMap_i::SetPixMap(const char* value) {
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  Handle(TDataStd_Comment)::DownCast(_myAttr)->Set(TCollection_ExtendedString(Str));
}
