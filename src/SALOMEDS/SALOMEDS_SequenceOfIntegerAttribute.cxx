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
//  File   : SALOMEDS_SequenceOfIntegerAttribute.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

using namespace std;
#include "SALOMEDS_SequenceOfIntegerAttribute.ixx"

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_SequenceOfIntegerAttribute::GetID () 
{
  static Standard_GUID SALOMEDS_SequenceOfIntegerAttributeID ("12837182-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_SequenceOfIntegerAttributeID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDS_SequenceOfIntegerAttribute) SALOMEDS_SequenceOfIntegerAttribute::Set (const TDF_Label& L) 
{
  Handle(SALOMEDS_SequenceOfIntegerAttribute) A;
  if (!L.FindAttribute(SALOMEDS_SequenceOfIntegerAttribute::GetID(),A)) {
    A = new  SALOMEDS_SequenceOfIntegerAttribute(); 
    L.AddAttribute(A);
  }
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDS_SequenceOfIntegerAttribute::SALOMEDS_SequenceOfIntegerAttribute()
{myValue = new TColStd_HSequenceOfInteger();}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_SequenceOfIntegerAttribute::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDS_SequenceOfIntegerAttribute::NewEmpty () const
{  
  return new SALOMEDS_SequenceOfIntegerAttribute(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDS_SequenceOfIntegerAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  Standard_Integer i;
  Handle(SALOMEDS_SequenceOfIntegerAttribute) anSeq = Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(with);
  if(!anSeq->myValue.IsNull()) {
    myValue = new TColStd_HSequenceOfInteger();
    Standard_Integer Len = anSeq->Length();
    for(i = 1; i<=Len; i++) Add(anSeq->Value(i)); 
  }
  else
    myValue.Nullify();
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDS_SequenceOfIntegerAttribute::Paste (const Handle(TDF_Attribute)& into,
                                    const Handle(TDF_RelocationTable)& ) const
{
  if(!myValue.IsNull()) {
    Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast (into)->Assign(myValue);
  }
}

void SALOMEDS_SequenceOfIntegerAttribute::Assign(const Handle(TColStd_HSequenceOfInteger)& other) 
{
  Backup();
  if (myValue.IsNull()) myValue = new TColStd_HSequenceOfInteger;
  myValue->ChangeSequence() = other->Sequence();
}

void SALOMEDS_SequenceOfIntegerAttribute::ChangeValue(const Standard_Integer Index,const Standard_Integer Value) 
{
  Backup();
  myValue->SetValue(Index, Value);
}

void SALOMEDS_SequenceOfIntegerAttribute::Add(const Standard_Integer Value) 
{
  Backup();
  myValue->Append(Value);
}

void SALOMEDS_SequenceOfIntegerAttribute::Remove(const Standard_Integer Index) 
{
  Backup();
  myValue->Remove(Index);
}

Standard_Integer SALOMEDS_SequenceOfIntegerAttribute::Length() 
{
  return myValue->Length();
}
Standard_Integer SALOMEDS_SequenceOfIntegerAttribute::Value(const Standard_Integer Index) 
{
  return myValue->Value(Index);
}

