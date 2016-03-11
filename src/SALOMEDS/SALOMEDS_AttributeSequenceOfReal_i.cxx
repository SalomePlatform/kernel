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

//  File   : SALOMEDS_AttributeSequenceOfReal_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeSequenceOfReal_i.hxx"
#include "SALOMEDS.hxx" 
#include <vector>

void SALOMEDS_AttributeSequenceOfReal_i::Assign(const SALOMEDS::DoubleSeq& other) 
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  std::vector<double> CasCadeSeq;
  for (int i = 0; i < other.length(); i++) {
    CasCadeSeq.push_back(other[i]);
  }
  dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(_impl)->Assign(CasCadeSeq);
}
 
SALOMEDS::DoubleSeq* SALOMEDS_AttributeSequenceOfReal_i::CorbaSequence()
{
  SALOMEDS::Locker lock; 
  SALOMEDS::DoubleSeq_var CorbaSeq = new SALOMEDS::DoubleSeq;
  const std::vector<double>& CasCadeSeq = dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(_impl)->Array();
  int len = CasCadeSeq.size();
  CorbaSeq->length(len);
  for (int i = 0; i < len; i++) {
    CorbaSeq[i] = CasCadeSeq[i];
  }
  return CorbaSeq._retn();
}
 
void SALOMEDS_AttributeSequenceOfReal_i::Add(CORBA::Double value) 
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(_impl)->Add(value);
}

void SALOMEDS_AttributeSequenceOfReal_i::Remove(CORBA::Long index) 
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(_impl)->Remove(index);
}
 
void SALOMEDS_AttributeSequenceOfReal_i::ChangeValue(CORBA::Long index, CORBA::Double value)
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(_impl)->ChangeValue(index, value);
}
 
CORBA::Double SALOMEDS_AttributeSequenceOfReal_i::Value(CORBA::Short index) 
{
  SALOMEDS::Locker lock; 
  return dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(_impl)->Value(index);
}

CORBA::Long SALOMEDS_AttributeSequenceOfReal_i::Length() 
{
  SALOMEDS::Locker lock; 
  return dynamic_cast<SALOMEDSImpl_AttributeSequenceOfReal*>(_impl)->Length();
}

