//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOMEDS_AttributeSequenceOfReal_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

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
  Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast(_myAttr)->Assign(CasCadeSeq);
}
 
SALOMEDS::DoubleSeq* SALOMEDS_AttributeSequenceOfReal_i::CorbaSequence()
{
  SALOMEDS::Locker lock;

  SALOMEDS::DoubleSeq_var CorbaSeq = new SALOMEDS::DoubleSeq;
  Handle(SALOMEDS_SequenceOfRealAttribute) CasCadeSeq = Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast(_myAttr);
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
  Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast(_myAttr)->Add(value);
}

void SALOMEDS_AttributeSequenceOfReal_i::Remove(CORBA::Long index) 
{
  SALOMEDS::Locker lock;

  CheckLocked();
  Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast(_myAttr)->Remove(index);
}
 
void SALOMEDS_AttributeSequenceOfReal_i::ChangeValue(CORBA::Long index, CORBA::Double value)
{
  SALOMEDS::Locker lock;

  CheckLocked();
  Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast(_myAttr)->ChangeValue(index, value);
}
 
CORBA::Double SALOMEDS_AttributeSequenceOfReal_i::Value(CORBA::Short index) 
{
  SALOMEDS::Locker lock;

  return Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast(_myAttr)->Value(index);
}

CORBA::Long SALOMEDS_AttributeSequenceOfReal_i::Length() 
{
  SALOMEDS::Locker lock;

  return Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast(_myAttr)->Length();
}

char* SALOMEDS_AttributeSequenceOfReal_i::Store() {
  SALOMEDS::Locker lock;

  Handle(SALOMEDS_SequenceOfRealAttribute) CasCadeSeq = Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast(_myAttr);
  Standard_Integer aLength = CasCadeSeq->Length();
  char* aResult = new char[aLength * 25];
  aResult[0] = 0;
  Standard_Integer aPosition = 0;
  for (int i = 1; i <= aLength; i++) {
    sprintf(aResult + aPosition , "%f ", CasCadeSeq->Value(i));
    aPosition += strlen(aResult + aPosition);
  }
  return aResult;
}

void SALOMEDS_AttributeSequenceOfReal_i::Restore(const char* value) {
  SALOMEDS::Locker lock;

  Handle(TColStd_HSequenceOfReal) CasCadeSeq = new TColStd_HSequenceOfReal;
  
  char* aCopy = strdup(value);
  char* adr = strtok(aCopy, " ");
  char *err = NULL;
  while (adr) {
    CORBA::Double r =  strtod(adr, &err);
    if (err == adr) break;
    else CasCadeSeq->Append(r);
    adr = strtok(NULL, " ");
  }
  free(aCopy);
  Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast(_myAttr)->Assign(CasCadeSeq);
}
