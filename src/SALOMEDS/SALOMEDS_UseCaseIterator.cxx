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

//  File   : SALOMEDS_UseCaseIterator.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_UseCaseIterator.hxx"

#include "SALOMEDS.hxx"
#include "SALOMEDS_SObject.hxx"

SALOMEDS_UseCaseIterator::SALOMEDS_UseCaseIterator(const SALOMEDSImpl_UseCaseIterator& theIterator)
{
  _isLocal = true;
  _local_impl = theIterator.GetPersistentCopy();
  _corba_impl = SALOMEDS::UseCaseIterator::_nil();
}

SALOMEDS_UseCaseIterator::SALOMEDS_UseCaseIterator(SALOMEDS::UseCaseIterator_ptr theIterator)
{
  _isLocal = false;
  _corba_impl = SALOMEDS::UseCaseIterator::_duplicate(theIterator);
}

SALOMEDS_UseCaseIterator::~SALOMEDS_UseCaseIterator()
{
  if(!_isLocal) _corba_impl->UnRegister();    
  else if(_local_impl) delete _local_impl;
}

void SALOMEDS_UseCaseIterator::Init(bool theAllLevels)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    if(_local_impl) _local_impl->Init(theAllLevels);
  }
  else _corba_impl->Init(theAllLevels);
}

bool SALOMEDS_UseCaseIterator::More()
{
  bool ret = false;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    if(_local_impl) ret = _local_impl->More();
  }
  else ret = _corba_impl->More();
  return ret;
}

void SALOMEDS_UseCaseIterator::Next()
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    if(_local_impl) _local_impl->Next();
  }
  else _corba_impl->Next();
}

_PTR(SObject) SALOMEDS_UseCaseIterator::Value()
{
  SALOMEDS_SObject* aSO = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    if(_local_impl) aSO = new SALOMEDS_SObject(_local_impl->Value());
  }
  else aSO = new SALOMEDS_SObject(_corba_impl->Value());
  return _PTR(SObject)(aSO);
}
