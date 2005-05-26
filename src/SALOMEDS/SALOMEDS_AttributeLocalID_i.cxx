//  File   : SALOMEDS_AttributeLocalID_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeLocalID_i.hxx"
#include "SALOMEDS.hxx"

CORBA::Long SALOMEDS_AttributeLocalID_i::Value() 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeLocalID)::DownCast(_impl)->Value();
}
                                                           
void SALOMEDS_AttributeLocalID_i::SetValue(CORBA::Long value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeLocalID)::DownCast(_impl)->SetValue(value);
}

