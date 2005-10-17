//  File   : SALOMEDS_AttributeExpandable_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDS_AttributeExpandable_i.hxx"
#include "SALOMEDS.hxx"

using namespace std;

CORBA::Boolean SALOMEDS_AttributeExpandable_i::IsExpandable() 
{
  SALOMEDS::Locker lock;
  return (Handle(SALOMEDSImpl_AttributeExpandable)::DownCast(_impl)->IsExpandable() == 1);
}
                                                           
void SALOMEDS_AttributeExpandable_i::SetExpandable(CORBA::Boolean value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeExpandable)::DownCast(_impl)->SetExpandable(value);
}

