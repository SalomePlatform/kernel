using namespace std;
//  File      : SALOMEDS_AttributeIOR_i.cxx
//  Created   : Fri Jul 05 15:17:19 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#include "SALOMEDS_AttributeIOR_i.hxx"


#include <TCollection_ExtendedString.hxx>
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_Study_i.hxx"

char* SALOMEDS_AttributeIOR_i::Value()
{
  TCollection_ExtendedString S = Handle(SALOMEDS_IORAttribute)::DownCast(_myAttr)->Get();
  CORBA::String_var c_s = CORBA::string_dup(TCollection_AsciiString(S).ToCString());
  return c_s._retn();
}

void SALOMEDS_AttributeIOR_i::SetValue(const char* value) 
{
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  Handle(TDataStd_Comment)::DownCast(_myAttr)->Set(TCollection_ExtendedString(Str));
  SALOMEDS_Study_i::IORUpdated(Handle(SALOMEDS_IORAttribute)::DownCast(_myAttr),_myOrb);
}
