//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_TableOfIntegerAttribute.cxx
//  Author : Michael Ponikarov
//  Module : SALOME
//  $Header$

using namespace std;
#include <SALOMEDS_TableOfIntegerAttribute.ixx>
#include <Standard_Failure.hxx>
#include <TColStd_DataMapIteratorOfDataMapOfIntegerInteger.hxx>

#include <string>

const Standard_GUID& SALOMEDS_TableOfIntegerAttribute::GetID() 
{
  static Standard_GUID SALOMEDS_TableOfIntegerAttributeID ("128371A0-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_TableOfIntegerAttributeID;
}

Handle(SALOMEDS_TableOfIntegerAttribute) SALOMEDS_TableOfIntegerAttribute::Set(const TDF_Label& label) 
{
  Handle(SALOMEDS_TableOfIntegerAttribute) anAttr;
  if (!label.FindAttribute(SALOMEDS_TableOfIntegerAttribute::GetID(),anAttr)) {
    anAttr = new SALOMEDS_TableOfIntegerAttribute();
    label.AddAttribute(anAttr);
  }
  return anAttr;
}

SALOMEDS_TableOfIntegerAttribute::SALOMEDS_TableOfIntegerAttribute() {
  myRows = new TColStd_HSequenceOfExtendedString();
  myCols = new TColStd_HSequenceOfExtendedString();
  myNbRows = 0;
  myNbColumns = 0;
}

void SALOMEDS_TableOfIntegerAttribute::SetNbColumns(const Standard_Integer theNbColumns)
{
  Backup();
  
  TColStd_DataMapOfIntegerInteger aMap;
  aMap = myTable;
  myTable.Clear();

  TColStd_DataMapIteratorOfDataMapOfIntegerInteger anIterator(aMap);
  for(; anIterator.More(); anIterator.Next()) {
    int aRow = (int)(anIterator.Key()/myNbColumns) + 1;
    int aCol = (int)(anIterator.Key() - myNbColumns*(aRow-1));
    if(aCol == 0) { aCol = myNbColumns; aRow--; }
    if(aCol > theNbColumns) continue;
    int aKey = (aRow-1)*theNbColumns+aCol;
    myTable.Bind(aKey, anIterator.Value());
  }

  myNbColumns = theNbColumns;

  while (myCols->Length() < myNbColumns) { // append empty columns titles
    myCols->Append(TCollection_ExtendedString(""));
  }
}

void SALOMEDS_TableOfIntegerAttribute::SetTitle(const TCollection_ExtendedString& theTitle) {
  Backup();
  myTitle = theTitle;
}

TCollection_ExtendedString SALOMEDS_TableOfIntegerAttribute::GetTitle() const {
  return myTitle;
}

void SALOMEDS_TableOfIntegerAttribute::SetRowData(const Standard_Integer theRow,
						  const Handle(TColStd_HSequenceOfInteger)& theData) 
{
  if(theData->Length() > myNbColumns) SetNbColumns(theData->Length());

  Backup();

  while (myRows->Length() < theRow) { // append new row titles
    myRows->Append(TCollection_ExtendedString(""));
  }

  Standard_Integer i, aShift = (theRow-1)*myNbColumns, aLength = theData->Length();
  for(i = 1; i <= aLength; i++) {
    myTable.Bind(aShift + i, theData->Value(i));
  }

  if(theRow > myNbRows) myNbRows = theRow;
}

Handle(TColStd_HSequenceOfInteger) SALOMEDS_TableOfIntegerAttribute::GetRowData(const Standard_Integer theRow)
{
  Handle(TColStd_HSequenceOfInteger) aSeq = new TColStd_HSequenceOfInteger();
  Standard_Integer i, aShift = (theRow-1)*myNbColumns;
  for(i = 1; i <= myNbColumns; i++) {
     if(myTable.IsBound(aShift+i)) 
       aSeq->Append(myTable.Find(aShift+i));
     else
       aSeq->Append(0);
  }
  
  return aSeq;
}

void SALOMEDS_TableOfIntegerAttribute::SetRowTitle(const Standard_Integer theRow,
						   const TCollection_ExtendedString& theTitle) {
  Backup();
  myRows->SetValue(theRow,theTitle);
}

TCollection_ExtendedString SALOMEDS_TableOfIntegerAttribute::GetRowTitle(const Standard_Integer theRow) const {
  return myRows->Value(theRow);
}


void SALOMEDS_TableOfIntegerAttribute::SetColumnData(const Standard_Integer theColumn,
						     const Handle(TColStd_HSequenceOfInteger)& theData) 
{
  if(theColumn > myNbColumns) SetNbColumns(theColumn);

  Backup();

  Standard_Integer i, aLength = theData->Length();
  for(i = 1; i <= aLength; i++) {
    myTable.Bind(myNbColumns*(i-1)+theColumn, theData->Value(i));
  }

  if(aLength > myNbRows) {
    myNbRows = aLength;
    while (myRows->Length() < myNbRows) { // append empty row titles
      myRows->Append(TCollection_ExtendedString(""));
    }
  }
}


Handle(TColStd_HSequenceOfInteger) SALOMEDS_TableOfIntegerAttribute::GetColumnData(const Standard_Integer theColumn)
{
  Handle(TColStd_HSequenceOfInteger) aSeq = new TColStd_HSequenceOfInteger;
  
  Standard_Integer i, anIndex;
  for(i = 1; i <= myNbRows; i++) {
    anIndex = myNbColumns*(i-1) + theColumn;
    if(myTable.IsBound(anIndex)) 
      aSeq->Append(myTable.Find(anIndex));
    else
      aSeq->Append(0);
  }
  
  return aSeq;
}

void SALOMEDS_TableOfIntegerAttribute::SetColumnTitle(const Standard_Integer theColumn,
						      const TCollection_ExtendedString& theTitle) {
  Backup();
  while(myCols->Length() < theColumn) myCols->Append(TCollection_ExtendedString(""));
  myCols->SetValue(theColumn,theTitle);
}

TCollection_ExtendedString SALOMEDS_TableOfIntegerAttribute::GetColumnTitle(const Standard_Integer theColumn) const {
  if(myCols.IsNull()) return "";
  if(myCols->Length() < theColumn) return "";
  return myCols->Value(theColumn);
}


Standard_Integer SALOMEDS_TableOfIntegerAttribute::GetNbRows() const
{
  return myNbRows;
}

Standard_Integer SALOMEDS_TableOfIntegerAttribute::GetNbColumns() const
{
  return myNbColumns;
}

void SALOMEDS_TableOfIntegerAttribute::PutValue(const Standard_Integer theValue,
						const Standard_Integer theRow,
						const Standard_Integer theColumn) 
{
  if(theColumn > myNbColumns) SetNbColumns(theColumn);

  Standard_Integer anIndex = (theRow-1)*myNbColumns + theColumn;
  myTable.Bind(anIndex, theValue);

  if(theRow > myNbRows) {
    while (myRows->Length() < theRow) { // append empty row titles
      myRows->Append(TCollection_ExtendedString(""));
    }
    myNbRows = theRow;
  }
}

Standard_Boolean SALOMEDS_TableOfIntegerAttribute::HasValue(const Standard_Integer theRow,
							    const Standard_Integer theColumn) 
{
  Standard_Integer anIndex = (theRow-1)*myNbColumns + theColumn;
  return myTable.IsBound(anIndex); 
}

Standard_Integer SALOMEDS_TableOfIntegerAttribute::GetValue(const Standard_Integer theRow,
							    const Standard_Integer theColumn) 
{
  Standard_Integer anIndex = (theRow-1)*myNbColumns + theColumn;
  if(myTable.IsBound(anIndex)) return myTable.Find(anIndex);
  
  Standard_Failure::Raise("Invalid cell index");
  return 0;
}

const Standard_GUID& SALOMEDS_TableOfIntegerAttribute::ID() const
{
  return GetID();
}

void SALOMEDS_TableOfIntegerAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  Standard_Integer anIndex;
  Handle(SALOMEDS_TableOfIntegerAttribute) aTable = Handle(SALOMEDS_TableOfIntegerAttribute)::DownCast(with);

  myTable.Clear();
  myCols->Clear();
  myRows->Clear();

  myTable = aTable->myTable;
  myNbRows = aTable->myNbRows;
  myNbColumns = aTable->myNbColumns;
  myTitle = aTable->myTitle;
  
  for(anIndex = 1; anIndex <= aTable->GetNbRows();anIndex++)
    myRows->Append(aTable->GetRowTitle(anIndex));

  for(anIndex = 1; anIndex <= aTable->GetNbColumns(); anIndex++) 
    myCols->Append(aTable->GetColumnTitle(anIndex));
}

