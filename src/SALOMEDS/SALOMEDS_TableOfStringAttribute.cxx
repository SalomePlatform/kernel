//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_TableOfStringAttribute.cxx
//  Author : Sergey Ruin
//  Module : SALOME

#include <SALOMEDS_TableOfStringAttribute.ixx>
#include <Standard_Failure.hxx>
#include <SALOMEDS_DataMapIteratorOfDataMapOfIntegerString.hxx>

#include <string>
#include <stdio.h>
using namespace std;

const Standard_GUID& SALOMEDS_TableOfStringAttribute::GetID() 
{
  static Standard_GUID SALOMEDS_TableOfStringAttributeID ("128371A4-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_TableOfStringAttributeID;
}

Handle(SALOMEDS_TableOfStringAttribute) SALOMEDS_TableOfStringAttribute::Set(const TDF_Label& label) 
{
  Handle(SALOMEDS_TableOfStringAttribute) anAttr;
  if (!label.FindAttribute(SALOMEDS_TableOfStringAttribute::GetID(),anAttr)) {
    anAttr = new SALOMEDS_TableOfStringAttribute();
    label.AddAttribute(anAttr);
  }
  return anAttr;
}

SALOMEDS_TableOfStringAttribute::SALOMEDS_TableOfStringAttribute() {
  myRows = new TColStd_HSequenceOfExtendedString();
  myCols = new TColStd_HSequenceOfExtendedString();
  myNbRows = 0;
  myNbColumns = 0;
}

void SALOMEDS_TableOfStringAttribute::SetNbColumns(const Standard_Integer theNbColumns)
{
  Backup();
  
  SALOMEDS_DataMapOfIntegerString aMap;
  aMap = myTable;
  myTable.Clear();

  SALOMEDS_DataMapIteratorOfDataMapOfIntegerString anIterator(aMap);
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

void SALOMEDS_TableOfStringAttribute::SetTitle(const TCollection_ExtendedString& theTitle) {
  Backup();
  myTitle = theTitle;
}

TCollection_ExtendedString SALOMEDS_TableOfStringAttribute::GetTitle() const {
  return myTitle;
}

void SALOMEDS_TableOfStringAttribute::SetRowData(const Standard_Integer theRow,
						  const Handle(TColStd_HSequenceOfExtendedString)& theData) 
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

Handle(TColStd_HSequenceOfExtendedString) SALOMEDS_TableOfStringAttribute::GetRowData(const Standard_Integer theRow)
{
  Handle(TColStd_HSequenceOfExtendedString) aSeq = new TColStd_HSequenceOfExtendedString();
  Standard_Integer i, aShift = (theRow-1)*myNbColumns;
  for(i = 1; i <= myNbColumns; i++) {
     if(myTable.IsBound(aShift+i)) 
       aSeq->Append(myTable.Find(aShift+i));
     else
       aSeq->Append(0.);
  }
  
  return aSeq;
}

void SALOMEDS_TableOfStringAttribute::SetRowTitle(const Standard_Integer theRow,
						   const TCollection_ExtendedString& theTitle) {
  Backup();
  myRows->SetValue(theRow,theTitle);
}

TCollection_ExtendedString SALOMEDS_TableOfStringAttribute::GetRowTitle(const Standard_Integer theRow) const {
  return myRows->Value(theRow);
}


void SALOMEDS_TableOfStringAttribute::SetColumnData(const Standard_Integer theColumn,
						     const Handle(TColStd_HSequenceOfExtendedString)& theData) 
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


Handle(TColStd_HSequenceOfExtendedString) SALOMEDS_TableOfStringAttribute::GetColumnData(const Standard_Integer theColumn)
{
  Handle(TColStd_HSequenceOfExtendedString) aSeq = new TColStd_HSequenceOfExtendedString;
  
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

void SALOMEDS_TableOfStringAttribute::SetColumnTitle(const Standard_Integer theColumn,
						      const TCollection_ExtendedString& theTitle) {
  Backup();
  while(myCols->Length() < theColumn) myCols->Append(TCollection_ExtendedString(""));
  myCols->SetValue(theColumn,theTitle);
}

TCollection_ExtendedString SALOMEDS_TableOfStringAttribute::GetColumnTitle(const Standard_Integer theColumn) const {
  if(myCols.IsNull()) return "";
  if(myCols->Length() < theColumn) return "";
  return myCols->Value(theColumn);
}


Standard_Integer SALOMEDS_TableOfStringAttribute::GetNbRows() const
{
  return myNbRows;
}

Standard_Integer SALOMEDS_TableOfStringAttribute::GetNbColumns() const
{
  return myNbColumns;
}

void SALOMEDS_TableOfStringAttribute::PutValue(const TCollection_ExtendedString& theValue,
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

Standard_Boolean SALOMEDS_TableOfStringAttribute::HasValue(const Standard_Integer theRow,
							   const Standard_Integer theColumn) 
{
  Standard_Integer anIndex = (theRow-1)*myNbColumns + theColumn;
  return myTable.IsBound(anIndex); 
}

TCollection_ExtendedString SALOMEDS_TableOfStringAttribute::GetValue(const Standard_Integer theRow,
						                     const Standard_Integer theColumn) 
{
  Standard_Integer anIndex = (theRow-1)*myNbColumns + theColumn;
  if(myTable.IsBound(anIndex)) return myTable.Find(anIndex);
  
  Standard_Failure::Raise("Invalid cell index");
  return 0.;
}

const Standard_GUID& SALOMEDS_TableOfStringAttribute::ID() const
{
  return GetID();
}

void SALOMEDS_TableOfStringAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  Standard_Integer anIndex;
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(with);

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

Handle(TDF_Attribute) SALOMEDS_TableOfStringAttribute::NewEmpty() const
{
  return new SALOMEDS_TableOfStringAttribute();
}

void SALOMEDS_TableOfStringAttribute::Paste(const Handle(TDF_Attribute)& into,
					     const Handle(TDF_RelocationTable)&) const
{
  Standard_Integer anIndex;
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(into);

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


Handle_TColStd_HSequenceOfInteger SALOMEDS_TableOfStringAttribute::GetSetRowIndices(const Standard_Integer theRow)
{
  Handle(TColStd_HSequenceOfInteger) aSeq = new TColStd_HSequenceOfInteger;

  Standard_Integer i, aShift = myNbColumns*(theRow-1);
  for(i = 1; i <= myNbColumns; i++) {
    if(myTable.IsBound(aShift + i)) aSeq->Append(i);
  }
  
  return aSeq;
}

Handle_TColStd_HSequenceOfInteger SALOMEDS_TableOfStringAttribute::GetSetColumnIndices(const Standard_Integer theColumn)
{
  Handle(TColStd_HSequenceOfInteger) aSeq = new TColStd_HSequenceOfInteger;

  Standard_Integer i, anIndex;
  for(i = 1; i <= myNbRows; i++) {
    anIndex = myNbColumns*(i-1)+theColumn;
    if(myTable.IsBound(anIndex)) aSeq->Append(i);
  }
  
  return aSeq;
}



void SALOMEDS_TableOfStringAttribute::ConvertToString(ostrstream& theStream)
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
  SALOMEDS_DataMapIteratorOfDataMapOfIntegerString anIterator(myTable);
  for(; anIterator.More(); anIterator.Next()) {
    if (anIterator.Value().Length()) { // check empty string in the value table
      theStream << anIterator.Key() << "\n";
      unsigned long aValueSize = anIterator.Value().Length();
      theStream<<aValueSize << "\n";
      theStream.write((TCollection_AsciiString(anIterator.Value()).ToCString()),aValueSize);
      theStream<<"\n";
    } else { // write index only of kind: "0key"; "05", for an example
      theStream << "0" << anIterator.Key() << "\n";
    }
  }
  return;
}

bool SALOMEDS_TableOfStringAttribute::RestoreFromString(istrstream& theStream)
{
  Backup();

  theStream.seekg(0, ios::end);
  long aSize = theStream.tellg();
  theStream.seekg(0, ios::beg);

  int i, j, l;
  char *aValueString = new char[aSize];

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
  TCollection_AsciiString aValue;
  theStream >> l;
  myTable.Clear();
  theStream.getline(aValueString,aSize,'\n');
  for(i=1; i<=l; i++) {
    Standard_Integer aKey;

    theStream.getline(aValueString,aSize,'\n');
    aValue = aValueString;
    aKey = aValue.IntegerValue();
    if (aValue.Value(1) == '0')
      aValue = "";
    else {
      unsigned long aValueSize;
      theStream >> aValueSize;
      theStream.read(aValueString, 1); // an '\n' omitting
      theStream.read(aValueString, aValueSize);
      theStream.read(aValueString, 1); // an '\n' omitting
      aValue = aValueString;
    }
    myTable.Bind(aKey, aValue);
  }
  delete(aValueString);
  return true;
}
