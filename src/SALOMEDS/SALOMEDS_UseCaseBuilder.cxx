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

//  File   : SALOMEDS_UseCaseBuilder.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_UseCaseBuilder.hxx"

#include "SALOMEDS.hxx"
#include "SALOMEDS_SObject.hxx"
#include "SALOMEDS_UseCaseIterator.hxx"

#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_UseCaseIterator.hxx"

#include <string>

SALOMEDS_UseCaseBuilder::SALOMEDS_UseCaseBuilder(SALOMEDSImpl_UseCaseBuilder* theBuilder)
{
  _isLocal = true;
  _local_impl = theBuilder;
  _corba_impl = SALOMEDS::UseCaseBuilder::_nil();
}

SALOMEDS_UseCaseBuilder::SALOMEDS_UseCaseBuilder(SALOMEDS::UseCaseBuilder_ptr theBuilder)
{
  _isLocal = false;
  _local_impl = NULL;
  _corba_impl = SALOMEDS::UseCaseBuilder::_duplicate(theBuilder);
}

SALOMEDS_UseCaseBuilder::~SALOMEDS_UseCaseBuilder()
{
  if(!_isLocal) _corba_impl->UnRegister(); 
}  

bool SALOMEDS_UseCaseBuilder::Append(const _PTR(SObject)& theObject)
{
  bool ret;
  SALOMEDS_SObject* obj = dynamic_cast<SALOMEDS_SObject*>(theObject.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->Append(*(obj->GetLocalImpl()));
  }
  else ret = _corba_impl->Append(obj->GetCORBAImpl());
  return ret;
}

bool SALOMEDS_UseCaseBuilder::Remove(const _PTR(SObject)& theObject)
{
  bool ret;
  SALOMEDS_SObject* obj = dynamic_cast<SALOMEDS_SObject*>(theObject.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->Remove(*(obj->GetLocalImpl()));
  }
  else ret = _corba_impl->Remove(obj->GetCORBAImpl());
  return ret;
}

bool SALOMEDS_UseCaseBuilder::AppendTo(const _PTR(SObject)& theFather, _PTR(SObject) theObject)
{
  bool ret;
  SALOMEDS_SObject* father = dynamic_cast<SALOMEDS_SObject*>(theFather.get());
  SALOMEDS_SObject* obj = dynamic_cast<SALOMEDS_SObject*>(theObject.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->AppendTo(*(father->GetLocalImpl()), *(obj->GetLocalImpl()));
  }
  else ret = _corba_impl->AppendTo(father->GetCORBAImpl(), obj->GetCORBAImpl());
  return ret;
}

bool SALOMEDS_UseCaseBuilder::InsertBefore(const _PTR(SObject)& theFirst, _PTR(SObject) theNext)
{
  bool ret;
  SALOMEDS_SObject* first = dynamic_cast<SALOMEDS_SObject*>(theFirst.get());
  SALOMEDS_SObject* next = dynamic_cast<SALOMEDS_SObject*>(theNext.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->InsertBefore(*(first->GetLocalImpl()), *(next->GetLocalImpl()));
  }
  else ret = _corba_impl->InsertBefore(first->GetCORBAImpl(), next->GetCORBAImpl());
  return ret;
}

bool SALOMEDS_UseCaseBuilder::SetCurrentObject(const _PTR(SObject)& theObject)
{
  bool ret;
  SALOMEDS_SObject* obj = dynamic_cast<SALOMEDS_SObject*>(theObject.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->SetCurrentObject(*(obj->GetLocalImpl()));
  }
  else ret = _corba_impl->SetCurrentObject(obj->GetCORBAImpl());
  return ret;
}

bool SALOMEDS_UseCaseBuilder::SetRootCurrent()
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->SetRootCurrent();
  }
  else ret = _corba_impl->SetRootCurrent();
  return ret;
}

bool SALOMEDS_UseCaseBuilder::HasChildren(const _PTR(SObject)& theObject)
{
  bool ret;
  SALOMEDS_SObject* obj = dynamic_cast<SALOMEDS_SObject*>(theObject.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->HasChildren(*(obj->GetLocalImpl()));
  }
  else ret = _corba_impl->HasChildren(obj->GetCORBAImpl());
  return ret;
}

