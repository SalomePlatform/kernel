//  File   : SALOMEDS_AttributeExternalFileDef.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeExternalFileDef.hxx"

#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeExternalFileDef::SALOMEDS_AttributeExternalFileDef(const Handle(SALOMEDSImpl_AttributeExternalFileDef)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeExternalFileDef::SALOMEDS_AttributeExternalFileDef(SALOMEDS::AttributeExternalFileDef_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeExternalFileDef::~SALOMEDS_AttributeExternalFileDef()
{}

std::string SALOMEDS_AttributeExternalFileDef::Value()
{
  std::string aValue;
  if(_isLocal) 
   aValue = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeExternalFileDef)::DownCast(_local_impl)->Value()).ToCString();
  else aValue = SALOMEDS::AttributeExternalFileDef::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributeExternalFileDef::SetValue(const std::string& value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeExternalFileDef)::DownCast(_local_impl)->SetValue((char*)value.c_str());
  else SALOMEDS::AttributeExternalFileDef::_narrow(_corba_impl)->SetValue(value.c_str());
}
