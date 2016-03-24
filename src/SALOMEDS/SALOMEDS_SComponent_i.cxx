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

//  File   : SALOMEDS_SComponent_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_SComponent_i.hxx"
#include "SALOMEDS.hxx"
#include "utilities.h"
#include <map>

SALOMEDS::SComponent_ptr SALOMEDS_SComponent_i::New(const SALOMEDSImpl_SComponent& theImpl, CORBA::ORB_ptr theORB)
{
  SALOMEDS_SComponent_i* sco_servant = new SALOMEDS_SComponent_i(theImpl, theORB);

  return sco_servant->_this();
}    

//============================================================================
/*! Function : constructor
 *  Purpose  : 
 */
//============================================================================
SALOMEDS_SComponent_i::SALOMEDS_SComponent_i(const SALOMEDSImpl_SComponent& theImpl, CORBA::ORB_ptr orb)
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
  std::string aType = dynamic_cast<SALOMEDSImpl_SComponent*>(_impl)->ComponentDataType();
  return CORBA::string_dup(aType.c_str());
}
  

//============================================================================
/*! Function : ComponentIOR
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_SComponent_i::ComponentIOR(CORBA::String_out IOR)
{
  SALOMEDS::Locker lock;
  std::string ior;
  if(!dynamic_cast<SALOMEDSImpl_SComponent*>(_impl)->ComponentIOR(ior)) {
    IOR = CORBA::string_dup("");
    return false;
  }
  IOR = CORBA::string_dup(ior.c_str());
  return true;
}
