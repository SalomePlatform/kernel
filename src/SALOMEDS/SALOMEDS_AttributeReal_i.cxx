//  File   : SALOMEDS_AttributeReal_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeReal_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS.hxx"
#include <sstream>

using namespace std;

CORBA::Double SALOMEDS_AttributeReal_i::Value() 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeReal)::DownCast(_impl)->Value();
}

void SALOMEDS_AttributeReal_i::SetValue(CORBA::Double value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeReal)::DownCast(_impl)->SetValue(value);
}

