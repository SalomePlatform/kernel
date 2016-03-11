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

//  File   : SALOMEDS_AttributeSequenceOfInteger.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeSequenceOfInteger.hxx"
#include "SALOMEDS.hxx"

SALOMEDS_AttributeSequenceOfInteger::SALOMEDS_AttributeSequenceOfInteger
                  (SALOMEDSImpl_AttributeSequenceOfInteger* theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeSequenceOfInteger::SALOMEDS_AttributeSequenceOfInteger
                  (SALOMEDS::AttributeSequenceOfInteger_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeSequenceOfInteger::~SALOMEDS_AttributeSequenceOfInteger()
{}

void SALOMEDS_AttributeSequenceOfInteger::Assign(const std::vector<int>& other)
{
  int i, aLength = other.size();
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeSequenceOfInteger*>(_local_impl)->Assign(other);
  }
  else {
    SALOMEDS::LongSeq_var aSeq = new SALOMEDS::LongSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = other[i];
    SALOMEDS::AttributeSequenceOfInteger::_narrow(_corba_impl)->Assign(aSeq);
  }
}

std::vector<int> SALOMEDS_AttributeSequenceOfInteger::CorbaSequence()
{
  std::vector<int> aVector;
  int i, aLength;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeSequenceOfInteger* aSeqAttr;
    aSeqAttr = dynamic_cast<SALOMEDSImpl_AttributeSequenceOfInteger*>(_local_impl);
    aLength = aSeqAttr->Length();
    for (i = 1; i <=aLength; i++) aVector.push_back(aSeqAttr->Value(i));
  }
  else {
    SALOMEDS::AttributeSequenceOfInteger_var aSeqAttr =
      SALOMEDS::AttributeSequenceOfInteger::_narrow(_corba_impl);
    aLength = aSeqAttr->Length();
    for (i = 1; i <=aLength; i++) aVector.push_back(aSeqAttr->Value(i));
  }
  return aVector;
}

void SALOMEDS_AttributeSequenceOfInteger::Add(int value)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeSequenceOfInteger*>(_local_impl)->Add(value);
  }
  else SALOMEDS::AttributeSequenceOfInteger::_narrow(_corba_impl)->Add(value);
}

void SALOMEDS_AttributeSequenceOfInteger::Remove(int index)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeSequenceOfInteger*>(_local_impl)->Remove(index);
  }
  else SALOMEDS::AttributeSequenceOfInteger::_narrow(_corba_impl)->Remove(index);
}

void SALOMEDS_AttributeSequenceOfInteger::ChangeValue(int index, int value)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeSequenceOfInteger*>(_local_impl)->ChangeValue(index, value);
  }
  else SALOMEDS::AttributeSequenceOfInteger::_narrow(_corba_impl)->ChangeValue(index, value);
}

int SALOMEDS_AttributeSequenceOfInteger::Value(int index)
{
  int aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aValue = dynamic_cast<SALOMEDSImpl_AttributeSequenceOfInteger*>(_local_impl)->Value(index);
  }
  else aValue = SALOMEDS::AttributeSequenceOfInteger::_narrow(_corba_impl)->Value(index);
  return aValue;
}

int SALOMEDS_AttributeSequenceOfInteger::Length()
{
  int aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aValue = dynamic_cast<SALOMEDSImpl_AttributeSequenceOfInteger*>(_local_impl)-> Length();
  }
  else aValue = SALOMEDS::AttributeSequenceOfInteger::_narrow(_corba_impl)-> Length();
  return aValue;
}
