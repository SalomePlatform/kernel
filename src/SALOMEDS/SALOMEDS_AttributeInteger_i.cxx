//  File   : SALOMEDS_AttributeInteger_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeInteger_i.hxx"
#include "SALOMEDS.hxx"

CORBA::Long SALOMEDS_AttributeInteger_i::Value() 
{
  SALOMEDS::Locker lock; 
  return Handle(SALOMEDSImpl_AttributeInteger)::DownCast(_impl)->Value();
}

void SALOMEDS_AttributeInteger_i::SetValue(CORBA::Long value) 
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeInteger)::DownCast(_impl)->SetValue(value);
}


