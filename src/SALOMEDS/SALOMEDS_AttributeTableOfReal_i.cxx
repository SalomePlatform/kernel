//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_AttributeTableOfReal_i.cxx
//  Author : Michael Ponikarov
//  Module : SALOME
//  $Header$

#include "SALOMEDS_AttributeTableOfReal_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include <TColStd_HSequenceOfReal.hxx>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>

#include <strstream>
#include <string>

#include "Utils_ExceptHandlers.hxx"
using namespace std;
UNEXPECT_CATCH(ATR_IncorrectIndex, SALOMEDS::AttributeTableOfReal::IncorrectIndex);
UNEXPECT_CATCH(ATR_IncorrectArgumentLength, SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength);

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



void SALOMEDS_AttributeTableOfReal_i::SetTitle(const char* theTitle) {
  CheckLocked();
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  CORBA::String_var aStr = CORBA::string_dup(theTitle);
  aTable->SetTitle(TCollection_ExtendedString(aStr));
}

char* SALOMEDS_AttributeTableOfReal_i::GetTitle() {
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  CORBA::String_var c_s = CORBA::string_dup(TCollection_AsciiString(aTable->GetTitle()).ToCString());
  return c_s._retn();
}

void SALOMEDS_AttributeTableOfReal_i::SetRowTitle(CORBA::Long theIndex, const char* theTitle)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  Unexpect aCatch (ATR_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  if (theIndex <= 0 || theIndex > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
  CORBA::String_var aStr = CORBA::string_dup(theTitle);
  TCollection_ExtendedString aTitle(aStr);
  TCollection_ExtendedString aUnit = getUnit(aTable->GetRowTitle(theIndex));
  if(aUnit.Length() > 0) {
    aTitle += SEPARATOR;
    aTitle += aUnit;
  }

  aTable->SetRowTitle(theIndex, aTitle);
}

void SALOMEDS_AttributeTableOfReal_i::SetRowTitles(const SALOMEDS::StringSeq& theTitles)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength)
{
  Unexpect aCatch (ATR_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  if (theTitles.length() != aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
  for (int i = 0; i < theTitles.length(); i++) {
    SetRowTitle(i + 1, theTitles[i]);
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfReal_i::GetRowTitles() {
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  SALOMEDS::StringSeq_var aTitles = new SALOMEDS::StringSeq;
  aTitles->length(aTable->GetNbRows());
  for(int i = 0; i < aTitles->length(); i++)
    aTitles[i] = CORBA::string_dup(TCollection_AsciiString(getTitle(aTable->GetRowTitle(i + 1))).ToCString());
  return aTitles._retn();
}

void SALOMEDS_AttributeTableOfReal_i::SetColumnTitle(CORBA::Long theIndex, const char* theTitle)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  Unexpect aCatch (ATR_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  if (theIndex <= 0 || theIndex > aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
  CORBA::String_var aStr = CORBA::string_dup(theTitle);
  aTable->SetColumnTitle(theIndex, TCollection_ExtendedString(aStr));
}

void SALOMEDS_AttributeTableOfReal_i::SetColumnTitles(const SALOMEDS::StringSeq& theTitles)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength)
{
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  if (theTitles.length() != aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
  for (int i = 0; i < theTitles.length(); i++) {
    SetColumnTitle(i + 1, theTitles[i]);
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfReal_i::GetColumnTitles() {
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  SALOMEDS::StringSeq_var aTitles = new SALOMEDS::StringSeq;
  aTitles->length(aTable->GetNbColumns());
  for(int i = 0; i < aTitles->length(); i++)
    aTitles[i] = CORBA::string_dup(TCollection_AsciiString(aTable->GetColumnTitle(i + 1)).ToCString());
  return aTitles._retn();
}

//Units support
void SALOMEDS_AttributeTableOfReal_i::SetRowUnit(CORBA::Long theIndex, const char* theUnit)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  Unexpect aCatch (ATR_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  if (theIndex <= 0 || theIndex > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
  CORBA::String_var aStr = CORBA::string_dup(theUnit);
  TCollection_ExtendedString aTitle = getTitle(aTable->GetRowTitle(theIndex));
  TCollection_ExtendedString aUnit(aStr);
  aTitle += SEPARATOR;
  aTitle += aUnit;
  aTable->SetRowTitle(theIndex, aTitle);
}

void SALOMEDS_AttributeTableOfReal_i::SetRowUnits(const SALOMEDS::StringSeq& theUnits)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength)
{
  Unexpect aCatch (ATR_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  if (theUnits.length() != aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
  for (int i = 0; i < theUnits.length(); i++) {
    SetRowUnit(i + 1, theUnits[i]);
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfReal_i::GetRowUnits() {
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  SALOMEDS::StringSeq_var aUnits = new SALOMEDS::StringSeq;
  aUnits->length(aTable->GetNbRows());
  for(int i = 0; i < aUnits->length(); i++)
    aUnits[i] = CORBA::string_dup(TCollection_AsciiString(getUnit(aTable->GetRowTitle(i + 1))).ToCString());
  return aUnits._retn();
}


CORBA::Long SALOMEDS_AttributeTableOfReal_i::GetNbRows() {
  return Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr)->GetNbRows();
}
CORBA::Long SALOMEDS_AttributeTableOfReal_i::GetNbColumns() {
  return Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr)->GetNbColumns();
}

void SALOMEDS_AttributeTableOfReal_i::AddRow(const SALOMEDS::DoubleSeq& theData)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength)
{
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  
  Handle(TColStd_HSequenceOfReal) aRow = new TColStd_HSequenceOfReal;
  for (int i = 0; i < theData.length(); i++) aRow->Append(theData[i]);
  aTable->SetRowData(aTable->GetNbRows() + 1, aRow);
}

void SALOMEDS_AttributeTableOfReal_i::SetRow(CORBA::Long theRow, const SALOMEDS::DoubleSeq& theData)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength, SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  
  Handle(TColStd_HSequenceOfReal) aRow = new TColStd_HSequenceOfReal;
  for (int i = 0; i < theData.length(); i++) aRow->Append(theData[i]);
  aTable->SetRowData(theRow, aRow);
}

SALOMEDS::DoubleSeq* SALOMEDS_AttributeTableOfReal_i::GetRow(CORBA::Long theRow)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  Unexpect aCatch(ATR_IncorrectIndex);
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  if (theRow <= 0 || theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();

  SALOMEDS::DoubleSeq_var CorbaSeq = new SALOMEDS::DoubleSeq;
  Handle(TColStd_HSequenceOfReal) aRow = aTable->GetRowData(theRow);
  CorbaSeq->length(aRow->Length());
  for (int i = 0; i < aRow->Length(); i++) {
    CorbaSeq[i] = aRow->Value(i + 1);
  }
  return CorbaSeq._retn();
}

void SALOMEDS_AttributeTableOfReal_i::AddColumn(const SALOMEDS::DoubleSeq& theData)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength)
{
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  
  Handle(TColStd_HSequenceOfReal) aColumn = new TColStd_HSequenceOfReal;
  for (int i = 0; i < theData.length(); i++) aColumn->Append(theData[i]);
  aTable->SetColumnData(aTable->GetNbColumns() + 1, aColumn);
}

void SALOMEDS_AttributeTableOfReal_i::SetColumn(CORBA::Long theColumn, const SALOMEDS::DoubleSeq& theData)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength, SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  
  Handle(TColStd_HSequenceOfReal) aColumn = new TColStd_HSequenceOfReal;
  for (int i = 0; i < theData.length(); i++) aColumn->Append(theData[i]);
  aTable->SetColumnData(theColumn, aColumn);
}

SALOMEDS::DoubleSeq* SALOMEDS_AttributeTableOfReal_i::GetColumn(CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  Unexpect aCatch(ATR_IncorrectIndex);
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  if (theColumn <= 0 || theColumn > aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();

  SALOMEDS::DoubleSeq_var CorbaSeq = new SALOMEDS::DoubleSeq;
  Handle(TColStd_HSequenceOfReal) aColumn = aTable->GetColumnData(theColumn);
  CorbaSeq->length(aColumn->Length());
  for (int i = 0; i < aColumn->Length(); i++) {
    CorbaSeq[i] = aColumn->Value(i + 1);
  }
  return CorbaSeq._retn();
}

void SALOMEDS_AttributeTableOfReal_i::PutValue(CORBA::Double theValue, CORBA::Long theRow, CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  Unexpect aCatch(ATR_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);

  aTable->PutValue(theValue, theRow, theColumn);
}

CORBA::Boolean SALOMEDS_AttributeTableOfReal_i::HasValue(CORBA::Long theRow, CORBA::Long theColumn) {
  return Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr)->HasValue(theRow, theColumn);
}

CORBA::Double SALOMEDS_AttributeTableOfReal_i::GetValue(CORBA::Long theRow, CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  Unexpect aCatch(ATR_IncorrectIndex);
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  if (theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();

  CORBA::Double aValue;

  try {
    aValue = aTable->GetValue(theRow, theColumn);
  }
  catch(Standard_Failure) {
    throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
  }

  return aValue;
}


SALOMEDS::LongSeq* SALOMEDS_AttributeTableOfReal_i::GetRowSetIndices(CORBA::Long theRow) 
{
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);

  if(theRow <= 0 || theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();

  SALOMEDS::LongSeq_var CorbaSeq = new SALOMEDS::LongSeq;
  Handle(TColStd_HSequenceOfInteger) aSeq = aTable->GetSetRowIndices(theRow);
  CorbaSeq->length(aSeq->Length());
  for (int i = 0; i < aSeq->Length(); i++) {
    CorbaSeq[i] = aSeq->Value(i + 1);
  }
  return CorbaSeq._retn(); 
}


void SALOMEDS_AttributeTableOfReal_i::SetNbColumns(CORBA::Long theNbColumns)
{
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);
  aTable->SetNbColumns(theNbColumns);
}

bool SALOMEDS_AttributeTableOfReal_i::ReadFromFile(const SALOMEDS::TMPFile& theStream) 
{
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);

  istrstream aStream((char*)&theStream[0], theStream.length());
  return aTable->RestoreFromString(aStream);
}

SALOMEDS::TMPFile*  SALOMEDS_AttributeTableOfReal_i::SaveToFile()
{
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);

  ostrstream ostr;
  string aString;
  aTable->ConvertToString(ostr);

  aString = ostr.rdbuf()->str();

  char* aBuffer = (char*)CORBA::string_dup(aString.c_str());
  int aBufferSize = strlen((char*)aBuffer);

  CORBA::Octet* anOctetBuf =  (CORBA::Octet*)aBuffer;

  SALOMEDS::TMPFile_var aStreamFile = new SALOMEDS::TMPFile(aBufferSize, aBufferSize, anOctetBuf, 1);

  return aStreamFile._retn();
}

char* SALOMEDS_AttributeTableOfReal_i::Store() {
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);

  ostrstream ostr;
  aTable->ConvertToString(ostr);
  string aString = ostr.rdbuf()->str();

  CORBA::String_var aBuffer = CORBA::string_dup(aString.c_str());
  return aBuffer._retn();
}

void SALOMEDS_AttributeTableOfReal_i::Restore(const char* value) {
  Handle(SALOMEDS_TableOfRealAttribute) aTable = Handle(SALOMEDS_TableOfRealAttribute)::DownCast(_myAttr);

  istrstream aStream(value, strlen(value));
  aTable->RestoreFromString(aStream);
}
