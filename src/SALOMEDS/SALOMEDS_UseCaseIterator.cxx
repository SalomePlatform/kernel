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
// See http://www.salome-platform.org/
//
//  File   : SALOMEDS_UseCaseIterator.cxx
//  Author : Sergey RUIN
//  Module : SALOME



#include "SALOMEDS_UseCaseIterator.hxx"
#include "SALOMEDS_SObject.hxx"

using namespace std; 

SALOMEDS_UseCaseIterator::SALOMEDS_UseCaseIterator(const Handle(SALOMEDSImpl_UseCaseIterator)& theIterator)
{
  _isLocal = true;
  _local_impl = theIterator;
  _corba_impl = SALOMEDS::UseCaseIterator::_nil();
}

SALOMEDS_UseCaseIterator::SALOMEDS_UseCaseIterator(SALOMEDS::UseCaseIterator_ptr theIterator)
{
  _isLocal = false;
  _local_impl = NULL;
  _corba_impl = SALOMEDS::UseCaseIterator::_duplicate(theIterator);
}

SALOMEDS_UseCaseIterator::~SALOMEDS_UseCaseIterator()
{
  if(!_isLocal) _corba_impl->Destroy();    
}

void SALOMEDS_UseCaseIterator::Init(bool theAllLevels)
{
  if(_isLocal) _local_impl->Init(theAllLevels);
  else _corba_impl->Init(theAllLevels);
}

bool SALOMEDS_UseCaseIterator::More()
{
  bool ret;
  if(_isLocal) ret = _local_impl->More();
  else ret = _corba_impl->More();
  return ret;
}

void SALOMEDS_UseCaseIterator::Next()
{
  if(_isLocal) _local_impl->Next();
  else _corba_impl->Next();
}

_PTR(SObject) SALOMEDS_UseCaseIterator::Value()
{
  SALOMEDS_SObject* aSO;
  if(_isLocal) aSO = new SALOMEDS_SObject(_local_impl->Value());
  else aSO = new SALOMEDS_SObject(_corba_impl->Value());
  return _PTR(SObject)(aSO);
}
