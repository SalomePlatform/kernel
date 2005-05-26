//  File   : SALOMEDS_AttributePixMap_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributePixMap_i.hxx"
#include "SALOMEDS.hxx"
#include <TCollection_AsciiString.hxx>

CORBA::Boolean SALOMEDS_AttributePixMap_i::HasPixMap() 
{
  return Handle(SALOMEDSImpl_AttributePixMap)::DownCast(_impl)->HasPixMap();
}

char* SALOMEDS_AttributePixMap_i::GetPixMap() 
{
  SALOMEDS::Locker lock;
  CORBA::String_var S = 
    CORBA::string_dup(TCollection_AsciiString(Handle(SALOMEDSImpl_AttributePixMap)::DownCast(_impl)->GetPixMap()).ToCString());
  return S._retn();
}

void SALOMEDS_AttributePixMap_i::SetPixMap(const char* value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  CORBA::String_var Str = CORBA::string_dup(value);
  Handle(SALOMEDSImpl_AttributePixMap)::DownCast(_impl)->SetPixMap(TCollection_ExtendedString(Str));
}

