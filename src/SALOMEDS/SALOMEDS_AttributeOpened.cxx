//  File   : SALOMEDS_AttributeOpened.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeOpened.hxx"

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeOpened::SALOMEDS_AttributeOpened(const Handle(SALOMEDSImpl_AttributeOpened)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeOpened::SALOMEDS_AttributeOpened(SALOMEDS::AttributeOpened_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeOpened::~SALOMEDS_AttributeOpened()
{}

bool SALOMEDS_AttributeOpened::IsOpened()
{
  bool aValue;
  if(_isLocal) aValue = (bool)Handle(SALOMEDSImpl_AttributeOpened)::DownCast(_local_impl)->IsOpened();
  else aValue = SALOMEDS::AttributeOpened::_narrow(_corba_impl)->IsOpened();
  return aValue;
}
 
void SALOMEDS_AttributeOpened::SetOpened(bool value)
{
  if(_isLocal) Handle(SALOMEDSImpl_AttributeOpened)::DownCast(_local_impl)->SetOpened((int)value);
  else SALOMEDS::AttributeOpened::_narrow(_corba_impl)->SetOpened(value);
}
