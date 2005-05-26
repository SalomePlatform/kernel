//  File   : SALOMEDS_AttributePixMap.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributePixMap.hxx"

#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributePixMap::SALOMEDS_AttributePixMap(const Handle(SALOMEDSImpl_AttributePixMap)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributePixMap::SALOMEDS_AttributePixMap(SALOMEDS::AttributePixMap_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributePixMap::~SALOMEDS_AttributePixMap()
{}

bool SALOMEDS_AttributePixMap::HasPixMap()
{
  bool ret;
  if(_isLocal) ret = Handle(SALOMEDSImpl_AttributePixMap)::DownCast(_local_impl)->HasPixMap();
  else ret = SALOMEDS::AttributePixMap::_narrow(_corba_impl)->HasPixMap();
}

std::string SALOMEDS_AttributePixMap::GetPixMap()
{
  std::string aValue;
  if(_isLocal) 
    aValue = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributePixMap)::DownCast(_local_impl)->GetPixMap()).ToCString();
  else aValue = SALOMEDS::AttributePixMap::_narrow(_corba_impl)->GetPixMap();
  return aValue;
}
 
void SALOMEDS_AttributePixMap::SetPixMap(const std::string& value)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributePixMap)::DownCast(_local_impl)->SetPixMap((char*)value.c_str());
  else SALOMEDS::AttributePixMap::_narrow(_corba_impl)->SetPixMap(value.c_str());
}
