//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_AttributeTableOfString_i.cxx
//  Author : Sergey Ruin
//  Module : SALOME

#include <TCollection_ExtendedString.hxx>
#include <TCollection_AsciiString.hxx>
#include <TColStd_HSequenceOfExtendedString.hxx>

#include <Standard_Failure.hxx>

#include <strstream>
#include <string>

#include "SALOMEDS_AttributeTableOfString_i.hxx"
#include "SALOMEDS.hxx"

#include "Utils_ExceptHandlers.hxx"

UNEXPECT_CATCH(ATS_IncorrectIndex, SALOMEDS::AttributeTableOfString::IncorrectIndex);
UNEXPECT_CATCH(ATS_IncorrectArgumentLength, SALOMEDS::AttributeTableOfString::IncorrectArgumentLength);

using namespace std;
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

void SALOMEDS_AttributeTableOfString_i::SetTitle(const char* theTitle) {
  SALOMEDS::Locker lock;

  CheckLocked();
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  CORBA::String_var aStr = CORBA::string_dup(theTitle);
  aTable->SetTitle(TCollection_ExtendedString(aStr));
}

char* SALOMEDS_AttributeTableOfString_i::GetTitle() {
  SALOMEDS::Locker lock;

  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  CORBA::String_var c_s = CORBA::string_dup(TCollection_AsciiString(aTable->GetTitle()).ToCString());
  return c_s._retn();
}

