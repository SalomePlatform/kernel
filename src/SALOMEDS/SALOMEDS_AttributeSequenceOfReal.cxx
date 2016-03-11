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

//  File   : SALOMEDS_AttributeSequenceOfReal.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeSequenceOfReal.hxx"
#include "SALOMEDS.hxx"

SALOMEDS_AttributeSequenceOfReal::SALOMEDS_AttributeSequenceOfReal
                  (SALOMEDSImpl_AttributeSequenceOfReal* theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeSequenceOfReal::SALOMEDS_AttributeSequenceOfReal
                  (SALOMEDS::AttributeSequenceOfReal_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeSequenceOfReal::~SALOMEDS_AttributeSequenceOfReal()
{}

void SALOMEDS_AttributeSequenceOfReal::Assign(const std::vector<double>& other)
{
  int i, aLength = other.size();
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(_local_impl)->Assign(other);
  }
  else {
    SALOMEDS::DoubleSeq_var aSeq = new SALOMEDS::DoubleSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = other[i];
    SALOMEDS::AttributeSequenceOfReal::_narrow(_corba_impl)->Assign(aSeq);
  }
}

std::vector<double> SALOMEDS_AttributeSequenceOfReal::CorbaSequence()
{
  std::vector<double> aVector;
  int i, aLength;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    SALOMEDSImpl_AttributeSequenceOfReal* aSeqAttr = NULL;
    aSeqAttr = dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(_local_impl);
    aLength = aSeqAttr->Length();
    for (i = 1; i <=aLength; i++) aVector.push_back(aSeqAttr->Value(i));
  }
  else {
    SALOMEDS::AttributeSequenceOfReal_var aSeqAttr = SALOMEDS::AttributeSequenceOfReal::_narrow(_corba_impl);
    aLength = aSeqAttr->Length();
    for (i = 1; i <=aLength; i++) aVector.push_back(aSeqAttr->Value(i));
  }
  return aVector;
}

void SALOMEDS_AttributeSequenceOfReal::Add(double value)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(_local_impl)->Add(value);
  }
  else SALOMEDS::AttributeSequenceOfReal::_narrow(_corba_impl)->Add(value);
}

void SALOMEDS_AttributeSequenceOfReal::Remove(int index)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(_local_impl)->Remove(index);
  }
  else SALOMEDS::AttributeSequenceOfReal::_narrow(_corba_impl)->Remove(index);
}

void SALOMEDS_AttributeSequenceOfReal::ChangeValue(int index, double value)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock; 
    dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(_local_impl)->ChangeValue(index, value);
  }
  else SALOMEDS::AttributeSequenceOfReal::_narrow(_corba_impl)->ChangeValue(index, value);
}

double SALOMEDS_AttributeSequenceOfReal::Value(int index)
{
  double aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    aValue = dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(_local_impl)->Value(index);
  }
  else aValue = SALOMEDS::AttributeSequenceOfReal::_narrow(_corba_impl)->Value(index);
  return aValue;
}

int SALOMEDS_AttributeSequenceOfReal::Length()
{
  int aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock; 
    aValue = dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(_local_impl)-> Length();
  }
  else aValue = SALOMEDS::AttributeSequenceOfReal::_narrow(_corba_impl)-> Length();
  return aValue;
}
