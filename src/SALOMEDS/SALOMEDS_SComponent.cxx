//  File   : SALOMEDS_SComponent.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std; 

#include "SALOMEDS_SComponent.hxx"
#include "SALOMEDS_SComponent_i.hxx"
#include <string> 
#include <TCollection_AsciiString.hxx> 

SALOMEDS_SComponent::SALOMEDS_SComponent(SALOMEDS::SComponent_ptr theSComponent)
:SALOMEDS_SObject(theSComponent) 
{}

SALOMEDS_SComponent::SALOMEDS_SComponent(const Handle(SALOMEDSImpl_SComponent)& theSComponent)
:SALOMEDS_SObject(theSComponent) 
{}

SALOMEDS_SComponent::~SALOMEDS_SComponent()
{}

std::string SALOMEDS_SComponent::ComponentDataType()
{
  std::string aType;
  if(_isLocal) {
    aType = (Handle(SALOMEDSImpl_SComponent)::DownCast(GetLocalImpl()))->ComponentDataType().ToCString();
  }
  else aType = (SALOMEDS::SComponent::_narrow(GetCORBAImpl()))->ComponentDataType();

  return aType;
}

bool SALOMEDS_SComponent::ComponentIOR(std::string& theID)
{
  bool ret;
  if(_isLocal) { 
    TCollection_AsciiString anIOR;
    ret = (Handle(SALOMEDSImpl_SComponent)::DownCast(GetLocalImpl()))->ComponentIOR(anIOR);
    theID = anIOR.ToCString();
  }
  else {
    CORBA::String_var anIOR;
    ret = (SALOMEDS::SComponent::_narrow(GetCORBAImpl()))->ComponentIOR(anIOR.out());
    theID = std::string(anIOR.in());    		
  }

  return ret;
}

SALOMEDS::SComponent_ptr SALOMEDS_SComponent::GetSComponent()
{
  if(_isLocal) {
    if(!CORBA::is_nil(_corba_impl)) return SALOMEDS::SComponent::_narrow(GetCORBAImpl());
    SALOMEDS::SComponent_var aSCO = SALOMEDS_SComponent_i::New(Handle(SALOMEDSImpl_SComponent)::DownCast(GetLocalImpl()),
							       _orb);
    return aSCO._retn();
  }
  else {
    return SALOMEDS::SComponent::_narrow(GetCORBAImpl());
  }

  return SALOMEDS::SComponent::_nil();
}

