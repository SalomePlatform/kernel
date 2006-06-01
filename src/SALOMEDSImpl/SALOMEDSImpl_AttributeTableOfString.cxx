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
//  File   : SALOMEDSImpl_AttributeTableOfString.cxx
//  Author : Sergey Ruin
//  Module : SALOME

#include <SALOMEDSImpl_AttributeTableOfString.hxx>
#include <Standard_Failure.hxx>
#include <Standard_GUID.hxx>
#include <stdio.h>
#include <TColStd_HSequenceOfExtendedString.hxx>  

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeTableOfString, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeTableOfString, SALOMEDSImpl_GenericAttribute )

typedef NCollection_DataMap<Standard_Integer, TCollection_ExtendedString>::Iterator DataMapIterator;

#define SEPARATOR '\1'

static TCollection_ExtendedString getUnit(TCollection_ExtendedString theString)
{
  TCollection_ExtendedString aString(theString);
  int aPos = aString.Search(SEPARATOR);
  if(aPos <= 0 || aPos == aString.Length() ) return TCollection_ExtendedString();
  return aString.Split(aPos);
}

static TCollection_ExtendedString getTitle(TCollection_ExtendedString theString)
{
  TCollection_ExtendedString aString(theString);
  int aPos = aString.Search(SEPARATOR);
  if(aPos < 1) return aString;
  if(aPos == 1) return TCollection_ExtendedString();
  aString.Split(aPos-1);
  return aString;
}

