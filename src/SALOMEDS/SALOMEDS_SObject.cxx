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

//  File   : SALOMEDS_SObject.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include <string>

#include "SALOMEDS_SObject.hxx"

#include "SALOMEDS.hxx"
#include "SALOMEDS_SComponent.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDS_Study.hxx"
#include "SALOMEDS_SObject_i.hxx"

#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_GenericAttribute.hxx"
#include "SALOMEDSImpl_Study.hxx"

#include "Utils_ORB_INIT.hxx" 
#include "Utils_SINGLETON.hxx" 

#include "Basics_Utils.hxx"

#include "utilities.h"

#ifdef WIN32
#include <windows.h>
#include <process.h>
#else
#include <sys/types.h>
#include <unistd.h>
#endif


SALOMEDS_SObject::SALOMEDS_SObject(SALOMEDS::SObject_ptr theSObject)
{
#ifdef WIN32
  long pid =  (long)_getpid();
#else
  long pid =  (long)getpid();
#endif  

  CORBA::LongLong addr =  // mpv: fix for IPAL13534: for 64-bit platforms use 8-bytes long for pointer storage
  theSObject->GetLocalImpl(Kernel_Utils::GetHostname().c_str(), pid, _isLocal);

  if(_isLocal) {
    _local_impl = reinterpret_cast<SALOMEDSImpl_SObject*>(addr);
    _corba_impl = SALOMEDS::SObject::_duplicate(theSObject);
  }
  else {
    _local_impl = NULL;
    _corba_impl = SALOMEDS::SObject::_duplicate(theSObject);
  }

  init_orb();
}

SALOMEDS_SObject::SALOMEDS_SObject(const SALOMEDSImpl_SObject& theSObject)
:_isLocal(true)
{
  _corba_impl = SALOMEDS::SObject::_nil();

  if(theSObject.IsComponent()) {
    SALOMEDSImpl_SComponent sco = theSObject;
    _local_impl = sco.GetPersistentCopy();
  }
  else {
    _local_impl = theSObject.GetPersistentCopy();
  }

  init_orb();
}

SALOMEDS_SObject::~SALOMEDS_SObject()
{
  if (!_isLocal) {
    _corba_impl->UnRegister();
  }
  else {
    if(_local_impl) delete _local_impl;
  }
}

bool SALOMEDS_SObject::IsNull() const
{
  return _isLocal ? ( !_local_impl || _local_impl->IsNull() ) : _corba_impl->IsNull();
}

std::string SALOMEDS_SObject::GetID()
{
  std::string aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aValue = _local_impl->GetID();
  }
  else aValue = (CORBA::String_var)_corba_impl->GetID();  
  return aValue;
}

_PTR(SComponent) SALOMEDS_SObject::GetFatherComponent()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    return _PTR(SComponent)(new SALOMEDS_SComponent(_local_impl->GetFatherComponent()));
  }
  return _PTR(SComponent)(new SALOMEDS_SComponent((SALOMEDS::SComponent_var)_corba_impl->GetFatherComponent()));
}

_PTR(SObject) SALOMEDS_SObject::GetFather()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    return _PTR(SObject)(new SALOMEDS_SObject(_local_impl->GetFather()));
  }
  return _PTR(SObject)(new SALOMEDS_SObject((SALOMEDS::SObject_var)_corba_impl->GetFather()));
}

bool SALOMEDS_SObject::FindAttribute(_PTR(GenericAttribute)& anAttribute,
                                     const std::string& aTypeOfAttribute)
{
  bool ret = false;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    DF_Attribute* anAttr = NULL;
    ret = _local_impl->FindAttribute(anAttr, aTypeOfAttribute);
    if(ret) {
      SALOMEDSImpl_GenericAttribute* ga = dynamic_cast<SALOMEDSImpl_GenericAttribute*>(anAttr);
      anAttribute = _PTR(GenericAttribute)(SALOMEDS_GenericAttribute::CreateAttribute(ga));
    }
  }
  else {
    SALOMEDS::GenericAttribute_var anAttr;
    ret = _corba_impl->FindAttribute(anAttr.out(), aTypeOfAttribute.c_str());
    if(ret) anAttribute = _PTR(GenericAttribute)(SALOMEDS_GenericAttribute::CreateAttribute(anAttr));
  }

  return ret;
}

bool SALOMEDS_SObject::ReferencedObject(_PTR(SObject)& theObject)
{
  bool ret = false;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_SObject aSO;
    ret = _local_impl->ReferencedObject(aSO);
    if(ret) theObject = _PTR(SObject)(new SALOMEDS_SObject(aSO));
  }
  else {
    SALOMEDS::SObject_var aSO;
    ret = _corba_impl->ReferencedObject(aSO.out());
    if(ret) theObject = _PTR(SObject)(new SALOMEDS_SObject(aSO));
  }

  return ret; 
}


