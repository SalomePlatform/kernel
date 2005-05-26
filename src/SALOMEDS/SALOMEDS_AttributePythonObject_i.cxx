//  File   : SALOMEDS_AttributePythonObject_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributePythonObject_i.hxx"
#include "SALOMEDS.hxx"
#include <TCollection_ExtendedString.hxx>
#include <TColStd_HArray1OfCharacter.hxx>

void SALOMEDS_AttributePythonObject_i::SetObject(const char* theSequence, CORBA::Boolean IsScript) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  char *aSeq = CORBA::string_dup(theSequence);
  Handle(SALOMEDSImpl_AttributePythonObject)::DownCast(_impl)->SetObject(aSeq, IsScript);
}

char* SALOMEDS_AttributePythonObject_i::GetObject() 
{
  SALOMEDS::Locker lock;
  char* aSeq = Handle(SALOMEDSImpl_AttributePythonObject)::DownCast(_impl)->GetObject().ToCString();
  CORBA::String_var aStr = CORBA::string_dup(aSeq);
  return aStr._retn();
}

CORBA::Boolean SALOMEDS_AttributePythonObject_i::IsScript() 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributePythonObject)::DownCast(_impl)->IsScript();
}


