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

//  File   : SALOMEDS_StudyBuilder.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "utilities.h"

#include "SALOMEDS_StudyBuilder.hxx"

#include "SALOMEDS.hxx"
#include "SALOMEDS_SObject.hxx"
#include "SALOMEDS_SComponent.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDS_StudyManager.hxx"
#include "SALOMEDS_StudyBuilder_i.hxx"

#include "SALOMEDS_Driver_i.hxx"

#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_GenericAttribute.hxx"

#include <string>
#include <stdexcept>

#include "DF_Attribute.hxx"

#include "Utils_CorbaException.hxx"
#include "Utils_ORB_INIT.hxx" 
#include "Utils_SINGLETON.hxx" 

SALOMEDS_StudyBuilder::SALOMEDS_StudyBuilder(SALOMEDSImpl_StudyBuilder* theBuilder)
{
  _isLocal = true;
  _local_impl = theBuilder;
  _corba_impl = SALOMEDS::StudyBuilder::_nil();

  init_orb();
}

SALOMEDS_StudyBuilder::SALOMEDS_StudyBuilder(SALOMEDS::StudyBuilder_ptr theBuilder)
{
  _isLocal = false;
  _local_impl = NULL;
  _corba_impl = SALOMEDS::StudyBuilder::_duplicate(theBuilder);

  init_orb();
}

SALOMEDS_StudyBuilder::~SALOMEDS_StudyBuilder() 
{
}

_PTR(SComponent) SALOMEDS_StudyBuilder::NewComponent(const std::string& ComponentDataType)
{
  SALOMEDSClient_SComponent* aSCO = NULL;

  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;

    SALOMEDSImpl_SComponent aSCO_impl =_local_impl->NewComponent(ComponentDataType);
    if(!aSCO_impl) return _PTR(SComponent)(aSCO);
    aSCO = new SALOMEDS_SComponent(aSCO_impl);
  }
  else {
    SALOMEDS::SComponent_var aSCO_impl = _corba_impl->NewComponent((char*)ComponentDataType.c_str());
    if(CORBA::is_nil(aSCO_impl)) return _PTR(SComponent)(aSCO);
    aSCO = new SALOMEDS_SComponent(aSCO_impl);
  }

  return _PTR(SComponent)(aSCO);
}

void SALOMEDS_StudyBuilder::DefineComponentInstance (const _PTR(SComponent)& theSCO, 
                                                     const std::string& ComponentIOR)
{
  if(!theSCO) return;

  SALOMEDS_SComponent* aSCO = dynamic_cast<SALOMEDS_SComponent*>(theSCO.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;

    _local_impl->DefineComponentInstance(*(dynamic_cast<SALOMEDSImpl_SComponent*>(aSCO->GetLocalImpl())),
                                         ComponentIOR);
  }
  else {
    CORBA::Object_var obj = _orb->string_to_object(ComponentIOR.c_str());
    _corba_impl->DefineComponentInstance(SALOMEDS::SComponent::_narrow(aSCO->GetCORBAImpl()), obj);
  }
}

void SALOMEDS_StudyBuilder::RemoveComponent(const _PTR(SComponent)& theSCO)
{
  if(!theSCO) return;
  SALOMEDS_SComponent* aSCO = dynamic_cast<SALOMEDS_SComponent*>(theSCO.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;

    _local_impl->RemoveComponent(*(dynamic_cast<SALOMEDSImpl_SComponent*>(aSCO->GetLocalImpl())));
  }
  else _corba_impl->RemoveComponent(SALOMEDS::SComponent::_narrow(aSCO->GetCORBAImpl()));
}

_PTR(SObject) SALOMEDS_StudyBuilder::NewObject(const _PTR(SObject)& theFatherObject)
{
  CheckLocked();

  SALOMEDSClient_SObject* aSO = NULL;
  SALOMEDS_SObject* father = dynamic_cast< SALOMEDS_SObject*>(theFatherObject.get());
  if (father == NULL) return _PTR(SObject)(aSO);
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_SObject aSO_impl = _local_impl->NewObject(*(father->GetLocalImpl()));
    if(!aSO_impl) return _PTR(SObject)(aSO);
    aSO = new SALOMEDS_SObject(aSO_impl);
  }
  else {
    SALOMEDS::SObject_var aSO_impl = _corba_impl->NewObject(father->GetCORBAImpl());
    if(CORBA::is_nil(aSO_impl)) return _PTR(SObject)(aSO);
    aSO = new SALOMEDS_SObject(aSO_impl);
  }

  return _PTR(SObject)(aSO);
}