Handle(TDF_Attribute) SALOMEDS_TableOfIntegerAttribute::NewEmpty() const
{
  return new SALOMEDS_TableOfIntegerAttribute();
}

void SALOMEDS_TableOfIntegerAttribute::Paste(const Handle(TDF_Attribute)& into,
					     const Handle(TDF_RelocationTable)&) const
{
  Standard_Integer anIndex;
  Handle(SALOMEDS_TableOfIntegerAttribute) aTable = Handle(SALOMEDS_TableOfIntegerAttribute)::DownCast(into);

  aTable->myTable.Clear();
  aTable->myCols->Clear();
  aTable->myRows->Clear();

  aTable->myTable = myTable;
  aTable->myTitle = myTitle;
  aTable->myNbRows = myNbRows;
  aTable->myNbColumns = myNbColumns;

  for(anIndex = 1; anIndex <= GetNbRows();anIndex++)
    aTable->myRows->Append(GetRowTitle(anIndex));
  for(anIndex = 1; anIndex <= GetNbColumns(); anIndex++) 
    aTable->myCols->Append(GetColumnTitle(anIndex));
}


Handle_TColStd_HSequenceOfInteger SALOMEDS_TableOfIntegerAttribute::GetSetRowIndices(const Standard_Integer theRow)
{
  Handle(TColStd_HSequenceOfInteger) aSeq = new TColStd_HSequenceOfInteger;

  Standard_Integer i, aShift = myNbColumns*(theRow-1);
  for(i = 1; i <= myNbColumns; i++) {
    if(myTable.IsBound(aShift + i)) aSeq->Append(i);
  }
  
  return aSeq;
}

