//  File   : SALOMEDS_AttributeName.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeName.hxx"

#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeName::SALOMEDS_AttributeName(const Handle(SALOMEDSImpl_AttributeName)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeName::SALOMEDS_AttributeName(SALOMEDS::AttributeName_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeName::~SALOMEDS_AttributeName()
{}

std::string SALOMEDS_AttributeName::Value()
{
  std::string aValue;
  if(_isLocal) 
    aValue = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeName)::DownCast(_local_impl)->Value()).ToCString();
  else aValue = SALOMEDS::AttributeName::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributeName::SetValue(const std::string& value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeName)::DownCast(_local_impl)->SetValue((char*)value.c_str());
  else SALOMEDS::AttributeName::_narrow(_corba_impl)->SetValue(value.c_str());
}
