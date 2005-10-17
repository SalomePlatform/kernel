//  File   : SALOMEDS_AttributeInteger.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeInteger.hxx"

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeInteger::SALOMEDS_AttributeInteger(const Handle(SALOMEDSImpl_AttributeInteger)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeInteger::SALOMEDS_AttributeInteger(SALOMEDS::AttributeInteger_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeInteger::~SALOMEDS_AttributeInteger()
{}

int SALOMEDS_AttributeInteger::Value()
{
  int aValue;
  if(_isLocal) aValue = Handle(SALOMEDSImpl_AttributeInteger)::DownCast(_local_impl)->Value();
  else aValue = SALOMEDS::AttributeInteger::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributeInteger::SetValue(int value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeInteger)::DownCast(_local_impl)->SetValue(value);
  else SALOMEDS::AttributeInteger::_narrow(_corba_impl)->SetValue(value);
}
