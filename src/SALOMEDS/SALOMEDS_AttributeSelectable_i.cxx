//  File   : SALOMEDS_AttributeSelectable_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeSelectable_i.hxx"
#include "SALOMEDS.hxx"

CORBA::Boolean SALOMEDS_AttributeSelectable_i::IsSelectable() 
{
  SALOMEDS::Locker lock;
  return (Handle(SALOMEDSImpl_AttributeSelectable)::DownCast(_impl)->IsSelectable() == 1);
}
                                                           
void SALOMEDS_AttributeSelectable_i::SetSelectable(CORBA::Boolean value) 
{
  SALOMEDS::Locker lock;
  Standard_Integer val = 0;
  if (value != 0) val = 1;
  Handle(SALOMEDSImpl_AttributeSelectable)::DownCast(_impl)->SetSelectable(val);
}

