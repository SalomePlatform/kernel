//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_GenericAttribute_i.cxx
//  Author : Mikhail PONIKAROV
//  Module : SALOME
//  $Header$

#include "SALOMEDS_StudyBuilder_i.hxx"
using namespace std;

void SALOMEDS_GenericAttribute_i::CheckLocked() throw (SALOMEDS::GenericAttribute::LockProtection) {
  if (!_myAttr.IsNull() && _myAttr->IsValid() && !CORBA::is_nil(_myOrb)) {
    Handle(SALOMEDS_IORAttribute) Att;
    if (_myAttr->Label().Root().FindAttribute(SALOMEDS_IORAttribute::GetID(),Att)){
      TCollection_AsciiString ch(Att->Get());
      char* IOR = CORBA::string_dup(ch.ToCString());
      CORBA::Object_var obj = _myOrb->string_to_object(IOR);
      SALOMEDS::Study_ptr aStudy = SALOMEDS::Study::_narrow(obj);
      //  	ASSERT(!CORBA::is_nil(aStudy));
      if (aStudy->NewBuilder()->HasOpenCommand()) return;
      if (aStudy->GetProperties()->IsLocked()) {
	//  	  INFOS("Locked document modification !!!");
	throw SALOMEDS::GenericAttribute::LockProtection();
      }
    }
  }
}
