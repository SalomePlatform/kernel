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
//  File   : SALOMEDS_AttributeSequenceOfInteger_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

using namespace std;
#include "SALOMEDS_AttributeSequenceOfInteger_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include <TColStd_HSequenceOfInteger.hxx>


void SALOMEDS_AttributeSequenceOfInteger_i::Assign(const SALOMEDS::LongSeq& other) 
{
  CheckLocked();
  Handle(TColStd_HSequenceOfInteger) CasCadeSeq = new TColStd_HSequenceOfInteger;
  for (int i = 0; i < other.length(); i++) {
    CasCadeSeq->Append(other[i]);
  }
  Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr)->Assign(CasCadeSeq);
}
 
SALOMEDS::LongSeq* SALOMEDS_AttributeSequenceOfInteger_i::CorbaSequence()
{
  SALOMEDS::LongSeq_var CorbaSeq = new SALOMEDS::LongSeq;
  Handle(SALOMEDS_SequenceOfIntegerAttribute) CasCadeSeq = Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr);
  CorbaSeq->length(CasCadeSeq->Length());
  for (int i = 0; i < CasCadeSeq->Length(); i++) {
    CorbaSeq[i] = CasCadeSeq->Value(i+1);;
  }
  return CorbaSeq._retn();
}
 
void SALOMEDS_AttributeSequenceOfInteger_i::Add(CORBA::Long value) 
{
  CheckLocked();
  Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr)->Add(value);
}

void SALOMEDS_AttributeSequenceOfInteger_i::Remove(CORBA::Long index) 
{
  CheckLocked();
  Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr)->Remove(index);
}
 
void SALOMEDS_AttributeSequenceOfInteger_i::ChangeValue(CORBA::Long index, CORBA::Long value)
{
  CheckLocked();
  Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr)->ChangeValue(index, value);
}
 
CORBA::Long SALOMEDS_AttributeSequenceOfInteger_i::Value(CORBA::Short index) 
{
  return Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr)->Value(index);
}

CORBA::Long SALOMEDS_AttributeSequenceOfInteger_i::Length() 
{
  return Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr)->Length();
}

char* SALOMEDS_AttributeSequenceOfInteger_i::Store() {
  Handle(SALOMEDS_SequenceOfIntegerAttribute) CasCadeSeq = Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr);
  Standard_Integer aLength = CasCadeSeq->Length();
  char* aResult = new char[aLength * 25];
  aResult[0] = 0;
  Standard_Integer aPosition = 0;
  for (int i = 1; i <= aLength; i++) {
    sprintf(aResult + aPosition , "%d ", CasCadeSeq->Value(i));
    aPosition += strlen(aResult + aPosition);
  }
  return aResult;
}

void SALOMEDS_AttributeSequenceOfInteger_i::Restore(const char* value) {
  Handle(TColStd_HSequenceOfInteger) CasCadeSeq = new TColStd_HSequenceOfInteger;
  
  char* aCopy = strdup(value);
  char* adr = strtok(aCopy, " ");
  while (adr) {
    CORBA::Long l =  atol(adr);
    CasCadeSeq->Append(l);
    adr = strtok(NULL, " ");
  }
  delete(aCopy);
  Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(_myAttr)->Assign(CasCadeSeq);
}
