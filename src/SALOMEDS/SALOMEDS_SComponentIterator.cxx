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

//  File   : SALOMEDS_SComponentIterator.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_SComponentIterator.hxx"

#include "SALOMEDS.hxx"
#include "SALOMEDS_SComponent.hxx"
#include "SALOMEDSImpl_SComponent.hxx"

SALOMEDS_SComponentIterator::SALOMEDS_SComponentIterator(const SALOMEDSImpl_SComponentIterator& theIterator)
:_local_impl(theIterator.GetPersistentCopy())
{
  _isLocal = true;
  _corba_impl = SALOMEDS::SComponentIterator::_nil();
}

SALOMEDS_SComponentIterator::SALOMEDS_SComponentIterator(SALOMEDS::SComponentIterator_ptr theIterator)
{
  _isLocal = false;
  _corba_impl = SALOMEDS::SComponentIterator::_duplicate(theIterator);
}

SALOMEDS_SComponentIterator::~SALOMEDS_SComponentIterator()
{
  if(!_isLocal) _corba_impl->UnRegister(); 
  else if(_local_impl) delete _local_impl;
}

void SALOMEDS_SComponentIterator::Init()
{
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    _local_impl->Init();
  }
  else _corba_impl->Init();
}

bool SALOMEDS_SComponentIterator::More()
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    ret = _local_impl->More();
  }
  else ret = _corba_impl->More();
  return ret;
}

void SALOMEDS_SComponentIterator::Next()
{
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    _local_impl->Next();
  }
  else _corba_impl->Next();
}

_PTR(SComponent) SALOMEDS_SComponentIterator::Value()  
{
  SALOMEDSClient_SComponent* aSCO = NULL;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    aSCO = new SALOMEDS_SComponent(_local_impl->Value());
  }
  else aSCO = new SALOMEDS_SComponent((SALOMEDS::SComponent_var)_corba_impl->Value());
  return _PTR(SComponent)(aSCO);
}
