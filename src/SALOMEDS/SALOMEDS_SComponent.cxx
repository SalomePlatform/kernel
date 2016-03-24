// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  File   : SALOMEDS_SComponent.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_SComponent.hxx"

#include "SALOMEDS.hxx"
#include "SALOMEDS_SComponent_i.hxx"

#include <string> 

SALOMEDS_SComponent::SALOMEDS_SComponent(SALOMEDS::SComponent_ptr theSComponent)
:SALOMEDS_SObject(theSComponent) 
{}

SALOMEDS_SComponent::SALOMEDS_SComponent(const SALOMEDSImpl_SComponent& theSComponent)
:SALOMEDS_SObject(theSComponent) 
{}

SALOMEDS_SComponent::~SALOMEDS_SComponent()
{}

std::string SALOMEDS_SComponent::ComponentDataType()
{
  std::string aType;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aType = (dynamic_cast<SALOMEDSImpl_SComponent*>(GetLocalImpl()))->ComponentDataType();
  }
  else 
    {
      SALOMEDS::SComponent_var aCompo=SALOMEDS::SComponent::_narrow(GetCORBAImpl());
      CORBA::String_var aString = aCompo->ComponentDataType();
      aType=aString.in();
    }

  return aType;
}

bool SALOMEDS_SComponent::ComponentIOR(std::string& theID)
{
  bool ret;
  if (_isLocal) { 
    SALOMEDS::Locker lock;
    ret = (dynamic_cast<SALOMEDSImpl_SComponent*>(GetLocalImpl()))->ComponentIOR(theID);
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
    SALOMEDS::SComponent_var aSCO = SALOMEDS_SComponent_i::New(*(dynamic_cast<SALOMEDSImpl_SComponent*>(GetLocalImpl())), _orb);
    _corba_impl = SALOMEDS::SComponent::_duplicate(aSCO);
    return aSCO._retn();
  }
  else {
    return SALOMEDS::SComponent::_narrow(GetCORBAImpl());
  }
  return SALOMEDS::SComponent::_nil();
}
