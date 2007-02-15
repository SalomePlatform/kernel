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
//  File   : SALOMEDSImpl_AttributeSequenceOfInteger.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDSImpl_AttributeSequenceOfInteger.hxx"
#include <Standard_GUID.hxx>

using namespace std;

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

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeSequenceOfInteger::ChangeValue(const Standard_Integer Index,const Standard_Integer Value) 
{
  CheckLocked();  
  Backup();

  if(Index <= 0 || Index > myValue->Length()) Standard_Failure::Raise("Out of range");

  myValue->SetValue(Index, Value);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeSequenceOfInteger::Add(const Standard_Integer Value) 
{
  CheckLocked();  
  Backup();
  myValue->Append(Value);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeSequenceOfInteger::Remove(const Standard_Integer Index) 
{
  CheckLocked();  
  Backup();

  if(Index <= 0 || Index > myValue->Length()) Standard_Failure::Raise("Out of range");

  myValue->Remove(Index);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

Standard_Integer SALOMEDSImpl_AttributeSequenceOfInteger::Length() 
{
  return myValue->Length();
}
Standard_Integer SALOMEDSImpl_AttributeSequenceOfInteger::Value(const Standard_Integer Index) 
{
  if(Index <= 0 || Index > myValue->Length()) Standard_Failure::Raise("Out of range");

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
