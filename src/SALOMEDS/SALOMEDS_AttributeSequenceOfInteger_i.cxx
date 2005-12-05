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
//  File   : SALOMEDS_AttributeSequenceOfInteger_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDS_AttributeSequenceOfInteger_i.hxx"
#include "SALOMEDS.hxx"
#include <TColStd_HSequenceOfInteger.hxx>

using namespace std;


void SALOMEDS_AttributeSequenceOfInteger_i::Assign(const SALOMEDS::LongSeq& other) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(TColStd_HSequenceOfInteger) CasCadeSeq = new TColStd_HSequenceOfInteger;
  for (int i = 0; i < other.length(); i++) {
    CasCadeSeq->Append(other[i]);
  }
  Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_impl)->Assign(CasCadeSeq);
}
 
SALOMEDS::LongSeq* SALOMEDS_AttributeSequenceOfInteger_i::CorbaSequence()
{
  SALOMEDS::Locker lock;
  SALOMEDS::LongSeq_var CorbaSeq = new SALOMEDS::LongSeq;
  Handle(SALOMEDSImpl_AttributeSequenceOfInteger) CasCadeSeq;
  CasCadeSeq = Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_impl);
  CorbaSeq->length(CasCadeSeq->Length());
  for (int i = 0; i < CasCadeSeq->Length(); i++) {
    CorbaSeq[i] = CasCadeSeq->Value(i+1);;
  }
  return CorbaSeq._retn();
}
 
void SALOMEDS_AttributeSequenceOfInteger_i::Add(CORBA::Long value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_impl)->Add(value);
}

void SALOMEDS_AttributeSequenceOfInteger_i::Remove(CORBA::Long index) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_impl)->Remove(index);
}
 
void SALOMEDS_AttributeSequenceOfInteger_i::ChangeValue(CORBA::Long index, CORBA::Long value)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_impl)->ChangeValue(index, value);
}
 
CORBA::Long SALOMEDS_AttributeSequenceOfInteger_i::Value(CORBA::Short index) 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_impl)->Value(index);
}

CORBA::Long SALOMEDS_AttributeSequenceOfInteger_i::Length() 
{  
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(_impl)->Length();
}