_PTR(SObject) SALOMEDS_StudyBuilder::NewObjectToTag(const _PTR(SObject)& theFatherObject, int theTag)
{  
  CheckLocked();

  SALOMEDSClient_SObject* aSO = NULL;
  SALOMEDS_SObject* father = dynamic_cast< SALOMEDS_SObject*>(theFatherObject.get());
  if (father == NULL) return _PTR(SObject)(aSO);
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_SObject aSO_impl = _local_impl->NewObjectToTag(*(father->GetLocalImpl()), theTag);
    if(aSO_impl.IsNull()) return _PTR(SObject)(aSO);
    aSO = new SALOMEDS_SObject(aSO_impl);
  }
  else {
    SALOMEDS::SObject_var aSO_impl = _corba_impl->NewObjectToTag(father->GetCORBAImpl(), theTag);
    if(CORBA::is_nil(aSO_impl)) return _PTR(SObject)(aSO);
    aSO = new SALOMEDS_SObject(aSO_impl);
  }

  return _PTR(SObject)(aSO);
}

void SALOMEDS_StudyBuilder::AddDirectory(const std::string& thePath)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;

    _local_impl->AddDirectory((char*)thePath.c_str());
    if (_local_impl->IsError()) {
      std::string anErrorCode = _local_impl->GetErrorCode();
      if (anErrorCode == "StudyNameAlreadyUsed")  throw SALOMEDS::Study::StudyNameAlreadyUsed(); 
      if (anErrorCode == "StudyInvalidDirectory") throw SALOMEDS::Study::StudyInvalidDirectory(); 
      if (anErrorCode == "StudyInvalidComponent") throw SALOMEDS::Study::StudyInvalidComponent();  
    }
  }
  else _corba_impl->AddDirectory((char*)thePath.c_str());
}

void SALOMEDS_StudyBuilder::LoadWith(const _PTR(SComponent)& theSCO, const std::string& theIOR)
{
  if(!theSCO) return;

  SALOMEDS_SComponent* aSCO = dynamic_cast<SALOMEDS_SComponent*>(theSCO.get());
  CORBA::Object_var obj = _orb->string_to_object(theIOR.c_str());
  Engines::EngineComponent_var anEngine = Engines::EngineComponent::_narrow(obj);
  SALOMEDS::Driver_var aDriver = SALOMEDS::Driver::_narrow(obj);
  
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDS_Driver_i* drv = new SALOMEDS_Driver_i(anEngine, _orb);    
    SALOMEDSImpl_SComponent aSCO_impl = *(dynamic_cast<SALOMEDSImpl_SComponent*>(aSCO->GetLocalImpl()));
    bool isDone = _local_impl->LoadWith(aSCO_impl, drv);
    delete drv;
    if(!isDone && _local_impl->IsError()) 
      THROW_SALOME_CORBA_EXCEPTION(_local_impl->GetErrorCode().c_str(),SALOME::BAD_PARAM);
  }
  else {
    _corba_impl->LoadWith(SALOMEDS::SComponent::_narrow(aSCO->GetCORBAImpl()), aDriver);
  }
}

void SALOMEDS_StudyBuilder::Load(const _PTR(SObject)& theSCO)
{
  SALOMEDS_SComponent* aSCO = dynamic_cast<SALOMEDS_SComponent*>(theSCO.get());
  if (_isLocal) _local_impl->Load(*(dynamic_cast<SALOMEDSImpl_SComponent*>(aSCO->GetLocalImpl())));
  else _corba_impl->Load(SALOMEDS::SComponent::_narrow(aSCO->GetCORBAImpl()));
}

void SALOMEDS_StudyBuilder::RemoveObject(const _PTR(SObject)& theSO)
{
  if(!theSO) return;

  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;

    _local_impl->RemoveObject(*(aSO->GetLocalImpl()));
  }
  else _corba_impl->RemoveObject(aSO->GetCORBAImpl());
}

void SALOMEDS_StudyBuilder::RemoveObjectWithChildren(const _PTR(SObject)& theSO)
{
  if(!theSO) return;

  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;

    _local_impl->RemoveObjectWithChildren(*(aSO->GetLocalImpl()));
  }
  else _corba_impl->RemoveObjectWithChildren(aSO->GetCORBAImpl());
}

