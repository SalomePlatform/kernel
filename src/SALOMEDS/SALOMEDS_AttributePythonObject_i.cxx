//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_AttributePythonObject_i.cxx
//  Author : Michael Ponikarov
//  Module : SALOME
//  $Header$

using namespace std;
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

char* SALOMEDS_AttributePythonObject_i::Store() {
  CORBA::String_var aString = GetObject();
  char* aResult = new char[strlen(aString) + 2];
  aResult[0] = IsScript()?'s':'n';
  strcpy(aResult+1, aString);
  return aResult;
}

void SALOMEDS_AttributePythonObject_i::Restore(const char* value) {
  char* aString = strdup(value);
  SetObject(aString + 1, aString[0]=='s');
}
