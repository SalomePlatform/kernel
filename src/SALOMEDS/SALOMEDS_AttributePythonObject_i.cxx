using namespace std;
// File:	SALOMEDS_AttributePythonObject_i.cxx
// Created:	Tue Oct  8 10:13:30 2002
// Author:	Michael Ponikarov
//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE
//  $Header$

#include "SALOMEDS_AttributePythonObject_i.hxx"
#include <TCollection_ExtendedString.hxx>
#include <TColStd_HArray1OfCharacter.hxx>
#include "SALOMEDS_SObject_i.hxx"

void SALOMEDS_AttributePythonObject_i::SetObject(const char* theSequence, CORBA::Boolean IsScript) {
  CheckLocked();
  char *aSeq = CORBA::string_dup(theSequence);
  Handle(SALOMEDS_PythonObjectAttribute)::DownCast(_myAttr)->SetObject(aSeq, IsScript);
}

char* SALOMEDS_AttributePythonObject_i::GetObject() {
  char* aSeq = Handle(SALOMEDS_PythonObjectAttribute)::DownCast(_myAttr)->GetObject();
  CORBA::String_var aStr = CORBA::string_dup(aSeq);
  return aStr._retn();
}

CORBA::Boolean SALOMEDS_AttributePythonObject_i::IsScript() {
  return Handle(SALOMEDS_PythonObjectAttribute)::DownCast(_myAttr)->IsScript();
}
