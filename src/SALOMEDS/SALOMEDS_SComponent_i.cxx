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
//  File   : SALOMEDS_SComponent_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDS_SComponent_i.hxx"
#include "SALOMEDS.hxx"
#include "utilities.h"
#include <map>

using namespace std;

SALOMEDS::SComponent_ptr SALOMEDS_SComponent_i::New(const Handle(SALOMEDSImpl_SComponent)& theImpl, CORBA::ORB_ptr theORB)
{
/*
  static std::map<SALOMEDSImpl_SComponent*, SALOMEDS_SComponent_i*> _mapOfSCO;
  SALOMEDS::SComponent_var sco;
  SALOMEDS_SComponent_i* sco_servant = NULL;

  if(_mapOfSCO.find(theImpl.operator->()) != _mapOfSCO.end()) {
    sco_servant = _mapOfSCO[theImpl.operator->()];
  }
  else {
    sco_servant = new SALOMEDS_SComponent_i(theImpl, theORB);
    _mapOfSCO[theImpl.operator->()] = sco_servant;
  }

  sco  = SALOMEDS::SComponent::_narrow(sco_servant->SComponent::_this()); 
*/
  SALOMEDS_SComponent_i* sco_servant = new SALOMEDS_SComponent_i(theImpl, theORB);
  SALOMEDS::SComponent_var sco  = SALOMEDS::SComponent::_narrow(sco_servant->SComponent::_this()); 

  return sco._retn();
}    

//============================================================================
/*! Function : constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDS_SComponent_i::SALOMEDS_SComponent_i(const Handle(SALOMEDSImpl_SComponent)& theImpl, CORBA::ORB_ptr orb)
  :SALOMEDS_SObject_i(theImpl, orb)
{}
  
//============================================================================
/*! Function : destructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDS_SComponent_i::~SALOMEDS_SComponent_i()
{}
   
//============================================================================
/*! Function : ComponentDataType
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_SComponent_i::ComponentDataType()
{
  SALOMEDS::Locker lock;
  TCollection_AsciiString aType = Handle(SALOMEDSImpl_SComponent)::DownCast(_impl)->ComponentDataType();
  return CORBA::string_dup(aType.ToCString());
}
  

//============================================================================
/*! Function : ComponentIOR
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_SComponent_i::ComponentIOR(CORBA::String_out IOR)
{
  SALOMEDS::Locker lock;
  TCollection_AsciiString ior;
  if(!Handle(SALOMEDSImpl_SComponent)::DownCast(_impl)->ComponentIOR(ior)) {
    IOR = CORBA::string_dup("");
    return false;
  }
  IOR = CORBA::string_dup(ior.ToCString());
  return true;
}
