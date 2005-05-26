//  File   : SALOMEDS_AttributeName_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeName_i.hxx"
#include "SALOMEDS.hxx"
#include <TCollection_ExtendedString.hxx>

char* SALOMEDS_AttributeName_i::Value() 
{
  SALOMEDS::Locker lock; 
  CORBA::String_var c_s = 
    CORBA::string_dup(TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeName)::DownCast(_impl)->Value()).ToCString());
  return c_s._retn();
}

void SALOMEDS_AttributeName_i::SetValue(const char* value) 
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeName)::DownCast(_impl)->SetValue(TCollection_ExtendedString((char*)value));
}
