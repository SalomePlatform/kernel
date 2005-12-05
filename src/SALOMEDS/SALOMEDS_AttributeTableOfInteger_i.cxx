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
// See http://www.salome-platform.org/
//
//  File   : SALOMEDS_AttributeTableOfInteger_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDS_AttributeTableOfInteger_i.hxx"
#include "SALOMEDS.hxx"

#include <TColStd_HSequenceOfInteger.hxx>
#include <Standard_Failure.hxx>
#include <Standard_ErrorHandler.hxx>
#include "Utils_ExceptHandlers.hxx"

#include <stdexcept>
#include <strstream>
#include <string>

using namespace std;

UNEXPECT_CATCH(ATI_IncorrectIndex, SALOMEDS::AttributeTableOfInteger::IncorrectIndex);
UNEXPECT_CATCH(ATI_IncorrectArgumentLength, SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength);

void SALOMEDS_AttributeTableOfInteger_i::SetTitle(const char* theTitle) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  CORBA::String_var aStr = CORBA::string_dup(theTitle);
  Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl)->SetTitle(TCollection_ExtendedString(aStr));
}

char* SALOMEDS_AttributeTableOfInteger_i::GetTitle() 
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  CORBA::String_var c_s = CORBA::string_dup(TCollection_AsciiString(aTable->GetTitle()).ToCString());
  return c_s._retn();
}

