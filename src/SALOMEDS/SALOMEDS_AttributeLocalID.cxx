//  File   : SALOMEDS_AttributeLocalID.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeLocalID.hxx"

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeLocalID::SALOMEDS_AttributeLocalID(const Handle(SALOMEDSImpl_AttributeLocalID)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeLocalID::SALOMEDS_AttributeLocalID(SALOMEDS::AttributeLocalID_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeLocalID::~SALOMEDS_AttributeLocalID()
{}

int SALOMEDS_AttributeLocalID::Value()
{
  int aValue;
  if(_isLocal) aValue = (bool)Handle(SALOMEDSImpl_AttributeLocalID)::DownCast(_local_impl)->Value();
  else aValue = SALOMEDS::AttributeLocalID::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributeLocalID::SetValue(int value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeLocalID)::DownCast(_local_impl)->SetValue(value);
  else SALOMEDS::AttributeLocalID::_narrow(_corba_impl)->SetValue(value);
}
