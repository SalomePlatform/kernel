// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
//  File   : SALOMEDS_SComponent.cxx
//  Author : Sergey RUIN
//  Module : SALOME



#include "SALOMEDS_SComponent.hxx"
#include "SALOMEDS_SComponent_i.hxx"
#include <string> 
#include <TCollection_AsciiString.hxx> 

using namespace std; 

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