const Standard_GUID& SALOMEDSImpl_AttributeTableOfString::GetID() 
{
  static Standard_GUID SALOMEDSImpl_AttributeTableOfStringID ("128371A4-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeTableOfStringID;
}

Handle(SALOMEDSImpl_AttributeTableOfString) SALOMEDSImpl_AttributeTableOfString::Set(const TDF_Label& label) 
{
  Handle(SALOMEDSImpl_AttributeTableOfString) anAttr;
  if (!label.FindAttribute(SALOMEDSImpl_AttributeTableOfString::GetID(),anAttr)) {
    anAttr = new SALOMEDSImpl_AttributeTableOfString();
    label.AddAttribute(anAttr);
  }
  return anAttr;
}

SALOMEDSImpl_AttributeTableOfString::SALOMEDSImpl_AttributeTableOfString() 
:SALOMEDSImpl_GenericAttribute("AttributeTableOfString")
{
  myRows = new TColStd_HSequenceOfExtendedString();
  myCols = new TColStd_HSequenceOfExtendedString();
  myNbRows = 0;
  myNbColumns = 0;
}

void SALOMEDSImpl_AttributeTableOfString::SetNbColumns(const Standard_Integer theNbColumns)
{
  CheckLocked();  
  Backup();
  
  DataMapOfIntegerString aMap;
  aMap = myTable;
  myTable.Clear();

  DataMapIterator anIterator(aMap);
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

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeTableOfString::SetRowTitle(const Standard_Integer theRow,
						      const TCollection_ExtendedString& theTitle) 
{
  CheckLocked();  
  Backup();
  TCollection_ExtendedString aTitle(theTitle), aUnit = GetRowUnit(theRow);
  if(aUnit.Length()>0) {
    aTitle += SEPARATOR;
    aTitle += aUnit;
  }
  myRows->SetValue(theRow, aTitle);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeTableOfString::SetRowUnit(const Standard_Integer theRow,
						      const TCollection_ExtendedString& theUnit) 
{
  CheckLocked();  
  Backup();
  TCollection_ExtendedString aTitle = GetRowTitle(theRow);
  aTitle += SEPARATOR;
  aTitle += theUnit;

  myRows->SetValue(theRow, aTitle);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeTableOfString::SetRowUnits(const Handle(TColStd_HSequenceOfExtendedString)& theUnits)
{
  if (theUnits->Length() != GetNbRows()) Standard_Failure::Raise("Invalid number of rows");
  int aLength = theUnits->Length(), i;
  for(i = 1; i <= aLength; i++) SetRowUnit(i, theUnits->Value(i));
}

Handle(TColStd_HSequenceOfExtendedString) SALOMEDSImpl_AttributeTableOfString::GetRowUnits()
{
  Handle(TColStd_HSequenceOfExtendedString) aSeq = new TColStd_HSequenceOfExtendedString;
  int aLength = myRows->Length(), i;
  for(i=1; i<=aLength; i++) aSeq->Append(getUnit(myRows->Value(i)));
  return aSeq;
}

void SALOMEDSImpl_AttributeTableOfString::SetRowTitles(const Handle(TColStd_HSequenceOfExtendedString)& theTitles)
{
  if (theTitles->Length() != GetNbRows()) Standard_Failure::Raise("Invalid number of rows");
  int aLength = theTitles->Length(), i;
  for(i = 1; i <= aLength; i++) SetRowTitle(i, theTitles->Value(i));
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

Handle(TColStd_HSequenceOfExtendedString) SALOMEDSImpl_AttributeTableOfString::GetRowTitles()
{
  Handle(TColStd_HSequenceOfExtendedString) aSeq = new TColStd_HSequenceOfExtendedString;
  int aLength = myRows->Length(), i;
  for(i=1; i<=aLength; i++) aSeq->Append(getTitle(myRows->Value(i)));
  return aSeq;
}


TCollection_ExtendedString SALOMEDSImpl_AttributeTableOfString::GetRowTitle(const Standard_Integer theRow) const 
{
  return getTitle(myRows->Value(theRow));
}


TCollection_ExtendedString SALOMEDSImpl_AttributeTableOfString::GetRowUnit(const Standard_Integer theRow) const 
{
  return getUnit(myRows->Value(theRow));
}

void SALOMEDSImpl_AttributeTableOfString::SetRowData(const Standard_Integer theRow,
						     const Handle(TColStd_HSequenceOfExtendedString)& theData) 
{
  CheckLocked();  
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
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeTableOfString::SetTitle(const TCollection_ExtendedString& theTitle) 
{
  CheckLocked();  
  Backup();
  myTitle = theTitle;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

TCollection_ExtendedString SALOMEDSImpl_AttributeTableOfString::GetTitle() const 
{
  return myTitle;
}

Handle(TColStd_HSequenceOfExtendedString) SALOMEDSImpl_AttributeTableOfString::GetRowData(const Standard_Integer theRow)
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

void SALOMEDSImpl_AttributeTableOfString::SetColumnData(const Standard_Integer theColumn,
						        const Handle(TColStd_HSequenceOfExtendedString)& theData) 
{
  CheckLocked();  
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
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}


Handle(TColStd_HSequenceOfExtendedString) SALOMEDSImpl_AttributeTableOfString::GetColumnData(const Standard_Integer theColumn)
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

void SALOMEDSImpl_AttributeTableOfString::SetColumnTitle(const Standard_Integer theColumn,
						         const TCollection_ExtendedString& theTitle) 
{
  CheckLocked();  
  Backup();
  while(myCols->Length() < theColumn) myCols->Append(TCollection_ExtendedString(""));
  myCols->SetValue(theColumn,theTitle);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

TCollection_ExtendedString SALOMEDSImpl_AttributeTableOfString::GetColumnTitle(const Standard_Integer theColumn) const 
{
  if(myCols.IsNull()) return "";
  if(myCols->Length() < theColumn) return "";
  return myCols->Value(theColumn);
}


void SALOMEDSImpl_AttributeTableOfString::SetColumnTitles(const Handle(TColStd_HSequenceOfExtendedString)& theTitles)
{
  if (theTitles->Length() != myNbColumns) Standard_Failure::Raise("Invalid number of columns");
  int aLength = theTitles->Length(), i;
  for(i = 1; i <= aLength; i++)  myCols->SetValue(i, theTitles->Value(i));
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

Handle(TColStd_HSequenceOfExtendedString) SALOMEDSImpl_AttributeTableOfString::GetColumnTitles()
{
  Handle(TColStd_HSequenceOfExtendedString) aSeq = new TColStd_HSequenceOfExtendedString;
  int aLength = myCols->Length(), i;
  for(i=1; i<=aLength; i++) aSeq->Append(myCols->Value(i));
  return aSeq;
}


Standard_Integer SALOMEDSImpl_AttributeTableOfString::GetNbRows() const
{
  return myNbRows;
}

Standard_Integer SALOMEDSImpl_AttributeTableOfString::GetNbColumns() const
{
  return myNbColumns;
}

void SALOMEDSImpl_AttributeTableOfString::PutValue(const TCollection_ExtendedString& theValue,
					           const Standard_Integer theRow,
					           const Standard_Integer theColumn) 
{
  CheckLocked();  
  if(theColumn > myNbColumns) SetNbColumns(theColumn);

  Standard_Integer anIndex = (theRow-1)*myNbColumns + theColumn;
  myTable.Bind(anIndex, theValue);

  if(theRow > myNbRows) {
    while (myRows->Length() < theRow) { // append empty row titles
      myRows->Append(TCollection_ExtendedString(""));
    }
    myNbRows = theRow;
  }
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

Standard_Boolean SALOMEDSImpl_AttributeTableOfString::HasValue(const Standard_Integer theRow,
							       const Standard_Integer theColumn) 
{
  if(theRow > myNbRows || theRow < 1) return Standard_False;
  if(theColumn > myNbColumns || theColumn < 1) return Standard_False;

  Standard_Integer anIndex = (theRow-1)*myNbColumns + theColumn;
  return myTable.IsBound(anIndex); 
}

TCollection_ExtendedString SALOMEDSImpl_AttributeTableOfString::GetValue(const Standard_Integer theRow,
									 const Standard_Integer theColumn) 
{
  if(theRow > myNbRows || theRow < 1) Standard_Failure::Raise("Invalid cell index");
  if(theColumn > myNbColumns || theColumn < 1) Standard_Failure::Raise("Invalid cell index");

  Standard_Integer anIndex = (theRow-1)*myNbColumns + theColumn;
  if(myTable.IsBound(anIndex)) return myTable.Find(anIndex);
  
  Standard_Failure::Raise("Invalid cell index");
  return 0.;
}

const Standard_GUID& SALOMEDSImpl_AttributeTableOfString::ID() const
{
  return GetID();
}

void SALOMEDSImpl_AttributeTableOfString::Restore(const Handle(TDF_Attribute)& with) 
{
  Standard_Integer anIndex;
  Handle(SALOMEDSImpl_AttributeTableOfString) aTable = Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(with);

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

Handle(TDF_Attribute) SALOMEDSImpl_AttributeTableOfString::NewEmpty() const
{
  return new SALOMEDSImpl_AttributeTableOfString();
}

void SALOMEDSImpl_AttributeTableOfString::Paste(const Handle(TDF_Attribute)& into,
					     const Handle(TDF_RelocationTable)&) const
{
  Standard_Integer anIndex;
  Handle(SALOMEDSImpl_AttributeTableOfString) aTable = Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(into);

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


Handle_TColStd_HSequenceOfInteger SALOMEDSImpl_AttributeTableOfString::GetSetRowIndices(const Standard_Integer theRow)
{
  Handle(TColStd_HSequenceOfInteger) aSeq = new TColStd_HSequenceOfInteger;

  Standard_Integer i, aShift = myNbColumns*(theRow-1);
  for(i = 1; i <= myNbColumns; i++) {
    if(myTable.IsBound(aShift + i)) aSeq->Append(i);
  }
  
  return aSeq;
}

Handle_TColStd_HSequenceOfInteger SALOMEDSImpl_AttributeTableOfString::GetSetColumnIndices(const Standard_Integer theColumn)
{
  Handle(TColStd_HSequenceOfInteger) aSeq = new TColStd_HSequenceOfInteger;

  Standard_Integer i, anIndex;
  for(i = 1; i <= myNbRows; i++) {
    anIndex = myNbColumns*(i-1)+theColumn;
    if(myTable.IsBound(anIndex)) aSeq->Append(i);
  }
  
  return aSeq;
}



void SALOMEDSImpl_AttributeTableOfString::ConvertToString(ostrstream& theStream)
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
  DataMapIterator anIterator(myTable);
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

bool SALOMEDSImpl_AttributeTableOfString::RestoreFromString(istrstream& theStream)
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

TCollection_AsciiString SALOMEDSImpl_AttributeTableOfString::Save() 
{
  ostrstream ostr;
  ConvertToString(ostr);
  TCollection_AsciiString aString((char*)ostr.rdbuf()->str());
  return aString;
}

void SALOMEDSImpl_AttributeTableOfString::Load(const TCollection_AsciiString& value) 
{
  istrstream aStream(value.ToCString(), strlen(value.ToCString()));
  RestoreFromString(aStream);
}
