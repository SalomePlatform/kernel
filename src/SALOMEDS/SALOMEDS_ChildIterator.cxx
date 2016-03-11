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

//  File   : SALOMEDS_ChildIterator.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_ChildIterator.hxx"
#include "SALOMEDS_SObject.hxx"
#include "SALOMEDS.hxx"

SALOMEDS_ChildIterator::SALOMEDS_ChildIterator(const SALOMEDSImpl_ChildIterator& theIterator)
{
  SALOMEDS::Locker lock;

  _isLocal = true;
  _local_impl = theIterator.GetPersistentCopy();
  _corba_impl = SALOMEDS::ChildIterator::_nil();
}

SALOMEDS_ChildIterator::SALOMEDS_ChildIterator(SALOMEDS::ChildIterator_ptr theIterator)
{
  _isLocal = false;
  _corba_impl = SALOMEDS::ChildIterator::_duplicate(theIterator);
}

SALOMEDS_ChildIterator::~SALOMEDS_ChildIterator()
{
  if(!_isLocal) _corba_impl->UnRegister(); 
  else if(_local_impl) delete _local_impl;
}

void SALOMEDS_ChildIterator::Init()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->Init();
  }
  else _corba_impl->Init();
}

void SALOMEDS_ChildIterator::InitEx(bool theAllLevels)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->InitEx(theAllLevels);
  }
  else _corba_impl->InitEx(theAllLevels);
}

bool SALOMEDS_ChildIterator::More()
{
  bool ret = false;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = _local_impl->More();
  }
  else ret = _corba_impl->More();
  return ret;
}

void SALOMEDS_ChildIterator::Next() 
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    _local_impl->Next();
  }
  else _corba_impl->Next();
}

_PTR(SObject) SALOMEDS_ChildIterator::Value()
{
  SALOMEDSClient_SObject* aSO;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aSO = new SALOMEDS_SObject(_local_impl->Value());
  }
  else {
    SALOMEDS::SObject_var so=_corba_impl->Value();
    aSO = new SALOMEDS_SObject(so);
  }
  return _PTR(SObject)(aSO);
}
