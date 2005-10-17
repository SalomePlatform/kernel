//  File   : SALOMEDS_AttributeSelectable.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeSelectable.hxx"

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeSelectable::SALOMEDS_AttributeSelectable(const Handle(SALOMEDSImpl_AttributeSelectable)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeSelectable::SALOMEDS_AttributeSelectable(SALOMEDS::AttributeSelectable_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeSelectable::~SALOMEDS_AttributeSelectable()
{}

bool SALOMEDS_AttributeSelectable::IsSelectable()
{
  bool aValue;
  if(_isLocal) aValue = (bool)Handle(SALOMEDSImpl_AttributeSelectable)::DownCast(_local_impl)->IsSelectable();
  else aValue = SALOMEDS::AttributeSelectable::_narrow(_corba_impl)->IsSelectable();
  return aValue;
}
 
void SALOMEDS_AttributeSelectable::SetSelectable(bool value)
{
  if(_isLocal) Handle(SALOMEDSImpl_AttributeSelectable)::DownCast(_local_impl)->SetSelectable((int)value);
  else SALOMEDS::AttributeSelectable::_narrow(_corba_impl)->SetSelectable(value);
}