_PTR(GenericAttribute) SALOMEDS_StudyBuilder::FindOrCreateAttribute(const _PTR(SObject)& theSO, 
                                                                    const std::string& aTypeOfAttribute)
{  
  SALOMEDSClient_GenericAttribute* anAttr = NULL;
  if(!theSO) return _PTR(GenericAttribute)(anAttr);
  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;

    SALOMEDSImpl_GenericAttribute* aGA;
    try {
      aGA = dynamic_cast<SALOMEDSImpl_GenericAttribute*>
        (_local_impl->FindOrCreateAttribute(*(aSO->GetLocalImpl()), aTypeOfAttribute));
    }
    catch (...) {
      throw SALOMEDS::StudyBuilder::LockProtection();
    }
    anAttr = SALOMEDS_GenericAttribute::CreateAttribute(aGA);
  }
  else {
    SALOMEDS::GenericAttribute_var aGA =
      _corba_impl->FindOrCreateAttribute(aSO->GetCORBAImpl(), (char*)aTypeOfAttribute.c_str());
    anAttr = SALOMEDS_GenericAttribute::CreateAttribute(aGA);
  }

  return _PTR(GenericAttribute)(anAttr);
}

bool SALOMEDS_StudyBuilder::FindAttribute(const _PTR(SObject)& theSO, 
                                          _PTR(GenericAttribute)& anAttribute, 
                                          const std::string& aTypeOfAttribute)
{
  bool ret;

  if(!theSO) return false;

  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;

    DF_Attribute* anAttr = NULL;
    ret = _local_impl->FindAttribute(*(aSO->GetLocalImpl()), anAttr, aTypeOfAttribute);
    if(ret) {
      SALOMEDSImpl_GenericAttribute* aGA = dynamic_cast<SALOMEDSImpl_GenericAttribute*>(anAttr);
      anAttribute = _PTR(GenericAttribute)(SALOMEDS_GenericAttribute::CreateAttribute(aGA));
    }
  }
  else {
    SALOMEDS::GenericAttribute_var aGA;
    ret = _corba_impl->FindAttribute(aSO->GetCORBAImpl(), aGA.out(), (char*)aTypeOfAttribute.c_str()); 
    if(ret) anAttribute = _PTR(GenericAttribute)(SALOMEDS_GenericAttribute::CreateAttribute(aGA));
  }

  return ret;
}

void SALOMEDS_StudyBuilder::RemoveAttribute(const _PTR(SObject)& theSO, const std::string& aTypeOfAttribute)
{
  if(!theSO) return;

  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;

    _local_impl->RemoveAttribute(*(aSO->GetLocalImpl()), (char*)aTypeOfAttribute.c_str());
  }
  else _corba_impl->RemoveAttribute(aSO->GetCORBAImpl(), (char*)aTypeOfAttribute.c_str());
}

void SALOMEDS_StudyBuilder::Addreference(const _PTR(SObject)& me, const _PTR(SObject)& thereferencedObject)
{
  if(!me || !thereferencedObject) {
    throw DFexception("Invalid arguments");
  }

  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(me.get());
  SALOMEDS_SObject* aRefSO = dynamic_cast<SALOMEDS_SObject*>(thereferencedObject.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;

    _local_impl->Addreference(*(aSO->GetLocalImpl()), *(aRefSO->GetLocalImpl()));
  }
  else _corba_impl->Addreference(aSO->GetCORBAImpl(), aRefSO->GetCORBAImpl());
}

void SALOMEDS_StudyBuilder::RemoveReference(const _PTR(SObject)& me)
{
  if(!me) return;
  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(me.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;

    _local_impl->RemoveReference(*(aSO->GetLocalImpl()));
  }
  else _corba_impl->RemoveReference(aSO->GetCORBAImpl());
}

void SALOMEDS_StudyBuilder::SetGUID(const _PTR(SObject)& theSO, const std::string& theGUID)
{
  if(!theSO) return;

  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;

    _local_impl->SetGUID(*(aSO->GetLocalImpl()), theGUID);
  }
  else _corba_impl->SetGUID(aSO->GetCORBAImpl(), (char*)theGUID.c_str());
}
 
bool SALOMEDS_StudyBuilder::IsGUID(const _PTR(SObject)& theSO, const std::string& theGUID)
{
  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;

    ret = _local_impl->IsGUID(*(aSO->GetLocalImpl()), (char*)theGUID.c_str());
  }
  else ret = _corba_impl->IsGUID(aSO->GetCORBAImpl(), (char*)theGUID.c_str());

  return ret;
}

