using namespace std;
//  File      : SALOMEDS_GenericAttribute_i.cxx
//  Created   : Fri Jul 05 15:49:37 2002
//  Author    : Mikhail PONIKAROV

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#include "SALOMEDS_StudyBuilder_i.hxx"

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