bool SALOMEDS_SObject::FindSubObject(int theTag, _PTR(SObject)& theObject)
{
  bool ret = false;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_SObject aSO;
    ret = _local_impl->FindSubObject(theTag, aSO);
    if(ret) theObject = _PTR(SObject)(new SALOMEDS_SObject(aSO));
  }
  else {
    SALOMEDS::SObject_var aSO;
    ret = _corba_impl->FindSubObject(theTag, aSO.out());
    if(ret) theObject = _PTR(SObject)(new SALOMEDS_SObject(aSO));
  }

  return ret;   
}

_PTR(Study) SALOMEDS_SObject::GetStudy()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    return _PTR(Study)(new SALOMEDS_Study(_local_impl->GetStudy()));
  }
  SALOMEDS::Study_var study=_corba_impl->GetStudy();
  return _PTR(Study)(new SALOMEDS_Study(study));
}

std::string SALOMEDS_SObject::Name()
{
  std::string aName;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aName = _local_impl->Name();
  }
  else aName = (CORBA::String_var)_corba_impl->Name();

  return aName;
}

void  SALOMEDS_SObject::Name(const std::string& theName)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->Name(theName);
  }
  else _corba_impl->Name(theName.c_str());
}

std::vector<_PTR(GenericAttribute)> SALOMEDS_SObject::GetAllAttributes()
{
  std::vector<_PTR(GenericAttribute)> aVector;
  int aLength = 0;
  SALOMEDSClient_GenericAttribute* anAttr;

  if (_isLocal) {
    SALOMEDS::Locker lock;
    std::vector<DF_Attribute*> aSeq = _local_impl->GetAllAttributes();
    aLength = aSeq.size();
    for (int i = 0; i < aLength; i++) {
      anAttr = SALOMEDS_GenericAttribute::CreateAttribute(dynamic_cast<SALOMEDSImpl_GenericAttribute*>(aSeq[i]));
      aVector.push_back(_PTR(GenericAttribute)(anAttr));
    }
  }
  else {
    SALOMEDS::ListOfAttributes_var aSeq = _corba_impl->GetAllAttributes();
    aLength = aSeq->length();
    for (int i = 0; i < aLength; i++) {
      anAttr = SALOMEDS_GenericAttribute::CreateAttribute(aSeq[i]);
      aVector.push_back(_PTR(GenericAttribute)(anAttr));
    }
  }

  return aVector;
}

std::string SALOMEDS_SObject::GetName()
{
  std::string aName;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aName = _local_impl->GetName();
  }
  else aName = (CORBA::String_var) _corba_impl->GetName();

  return aName;
}

std::string SALOMEDS_SObject::GetComment()
{
  std::string aComment;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aComment = _local_impl->GetComment();
  }
  else aComment = (CORBA::String_var) _corba_impl->GetComment();

  return aComment;
}

std::string SALOMEDS_SObject::GetIOR()
{
  std::string anIOR;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    anIOR = _local_impl->GetIOR();
  }
  else anIOR = (CORBA::String_var) _corba_impl->GetIOR();

  return anIOR;
}

int SALOMEDS_SObject::Tag()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    return _local_impl->Tag();
  }
  return _corba_impl->Tag(); 
}

int SALOMEDS_SObject::Depth()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    return _local_impl->Depth();
  }
  return _corba_impl->Depth();  
}

CORBA::Object_ptr SALOMEDS_SObject::GetObject()
{
  CORBA::Object_var obj;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    std::string anIOR = GetIOR();
    if (!anIOR.empty())
      obj = _orb->string_to_object(anIOR.c_str());
    return obj._retn();
  }
  else {
    obj = _corba_impl->GetObject();
    return obj._retn();
  }

  return CORBA::Object::_nil();
}

SALOMEDS::SObject_ptr SALOMEDS_SObject::GetSObject()
{
  if(_isLocal) {
    if(!CORBA::is_nil(_corba_impl)) return SALOMEDS::SObject::_duplicate(_corba_impl);
    SALOMEDS::SObject_var aSO = SALOMEDS_SObject_i::New(*_local_impl, _orb);
    _corba_impl = SALOMEDS::SObject::_duplicate(aSO);
    return aSO._retn();
  }
  else {
    return SALOMEDS::SObject::_duplicate(_corba_impl);
  }
  return SALOMEDS::SObject::_nil();
}


void SALOMEDS_SObject::init_orb()
{
  ORB_INIT &init = *SINGLETON_<ORB_INIT>::Instance() ;
  ASSERT(SINGLETON_<ORB_INIT>::IsAlreadyExisting());
  _orb = init(0 , 0 ) ;     
}

void SALOMEDS_SObject::SetAttrString(const std::string& name, const std::string& value)
{
  if(_isLocal)
    {
      SALOMEDS::Locker lock;
      _local_impl->SetAttrString(name,value);
    }
  else
    {
      _corba_impl->SetAttrString(name.c_str(),value.c_str());
    }
}
