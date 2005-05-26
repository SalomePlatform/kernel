//  File   : SALOMEDS_AttributePersistentRef.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributePersistentRef.hxx"

#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributePersistentRef::SALOMEDS_AttributePersistentRef(const Handle(SALOMEDSImpl_AttributePersistentRef)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributePersistentRef::SALOMEDS_AttributePersistentRef(SALOMEDS::AttributePersistentRef_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributePersistentRef::~SALOMEDS_AttributePersistentRef()
{}

std::string SALOMEDS_AttributePersistentRef::Value()
{
  std::string aValue;
  if(_isLocal) 
    aValue = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributePersistentRef)::DownCast(_local_impl)->Value()).ToCString();
  else aValue = SALOMEDS::AttributePersistentRef::_narrow(_corba_impl)->Value();
  return aValue;
}
 
void SALOMEDS_AttributePersistentRef::SetValue(const std::string& value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributePersistentRef)::DownCast(_local_impl)->SetValue((char*)value.c_str());
  else SALOMEDS::AttributePersistentRef::_narrow(_corba_impl)->SetValue(value.c_str());
}
