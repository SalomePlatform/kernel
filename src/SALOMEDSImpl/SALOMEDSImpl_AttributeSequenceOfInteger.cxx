//  File   : SALOMEDSImpl_AttributeSequenceOfInteger.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDSImpl_AttributeSequenceOfInteger.hxx"
#include <Standard_GUID.hxx>

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeSequenceOfInteger, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeSequenceOfInteger, SALOMEDSImpl_GenericAttribute )


//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeSequenceOfInteger::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeSequenceOfIntegerID ("12837182-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeSequenceOfIntegerID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributeSequenceOfInteger) SALOMEDSImpl_AttributeSequenceOfInteger::Set (const TDF_Label& L) 
{
  Handle(SALOMEDSImpl_AttributeSequenceOfInteger) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeSequenceOfInteger::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeSequenceOfInteger(); 
    L.AddAttribute(A);
  }
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeSequenceOfInteger::SALOMEDSImpl_AttributeSequenceOfInteger()
:SALOMEDSImpl_GenericAttribute("AttributeSequenceOfInteger")
{ 
  myValue = new TColStd_HSequenceOfInteger();
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeSequenceOfInteger::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeSequenceOfInteger::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeSequenceOfInteger(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeSequenceOfInteger::Restore(const Handle(TDF_Attribute)& with) 
{
  Standard_Integer i;
  Handle(SALOMEDSImpl_AttributeSequenceOfInteger) anSeq = Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast(with);
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

void SALOMEDSImpl_AttributeSequenceOfInteger::Paste (const Handle(TDF_Attribute)& into,
                                    const Handle(TDF_RelocationTable)& ) const
{
  if(!myValue.IsNull()) {
    Handle(SALOMEDSImpl_AttributeSequenceOfInteger)::DownCast (into)->Assign(myValue);
  }
}

void SALOMEDSImpl_AttributeSequenceOfInteger::Assign(const Handle(TColStd_HSequenceOfInteger)& other) 
{
  CheckLocked();
  Backup();
  if (myValue.IsNull()) myValue = new TColStd_HSequenceOfInteger;
  myValue->ChangeSequence() = other->Sequence();
}

void SALOMEDSImpl_AttributeSequenceOfInteger::ChangeValue(const Standard_Integer Index,const Standard_Integer Value) 
{
  CheckLocked();  
  Backup();
  myValue->SetValue(Index, Value);
}

void SALOMEDSImpl_AttributeSequenceOfInteger::Add(const Standard_Integer Value) 
{
  CheckLocked();  
  Backup();
  myValue->Append(Value);
}

void SALOMEDSImpl_AttributeSequenceOfInteger::Remove(const Standard_Integer Index) 
{
  CheckLocked();  
  Backup();
  myValue->Remove(Index);
}

Standard_Integer SALOMEDSImpl_AttributeSequenceOfInteger::Length() 
{
  return myValue->Length();
}
Standard_Integer SALOMEDSImpl_AttributeSequenceOfInteger::Value(const Standard_Integer Index) 
{
  return myValue->Value(Index);
}



TCollection_AsciiString SALOMEDSImpl_AttributeSequenceOfInteger::Save() 
{
  Standard_Integer aLength = Length();
  char* aResult = new char[aLength * 25];
  aResult[0] = 0;
  Standard_Integer aPosition = 0;
  for (int i = 1; i <= aLength; i++) {
    sprintf(aResult + aPosition , "%d ", Value(i));
    aPosition += strlen(aResult + aPosition);
  }
  TCollection_AsciiString ret(aResult);
  delete aResult;
  
  return ret;
}
			
void SALOMEDSImpl_AttributeSequenceOfInteger::Load(const TCollection_AsciiString& value) 
{
  Handle(TColStd_HSequenceOfInteger) CasCadeSeq = new TColStd_HSequenceOfInteger;
			  
  char* aCopy = value.ToCString();
  char* adr = strtok(aCopy, " ");
  while (adr) {
    int l =  atol(adr);
    CasCadeSeq->Append(l);
    adr = strtok(NULL, " ");
  }
  Assign(CasCadeSeq);
}
