//  File   : SALOMEDS_AttributeFileType_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeFileType_i.hxx"
#include "SALOMEDS.hxx"
#include <TCollection_ExtendedString.hxx>

char* SALOMEDS_AttributeFileType_i::Value()
{
  SALOMEDS::Locker lock;
  CORBA::String_var c_s = 
    CORBA::string_dup(TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeFileType)::DownCast(_impl)->Value()).ToCString());
  return c_s._retn();
}

void SALOMEDS_AttributeFileType_i::SetValue(const char* value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  Handle(SALOMEDSImpl_AttributeFileType)::DownCast(_impl)->SetValue(TCollection_ExtendedString(Str));
}
