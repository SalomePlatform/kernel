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
//  File   : SALOMEDS_AttributeTableOfString_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDS_AttributeTableOfString_i.hxx"
#include "SALOMEDS.hxx"

#include <strstream>
#include <string>
#include <vector>

#include "Utils_ExceptHandlers.hxx"

using namespace std;

UNEXPECT_CATCH(ATS_IncorrectIndex, SALOMEDS::AttributeTableOfString::IncorrectIndex);
UNEXPECT_CATCH(ATS_IncorrectArgumentLength, SALOMEDS::AttributeTableOfString::IncorrectArgumentLength);

void SALOMEDS_AttributeTableOfString_i::SetTitle(const char* theTitle) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  CORBA::String_var aStr = CORBA::string_dup(theTitle);
  aTable->SetTitle(string(aStr));
}

char* SALOMEDS_AttributeTableOfString_i::GetTitle() 
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  CORBA::String_var c_s = CORBA::string_dup(aTable->GetTitle().c_str());
  return c_s._retn();
}

void SALOMEDS_AttributeTableOfString_i::SetRowTitle(CORBA::Long theIndex, const char* theTitle)
     throw (SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATS_IncorrectIndex);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  if (theIndex <= 0 || theIndex > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfString::IncorrectIndex();

  aTable->SetRowTitle(theIndex, string(theTitle));
}

void SALOMEDS_AttributeTableOfString_i::SetRowTitles(const SALOMEDS::StringSeq& theTitles)
     throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATS_IncorrectArgumentLength);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  if (theTitles.length() != aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfString::IncorrectArgumentLength();
  for (int i = 0; i < theTitles.length(); i++) {
    aTable->SetRowTitle(i + 1, string((char*)theTitles[i].in()));
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfString_i::GetRowTitles() 
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  SALOMEDS::StringSeq_var aTitles = new SALOMEDS::StringSeq;
  aTitles->length(aTable->GetNbRows());
  for(int i = 0; i < aTitles->length(); i++)
    aTitles[i] =CORBA::string_dup(aTable->GetRowTitle(i + 1).c_str());
  return aTitles._retn();
}

void SALOMEDS_AttributeTableOfString_i::SetColumnTitle(CORBA::Long theIndex, const char* theTitle)
     throw (SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATS_IncorrectIndex);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  if (theIndex <= 0 || theIndex > aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfString::IncorrectIndex();
  aTable->SetColumnTitle(theIndex, string((char*)theTitle));
}

void SALOMEDS_AttributeTableOfString_i::SetColumnTitles(const SALOMEDS::StringSeq& theTitles)
     throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATS_IncorrectArgumentLength);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  if (theTitles.length() != aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfString::IncorrectArgumentLength();
  for (int i = 0; i < theTitles.length(); i++) {
    aTable->SetColumnTitle(i + 1, string((char*)theTitles[i].in()));
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfString_i::GetColumnTitles() 
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  SALOMEDS::StringSeq_var aTitles = new SALOMEDS::StringSeq;
  aTitles->length(aTable->GetNbColumns());
  for(int i = 0; i < aTitles->length(); i++)
    aTitles[i] = CORBA::string_dup(aTable->GetColumnTitle(i + 1).c_str());
  return aTitles._retn();
}

//Units support
void SALOMEDS_AttributeTableOfString_i::SetRowUnit(CORBA::Long theIndex, const char* theUnit)
     throw (SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATS_IncorrectIndex);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  if (theIndex <= 0 || theIndex > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfString::IncorrectIndex();
  aTable->SetRowUnit(theIndex, string((char*)theUnit));
}

void SALOMEDS_AttributeTableOfString_i::SetRowUnits(const SALOMEDS::StringSeq& theUnits)
     throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATS_IncorrectArgumentLength);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  if (theUnits.length() != aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfString::IncorrectArgumentLength();
  for (int i = 0; i < theUnits.length(); i++) {
    aTable->SetRowUnit(i + 1, string((char*)theUnits[i].in()));
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfString_i::GetRowUnits() 
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  SALOMEDS::StringSeq_var aUnits = new SALOMEDS::StringSeq;
  aUnits->length(aTable->GetNbRows());
  for(int i = 0; i < aUnits->length(); i++)
    aUnits[i] = CORBA::string_dup(aTable->GetRowUnit(i + 1).c_str());
  return aUnits._retn();
}


CORBA::Long SALOMEDS_AttributeTableOfString_i::GetNbRows() 
{
  SALOMEDS::Locker lock;
  return dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl)->GetNbRows();
}

CORBA::Long SALOMEDS_AttributeTableOfString_i::GetNbColumns() 
{
  SALOMEDS::Locker lock;
  return dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl)->GetNbColumns();
}

void SALOMEDS_AttributeTableOfString_i::AddRow(const SALOMEDS::StringSeq& theData)
     throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATS_IncorrectArgumentLength);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  
  vector<string> aRow;
  for (int i = 0; i < theData.length(); i++) aRow.push_back(string(CORBA::string_dup(theData[i])));
  aTable->SetRowData(aTable->GetNbRows() + 1, aRow);
}

