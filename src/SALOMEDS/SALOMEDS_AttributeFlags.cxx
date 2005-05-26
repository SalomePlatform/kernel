//  File   : SALOMEDS_AttributeFlags.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeFlags.hxx"

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>

SALOMEDS_AttributeFlags::SALOMEDS_AttributeFlags(const Handle(SALOMEDSImpl_AttributeFlags)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeFlags::SALOMEDS_AttributeFlags(SALOMEDS::AttributeFlags_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeFlags::~SALOMEDS_AttributeFlags()
{}

int SALOMEDS_AttributeFlags::GetFlags()
{
  int aValue;
  if(_isLocal) aValue = Handle(SALOMEDSImpl_AttributeFlags)::DownCast(_local_impl)->Get();
  else aValue = SALOMEDS::AttributeFlags::_narrow(_corba_impl)->GetFlags();
  return aValue;
}

void SALOMEDS_AttributeFlags::SetFlags(int theFlags)
{
  if(_isLocal) Handle(SALOMEDSImpl_AttributeFlags)::DownCast(_local_impl)->Set(theFlags);
  else SALOMEDS::AttributeFlags::_narrow(_corba_impl)->SetFlags(theFlags);
}

bool SALOMEDS_AttributeFlags::Get(int theFlag)
{
  return (GetFlags() & theFlag) ? true : false;
}
 
void SALOMEDS_AttributeFlags::Set(int theFlag, bool theValue)
{
  if(_isLocal)  {
    Handle(SALOMEDSImpl_AttributeFlags) anAttr = Handle(SALOMEDSImpl_AttributeFlags)::DownCast(_local_impl);
    if ( theValue )
      anAttr->Set( anAttr->Get() | theFlag );
    else
      anAttr->Set( anAttr->Get() & ~theFlag );    
  }
  else SALOMEDS::AttributeFlags::_narrow(_corba_impl)->Set(theFlag, theValue);
}
