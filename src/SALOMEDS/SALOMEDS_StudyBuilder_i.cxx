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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_StudyBuilder_i.cxx
//  Author : Seregy RUIN
//  Module : SALOME


#include "utilities.h"
#include "SALOMEDS_StudyBuilder_i.hxx"
#include "SALOMEDS_Study_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_SComponent_i.hxx"
#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDS_Driver_i.hxx"
#include "SALOMEDS.hxx"

#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_SComponent.hxx"

#include "Utils_CorbaException.hxx"
#include "Utils_ExceptHandlers.hxx"

#include <DF_Attribute.hxx>
#include <DF_Document.hxx>
#include <stdlib.h> 

using namespace std;

UNEXPECT_CATCH(SBSalomeException, SALOME::SALOME_Exception);
UNEXPECT_CATCH(SBLockProtection, SALOMEDS::StudyBuilder::LockProtection);

//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_StudyBuilder_i::SALOMEDS_StudyBuilder_i(SALOMEDSImpl_StudyBuilder* theImpl, 
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
SALOMEDS_StudyBuilder_i::~SALOMEDS_StudyBuilder_i()
{}

//============================================================================
/*! Function : NewComponent
 *  Purpose  : Create a new component (Scomponent)
 */
//============================================================================
SALOMEDS::SComponent_ptr SALOMEDS_StudyBuilder_i::NewComponent(const char* DataType)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  //char* aDataType = CORBA::string_dup(DataType);
  SALOMEDSImpl_SComponent aSCO = _impl->NewComponent(string(DataType));
  //CORBA::free_string(aDataType);
  if(aSCO.IsNull()) return SALOMEDS::SComponent::_nil();

  SALOMEDS::SComponent_var sco = SALOMEDS_SComponent_i::New (aSCO,_orb);
  return sco._retn();
}

//============================================================================
/*! Function : DefineComponentInstance
 *  Purpose  : Add IOR attribute of a Scomponent
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::DefineComponentInstance(SALOMEDS::SComponent_ptr aComponent,
						      CORBA::Object_ptr IOR)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_SComponent aSCO = _impl->GetOwner()->GetSComponent(aComponent->GetID());

  CORBA::String_var iorstr = _orb->object_to_string(IOR);
  _impl->DefineComponentInstance(aSCO, (char*)iorstr.in());
}

//============================================================================
/*! Function : RemoveComponent
 *  Purpose  : Delete a Scomponent
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::RemoveComponent(SALOMEDS::SComponent_ptr aComponent)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  ASSERT(!CORBA::is_nil(aComponent));
  SALOMEDSImpl_SComponent aSCO = _impl->GetOwner()->GetSComponent(aComponent->GetID());
  _impl->RemoveComponent(aSCO);
}

//============================================================================
/*! Function : NewObject
 *  Purpose  : Create a new SObject
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_StudyBuilder_i::NewObject(SALOMEDS::SObject_ptr theFatherObject)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  
  SALOMEDSImpl_SObject aFO, aSO;
  aFO = _impl->GetOwner()->GetSObject(theFatherObject->GetID());
  aSO = _impl->NewObject(aFO);
  if(aSO.IsNull()) return SALOMEDS::SObject::_nil();
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aSO,_orb);

  return so._retn();
}

//============================================================================
/*! Function : NewObjectToTag
 *  Purpose  :
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_StudyBuilder_i::NewObjectToTag(SALOMEDS::SObject_ptr theFatherObject,
							      CORBA::Long atag)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_SObject aFO, aSO;
  aFO = _impl->GetOwner()->GetSObject(theFatherObject->GetID());
  aSO = _impl->NewObjectToTag(aFO, atag);
  if(aSO.IsNull()) return SALOMEDS::SObject::_nil();
  SALOMEDS::SObject_var so = SALOMEDS_SObject_i::New (aSO, _orb);
  return so._retn();
}

//============================================================================
/*! Function : RemoveObject
 *  Purpose  :
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::RemoveObject(SALOMEDS::SObject_ptr anObject)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_SObject aSO = _impl->GetOwner()->GetSObject(anObject->GetID());
  _impl->RemoveObject(aSO);
}

//============================================================================
/*! Function : RemoveObjectWithChildren
 *  Purpose  :
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::RemoveObjectWithChildren(SALOMEDS::SObject_ptr anObject)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_SObject aSO = _impl->GetOwner()->GetSObject(anObject->GetID());
  _impl->RemoveObjectWithChildren(aSO);
}

//============================================================================
/*! Function : LoadWith
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::LoadWith(SALOMEDS::SComponent_ptr anSCO, 
				       SALOMEDS::Driver_ptr aDriver) throw(SALOME::SALOME_Exception)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(SBSalomeException);

  SALOMEDSImpl_SComponent aSCO = _impl->GetOwner()->GetSComponent(anSCO->GetID());
  SALOMEDS_Driver_i* driver = new SALOMEDS_Driver_i(aDriver, _orb);
  bool isDone = _impl->LoadWith(aSCO, driver); 
  delete driver;

  if(!isDone && _impl->IsError()) {
    THROW_SALOME_CORBA_EXCEPTION(_impl->GetErrorCode().c_str(),SALOME::BAD_PARAM);
  }
}


//============================================================================
/*! Function : Load
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::Load(SALOMEDS::SObject_ptr sco)
{
  MESSAGE ( "This function is not yet implemented");
}

//============================================================================
/*! Function : FindOrCreateAttribute
 *  Purpose  : Add attribute of given type to SObject, if there is attribute of such type, returns
 *  existing one
 */
