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

//  File   : SALOMEDS_ChildIterator_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_ChildIterator_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "utilities.h"

//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_ChildIterator_i::SALOMEDS_ChildIterator_i(const SALOMEDSImpl_ChildIterator& theImpl,
                                                   CORBA::ORB_ptr orb) 
  : _it(theImpl.GetPersistentCopy())
{
  SALOMEDS::Locker lock;
  _orb = CORBA::ORB::_duplicate(orb);
}

//============================================================================
/*! Function : destructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_ChildIterator_i::~SALOMEDS_ChildIterator_i()
{
    if(_it) delete _it;
}

//============================================================================
/*! Function :Init
 * 
 */
//============================================================================
void SALOMEDS_ChildIterator_i::Init()
{ 
  SALOMEDS::Locker lock;
  _it->Init();
}

//============================================================================
/*! Function :InitEx
 * 
 */
//============================================================================
void SALOMEDS_ChildIterator_i::InitEx(CORBA::Boolean allLevels)
{ 
  SALOMEDS::Locker lock;
  _it->InitEx (allLevels);
}

//============================================================================
/*! Function : More
 * 
 */
//============================================================================
CORBA::Boolean SALOMEDS_ChildIterator_i::More()
{
  SALOMEDS::Locker lock;
  return _it->More();
}

 //============================================================================
/*! Function : Next
 * 
 */
//============================================================================
void SALOMEDS_ChildIterator_i::Next()
{
  SALOMEDS::Locker lock;
  _it->Next();
}


//============================================================================
/*! Function : Value
 *  Purpose  :
 */
//============================================================================

SALOMEDS::SObject_ptr SALOMEDS_ChildIterator_i::Value()
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_SObject aSO = _it->Value();
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aSO, _orb);
  return so._retn();
}