void SALOMEDS_AttributeTableOfInteger_i::SetRowTitle(CORBA::Long theIndex, const char* theTitle)
     throw (SALOMEDS::AttributeTableOfInteger::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch (ATI_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  if (theIndex <= 0 || theIndex > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfInteger::IncorrectIndex();
  aTable->SetRowTitle(theIndex, TCollection_ExtendedString((char*)theTitle));
}

void SALOMEDS_AttributeTableOfInteger_i::SetRowTitles(const SALOMEDS::StringSeq& theTitles)
     throw (SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch (ATI_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  if (theTitles.length() != aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength();
  for (int i = 0; i < theTitles.length(); i++) {
    SetRowTitle(i + 1, theTitles[i]);
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfInteger_i::GetRowTitles() 
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  SALOMEDS::StringSeq_var aTitles = new SALOMEDS::StringSeq;
  aTitles->length(aTable->GetNbRows());
  for(int i = 0; i < aTitles->length(); i++)
    aTitles[i] = CORBA::string_dup(TCollection_AsciiString(aTable->GetRowTitle(i + 1)).ToCString());
  return aTitles._retn();
}

void SALOMEDS_AttributeTableOfInteger_i::SetColumnTitle(CORBA::Long theIndex, const char* theTitle)
     throw (SALOMEDS::AttributeTableOfInteger::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch (ATI_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  if (theIndex <= 0 || theIndex > aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfInteger::IncorrectIndex();
  CORBA::String_var aStr = CORBA::string_dup(theTitle);
  aTable->SetColumnTitle(theIndex, TCollection_ExtendedString(aStr));
}

void SALOMEDS_AttributeTableOfInteger_i::SetColumnTitles(const SALOMEDS::StringSeq& theTitles)
     throw (SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATI_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  if (theTitles.length() != aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength();
  for (int i = 0; i < theTitles.length(); i++) {
    aTable->SetColumnTitle(i + 1, (char*)theTitles[i].in());
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfInteger_i::GetColumnTitles() 
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  SALOMEDS::StringSeq_var aTitles = new SALOMEDS::StringSeq;
  aTitles->length(aTable->GetNbColumns());
  for(int i = 0; i < aTitles->length(); i++)
    aTitles[i] = CORBA::string_dup(TCollection_AsciiString(aTable->GetColumnTitle(i + 1)).ToCString());
  return aTitles._retn();
}

//Units support
void SALOMEDS_AttributeTableOfInteger_i::SetRowUnit(CORBA::Long theIndex, const char* theUnit)
     throw (SALOMEDS::AttributeTableOfInteger::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch (ATI_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  if (theIndex <= 0 || theIndex > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfInteger::IncorrectIndex();
  aTable->SetRowUnit(theIndex, TCollection_ExtendedString((char*)theUnit));
}

void SALOMEDS_AttributeTableOfInteger_i::SetRowUnits(const SALOMEDS::StringSeq& theUnits)
     throw (SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch (ATI_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  if (theUnits.length() != aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength();
  for (int i = 0; i < theUnits.length(); i++) {
    aTable->SetRowUnit(i + 1, (char*)theUnits[i].in());
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfInteger_i::GetRowUnits() 
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  SALOMEDS::StringSeq_var aUnits = new SALOMEDS::StringSeq;
  aUnits->length(aTable->GetNbRows());
  for(int i = 0; i < aUnits->length(); i++)
    aUnits[i] = CORBA::string_dup(TCollection_AsciiString(aTable->GetRowUnit(i + 1)).ToCString());
  return aUnits._retn();
}

CORBA::Long SALOMEDS_AttributeTableOfInteger_i::GetNbRows() 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl)->GetNbRows();
}

CORBA::Long SALOMEDS_AttributeTableOfInteger_i::GetNbColumns() 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl)->GetNbColumns();
}

void SALOMEDS_AttributeTableOfInteger_i::AddRow(const SALOMEDS::LongSeq& theData)
     throw (SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATI_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  
  Handle(TColStd_HSequenceOfInteger) aRow = new TColStd_HSequenceOfInteger;
  for (int i = 0; i < theData.length(); i++) aRow->Append(theData[i]);
  try {
    aTable->SetRowData(aTable->GetNbRows() + 1, aRow);
  }
  catch(Standard_Failure) {
    throw SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength();
  }
}

void SALOMEDS_AttributeTableOfInteger_i::SetRow(CORBA::Long theRow, const SALOMEDS::LongSeq& theData)
     throw (SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength, SALOMEDS::AttributeTableOfInteger::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATI_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  
  Handle(TColStd_HSequenceOfInteger) aRow = new TColStd_HSequenceOfInteger;
  for (int i = 0; i < theData.length(); i++) aRow->Append(theData[i]);
  try {
    aTable->SetRowData(theRow, aRow);
  }
  catch(Standard_Failure) {
    throw SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength();
  }  
}

SALOMEDS::LongSeq* SALOMEDS_AttributeTableOfInteger_i::GetRow(CORBA::Long theRow)
     throw (SALOMEDS::AttributeTableOfInteger::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATI_IncorrectIndex);
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  if (theRow <= 0 || theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfInteger::IncorrectIndex();

  SALOMEDS::LongSeq_var CorbaSeq = new SALOMEDS::LongSeq;
  Handle(TColStd_HSequenceOfInteger) aRow = aTable->GetRowData(theRow);
  CorbaSeq->length(aRow->Length());
  for (int i = 0; i < aRow->Length(); i++) {
    CorbaSeq[i] = aRow->Value(i + 1);
  }
  return CorbaSeq._retn();
}

void SALOMEDS_AttributeTableOfInteger_i::AddColumn(const SALOMEDS::LongSeq& theData)
     throw (SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATI_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  
  Handle(TColStd_HSequenceOfInteger) aColumn = new TColStd_HSequenceOfInteger;
  for (int i = 0; i < theData.length(); i++) aColumn->Append(theData[i]);
  try {
    aTable->SetColumnData(aTable->GetNbColumns() + 1, aColumn);
  }
  catch(Standard_Failure) {
    throw SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength();
  }  
}

void SALOMEDS_AttributeTableOfInteger_i::SetColumn(CORBA::Long theColumn, const SALOMEDS::LongSeq& theData)
     throw (SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength, SALOMEDS::AttributeTableOfInteger::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATI_IncorrectArgumentLength);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  
  Handle(TColStd_HSequenceOfInteger) aColumn = new TColStd_HSequenceOfInteger;
  for (int i = 0; i < theData.length(); i++) aColumn->Append(theData[i]);
  try {
    aTable->SetColumnData(theColumn, aColumn);
  }
  catch(Standard_Failure) {
    throw SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength();
  }
}

SALOMEDS::LongSeq* SALOMEDS_AttributeTableOfInteger_i::GetColumn(CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfInteger::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATI_IncorrectIndex);
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  if (theColumn <= 0 || theColumn > aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfInteger::IncorrectIndex();

  SALOMEDS::LongSeq_var CorbaSeq = new SALOMEDS::LongSeq;
  Handle(TColStd_HSequenceOfInteger) aColumn = aTable->GetColumnData(theColumn);
  CorbaSeq->length(aColumn->Length());
  for (int i = 0; i < aColumn->Length(); i++) {
    CorbaSeq[i] = aColumn->Value(i + 1);
  }
  return CorbaSeq._retn();
}

void SALOMEDS_AttributeTableOfInteger_i::PutValue(CORBA::Long theValue, CORBA::Long theRow, CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfInteger::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATI_IncorrectIndex);
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);

  try {
    aTable->PutValue(theValue, theRow, theColumn);
  }
  catch(Standard_Failure) {
    throw SALOMEDS::AttributeTableOfInteger::IncorrectIndex();
  }
}

CORBA::Boolean SALOMEDS_AttributeTableOfInteger_i::HasValue(CORBA::Long theRow, CORBA::Long theColumn) 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl)->HasValue(theRow, theColumn);
}

CORBA::Long SALOMEDS_AttributeTableOfInteger_i::GetValue(CORBA::Long theRow, CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfInteger::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATI_IncorrectIndex);
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  if (theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfInteger::IncorrectIndex();

  CORBA::Long aValue;
  try {
    aValue = aTable->GetValue(theRow, theColumn);
  }
  catch(Standard_Failure) {
    throw SALOMEDS::AttributeTableOfInteger::IncorrectIndex();
  }
  return aValue;
}

SALOMEDS::LongSeq* SALOMEDS_AttributeTableOfInteger_i::GetRowSetIndices(CORBA::Long theRow) 
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);

  if(theRow <= 0 || theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfInteger::IncorrectIndex();

  SALOMEDS::LongSeq_var CorbaSeq = new SALOMEDS::LongSeq;
  Handle(TColStd_HSequenceOfInteger) aSeq = aTable->GetSetRowIndices(theRow);
  CorbaSeq->length(aSeq->Length());
  for (int i = 0; i < aSeq->Length(); i++) {
    CorbaSeq[i] = aSeq->Value(i + 1);
  }
  return CorbaSeq._retn(); 
}


void SALOMEDS_AttributeTableOfInteger_i::SetNbColumns(CORBA::Long theNbColumns)
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);
  aTable->SetNbColumns(theNbColumns);
}

bool SALOMEDS_AttributeTableOfInteger_i::ReadFromFile(const SALOMEDS::TMPFile& theStream) 
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);

  istrstream aStream((char*)&theStream[0], theStream.length());
  return aTable->RestoreFromString(aStream);
}

SALOMEDS::TMPFile*  SALOMEDS_AttributeTableOfInteger_i::SaveToFile()
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_impl);

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