//============================================================================
SALOMEDS::GenericAttribute_ptr SALOMEDS_StudyBuilder_i::FindOrCreateAttribute(SALOMEDS::SObject_ptr anObject, 
									      const char* aTypeOfAttribute)
{
  SALOMEDS::Locker lock;
  CORBA::String_var anID = anObject->GetID();
  SALOMEDSImpl_SObject aSO = _impl->GetOwner()->GetSObject(anID.inout());
  DF_Attribute* anAttr;
  try {
     anAttr = _impl->FindOrCreateAttribute(aSO, string(aTypeOfAttribute));
  }
  catch (...) {
    throw SALOMEDS::StudyBuilder::LockProtection();
  }

  SALOMEDS::GenericAttribute_var anAttribute;
  
  if(anAttr)     
     anAttribute = SALOMEDS::GenericAttribute::_duplicate(SALOMEDS_GenericAttribute_i::CreateAttribute(anAttr, _orb));

  return anAttribute._retn();
}

//============================================================================
/*! Function : FindAttribute
 *  Purpose  : Find attribute of given type assigned SObject, returns true if it is found
 */
//============================================================================

CORBA::Boolean SALOMEDS_StudyBuilder_i::FindAttribute(SALOMEDS::SObject_ptr anObject, 
						      SALOMEDS::GenericAttribute_out anAttribute, 
						      const char* aTypeOfAttribute)
{
  SALOMEDS::Locker lock;
  ASSERT(!CORBA::is_nil(anObject));
  SALOMEDSImpl_SObject aSO = _impl->GetOwner()->GetSObject(anObject->GetID());
  DF_Attribute* anAttr;

  if(!_impl->FindAttribute(aSO, anAttr, string(aTypeOfAttribute))) return false;
    
  anAttribute = SALOMEDS::GenericAttribute::_duplicate(SALOMEDS_GenericAttribute_i::CreateAttribute(anAttr, _orb));
  return true;  
}

//============================================================================
/*! Function : RemoveAttribute
 *  Purpose  : Remove attribute of given type assigned SObject
 */
//============================================================================

void SALOMEDS_StudyBuilder_i::RemoveAttribute(SALOMEDS::SObject_ptr anObject, 
					      const char* aTypeOfAttribute)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  ASSERT(!CORBA::is_nil(anObject));
  SALOMEDSImpl_SObject aSO = _impl->GetOwner()->GetSObject(anObject->GetID());
  _impl->RemoveAttribute(aSO, string(aTypeOfAttribute));
}

