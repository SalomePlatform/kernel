//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_TableOfRealAttribute.cxx
//  Author : Michael Ponikarov
//  Module : SALOME
//  $Header$

#include <SALOMEDS_TableOfRealAttribute.ixx>
#include <Standard_Failure.hxx>
#include <TColStd_DataMapIteratorOfDataMapOfIntegerReal.hxx>

#include <string>
#include <stdio.h>
using namespace std;

const Standard_GUID& SALOMEDS_TableOfRealAttribute::GetID() 
{
  static Standard_GUID SALOMEDS_TableOfRealAttributeID ("128371A1-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_TableOfRealAttributeID;
}

Handle(SALOMEDS_TableOfRealAttribute) SALOMEDS_TableOfRealAttribute::Set(const TDF_Label& label) 
{
  Handle(SALOMEDS_TableOfRealAttribute) anAttr;
  if (!label.FindAttribute(SALOMEDS_TableOfRealAttribute::GetID(),anAttr)) {
    anAttr = new SALOMEDS_TableOfRealAttribute();
    label.AddAttribute(anAttr);
  }
  return anAttr;
}

SALOMEDS_TableOfRealAttribute::SALOMEDS_TableOfRealAttribute() {
  myRows = new TColStd_HSequenceOfExtendedString();
  myCols = new TColStd_HSequenceOfExtendedString();
  myNbRows = 0;
  myNbColumns = 0;
}

void SALOMEDS_TableOfRealAttribute::SetNbColumns(const Standard_Integer theNbColumns)
{
  Backup();
  
  TColStd_DataMapOfIntegerReal aMap;
  aMap = myTable;
  myTable.Clear();

  TColStd_DataMapIteratorOfDataMapOfIntegerReal anIterator(aMap);
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

void SALOMEDS_TableOfRealAttribute::SetTitle(const TCollection_ExtendedString& theTitle) {
  Backup();
  myTitle = theTitle;
}

TCollection_ExtendedString SALOMEDS_TableOfRealAttribute::GetTitle() const {
  return myTitle;
}

void SALOMEDS_TableOfRealAttribute::SetRowData(const Standard_Integer theRow,
						  const Handle(TColStd_HSequenceOfReal)& theData) 
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

Handle(TColStd_HSequenceOfReal) SALOMEDS_TableOfRealAttribute::GetRowData(const Standard_Integer theRow)
{
  Handle(TColStd_HSequenceOfReal) aSeq = new TColStd_HSequenceOfReal();
  Standard_Integer i, aShift = (theRow-1)*myNbColumns;
  for(i = 1; i <= myNbColumns; i++) {
     if(myTable.IsBound(aShift+i)) 
       aSeq->Append(myTable.Find(aShift+i));
     else
       aSeq->Append(0.);
  }
  
  return aSeq;
}

void SALOMEDS_TableOfRealAttribute::SetRowTitle(const Standard_Integer theRow,
						   const TCollection_ExtendedString& theTitle) {
  Backup();
  myRows->SetValue(theRow,theTitle);
}

TCollection_ExtendedString SALOMEDS_TableOfRealAttribute::GetRowTitle(const Standard_Integer theRow) const {
  return myRows->Value(theRow);
}


void SALOMEDS_TableOfRealAttribute::SetColumnData(const Standard_Integer theColumn,
						     const Handle(TColStd_HSequenceOfReal)& theData) 
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


Handle(TColStd_HSequenceOfReal) SALOMEDS_TableOfRealAttribute::GetColumnData(const Standard_Integer theColumn)
{
  Handle(TColStd_HSequenceOfReal) aSeq = new TColStd_HSequenceOfReal;
  
  Standard_Integer i, anIndex;
  for(i = 1; i <= myNbRows; i++) {
    anIndex = myNbColumns*(i-1) + theColumn;
    if(myTable.IsBound(anIndex)) 
      aSeq->Append(myTable.Find(anIndex));
    else
      aSeq->Append(0.);
  }
  
  return aSeq;
}

void SALOMEDS_TableOfRealAttribute::SetColumnTitle(const Standard_Integer theColumn,
						      const TCollection_ExtendedString& theTitle) {
  Backup();
  while(myCols->Length() < theColumn) myCols->Append(TCollection_ExtendedString(""));
  myCols->SetValue(theColumn,theTitle);
}

TCollection_ExtendedString SALOMEDS_TableOfRealAttribute::GetColumnTitle(const Standard_Integer theColumn) const {
  if(myCols.IsNull()) return "";
  if(myCols->Length() < theColumn) return "";
  return myCols->Value(theColumn);
}


Standard_Integer SALOMEDS_TableOfRealAttribute::GetNbRows() const
{
  return myNbRows;
}

Standard_Integer SALOMEDS_TableOfRealAttribute::GetNbColumns() const
{
  return myNbColumns;
}

void SALOMEDS_TableOfRealAttribute::PutValue(const Standard_Real theValue,
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

Standard_Boolean SALOMEDS_TableOfRealAttribute::HasValue(const Standard_Integer theRow,
							 const Standard_Integer theColumn) 
{
  Standard_Integer anIndex = (theRow-1)*myNbColumns + theColumn;
  return myTable.IsBound(anIndex); 
}

Standard_Real SALOMEDS_TableOfRealAttribute::GetValue(const Standard_Integer theRow,
						      const Standard_Integer theColumn) 
{
  Standard_Integer anIndex = (theRow-1)*myNbColumns + theColumn;
  if(myTable.IsBound(anIndex)) return myTable.Find(anIndex);
  
  Standard_Failure::Raise("Invalid cell index");
  return 0.;
}

const Standard_GUID& SALOMEDS_TableOfRealAttribute::ID() const
{
  return GetID();
}

void SALOMEDS_TableOfRealAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  Standard_Integer anIndex;
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(with);

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

Handle(TDF_Attribute) SALOMEDS_TableOfRealAttribute::NewEmpty() const
{
  return new SALOMEDS_TableOfRealAttribute();
}

void SALOMEDS_TableOfRealAttribute::Paste(const Handle(TDF_Attribute)& into,
					     const Handle(TDF_RelocationTable)&) const
{
  Standard_Integer anIndex;
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(into);

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


Handle_TColStd_HSequenceOfInteger SALOMEDS_TableOfRealAttribute::GetSetRowIndices(const Standard_Integer theRow)
{
  Handle(TColStd_HSequenceOfInteger) aSeq = new TColStd_HSequenceOfInteger;

  Standard_Integer i, aShift = myNbColumns*(theRow-1);
  for(i = 1; i <= myNbColumns; i++) {
    if(myTable.IsBound(aShift + i)) aSeq->Append(i);
  }
  
  return aSeq;
}

Handle_TColStd_HSequenceOfInteger SALOMEDS_TableOfRealAttribute::GetSetColumnIndices(const Standard_Integer theColumn)
{
  Handle(TColStd_HSequenceOfInteger) aSeq = new TColStd_HSequenceOfInteger;

  Standard_Integer i, anIndex;
  for(i = 1; i <= myNbRows; i++) {
    anIndex = myNbColumns*(i-1)+theColumn;
    if(myTable.IsBound(anIndex)) aSeq->Append(i);
  }
  
  return aSeq;
}



void SALOMEDS_TableOfRealAttribute::ConvertToString(ostrstream& theStream)
{
  int i, j, l;

  
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
  char *aBuffer = new char[128];
  TColStd_DataMapIteratorOfDataMapOfIntegerReal anIterator(myTable);
  for(; anIterator.More(); anIterator.Next()) {
    theStream << anIterator.Key() << "\n";
    sprintf(aBuffer, "%.64e", anIterator.Value());
    theStream << aBuffer << "\n";
  }
  
  delete []aBuffer;

  return;
}

bool SALOMEDS_TableOfRealAttribute::RestoreFromString(istrstream& theStream)
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
    Standard_Integer aKey;
    Standard_Real aValue;
    theStream >> aKey;
    theStream >> aValue;
    myTable.Bind(aKey, aValue);
  }

  return true;
}
