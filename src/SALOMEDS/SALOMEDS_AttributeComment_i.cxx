using namespace std;
//  File      : SALOMEDS_AttributeComment_i.cxx
//  Created   : Fri Jul 05 14:55:07 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#include "SALOMEDS_AttributeComment_i.hxx"

#include <TCollection_ExtendedString.hxx>
#include "SALOMEDS_SObject_i.hxx"

char* SALOMEDS_AttributeComment_i::Value()
{
  TCollection_ExtendedString S = Handle(TDataStd_Comment)::DownCast(_myAttr)->Get();
  CORBA::String_var c_s = CORBA::string_dup(TCollection_AsciiString(S).ToCString());
  return c_s._retn();
}

void SALOMEDS_AttributeComment_i::SetValue(const char* value) 
{
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  Handle(TDataStd_Comment)::DownCast(_myAttr)->Set(TCollection_ExtendedString(Str));
}
