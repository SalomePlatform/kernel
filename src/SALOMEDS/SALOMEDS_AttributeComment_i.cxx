//  File   : SALOMEDS_AttributeComment_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeComment_i.hxx"

#include <TCollection_ExtendedString.hxx>
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS.hxx"

char* SALOMEDS_AttributeComment_i::Value()
{
  SALOMEDS::Locker lock;
  
  CORBA::String_var c_s =
    CORBA::string_dup(TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeComment)::DownCast(_impl)->Value()).ToCString());
  return c_s._retn();
}

void SALOMEDS_AttributeComment_i::SetValue(const char* value) 
{
  SALOMEDS::Locker lock; 

  CheckLocked();
  TCollection_AsciiString aStr((char*)value);
  Handle(SALOMEDSImpl_AttributeComment)::DownCast(_impl)->SetValue(TCollection_ExtendedString(aStr));
}
