//  File   : SALOMEDS_AttributePersistentRef_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributePersistentRef_i.hxx"
#include "SALOMEDS.hxx"
#include <TCollection_ExtendedString.hxx>

char* SALOMEDS_AttributePersistentRef_i::Value()
{
  SALOMEDS::Locker lock;
  CORBA::String_var c_s = 
    CORBA::string_dup(TCollection_AsciiString(Handle(SALOMEDSImpl_AttributePersistentRef)::DownCast(_impl)->Value()).ToCString());
  return c_s._retn();
}

void SALOMEDS_AttributePersistentRef_i::SetValue(const char* value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  Handle(SALOMEDSImpl_AttributePersistentRef)::DownCast(_impl)->SetValue(TCollection_ExtendedString(Str));
}
