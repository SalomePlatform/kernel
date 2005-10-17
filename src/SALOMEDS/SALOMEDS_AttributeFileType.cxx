//  File   : SALOMEDS_AttributeFileType.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeFileType.hxx"

#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeFileType::SALOMEDS_AttributeFileType(const Handle(SALOMEDSImpl_AttributeFileType)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeFileType::SALOMEDS_AttributeFileType(SALOMEDS::AttributeFileType_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeFileType::~SALOMEDS_AttributeFileType()
{}

std::string SALOMEDS_AttributeFileType::Value()
{
  std::string aValue;
  if(_isLocal) 
    aValue = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeFileType)::DownCast(_local_impl)->Value()).ToCString();
  else aValue = SALOMEDS::AttributeFileType::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributeFileType::SetValue(const std::string& value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeFileType)::DownCast(_local_impl)->SetValue((char*)value.c_str());
  else SALOMEDS::AttributeFileType::_narrow(_corba_impl)->SetValue(value.c_str());
}