void SALOMEDS_AttributeTableOfString_i::SetRow(CORBA::Long theRow, const SALOMEDS::StringSeq& theData)
     throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength, SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATS_IncorrectArgumentLength);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  
  vector<string> aRow;
  for (int i = 0; i < theData.length(); i++) aRow.push_back(string(CORBA::string_dup(theData[i].in())));
  aTable->SetRowData(theRow, aRow);
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfString_i::GetRow(CORBA::Long theRow)
     throw (SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATS_IncorrectIndex);
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  if (theRow <= 0 || theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfString::IncorrectIndex();

  SALOMEDS::StringSeq_var CorbaSeq = new SALOMEDS::StringSeq;
  vector<string> aRow = aTable->GetRowData(theRow);
  CorbaSeq->length(aRow.size());
  for (int i = 0; i < aRow.size(); i++) {
    CorbaSeq[i] = CORBA::string_dup(aRow[i].c_str());
  }
  return CorbaSeq._retn();
}

void SALOMEDS_AttributeTableOfString_i::AddColumn(const SALOMEDS::StringSeq& theData)
     throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATS_IncorrectArgumentLength);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  
  vector<string> aColumn;
  for (int i = 0; i < theData.length(); i++) aColumn.push_back(string(CORBA::string_dup(theData[i])));
  aTable->SetColumnData(aTable->GetNbColumns() + 1, aColumn);
}

void SALOMEDS_AttributeTableOfString_i::SetColumn(CORBA::Long theColumn, const SALOMEDS::StringSeq& theData)
     throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength, SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATS_IncorrectArgumentLength);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  
  vector<string> aColumn;
  for (int i = 0; i < theData.length(); i++) aColumn.push_back(string(CORBA::string_dup(theData[i])));
  aTable->SetColumnData(theColumn, aColumn);
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfString_i::GetColumn(CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATS_IncorrectIndex);
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  if (theColumn <= 0 || theColumn > aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfString::IncorrectIndex();

  SALOMEDS::StringSeq_var CorbaSeq = new SALOMEDS::StringSeq;
  vector<string> aColumn = aTable->GetColumnData(theColumn);
  CorbaSeq->length(aColumn.size());
  for (int i = 0; i < aColumn.size(); i++) {
    CorbaSeq[i] = CORBA::string_dup(aColumn[i].c_str());
  }
  return CorbaSeq._retn();
}

void SALOMEDS_AttributeTableOfString_i::PutValue(const char* theValue, CORBA::Long theRow, CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATS_IncorrectIndex);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  char* aValue = CORBA::string_dup(theValue);

  aTable->PutValue(aValue, theRow, theColumn);
}

CORBA::Boolean SALOMEDS_AttributeTableOfString_i::HasValue(CORBA::Long theRow, CORBA::Long theColumn) 
{
  SALOMEDS::Locker lock;
  return dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl)->HasValue(theRow, theColumn);
}

char* SALOMEDS_AttributeTableOfString_i::GetValue(CORBA::Long theRow, CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfString::IncorrectIndex)
{
  SALOMEDS::Locker lock;
  Unexpect aCatch(ATS_IncorrectIndex);
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  if (theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfString::IncorrectIndex();

  string aValue;
  try {
    aValue = aTable->GetValue(theRow, theColumn);
  }
  catch(...) {
    throw SALOMEDS::AttributeTableOfString::IncorrectIndex();
  }

  return CORBA::string_dup(aValue.c_str());
}


SALOMEDS::LongSeq* SALOMEDS_AttributeTableOfString_i::GetRowSetIndices(CORBA::Long theRow) 
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);

  if(theRow <= 0 || theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfString::IncorrectIndex();

  SALOMEDS::LongSeq_var CorbaSeq = new SALOMEDS::LongSeq;
  vector<int> aSeq = aTable->GetSetRowIndices(theRow);
  CorbaSeq->length(aSeq.size());
  for (int i = 0; i < aSeq.size(); i++) {
    CorbaSeq[i] = aSeq[i];
  }
  return CorbaSeq._retn(); 
}


void SALOMEDS_AttributeTableOfString_i::SetNbColumns(CORBA::Long theNbColumns)
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);
  aTable->SetNbColumns(theNbColumns);
}

bool SALOMEDS_AttributeTableOfString_i::ReadFromFile(const SALOMEDS::TMPFile& theStream) 
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);

  string aStream((char*)&theStream[0], theStream.length());
  aTable->Load(aStream);
  return true;
}

SALOMEDS::TMPFile*  SALOMEDS_AttributeTableOfString_i::SaveToFile()
{
  SALOMEDS::Locker lock;
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(_impl);

 string aString = aTable->Save();
  char* aBuffer = (char*)CORBA::string_dup(aString.c_str());
  int aBufferSize = strlen((char*)aBuffer);

  CORBA::Octet* anOctetBuf =  (CORBA::Octet*)aBuffer;

  SALOMEDS::TMPFile_var aStreamFile = new SALOMEDS::TMPFile(aBufferSize, aBufferSize, anOctetBuf, 1);

  return aStreamFile._retn();
}