Handle_TColStd_HSequenceOfInteger SALOMEDS_TableOfIntegerAttribute::GetSetColumnIndices(const Standard_Integer theColumn)
{
  Handle(TColStd_HSequenceOfInteger) aSeq = new TColStd_HSequenceOfInteger;

  Standard_Integer i, anIndex;
  for(i = 1; i <= myNbRows; i++) {
    anIndex = myNbColumns*(i-1)+theColumn;
    if(myTable.IsBound(anIndex)) aSeq->Append(i);
  }
  
  return aSeq;
}


void SALOMEDS_TableOfIntegerAttribute::ConvertToString(ostrstream& theStream)
{
  int i, j, l;

  theStream.precision(64);
  
  //Title
  l = myTitle.Length();
  theStream << l << "\n";
  for(i=1; i<=l; i++)
    theStream << myTitle.Value(i) << "\n";

  //Nb rows
  theStream << myNbRows << "\n";

  //Rows titles
  for(i=1; i<=myNbRows; i++) {
    l = myRows->Value(i).Length();
    theStream << l << "\n";
    for(j=1; j<=l; j++)
      theStream << myRows->Value(i).Value(j) << "\n";
  }

  //Nb columns
  theStream << myNbColumns << "\n";

  //Columns titles
  for(i=1; i<=myNbColumns; i++) {
    l = myCols->Value(i).Length();
    theStream << l << "\n";
    for(j=1; j<=l; j++)
      theStream << myCols->Value(i).Value(j) << "\n";
  }

  //Store the table values
  l = myTable.Extent();
  theStream << l << "\n";
  TColStd_DataMapIteratorOfDataMapOfIntegerInteger anIterator(myTable);
  for(; anIterator.More(); anIterator.Next()) {
    theStream << anIterator.Key() << "\n";
    theStream << anIterator.Value() << "\n";
  }

  return;
}

bool SALOMEDS_TableOfIntegerAttribute::RestoreFromString(istrstream& theStream)
{
  Backup();

  int i, j, l;

  Standard_ExtCharacter anExtChar;
  TCollection_ExtendedString aStr;

  //Title
  theStream >> l;

  myTitle = TCollection_ExtendedString(l, 0);
  for(i=1; i<=l; i++) {
    theStream >> anExtChar;
    myTitle.SetValue(i, anExtChar);
  }

  //Nb rows
  theStream >> myNbRows;

  //Rows titles
  myRows->Clear();  
  for(i=1; i<=myNbRows; i++) { 
    theStream >> l;
    aStr = TCollection_ExtendedString(l,0);
    for(j=1; j<=l; j++) {
      theStream >> anExtChar;
      aStr.SetValue(j, anExtChar);
    }
    myRows->Append(aStr);
  }

  //Nb columns
  theStream >> myNbColumns;

  //Columns titles
  myCols->Clear();
  for(i=1; i<=myNbColumns; i++) {
    theStream >> l;
    aStr = TCollection_ExtendedString(l,0);
    for(j=1; j<=l; j++) {
      theStream >> anExtChar;
      aStr.SetValue(j, anExtChar);
    }
    myCols->Append(aStr);
  }

  //Restore the table values
  theStream >> l;
  myTable.Clear();
  for(i=1; i<=l; i++) {
    int aKey, aValue;
    theStream >> aKey;
    theStream >> aValue;
    myTable.Bind(aKey, aValue);
  }

  return true;
}