void SALOMEDS_AttributeTableOfString_i::SetRowTitle(CORBA::Long theIndex, const char* theTitle)
     throw (SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(ATS_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  if (theIndex <= 0 || theIndex > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfString::IncorrectIndex();
  CORBA::String_var aStr = CORBA::string_dup(theTitle);
  TCollection_ExtendedString aTitle(aStr);
  TCollection_ExtendedString aUnit = getUnit(aTable->GetRowTitle(theIndex));
  if(aUnit.Length() > 0) {
    aTitle += SEPARATOR;
    aTitle += aUnit;
  }

  aTable->SetRowTitle(theIndex, aTitle);
}

void SALOMEDS_AttributeTableOfString_i::SetRowTitles(const SALOMEDS::StringSeq& theTitles)
     throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(ATS_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  if (theTitles.length() != aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfString::IncorrectArgumentLength();
  for (int i = 0; i < theTitles.length(); i++) {
    SetRowTitle(i + 1, CORBA::string_dup(theTitles[i]));
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfString_i::GetRowTitles() {
  SALOMEDS::Locker lock;

  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  SALOMEDS::StringSeq_var aTitles = new SALOMEDS::StringSeq;
  aTitles->length(aTable->GetNbRows());
  for(int i = 0; i < aTitles->length(); i++)
    aTitles[i] = CORBA::string_dup(TCollection_AsciiString(getTitle(aTable->GetRowTitle(i + 1))).ToCString());
  return aTitles._retn();
}

void SALOMEDS_AttributeTableOfString_i::SetColumnTitle(CORBA::Long theIndex, const char* theTitle)
     throw (SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(ATS_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  if (theIndex <= 0 || theIndex > aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfString::IncorrectIndex();
  CORBA::String_var aStr = CORBA::string_dup(theTitle);
  aTable->SetColumnTitle(theIndex, TCollection_ExtendedString(aStr));
}

void SALOMEDS_AttributeTableOfString_i::SetColumnTitles(const SALOMEDS::StringSeq& theTitles)
     throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(ATS_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  if (theTitles.length() != aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfString::IncorrectArgumentLength();
  for (int i = 0; i < theTitles.length(); i++) {
    SetColumnTitle(i + 1, CORBA::string_dup(theTitles[i]));
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfString_i::GetColumnTitles() {
  SALOMEDS::Locker lock;

  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  SALOMEDS::StringSeq_var aTitles = new SALOMEDS::StringSeq;
  aTitles->length(aTable->GetNbColumns());
  for(int i = 0; i < aTitles->length(); i++)
    aTitles[i] = CORBA::string_dup(TCollection_AsciiString(aTable->GetColumnTitle(i + 1)).ToCString());
  return aTitles._retn();
}

//Units support
void SALOMEDS_AttributeTableOfString_i::SetRowUnit(CORBA::Long theIndex, const char* theUnit)
     throw (SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(ATS_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  if (theIndex <= 0 || theIndex > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfString::IncorrectIndex();
  CORBA::String_var aStr = CORBA::string_dup(theUnit);
  TCollection_ExtendedString aTitle = getTitle(aTable->GetRowTitle(theIndex));
  TCollection_ExtendedString aUnit(aStr);
  aTitle += SEPARATOR;
  aTitle += aUnit;
  aTable->SetRowTitle(theIndex, aTitle);
}

void SALOMEDS_AttributeTableOfString_i::SetRowUnits(const SALOMEDS::StringSeq& theUnits)
     throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(ATS_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  if (theUnits.length() != aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfString::IncorrectArgumentLength();
  for (int i = 0; i < theUnits.length(); i++) {
    SetRowUnit(i + 1, CORBA::string_dup(theUnits[i]));
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfString_i::GetRowUnits() {
  SALOMEDS::Locker lock;

  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  SALOMEDS::StringSeq_var aUnits = new SALOMEDS::StringSeq;
  aUnits->length(aTable->GetNbRows());
  for(int i = 0; i < aUnits->length(); i++)
    aUnits[i] = CORBA::string_dup(TCollection_AsciiString(getUnit(aTable->GetRowTitle(i + 1))).ToCString());
  return aUnits._retn();
}


CORBA::Long SALOMEDS_AttributeTableOfString_i::GetNbRows() {
  SALOMEDS::Locker lock;

  return Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr)->GetNbRows();
}
CORBA::Long SALOMEDS_AttributeTableOfString_i::GetNbColumns() {
  SALOMEDS::Locker lock;

  return Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr)->GetNbColumns();
}

void SALOMEDS_AttributeTableOfString_i::AddRow(const SALOMEDS::StringSeq& theData)
     throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(ATS_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  
  Handle(TColStd_HSequenceOfExtendedString) aRow = new TColStd_HSequenceOfExtendedString;
  for (int i = 0; i < theData.length(); i++) aRow->Append(TCollection_AsciiString(CORBA::string_dup(theData[i])));
  aTable->SetRowData(aTable->GetNbRows() + 1, aRow);
}

void SALOMEDS_AttributeTableOfString_i::SetRow(CORBA::Long theRow, const SALOMEDS::StringSeq& theData)
     throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength, SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(ATS_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  
  Handle(TColStd_HSequenceOfExtendedString) aRow = new TColStd_HSequenceOfExtendedString;
  for (int i = 0; i < theData.length(); i++) aRow->Append(TCollection_AsciiString(CORBA::string_dup(theData[i])));
  aTable->SetRowData(theRow, aRow);
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfString_i::GetRow(CORBA::Long theRow)
     throw (SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(ATS_IncorrectIndex);
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  if (theRow <= 0 || theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfString::IncorrectIndex();

  SALOMEDS::StringSeq_var CorbaSeq = new SALOMEDS::StringSeq;
  Handle(TColStd_HSequenceOfExtendedString) aRow = aTable->GetRowData(theRow);
  CorbaSeq->length(aRow->Length());
  for (int i = 0; i < aRow->Length(); i++) {
    CorbaSeq[i] = CORBA::string_dup((TCollection_AsciiString(aRow->Value(i + 1))).ToCString());
  }
  return CorbaSeq._retn();
}

void SALOMEDS_AttributeTableOfString_i::AddColumn(const SALOMEDS::StringSeq& theData)
     throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(ATS_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  
  Handle(TColStd_HSequenceOfExtendedString) aColumn = new TColStd_HSequenceOfExtendedString;
  for (int i = 0; i < theData.length(); i++) aColumn->Append(TCollection_AsciiString(CORBA::string_dup(theData[i])));
  aTable->SetColumnData(aTable->GetNbColumns() + 1, aColumn);
}

void SALOMEDS_AttributeTableOfString_i::SetColumn(CORBA::Long theColumn, const SALOMEDS::StringSeq& theData)
     throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength, SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(ATS_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  
  Handle(TColStd_HSequenceOfExtendedString) aColumn = new TColStd_HSequenceOfExtendedString;
  for (int i = 0; i < theData.length(); i++) aColumn->Append(TCollection_AsciiString(CORBA::string_dup(theData[i])));
  aTable->SetColumnData(theColumn, aColumn);
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfString_i::GetColumn(CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(ATS_IncorrectIndex);
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  if (theColumn <= 0 || theColumn > aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfString::IncorrectIndex();

  SALOMEDS::StringSeq_var CorbaSeq = new SALOMEDS::StringSeq;
  Handle(TColStd_HSequenceOfExtendedString) aColumn = aTable->GetColumnData(theColumn);
  CorbaSeq->length(aColumn->Length());
  for (int i = 0; i < aColumn->Length(); i++) {
    CorbaSeq[i] = CORBA::string_dup((TCollection_AsciiString(aColumn->Value(i + 1))).ToCString());
  }
  return CorbaSeq._retn();
}

void SALOMEDS_AttributeTableOfString_i::PutValue(const char* theValue, CORBA::Long theRow, CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(ATS_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  char* aValue = CORBA::string_dup(theValue);

  aTable->PutValue(aValue, theRow, theColumn);
}

CORBA::Boolean SALOMEDS_AttributeTableOfString_i::HasValue(CORBA::Long theRow, CORBA::Long theColumn) {
  SALOMEDS::Locker lock;

  return Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr)->HasValue(theRow, theColumn);
}

char* SALOMEDS_AttributeTableOfString_i::GetValue(CORBA::Long theRow, CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(ATS_IncorrectIndex);
  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  if (theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfString::IncorrectIndex();

  TCollection_AsciiString aValue;

  try {
    aValue = aTable->GetValue(theRow, theColumn);
  }
  catch(Standard_Failure) {
    throw SALOMEDS::AttributeTableOfString::IncorrectIndex();
  }

  return CORBA::string_dup(aValue.ToCString());
}


SALOMEDS::LongSeq* SALOMEDS_AttributeTableOfString_i::GetRowSetIndices(CORBA::Long theRow) 
{
  SALOMEDS::Locker lock;

  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);

  if(theRow <= 0 || theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfString::IncorrectIndex();

  SALOMEDS::LongSeq_var CorbaSeq = new SALOMEDS::LongSeq;
  Handle(TColStd_HSequenceOfInteger) aSeq = aTable->GetSetRowIndices(theRow);
  CorbaSeq->length(aSeq->Length());
  for (int i = 0; i < aSeq->Length(); i++) {
    CorbaSeq[i] = aSeq->Value(i + 1);
  }
  return CorbaSeq._retn(); 
}


void SALOMEDS_AttributeTableOfString_i::SetNbColumns(CORBA::Long theNbColumns)
{
  SALOMEDS::Locker lock;

  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);
  aTable->SetNbColumns(theNbColumns);
}

bool SALOMEDS_AttributeTableOfString_i::ReadFromFile(const SALOMEDS::TMPFile& theStream) 
{
  SALOMEDS::Locker lock;

  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);

  istrstream aStream((char*)&theStream[0], theStream.length());
  return aTable->RestoreFromString(aStream);
}

SALOMEDS::TMPFile*  SALOMEDS_AttributeTableOfString_i::SaveToFile()
{
  SALOMEDS::Locker lock;

  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);

  ostrstream ostr;
  aTable->ConvertToString(ostr);
  CORBA::Octet* anOctetBuf =  (CORBA::Octet*)ostr.rdbuf()->str();
  unsigned long aSize = ostr.pcount();
  SALOMEDS::TMPFile_var aStreamFile = new SALOMEDS::TMPFile(aSize, aSize, anOctetBuf, 1);
  return aStreamFile._retn();
}

char* SALOMEDS_AttributeTableOfString_i::Store() {
  SALOMEDS::Locker lock;

  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);

  ostrstream ostr;
  aTable->ConvertToString(ostr);
  string aString = ostr.rdbuf()->str();

  CORBA::String_var aBuffer = CORBA::string_dup(aString.c_str());
  return aBuffer._retn();
}

void SALOMEDS_AttributeTableOfString_i::Restore(const char* value) {
  SALOMEDS::Locker lock;

  Handle(SALOMEDS_TableOfStringAttribute) aTable = Handle(SALOMEDS_TableOfStringAttribute)::DownCast(_myAttr);

  istrstream aStream(value, strlen(value));
  aTable->RestoreFromString(aStream);
}
