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
//  File   : SALOMEDS_AttributeTableOfReal_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDS_AttributeTableOfReal_i.hxx"
#include "SALOMEDS.hxx"

#include <strstream>
#include <string>
#include <vector>

using namespace std;

#include "Utils_ExceptHandlers.hxx"
UNEXPECT_CATCH(ATR_IncorrectIndex, SALOMEDS::AttributeTableOfReal::IncorrectIndex);
UNEXPECT_CATCH(ATR_IncorrectArgumentLength, SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength);

void SALOMEDS_AttributeTableOfReal_i::SetTitle(const char* theTitle) 
{
  SALOMEDS::Locker lock;     
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  CORBA::String_var aStr = CORBA::string_dup(theTitle);
  aTable->SetTitle(string(aStr));
}

char* SALOMEDS_AttributeTableOfReal_i::GetTitle() 
{
  SALOMEDS::Locker lock; 
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  CORBA::String_var c_s = CORBA::string_dup(aTable->GetTitle().c_str());
  return c_s._retn();
}

void SALOMEDS_AttributeTableOfReal_i::SetRowTitle(CORBA::Long theIndex, const char* theTitle)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (ATR_IncorrectIndex);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  if (theIndex <= 0 || theIndex > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
  aTable->SetRowTitle(theIndex, string(theTitle));
}

void SALOMEDS_AttributeTableOfReal_i::SetRowTitles(const SALOMEDS::StringSeq& theTitles)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (ATR_IncorrectArgumentLength);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  if (theTitles.length() != aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
  for (int i = 0; i < theTitles.length(); i++) {
    aTable->SetRowTitle(i + 1, (char*)theTitles[i].in());
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfReal_i::GetRowTitles() 
{
  SALOMEDS::Locker lock; 
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  SALOMEDS::StringSeq_var aTitles = new SALOMEDS::StringSeq;
  aTitles->length(aTable->GetNbRows());
  for(int i = 0; i < aTitles->length(); i++)
    aTitles[i] = CORBA::string_dup(aTable->GetRowTitle(i + 1).c_str());
  return aTitles._retn();
}

void SALOMEDS_AttributeTableOfReal_i::SetColumnTitle(CORBA::Long theIndex, const char* theTitle)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (ATR_IncorrectIndex);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  if (theIndex <= 0 || theIndex > aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
  CORBA::String_var aStr = CORBA::string_dup(theTitle);
  aTable->SetColumnTitle(theIndex, string(aStr));
}

void SALOMEDS_AttributeTableOfReal_i::SetColumnTitles(const SALOMEDS::StringSeq& theTitles)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  if (theTitles.length() != aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
  for (int i = 0; i < theTitles.length(); i++) {
    aTable->SetColumnTitle(i + 1, (char*)theTitles[i].in());
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfReal_i::GetColumnTitles() 
{
  SALOMEDS::Locker lock; 
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  SALOMEDS::StringSeq_var aTitles = new SALOMEDS::StringSeq;
  aTitles->length(aTable->GetNbColumns());
  for(int i = 0; i < aTitles->length(); i++)
    aTitles[i] = CORBA::string_dup(aTable->GetColumnTitle(i + 1).c_str());
  return aTitles._retn();
}

//Units support
void SALOMEDS_AttributeTableOfReal_i::SetRowUnit(CORBA::Long theIndex, const char* theUnit)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (ATR_IncorrectIndex);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  if (theIndex <= 0 || theIndex > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
  aTable->SetRowUnit(theIndex, string(theUnit));
}

void SALOMEDS_AttributeTableOfReal_i::SetRowUnits(const SALOMEDS::StringSeq& theUnits)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch (ATR_IncorrectArgumentLength);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  if (theUnits.length() != aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
  for (int i = 0; i < theUnits.length(); i++) {
    aTable->SetRowUnit(i + 1, (char*)theUnits[i].in());
  }
}

SALOMEDS::StringSeq* SALOMEDS_AttributeTableOfReal_i::GetRowUnits() 
{
  SALOMEDS::Locker lock; 
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  SALOMEDS::StringSeq_var aUnits = new SALOMEDS::StringSeq;
  aUnits->length(aTable->GetNbRows());
  for(int i = 0; i < aUnits->length(); i++)
    aUnits[i] = CORBA::string_dup(aTable->GetRowUnit(i + 1).c_str());
  return aUnits._retn();
}


CORBA::Long SALOMEDS_AttributeTableOfReal_i::GetNbRows() 
{
  SALOMEDS::Locker lock; 
  return dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl)->GetNbRows();
}

CORBA::Long SALOMEDS_AttributeTableOfReal_i::GetNbColumns() 
{
  SALOMEDS::Locker lock; 
  return dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl)->GetNbColumns();
}

void SALOMEDS_AttributeTableOfReal_i::AddRow(const SALOMEDS::DoubleSeq& theData)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  
  vector<double> aRow;
  for (int i = 0; i < theData.length(); i++) aRow.push_back(theData[i]);
  aTable->SetRowData(aTable->GetNbRows() + 1, aRow);
}

void SALOMEDS_AttributeTableOfReal_i::SetRow(CORBA::Long theRow, const SALOMEDS::DoubleSeq& theData)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength, SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  
  vector<double> aRow;
  for (int i = 0; i < theData.length(); i++) aRow.push_back(theData[i]);
  aTable->SetRowData(theRow, aRow);
}

