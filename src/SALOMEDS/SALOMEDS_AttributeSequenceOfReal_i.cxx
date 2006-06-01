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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_AttributeSequenceOfReal_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDS_AttributeSequenceOfReal_i.hxx"
#include "SALOMEDS.hxx" 
#include <TColStd_HSequenceOfReal.hxx>

using namespace std;

void SALOMEDS_AttributeSequenceOfReal_i::Assign(const SALOMEDS::DoubleSeq& other) 
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  Handle(TColStd_HSequenceOfReal) CasCadeSeq = new TColStd_HSequenceOfReal;
  for (int i = 0; i < other.length(); i++) {
    CasCadeSeq->Append(other[i]);
  }
  Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(_impl)->Assign(CasCadeSeq);
}
 
SALOMEDS::DoubleSeq* SALOMEDS_AttributeSequenceOfReal_i::CorbaSequence()
{
  SALOMEDS::Locker lock; 
  SALOMEDS::DoubleSeq_var CorbaSeq = new SALOMEDS::DoubleSeq;
  Handle(SALOMEDSImpl_AttributeSequenceOfReal) CasCadeSeq = Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(_impl);
  CorbaSeq->length(CasCadeSeq->Length());
  for (int i = 0; i < CasCadeSeq->Length(); i++) {
    CorbaSeq[i] = CasCadeSeq->Value(i+1);;
  }
  return CorbaSeq._retn();
}
 
void SALOMEDS_AttributeSequenceOfReal_i::Add(CORBA::Double value) 
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(_impl)->Add(value);
}

void SALOMEDS_AttributeSequenceOfReal_i::Remove(CORBA::Long index) 
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(_impl)->Remove(index);
}
 
void SALOMEDS_AttributeSequenceOfReal_i::ChangeValue(CORBA::Long index, CORBA::Double value)
{
  SALOMEDS::Locker lock; 
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(_impl)->ChangeValue(index, value);
}
 
CORBA::Double SALOMEDS_AttributeSequenceOfReal_i::Value(CORBA::Short index) 
{
  SALOMEDS::Locker lock; 
  return Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(_impl)->Value(index);
}

CORBA::Long SALOMEDS_AttributeSequenceOfReal_i::Length() 
{
  SALOMEDS::Locker lock; 
  return Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(_impl)->Length();
}

