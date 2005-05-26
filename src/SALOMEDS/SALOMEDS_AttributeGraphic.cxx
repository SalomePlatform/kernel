//  File   : SALOMEDS_AttributeGraphic.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeGraphic.hxx"

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeGraphic::SALOMEDS_AttributeGraphic(const Handle(SALOMEDSImpl_AttributeGraphic)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeGraphic::SALOMEDS_AttributeGraphic(SALOMEDS::AttributeGraphic_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeGraphic::~SALOMEDS_AttributeGraphic()
{}


bool SALOMEDS_AttributeGraphic:: GetVisibility(int theViewId)
{
  bool aValue;
  if(_isLocal) aValue = (bool)Handle(SALOMEDSImpl_AttributeGraphic)::DownCast(_local_impl)->GetVisibility(theViewId);
  else aValue = SALOMEDS::AttributeGraphic::_narrow(_corba_impl)->GetVisibility(theViewId);
  return aValue;
}
 
void SALOMEDS_AttributeGraphic::SetVisibility(int theViewId, bool theValue)
{
  if(_isLocal) Handle(SALOMEDSImpl_AttributeGraphic)::DownCast(_local_impl)->SetVisibility(theViewId, theValue);
  else SALOMEDS::AttributeGraphic::_narrow(_corba_impl)->SetVisibility(theViewId, theValue);
}
