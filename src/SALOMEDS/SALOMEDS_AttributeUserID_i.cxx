//  File   : SALOMEDS_AttributeName_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeUserID_i.hxx"
#include "SALOMEDS.hxx"
#include <TCollection_ExtendedString.hxx>

char* SALOMEDS_AttributeUserID_i::Value() 
{
  SALOMEDS::Locker lock;
  char aGUID[40];
  Handle(SALOMEDSImpl_AttributeUserID)::DownCast(_impl)->ID().ToCString(aGUID);
  CORBA::String_var c_s = CORBA::string_dup(aGUID);
  return c_s._retn();
}

void SALOMEDS_AttributeUserID_i::SetValue(const char* value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  Handle(SALOMEDSImpl_AttributeUserID)::DownCast(_impl)->SetValue(Standard_GUID(Standard_CString(Str)));
}

