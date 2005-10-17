//  File   : SALOMEDS_AttributeReal.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeReal.hxx"

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeReal::SALOMEDS_AttributeReal(const Handle(SALOMEDSImpl_AttributeReal)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeReal::SALOMEDS_AttributeReal(SALOMEDS::AttributeReal_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeReal::~SALOMEDS_AttributeReal()
{}

double SALOMEDS_AttributeReal::Value()
{
  double aValue;
  if(_isLocal) aValue = Handle(SALOMEDSImpl_AttributeReal)::DownCast(_local_impl)->Value();
  else aValue = SALOMEDS::AttributeReal::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributeReal::SetValue(double value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeReal)::DownCast(_local_impl)->SetValue(value);
  else SALOMEDS::AttributeReal::_narrow(_corba_impl)->SetValue(value);
}
