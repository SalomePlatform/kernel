//  File   : SALOMEDS_AttributeComment.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeComment.hxx"

#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeComment::SALOMEDS_AttributeComment(const Handle(SALOMEDSImpl_AttributeComment)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeComment::SALOMEDS_AttributeComment(SALOMEDS::AttributeComment_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeComment::~SALOMEDS_AttributeComment()
{}

std::string SALOMEDS_AttributeComment::Value()
{
  std::string aValue;
  if(_isLocal) 
    aValue = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeComment)::DownCast(_local_impl)->Value()).ToCString();
  else aValue = SALOMEDS::AttributeComment::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributeComment::SetValue(const std::string& value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeComment)::DownCast(_local_impl)->SetValue((char*)value.c_str());
  else SALOMEDS::AttributeComment::_narrow(_corba_impl)->SetValue(value.c_str());
}
