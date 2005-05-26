//  File   : SALOMEDS_AttributeIOR.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeIOR.hxx"

#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeIOR::SALOMEDS_AttributeIOR(const Handle(SALOMEDSImpl_AttributeIOR)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeIOR::SALOMEDS_AttributeIOR(SALOMEDS::AttributeIOR_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeIOR::~SALOMEDS_AttributeIOR()
{}

std::string SALOMEDS_AttributeIOR::Value()
{
  std::string aValue;
  if(_isLocal) 
    aValue = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeIOR)::DownCast(_local_impl)->Value()).ToCString();
  else aValue = SALOMEDS::AttributeIOR::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributeIOR::SetValue(const std::string& value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeIOR)::DownCast(_local_impl)->SetValue((char*)value.c_str());
  else SALOMEDS::AttributeIOR::_narrow(_corba_impl)->SetValue(value.c_str());
}
