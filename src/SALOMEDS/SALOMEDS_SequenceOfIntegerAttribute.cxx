using namespace std;
//  File      : SALOMEDS_SequenceOfIntegerAttribute.cxx
//  Created   : Wed Nov 28 16:09:35 2001
//  Author    : Yves FRICAUD
//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header: 

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

