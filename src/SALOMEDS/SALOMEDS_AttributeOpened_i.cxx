//  File   : SALOMEDS_AttributeOpened_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeOpened_i.hxx"
#include "SALOMEDS.hxx"

CORBA::Boolean SALOMEDS_AttributeOpened_i::IsOpened() 
{
  SALOMEDS::Locker lock; 
  return (Handle(SALOMEDSImpl_AttributeOpened)::DownCast(_impl)->IsOpened() == 1);
}
                                                           
void SALOMEDS_AttributeOpened_i::SetOpened(CORBA::Boolean value) 
{
  SALOMEDS::Locker lock; 
  
  Standard_Integer val = 0;
  if (value != 0) val = 1;
  Handle(SALOMEDSImpl_AttributeOpened)::DownCast(_impl)->SetOpened(val);
}
