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

//  File   : SALOMEDS_SComponentIterator_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_SComponentIterator_i.hxx"
#include "SALOMEDS.hxx"
#include "SALOMEDSImpl_SComponent.hxx"

//============================================================================
/*! Function : constructor
 * 
 */
//============================================================================

SALOMEDS_SComponentIterator_i::SALOMEDS_SComponentIterator_i(const SALOMEDSImpl_SComponentIterator& theImpl, 
                                                             CORBA::ORB_ptr orb) 
{
  _orb = CORBA::ORB::_duplicate(orb);
  _impl = theImpl.GetPersistentCopy();
}

//============================================================================
/*! Function : destructor
 * 
 */
//============================================================================
SALOMEDS_SComponentIterator_i::~SALOMEDS_SComponentIterator_i()
{
   if(_impl) delete _impl;
}

//============================================================================
/*! Function : Init
 * 
 */
//============================================================================
void SALOMEDS_SComponentIterator_i::Init()
{ 
  SALOMEDS::Locker lock; 
  _impl->Init();
}

//============================================================================
/*! Function : More
 * 
 */
//============================================================================
CORBA::Boolean SALOMEDS_SComponentIterator_i::More()
{
  SALOMEDS::Locker lock; 
  return _impl->More();
}

 //============================================================================
/*! Function : Next
  */
//============================================================================
void SALOMEDS_SComponentIterator_i::Next()
{ 
  SALOMEDS::Locker lock; 
  _impl->Next();
}


//============================================================================
/*! Function : Value
 * 
 */
//============================================================================
SALOMEDS::SComponent_ptr SALOMEDS_SComponentIterator_i::Value()
{
  SALOMEDS::Locker lock; 
  SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (_impl->Value(), _orb);
  return sco._retn();
}