//============================================================================
/*! Function : Addreference
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::Addreference(SALOMEDS::SObject_ptr me, 
					   SALOMEDS::SObject_ptr theReferencedObject)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  ASSERT(!CORBA::is_nil(me));
  ASSERT(!CORBA::is_nil(theReferencedObject));
 
  SALOMEDSImpl_SObject aSO, aRefSO;
  aSO = _impl->GetOwner()->GetSObject(me->GetID());
  aRefSO = _impl->GetOwner()->GetSObject(theReferencedObject->GetID());
  _impl->Addreference(aSO, aRefSO);
}

//============================================================================
/*! Function : RemoveReference
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::RemoveReference(SALOMEDS::SObject_ptr me)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  ASSERT(!CORBA::is_nil(me));
  SALOMEDSImpl_SObject aSO = _impl->GetOwner()->GetSObject(me->GetID());
  _impl->RemoveReference(aSO);
}


//============================================================================
/*! Function : AddDirectory
 *  Purpose  : adds a new directory with a path = thePath
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::AddDirectory(const char* thePath) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  if(thePath == NULL || strlen(thePath) == 0) throw SALOMEDS::Study::StudyInvalidDirectory();
  if(!_impl->AddDirectory(string(thePath))) {
    string anErrorCode = _impl->GetErrorCode();
    if(anErrorCode == "StudyNameAlreadyUsed") throw SALOMEDS::Study::StudyNameAlreadyUsed(); 
    if(anErrorCode == "StudyInvalidDirectory") throw SALOMEDS::Study::StudyInvalidDirectory(); 
    if(anErrorCode == "StudyInvalidComponent") throw SALOMEDS::Study::StudyInvalidComponent();  
  }
}


//============================================================================
/*! Function : SetGUID
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::SetGUID(SALOMEDS::SObject_ptr anObject, const char* theGUID)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  ASSERT(!CORBA::is_nil(anObject));
  SALOMEDSImpl_SObject aSO = _impl->GetOwner()->GetSObject(anObject->GetID());
  _impl->SetGUID(aSO, string(theGUID));
}

//============================================================================
/*! Function : IsGUID
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDS_StudyBuilder_i::IsGUID(SALOMEDS::SObject_ptr anObject, const char* theGUID)
{
  SALOMEDS::Locker lock;
  ASSERT(!CORBA::is_nil(anObject));
  SALOMEDSImpl_SObject aSO = _impl->GetOwner()->GetSObject(anObject->GetID());
  return _impl->IsGUID(aSO, string(theGUID));
}


//============================================================================
/*! Function : NewCommand
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::NewCommand()
{
  SALOMEDS::Locker lock;
  _impl->NewCommand();
}

//============================================================================
/*! Function : CommitCommand
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::CommitCommand() throw (SALOMEDS::StudyBuilder::LockProtection)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(SBLockProtection);
  try {
    _impl->CommitCommand();
  }
  catch(...) {
    MESSAGE("Locked document modification !!!");
    throw SALOMEDS::StudyBuilder::LockProtection();
  }
}

//============================================================================
/*! Function : HasOpenCommand
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_StudyBuilder_i::HasOpenCommand()
{
  SALOMEDS::Locker lock;
  return _impl->HasOpenCommand();
}

//============================================================================
/*! Function : AbortCommand
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::AbortCommand()
{
  SALOMEDS::Locker lock;
  _impl->AbortCommand();
}

//============================================================================
/*! Function : Undo
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::Undo() throw (SALOMEDS::StudyBuilder::LockProtection)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(SBLockProtection);
  try {
    _impl->Undo();
  }
  catch(...) {
    MESSAGE("Locked document modification !!!");
    throw SALOMEDS::StudyBuilder::LockProtection();
  }
}

//============================================================================
/*! Function : Redo
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::Redo() throw (SALOMEDS::StudyBuilder::LockProtection)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(SBLockProtection);
  try {
    _impl->Redo();
  }
  catch(...) {
    MESSAGE("Locked document modification !!!");
    throw SALOMEDS::StudyBuilder::LockProtection();
  }
}

//============================================================================
/*! Function : GetAvailableUndos
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_StudyBuilder_i::GetAvailableUndos()
{
  SALOMEDS::Locker lock;
  return _impl->GetAvailableUndos();
}

//============================================================================
/*! Function : GetAvailableRedos
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean  SALOMEDS_StudyBuilder_i::GetAvailableRedos()
{
  SALOMEDS::Locker lock;
  return _impl->GetAvailableRedos();
}

//============================================================================
/*! Function : UndoLimit
 *  Purpose  : 
 */
//============================================================================
CORBA::Long  SALOMEDS_StudyBuilder_i::UndoLimit()
{
  SALOMEDS::Locker lock;
  return _impl->UndoLimit();
}

//============================================================================
/*! Function : UndoLimit
 *  Purpose  : 
 */
//============================================================================
void  SALOMEDS_StudyBuilder_i::UndoLimit(CORBA::Long n)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  _impl->UndoLimit(n);
}

//============================================================================
/*! Function : CheckLocked
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::CheckLocked() throw (SALOMEDS::StudyBuilder::LockProtection) 
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(SBLockProtection);
  try {
    _impl->CheckLocked();
  }
  catch(...) {
    throw SALOMEDS::StudyBuilder::LockProtection();
  }
}

//============================================================================
/*! Function : SetName
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::SetName(SALOMEDS::SObject_ptr theSO, const char* theValue)
     throw(SALOMEDS::StudyBuilder::LockProtection)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(SBLockProtection);
  CheckLocked();
 
  SALOMEDSImpl_SObject aSO = _impl->GetOwner()->GetSObject(theSO->GetID());  
  _impl->SetName(aSO, string(theValue));
}

//============================================================================
/*! Function : SetComment
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::SetComment(SALOMEDS::SObject_ptr theSO, const char* theValue)
     throw(SALOMEDS::StudyBuilder::LockProtection)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(SBLockProtection);
  CheckLocked();

  SALOMEDSImpl_SObject aSO = _impl->GetOwner()->GetSObject(theSO->GetID());  
  _impl->SetComment(aSO, string(theValue));
}

//============================================================================
/*! Function : SetIOR
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::SetIOR(SALOMEDS::SObject_ptr theSO, const char* theValue)
 throw(SALOMEDS::StudyBuilder::LockProtection)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(SBLockProtection);
  CheckLocked();

  SALOMEDSImpl_SObject aSO = _impl->GetOwner()->GetSObject(theSO->GetID());  
  _impl->SetIOR(aSO, string(theValue));
}
