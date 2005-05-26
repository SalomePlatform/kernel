//  File   : SALOMEDS_UseCaseBuilder_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
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
SALOMEDS_UseCaseBuilder_i::SALOMEDS_UseCaseBuilder_i(const Handle(SALOMEDSImpl_UseCaseBuilder)& theImpl,
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
  if(_impl.IsNull() || theObject->_is_nil()) return 0;
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
  if(_impl.IsNull() || theObject->_is_nil()) return 0;
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
  if(_impl.IsNull() || theFather->_is_nil() || theObject->_is_nil()) return 0;
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
  if(_impl.IsNull() || theFirst->_is_nil() || theNext->_is_nil()) return 0;
  return _impl->AppendTo(_impl->GetSObject(theFirst->GetID()), _impl->GetSObject(theNext->GetID()));
}


//============================================================================
/*! Function : SetCurrentObject
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::SetCurrentObject(SALOMEDS::SObject_ptr theObject)
{
  SALOMEDS::Locker lock;
  if(_impl.IsNull() || theObject->_is_nil()) return 0;
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
  if(_impl.IsNull()) return 0;
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
  if(_impl.IsNull()) return 0;
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
  if(_impl.IsNull()) return 0;
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
  
  if(_impl.IsNull()) return NULL;
  Handle(SALOMEDSImpl_SObject) aSO = _impl->GetCurrentObject();
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
  return CORBA::string_dup(_impl->GetName().ToCString());
}

//============================================================================ 
/*! Function :  IsUseCase
 *  Purpose  :  
 */ 
//============================================================================ 
CORBA::Boolean SALOMEDS_UseCaseBuilder_i::IsUseCase(SALOMEDS::SObject_ptr theObject)
{
  SALOMEDS::Locker lock;
  
  if(_impl.IsNull() || theObject->_is_nil()) return false;
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
  
  if(_impl.IsNull()) return SALOMEDS::SObject::_nil();
  Handle(SALOMEDSImpl_SObject) aSO = _impl->AddUseCase((char*)theName);
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
  
  if(_impl.IsNull()) return SALOMEDS::UseCaseIterator::_nil();
  Handle(SALOMEDSImpl_UseCaseIterator) anItr = _impl->GetUseCaseIterator(_impl->GetSObject(theObject->GetID()));
  SALOMEDS_UseCaseIterator_i* aServant = new SALOMEDS_UseCaseIterator_i(anItr, _orb);
  SALOMEDS::UseCaseIterator_var anIterator = SALOMEDS::UseCaseIterator::_narrow(aServant->_this());
  return anIterator._retn(); 
}
