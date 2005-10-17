//  File   : SALOMEDSImpl_AttributeSequenceOfReal.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDSImpl_AttributeSequenceOfReal.hxx"
#include <Standard_GUID.hxx>

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeSequenceOfReal, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeSequenceOfReal, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeSequenceOfReal::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeSequenceOfRealID ("12837183-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeSequenceOfRealID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributeSequenceOfReal) SALOMEDSImpl_AttributeSequenceOfReal::Set (const TDF_Label& L) 
{
  Handle(SALOMEDSImpl_AttributeSequenceOfReal) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeSequenceOfReal::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeSequenceOfReal(); 
    L.AddAttribute(A);
  }
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeSequenceOfReal::SALOMEDSImpl_AttributeSequenceOfReal()
:SALOMEDSImpl_GenericAttribute("AttributeSequenceOfReal")
{
  myValue = new TColStd_HSequenceOfReal();
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeSequenceOfReal::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeSequenceOfReal::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeSequenceOfReal(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeSequenceOfReal::Restore(const Handle(TDF_Attribute)& with) 
{
  Standard_Integer i;
  Handle(SALOMEDSImpl_AttributeSequenceOfReal) anSeq = Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast(with);
  if(!anSeq->myValue.IsNull()) {
    myValue = new TColStd_HSequenceOfReal();
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

void SALOMEDSImpl_AttributeSequenceOfReal::Paste (const Handle(TDF_Attribute)& into,
                                    const Handle(TDF_RelocationTable)& ) const
{
  if(!myValue.IsNull()) {
    Handle(SALOMEDSImpl_AttributeSequenceOfReal)::DownCast (into)->Assign(myValue);
  }
}

void SALOMEDSImpl_AttributeSequenceOfReal::Assign(const Handle(TColStd_HSequenceOfReal)& other) 
{
  CheckLocked();  
  Backup();
  if (myValue.IsNull()) myValue = new TColStd_HSequenceOfReal;
  myValue->ChangeSequence() = other->Sequence();

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeSequenceOfReal::ChangeValue(const Standard_Integer Index,const Standard_Real Value) 
{
  CheckLocked();  
  Backup();
  myValue->SetValue(Index, Value);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeSequenceOfReal::Add(const Standard_Real Value) 
{
  CheckLocked();  
  Backup();
  myValue->Append(Value);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeSequenceOfReal::Remove(const Standard_Integer Index) 
{
  CheckLocked();  
  Backup();
  myValue->Remove(Index);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

Standard_Integer SALOMEDSImpl_AttributeSequenceOfReal::Length() 
{
  return myValue->Length();
}

Standard_Real SALOMEDSImpl_AttributeSequenceOfReal::Value(const Standard_Integer Index) 
{
  return myValue->Value(Index);
}


TCollection_AsciiString SALOMEDSImpl_AttributeSequenceOfReal::Save()
{
  Standard_Integer aLength = Length();
  char* aResult = new char[aLength * 25];
  aResult[0] = 0;
  Standard_Integer aPosition = 0;
  for (int i = 1; i <= aLength; i++) {
    sprintf(aResult + aPosition , "%f ", Value(i));
    aPosition += strlen(aResult + aPosition);
  }
  TCollection_AsciiString ret(aResult);
  delete aResult;
			  
  return ret;
}
			    
void SALOMEDSImpl_AttributeSequenceOfReal::Load(const TCollection_AsciiString& value)
{
  Handle(TColStd_HSequenceOfReal) CasCadeSeq = new TColStd_HSequenceOfReal;
			      
  char* aCopy = value.ToCString();
  char* adr = strtok(aCopy, " ");
  char *err = NULL; 
  while (adr) {
    Standard_Real r =  strtod(adr, &err); 
    CasCadeSeq->Append(r);
    adr = strtok(NULL, " ");
  }
  Assign(CasCadeSeq);
}    