bool SALOMEDS_UseCaseBuilder::SortChildren(const _PTR(SObject)& theObject, bool theAscendingOrder)
{
  bool ret;
  SALOMEDS_SObject* obj = dynamic_cast<SALOMEDS_SObject*>(theObject.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->SortChildren(*(obj->GetLocalImpl()), theAscendingOrder);
  }
  else ret = _corba_impl->SortChildren(obj->GetCORBAImpl(), theAscendingOrder);
  return ret;
}

_PTR(SObject) SALOMEDS_UseCaseBuilder::GetFather(const _PTR(SObject)& theObject)
{
  SALOMEDS_SObject* father = NULL;
  SALOMEDS_SObject* obj = dynamic_cast<SALOMEDS_SObject*>(theObject.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;
    father = new SALOMEDS_SObject(_local_impl->GetFather(*(obj->GetLocalImpl())));
  }
  else father = new SALOMEDS_SObject(_corba_impl->GetFather(obj->GetCORBAImpl()));
  return _PTR(SObject)(father);
}

bool SALOMEDS_UseCaseBuilder::IsUseCase(const _PTR(SObject)& theObject)
{
  bool ret;
  SALOMEDS_SObject* obj = dynamic_cast<SALOMEDS_SObject*>(theObject.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->IsUseCase(*(obj->GetLocalImpl()));
  }
  else ret = _corba_impl->IsUseCase(obj->GetCORBAImpl());
  return ret;
}

bool SALOMEDS_UseCaseBuilder::IsUseCaseNode(const _PTR(SObject)& theObject)
{
  bool ret;
  SALOMEDS_SObject* obj = dynamic_cast<SALOMEDS_SObject*>(theObject.get());
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->IsUseCaseNode(*(obj->GetLocalImpl()));
  }
  else ret = _corba_impl->IsUseCaseNode(obj->GetCORBAImpl());
  return ret;
}

bool SALOMEDS_UseCaseBuilder::SetName(const std::string& theName)
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->SetName(theName);
  }
  else ret = _corba_impl->SetName((char*)theName.c_str());
  return ret;
}

_PTR(SObject) SALOMEDS_UseCaseBuilder::GetCurrentObject()
{
  SALOMEDS_SObject* obj = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    obj = new SALOMEDS_SObject(_local_impl->GetCurrentObject());
  }
  else obj = new SALOMEDS_SObject(_corba_impl->GetCurrentObject());
  return _PTR(SObject)(obj);
}

std::string SALOMEDS_UseCaseBuilder::GetName()
{
  std::string aName;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aName = _local_impl->GetName();
  }
  else aName = _corba_impl->GetName();
  return aName;
}

_PTR(SObject) SALOMEDS_UseCaseBuilder::AddUseCase(const std::string& theName)
{
  SALOMEDS_SObject* obj = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    obj = new SALOMEDS_SObject(_local_impl->AddUseCase(theName));
  }
  else obj = new SALOMEDS_SObject(_corba_impl->AddUseCase((char*)theName.c_str()));
  return _PTR(SObject)(obj);
}

_PTR(UseCaseIterator) SALOMEDS_UseCaseBuilder::GetUseCaseIterator(const _PTR(SObject)& theObject) 
{
  SALOMEDS_UseCaseIterator* it = NULL;
  SALOMEDS_SObject* obj = (theObject)?dynamic_cast<SALOMEDS_SObject*>(theObject.get()):NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    if(obj)
      it = new SALOMEDS_UseCaseIterator(_local_impl->GetUseCaseIterator(*(obj->GetLocalImpl())));
    else 
      it = new SALOMEDS_UseCaseIterator(_local_impl->GetUseCaseIterator(SALOMEDSImpl_SObject()));
  }
  else {
    if(obj)
      it = new SALOMEDS_UseCaseIterator(_corba_impl->GetUseCaseIterator(obj->GetCORBAImpl()));
    else
      it = new SALOMEDS_UseCaseIterator(_corba_impl->GetUseCaseIterator(SALOMEDS::SObject::_nil()));
  }
  return _PTR(UseCaseIterator)(it);
}