void SALOMEDS_StudyBuilder::NewCommand()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->NewCommand();
  }
  else _corba_impl->NewCommand();
}
 
void SALOMEDS_StudyBuilder::CommitCommand()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      _local_impl->CommitCommand();
    }
    catch(...) {
      throw SALOMEDS::StudyBuilder::LockProtection();
    }
  }
  else _corba_impl->CommitCommand();
}

bool SALOMEDS_StudyBuilder::HasOpenCommand()
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->HasOpenCommand();
  }
  else ret = _corba_impl->HasOpenCommand();
  return ret;
}

void SALOMEDS_StudyBuilder::AbortCommand()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->AbortCommand();
  }
  else _corba_impl->AbortCommand();
}

void SALOMEDS_StudyBuilder::Undo()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      _local_impl->Undo();
    }
    catch(...) {
      throw SALOMEDS::StudyBuilder::LockProtection();
    }
  }
  else _corba_impl->Undo();
}

void SALOMEDS_StudyBuilder::Redo()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      _local_impl->Redo();
    }
    catch(...) {
      throw SALOMEDS::StudyBuilder::LockProtection();
    }
  }
  else _corba_impl->Redo(); 
}

bool SALOMEDS_StudyBuilder::GetAvailableUndos()
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->GetAvailableUndos();
  }
  else ret = _corba_impl->GetAvailableUndos();
  return ret;
}

bool SALOMEDS_StudyBuilder::GetAvailableRedos()
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->GetAvailableRedos();
  }
  else ret = _corba_impl->GetAvailableRedos();
  return ret; 
}

int SALOMEDS_StudyBuilder::UndoLimit()
{
  int aLimit;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aLimit = _local_impl->UndoLimit();
  }
  else aLimit = _corba_impl->UndoLimit();
  return aLimit;
}
 
void SALOMEDS_StudyBuilder::UndoLimit(int theLimit)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;

    _local_impl->UndoLimit(theLimit);
  }
  else _corba_impl->UndoLimit(theLimit);
}
 
void SALOMEDS_StudyBuilder::CheckLocked()
{
  //There is only local part as CORBA part throws the correct exeception
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      _local_impl->CheckLocked();
    }
    catch(...) {
      throw SALOMEDS::StudyBuilder::LockProtection();
    }
  }
}

void SALOMEDS_StudyBuilder::SetName(const _PTR(SObject)& theSO, const std::string& theValue)
{
  if(!theSO) return;

  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;

    _local_impl->SetName(*(aSO->GetLocalImpl()), theValue);
  }
  else _corba_impl->SetName(aSO->GetCORBAImpl(), (char*)theValue.c_str());
}

void SALOMEDS_StudyBuilder::SetComment(const _PTR(SObject)& theSO, const std::string& theValue)
{
  if(!theSO) return;

  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;

    _local_impl->SetComment(*(aSO->GetLocalImpl()), theValue);
  }
  else _corba_impl->SetComment(aSO->GetCORBAImpl(), (char*)theValue.c_str());
}

void SALOMEDS_StudyBuilder::SetIOR(const _PTR(SObject)& theSO, const std::string& theValue)
{
  if(!theSO) return;

  SALOMEDS_SObject* aSO = dynamic_cast<SALOMEDS_SObject*>(theSO.get());
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;

    _local_impl->SetIOR(*(aSO->GetLocalImpl()), theValue);
  }
  else _corba_impl->SetIOR(aSO->GetCORBAImpl(), (char*)theValue.c_str());
}

SALOMEDS::StudyBuilder_ptr SALOMEDS_StudyBuilder::GetBuilder()
{
  if(_isLocal) {
    if(!CORBA::is_nil(_corba_impl)) return SALOMEDS::StudyBuilder::_duplicate(_corba_impl);
    SALOMEDS_StudyBuilder_i* servant = new SALOMEDS_StudyBuilder_i(_local_impl, _orb);
    SALOMEDS::StudyBuilder_var aBuilder = servant->StudyBuilder::_this();
    _corba_impl = SALOMEDS::StudyBuilder::_duplicate(aBuilder);
    return aBuilder._retn();
  }
  else {
    return SALOMEDS::StudyBuilder::_duplicate(_corba_impl);
  }
  return SALOMEDS::StudyBuilder::_nil();
}

void SALOMEDS_StudyBuilder::init_orb()
{
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance();
  ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting()); 
  _orb = init(0 , 0 );     
}
