//  File   : SALOMEDS_AttributePythonObject.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributePythonObject.hxx"

#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributePythonObject::SALOMEDS_AttributePythonObject(const Handle(SALOMEDSImpl_AttributePythonObject)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributePythonObject::SALOMEDS_AttributePythonObject(SALOMEDS::AttributePythonObject_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributePythonObject::~SALOMEDS_AttributePythonObject()
{}

bool SALOMEDS_AttributePythonObject::IsScript()
{
  bool ret;
  if(_isLocal) ret = Handle(SALOMEDSImpl_AttributePythonObject)::DownCast(_local_impl)->IsScript();
  else ret = SALOMEDS::AttributePythonObject::_narrow(_corba_impl)->IsScript();
  return ret;
}

std::string SALOMEDS_AttributePythonObject::GetObject()
{
  std::string aValue;
  if(_isLocal) 
    aValue = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributePythonObject)::DownCast(_local_impl)->GetObject()).ToCString();
  else aValue = SALOMEDS::AttributePythonObject::_narrow(_corba_impl)->GetObject();
  return aValue;
}
 
void SALOMEDS_AttributePythonObject::SetObject(const std::string& theSequence, bool IsScript)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributePythonObject)::DownCast(_local_impl)->SetObject((char*)theSequence.c_str(), IsScript);
  else SALOMEDS::AttributePythonObject::_narrow(_corba_impl)->SetObject(theSequence.c_str(), IsScript);
}
