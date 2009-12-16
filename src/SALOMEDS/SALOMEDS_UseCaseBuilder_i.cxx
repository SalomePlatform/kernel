//  Copyright (C) 2007-2008  CEA/DEN, EDF R&D, OPEN CASCADE
//
//  Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_UseCaseBuilder_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_UseCaseBuilder_i.hxx"
#include "SALOMEDS_UseCaseIterator_i.hxx"
#include "SALOMEDS_SObject_i.hxx"  
#include "SALOMEDS.hxx"

#include "utilities.h"

using namespace std;

//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_UseCaseBuilder_i::SALOMEDS_UseCaseBuilder_i(SALOMEDSImpl_UseCaseBuilder* theImpl,
                                                     CORBA::ORB_ptr orb)
{
  _orb = CORBA::ORB::_duplicate(orb);
  _impl = theImpl;
}

//============================================================================
/*! Function : destructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_UseCaseBuilder_i::~SALOMEDS_UseCaseBuilder_i()
{
}


//============================================================================
/*! Function : Append
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::Append(SALOMEDS::SObject_ptr theObject)
{
  SALOMEDS::Locker lock;
  if(!_impl || theObject->_is_nil()) return 0;
  return _impl->Append(_impl->GetSObject(theObject->GetID()));
}

 //============================================================================
/*! Function : Remove
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::Remove(SALOMEDS::SObject_ptr theObject)
{
  SALOMEDS::Locker lock;
  if(!_impl || theObject->_is_nil()) return 0;
  return _impl->Remove(_impl->GetSObject(theObject->GetID()));
}


//============================================================================
/*! Function : AppendTo
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::AppendTo(SALOMEDS::SObject_ptr theFather, 
                                                   SALOMEDS::SObject_ptr theObject)
{
  SALOMEDS::Locker lock;
  if(!_impl || theFather->_is_nil() || theObject->_is_nil()) return 0;
  return _impl->AppendTo(_impl->GetSObject(theFather->GetID()), _impl->GetSObject(theObject->GetID()));
}

//============================================================================
/*! Function : InsertBefore
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::InsertBefore(SALOMEDS::SObject_ptr theFirst, 
                                                       SALOMEDS::SObject_ptr theNext)
{
  SALOMEDS::Locker lock;
  if(!_impl || theFirst->_is_nil() || theNext->_is_nil()) return 0;
  return _impl->InsertBefore(_impl->GetSObject(theFirst->GetID()), _impl->GetSObject(theNext->GetID()));
}


//============================================================================
/*! Function : SetCurrentObject
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::SetCurrentObject(SALOMEDS::SObject_ptr theObject)
{
  SALOMEDS::Locker lock;
  if(!_impl || theObject->_is_nil()) return 0;
  return _impl->SetCurrentObject(_impl->GetSObject(theObject->GetID()));
}

//============================================================================
/*! Function : SetRootCurrent
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::SetRootCurrent()
{
  SALOMEDS::Locker lock;
  if(!_impl) return 0;
  return _impl->SetRootCurrent();
}

//============================================================================
/*! Function : HasChildren
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::HasChildren(SALOMEDS::SObject_ptr theObject)
{
  SALOMEDS::Locker lock;
  if(!_impl) return 0;
  return _impl->HasChildren(_impl->GetSObject(theObject->GetID()));
}

//============================================================================
/*! Function : SetName
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::SetName(const char* theName) 
{
  SALOMEDS::Locker lock;
  if(!_impl) return 0;
  return _impl->SetName((char*)theName);
}


//============================================================================
/*! Function : GetCurrentObject
 *  Purpose  :
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_UseCaseBuilder_i::GetCurrentObject()
{
  SALOMEDS::Locker lock; 
  
  if(!_impl) return NULL;
  SALOMEDSImpl_SObject aSO = _impl->GetCurrentObject();
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aSO, _orb);
  return so._retn();
}

//============================================================================
/*! Function : GetName
 *  Purpose  :
 */
//============================================================================
char* SALOMEDS_UseCaseBuilder_i::GetName() 
{
  SALOMEDS::Locker lock;
  return CORBA::string_dup(_impl->GetName().c_str());
}

//============================================================================ 
/*! Function :  IsUseCase
 *  Purpose  :  
 */ 
//============================================================================ 
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::IsUseCase(SALOMEDS::SObject_ptr theObject)
{
  SALOMEDS::Locker lock;
  
  if(!_impl || theObject->_is_nil()) return false;
  return _impl->IsUseCase(_impl->GetSObject(theObject->GetID()));
}

//============================================================================ 
/*! Function : NewUseCase 
 *  Purpose  :  
 */ 
//============================================================================ 
SALOMEDS::SObject_ptr SALOMEDS_UseCaseBuilder_i::AddUseCase(const char* theName)
{
  SALOMEDS::Locker lock;
  
  if(!_impl) return SALOMEDS::SObject::_nil();
  SALOMEDSImpl_SObject aSO = _impl->AddUseCase((char*)theName);
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aSO, _orb);
  return so._retn();
}

//============================================================================
/*! Function : GetUseCaseIterator
 *  Purpose  : Creates a new UseCase iterator, if anObject is null all use cases are iterated 
 */
//============================================================================
SALOMEDS::UseCaseIterator_ptr SALOMEDS_UseCaseBuilder_i::GetUseCaseIterator(SALOMEDS::SObject_ptr theObject) 
{
  SALOMEDS::Locker lock;
  
  if(!_impl) return SALOMEDS::UseCaseIterator::_nil();
  SALOMEDSImpl_UseCaseIterator anItr;
  if(!CORBA::is_nil(theObject)) anItr = _impl->GetUseCaseIterator(_impl->GetSObject(theObject->GetID()));
  else anItr = _impl->GetUseCaseIterator(SALOMEDSImpl_SObject());
  SALOMEDS_UseCaseIterator_i* aServant = new SALOMEDS_UseCaseIterator_i(anItr, _orb);
  SALOMEDS::UseCaseIterator_var anIterator = SALOMEDS::UseCaseIterator::_narrow(aServant->_this());
  return anIterator._retn(); 
}
