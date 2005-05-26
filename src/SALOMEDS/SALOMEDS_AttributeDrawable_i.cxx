//  File   : SALOMEDS_AttributeDrawable_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


using namespace std;
#include "SALOMEDS_AttributeDrawable_i.hxx"
#include "SALOMEDS.hxx"

CORBA::Boolean SALOMEDS_AttributeDrawable_i::IsDrawable() 
{
  SALOMEDS::Locker lock;
  return (Handle(SALOMEDSImpl_AttributeDrawable)::DownCast(_impl)->IsDrawable() == 1);
}
                                                           
void SALOMEDS_AttributeDrawable_i::SetDrawable(CORBA::Boolean value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeDrawable)::DownCast(_impl)->SetDrawable(value);
}

