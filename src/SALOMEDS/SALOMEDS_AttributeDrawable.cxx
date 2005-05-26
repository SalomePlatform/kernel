//  File   : SALOMEDS_AttributeDrawable.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeDrawable.hxx"

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeDrawable::SALOMEDS_AttributeDrawable(const Handle(SALOMEDSImpl_AttributeDrawable)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeDrawable::SALOMEDS_AttributeDrawable(SALOMEDS::AttributeDrawable_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeDrawable::~SALOMEDS_AttributeDrawable()
{}

bool SALOMEDS_AttributeDrawable::IsDrawable()
{
  bool aValue;
  if(_isLocal) aValue = (bool)Handle(SALOMEDSImpl_AttributeDrawable)::DownCast(_local_impl)->IsDrawable();
  else aValue = SALOMEDS::AttributeDrawable::_narrow(_corba_impl)->IsDrawable();
  return aValue;
}
 
void SALOMEDS_AttributeDrawable::SetDrawable(bool value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeDrawable)::DownCast(_local_impl)->SetDrawable((int)value);
  else SALOMEDS::AttributeDrawable::_narrow(_corba_impl)->SetDrawable(value);
}
