//  File   : SALOMEDS_AttributeExpandable.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeExpandable.hxx"

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeExpandable::SALOMEDS_AttributeExpandable(const Handle(SALOMEDSImpl_AttributeExpandable)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeExpandable::SALOMEDS_AttributeExpandable(SALOMEDS::AttributeExpandable_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeExpandable::~SALOMEDS_AttributeExpandable()
{}

bool SALOMEDS_AttributeExpandable::IsExpandable()
{
  bool aValue;
  if(_isLocal) aValue = (bool)Handle(SALOMEDSImpl_AttributeExpandable)::DownCast(_local_impl)->IsExpandable();
  else aValue = SALOMEDS::AttributeExpandable::_narrow(_corba_impl)->IsExpandable();
  return aValue;
}
 
void SALOMEDS_AttributeExpandable::SetExpandable(bool value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeExpandable)::DownCast(_local_impl)->SetExpandable((int)value);
  else SALOMEDS::AttributeExpandable::_narrow(_corba_impl)->SetExpandable(value);
}