SALOMEDS::DoubleSeq* SALOMEDS_AttributeTableOfReal_i::GetRow(CORBA::Long theRow)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectIndex);
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  if (theRow <= 0 || theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();

  SALOMEDS::DoubleSeq_var CorbaSeq = new SALOMEDS::DoubleSeq;
  vector<double> aRow = aTable->GetRowData(theRow);
  CorbaSeq->length(aRow.size());
  for (int i = 0; i < aRow.size(); i++) {
    CorbaSeq[i] = aRow[i];
  }
  return CorbaSeq._retn();
}

void SALOMEDS_AttributeTableOfReal_i::AddColumn(const SALOMEDS::DoubleSeq& theData)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  
  vector<double> aColumn;
  for (int i = 0; i < theData.length(); i++) aColumn.push_back(theData[i]);
  aTable->SetColumnData(aTable->GetNbColumns() + 1, aColumn);
}

void SALOMEDS_AttributeTableOfReal_i::SetColumn(CORBA::Long theColumn, const SALOMEDS::DoubleSeq& theData)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength, SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectArgumentLength);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  
  vector<double> aColumn;
  for (int i = 0; i < theData.length(); i++) aColumn.push_back(theData[i]);
  aTable->SetColumnData(theColumn, aColumn);
}

SALOMEDS::DoubleSeq* SALOMEDS_AttributeTableOfReal_i::GetColumn(CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectIndex);
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  if (theColumn <= 0 || theColumn > aTable->GetNbColumns()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();

  SALOMEDS::DoubleSeq_var CorbaSeq = new SALOMEDS::DoubleSeq;
  vector<double> aColumn = aTable->GetColumnData(theColumn);
  CorbaSeq->length(aColumn.size());
  for (int i = 0; i < aColumn.size(); i++) {
    CorbaSeq[i] = aColumn[i];
  }
  return CorbaSeq._retn();
}

void SALOMEDS_AttributeTableOfReal_i::PutValue(CORBA::Double theValue, CORBA::Long theRow, CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectIndex);
  CheckLocked();
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);

  aTable->PutValue(theValue, theRow, theColumn);
}

CORBA::Boolean SALOMEDS_AttributeTableOfReal_i::HasValue(CORBA::Long theRow, CORBA::Long theColumn) 
{
  SALOMEDS::Locker lock; 
  return dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl)->HasValue(theRow, theColumn);
}

CORBA::Double SALOMEDS_AttributeTableOfReal_i::GetValue(CORBA::Long theRow, CORBA::Long theColumn)
     throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex)
{
  SALOMEDS::Locker lock; 
  Unexpect aCatch(ATR_IncorrectIndex);
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  if (theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();

  CORBA::Double aValue;

  try {
    aValue = aTable->GetValue(theRow, theColumn);
  }
  catch(...) {
    throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
  }

  return aValue;
}


SALOMEDS::LongSeq* SALOMEDS_AttributeTableOfReal_i::GetRowSetIndices(CORBA::Long theRow) 
{
  SALOMEDS::Locker lock; 
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);

  if(theRow <= 0 || theRow > aTable->GetNbRows()) throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();

  SALOMEDS::LongSeq_var CorbaSeq = new SALOMEDS::LongSeq;
  vector<int> aSeq = aTable->GetSetRowIndices(theRow);
  int len = aSeq.size();
  CorbaSeq->length(len);
  for (int i = 0; i < len; i++) {
    CorbaSeq[i] = aSeq[i];
  }
  return CorbaSeq._retn(); 
}


void SALOMEDS_AttributeTableOfReal_i::SetNbColumns(CORBA::Long theNbColumns)
{
  SALOMEDS::Locker lock; 
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);
  aTable->SetNbColumns(theNbColumns);
}

bool SALOMEDS_AttributeTableOfReal_i::ReadFromFile(const SALOMEDS::TMPFile& theStream) 
{
  SALOMEDS::Locker lock; 
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);

  string aStream((char*)&theStream[0], theStream.length());
  aTable->Load(aStream);
  return true;
}

SALOMEDS::TMPFile*  SALOMEDS_AttributeTableOfReal_i::SaveToFile()
{
  SALOMEDS::Locker lock; 
  SALOMEDSImpl_AttributeTableOfReal* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfReal*>(_impl);

  string aString = aTable->Save();
  char* aBuffer = (char*)CORBA::string_dup(aString.c_str());
  int aBufferSize = strlen((char*)aBuffer);

  CORBA::Octet* anOctetBuf =  (CORBA::Octet*)aBuffer;

  SALOMEDS::TMPFile_var aStreamFile = new SALOMEDS::TMPFile(aBufferSize, aBufferSize, anOctetBuf, 1);

  return aStreamFile._retn();
}
