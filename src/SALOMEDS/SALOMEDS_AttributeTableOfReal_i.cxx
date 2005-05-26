//  File   : SALOMEDS_AttributeTableOfReal_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeTableOfReal_i.hxx"
#include "SALOMEDS.hxx"

#include <TColStd_HSequenceOfReal.hxx>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>

#include <strstream>
#include <string>

#include "Utils_ExceptHandlers.hxx"
UNEXPECT_CATCH(ATR_IncorrectIndex, SALOMEDS::AttributeTableOfReal::IncorrectIndex);
UNEXPECT_CATCH(ATR_IncorrectArgumentLength, SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength);

void SALOMEDS_AttributeTableOfReal_i::SetTitle(const char* theTitle) 
{
  SALOMEDS::Locker lock;     
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  CORBA::String_var aStr = CORBA::string_dup(theTitle);
  aTable->SetTitle(TCollection_ExtendedString(aStr));
}

char* SALOMEDS_AttributeTableOfReal_i::GetTitle() 
{
  SALOMEDS::Locker lock; 
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  CORBA::String_var c_s = CORBA::string_dup(TCollection_AsciiString(aTable->GetTitle()).ToCString());
  return c_s._retn();
}

void SALOMEDS_AttributeTableOfReal_i::SetRowTitle(CORBA::Long theIndex, const char* theTitle)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (ATR_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  if (theIndex <= 0 || theIndex > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
  aTable->SetRowTitle(theIndex, TCollection_ExtendedString((char*)theTitle));
}

void SALOMEDS_AttributeTableOfReal_i::SetRowTitles(const SALOMEDS::StringSeq& theTitles)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (ATR_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  if (theTitles.length() != aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
  for (int i = 0; i < theTitles.length(); i++) {
    aTable->SetRowTitle(i + 1, (char*)theTitles[i].in());
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfReal_i::GetRowTitles() 
{
  SALOMEDS::Locker lock; 
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  SALOMEDS::StringSeq_var aTitles = new SALOMEDS::StringSeq;
  aTitles->length(aTable->GetNbRows());
  for(int i = 0; i < aTitles->length(); i++)
    aTitles[i] = CORBA::string_dup(TCollection_AsciiString(aTable->GetRowTitle(i + 1)).ToCString());
  return aTitles._retn();
}

void SALOMEDS_AttributeTableOfReal_i::SetColumnTitle(CORBA::Long theIndex, const char* theTitle)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (ATR_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  if (theIndex <= 0 || theIndex > aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
  CORBA::String_var aStr = CORBA::string_dup(theTitle);
  aTable->SetColumnTitle(theIndex, TCollection_ExtendedString(aStr));
}

void SALOMEDS_AttributeTableOfReal_i::SetColumnTitles(const SALOMEDS::StringSeq& theTitles)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  if (theTitles.length() != aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
  for (int i = 0; i < theTitles.length(); i++) {
    aTable->SetColumnTitle(i + 1, (char*)theTitles[i].in());
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfReal_i::GetColumnTitles() 
{
  SALOMEDS::Locker lock; 
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
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
  SALOMEDS::Locker lock; 
  Unexpect aCatch (ATR_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  if (theIndex <= 0 || theIndex > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
  aTable->SetRowUnit(theIndex, TCollection_ExtendedString((char*)theUnit));
}

void SALOMEDS_AttributeTableOfReal_i::SetRowUnits(const SALOMEDS::StringSeq& theUnits)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (ATR_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  if (theUnits.length() != aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
  for (int i = 0; i < theUnits.length(); i++) {
    aTable->SetRowUnit(i + 1, (char*)theUnits[i].in());
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfReal_i::GetRowUnits() 
{
  SALOMEDS::Locker lock; 
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  SALOMEDS::StringSeq_var aUnits = new SALOMEDS::StringSeq;
  aUnits->length(aTable->GetNbRows());
  for(int i = 0; i < aUnits->length(); i++)
    aUnits[i] = CORBA::string_dup(TCollection_AsciiString(aTable->GetRowTitle(i + 1)).ToCString());
  return aUnits._retn();
}


CORBA::Long SALOMEDS_AttributeTableOfReal_i::GetNbRows() 
{
  SALOMEDS::Locker lock; 
  return Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl)->GetNbRows();
}

CORBA::Long SALOMEDS_AttributeTableOfReal_i::GetNbColumns() 
{
  SALOMEDS::Locker lock; 
  return Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl)->GetNbColumns();
}

void SALOMEDS_AttributeTableOfReal_i::AddRow(const SALOMEDS::DoubleSeq& theData)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  
  Handle(TColStd_HSequenceOfReal) aRow = new TColStd_HSequenceOfReal;
  for (int i = 0; i < theData.length(); i++) aRow->Append(theData[i]);
  aTable->SetRowData(aTable->GetNbRows() + 1, aRow);
}

void SALOMEDS_AttributeTableOfReal_i::SetRow(CORBA::Long theRow, const SALOMEDS::DoubleSeq& theData)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength, SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  
  Handle(TColStd_HSequenceOfReal) aRow = new TColStd_HSequenceOfReal;
  for (int i = 0; i < theData.length(); i++) aRow->Append(theData[i]);
  aTable->SetRowData(theRow, aRow);
}

SALOMEDS::DoubleSeq* SALOMEDS_AttributeTableOfReal_i::GetRow(CORBA::Long theRow)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectIndex);
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
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
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  
  Handle(TColStd_HSequenceOfReal) aColumn = new TColStd_HSequenceOfReal;
  for (int i = 0; i < theData.length(); i++) aColumn->Append(theData[i]);
  aTable->SetColumnData(aTable->GetNbColumns() + 1, aColumn);
}

void SALOMEDS_AttributeTableOfReal_i::SetColumn(CORBA::Long theColumn, const SALOMEDS::DoubleSeq& theData)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength, SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  
  Handle(TColStd_HSequenceOfReal) aColumn = new TColStd_HSequenceOfReal;
  for (int i = 0; i < theData.length(); i++) aColumn->Append(theData[i]);
  aTable->SetColumnData(theColumn, aColumn);
}

SALOMEDS::DoubleSeq* SALOMEDS_AttributeTableOfReal_i::GetColumn(CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectIndex);
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
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
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);

  aTable->PutValue(theValue, theRow, theColumn);
}

CORBA::Boolean SALOMEDS_AttributeTableOfReal_i::HasValue(CORBA::Long theRow, CORBA::Long theColumn) 
{
  SALOMEDS::Locker lock; 
  return Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl)->HasValue(theRow, theColumn);
}

CORBA::Double SALOMEDS_AttributeTableOfReal_i::GetValue(CORBA::Long theRow, CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectIndex);
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
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
  SALOMEDS::Locker lock; 
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);

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
  SALOMEDS::Locker lock; 
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);
  aTable->SetNbColumns(theNbColumns);
}

bool SALOMEDS_AttributeTableOfReal_i::ReadFromFile(const SALOMEDS::TMPFile& theStream) 
{
  SALOMEDS::Locker lock; 
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);

  istrstream aStream((char*)&theStream[0], theStream.length());
  return aTable->RestoreFromString(aStream);
}

SALOMEDS::TMPFile*  SALOMEDS_AttributeTableOfReal_i::SaveToFile()
{
  SALOMEDS::Locker lock; 
  Handle(SALOMEDSImpl_AttributeTableOfReal) aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_impl);

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
