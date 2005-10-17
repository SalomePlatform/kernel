//  File   : SALOMEDS_AttributeUserID.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeUserID.hxx"

#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TCollection_ExtendedString.hxx>
#include <Standard_GUID.hxx>

SALOMEDS_AttributeUserID::SALOMEDS_AttributeUserID(const Handle(SALOMEDSImpl_AttributeUserID)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeUserID::SALOMEDS_AttributeUserID(SALOMEDS::AttributeUserID_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeUserID::~SALOMEDS_AttributeUserID()
{}

std::string SALOMEDS_AttributeUserID::Value()
{
  std::string aValue;
  if(_isLocal) {
    char guid[40];
    Handle(SALOMEDSImpl_AttributeUserID)::DownCast(_local_impl)->Value().ToCString(guid);
    aValue = std::string(guid);
  }
  else aValue = SALOMEDS::AttributeUserID::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributeUserID::SetValue(const std::string& value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeUserID)::DownCast(_local_impl)->SetValue(Standard_GUID((char*)value.c_str()));
  else SALOMEDS::AttributeUserID::_narrow(_corba_impl)->SetValue(value.c_str());
}
