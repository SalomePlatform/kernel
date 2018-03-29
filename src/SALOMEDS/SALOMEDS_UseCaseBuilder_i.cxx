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

//  File   : SALOMEDS_UseCaseBuilder_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_UseCaseBuilder_i.hxx"
#include "SALOMEDS_UseCaseIterator_i.hxx"
#include "SALOMEDS_SObject_i.hxx"  
#include "SALOMEDS.hxx"

#include "utilities.h"

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
  CORBA::String_var id = theObject->GetID();
  return _impl->Append(_impl->GetSObject(  id.in() ));
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
  CORBA::String_var id = theObject->GetID();
  return _impl->Remove(_impl->GetSObject( id.in() ));
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
  CORBA::String_var idF = theFather->GetID();
  CORBA::String_var idO = theObject->GetID();
  return _impl->AppendTo(_impl->GetSObject(idF.in()), _impl->GetSObject( idO.in()));
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
  CORBA::String_var idF = theFirst->GetID();
  CORBA::String_var idN = theNext->GetID();
  return _impl->InsertBefore(_impl->GetSObject(idF.in()), _impl->GetSObject(idN.in()));
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
  CORBA::String_var id = theObject->GetID();
  return _impl->SetCurrentObject(_impl->GetSObject( id.in()));
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
  CORBA::String_var id = theObject->GetID();
  return _impl->HasChildren(_impl->GetSObject( id.in()));
}

//============================================================================
/*! Function : SortChildren
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::SortChildren(SALOMEDS::SObject_ptr theObject, CORBA::Boolean theAscendingOrder)
{
  SALOMEDS::Locker lock;
  if(!_impl) return 0;
  CORBA::String_var id = theObject->GetID();
  return _impl->SortChildren(_impl->GetSObject( id.in()), theAscendingOrder);
}

//============================================================================
/*! Function : GetFather
 *  Purpose  :
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_UseCaseBuilder_i::GetFather(SALOMEDS::SObject_ptr theObject)
{
  SALOMEDS::Locker lock; 

  if(!_impl) return NULL;
  CORBA::String_var id = theObject->GetID();
  SALOMEDSImpl_SObject aSO = _impl->GetFather(_impl->GetSObject( id.in()));
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aSO, _orb);
  return so._retn();
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
  return _impl->SetName(theName);
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
  CORBA::String_var id = theObject->GetID();
  return _impl->IsUseCase(_impl->GetSObject( id.in() ));
}

//============================================================================ 
/*! Function :  IsUseCaseNode
 *  Purpose  :  
 */ 
//============================================================================ 
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::IsUseCaseNode(SALOMEDS::SObject_ptr theObject)
{
  SALOMEDS::Locker lock;
  
  if(!_impl || theObject->_is_nil()) return false;
  CORBA::String_var id = theObject->GetID();
  return _impl->IsUseCaseNode(_impl->GetSObject( id.in() ));
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
  if(!CORBA::is_nil(theObject)) {
    CORBA::String_var id = theObject->GetID();
    anItr = _impl->GetUseCaseIterator(_impl->GetSObject( id.in()));
  }
  else anItr = _impl->GetUseCaseIterator(SALOMEDSImpl_SObject());
  SALOMEDS_UseCaseIterator_i* aServant = new SALOMEDS_UseCaseIterator_i(anItr, _orb);
  SALOMEDS::UseCaseIterator_var anIterator = aServant->_this();
  return anIterator._retn(); 
}
