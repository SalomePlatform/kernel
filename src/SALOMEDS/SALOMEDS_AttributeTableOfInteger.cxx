// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
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

//  File   : SALOMEDS_AttributeTableOfInteger.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeTableOfInteger.hxx"
#include "SALOMEDS.hxx"

SALOMEDS_AttributeTableOfInteger::SALOMEDS_AttributeTableOfInteger
                  (SALOMEDSImpl_AttributeTableOfInteger* theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTableOfInteger::SALOMEDS_AttributeTableOfInteger
                  (SALOMEDS::AttributeTableOfInteger_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTableOfInteger::~SALOMEDS_AttributeTableOfInteger()
{}


void SALOMEDS_AttributeTableOfInteger::SetTitle(const std::string& theTitle)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SetTitle(theTitle);
  }
  else SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetTitle(theTitle.c_str());
}

std::string SALOMEDS_AttributeTableOfInteger::GetTitle()
{
  std::string aStr;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aStr = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->GetTitle();
  }
  else aStr = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetTitle();
  return aStr;
}

void SALOMEDS_AttributeTableOfInteger::SetRowTitle(int theIndex, const std::string& theTitle)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SetRowTitle(theIndex, theTitle);
  }
  else SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetRowTitle(theIndex, theTitle.c_str());
}

std::string SALOMEDS_AttributeTableOfInteger::GetRowTitle(int theIndex)
{
  std::string aTitle;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      aTitle = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->GetRowTitle(theIndex);
    }
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectIndex();
    }
  }
  else {
    aTitle = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetRowTitle(theIndex);
  }
  return aTitle;
}

void SALOMEDS_AttributeTableOfInteger::SetRowTitles(const std::vector<std::string>& theTitles)
{
  int aLength = theTitles.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    std::vector<std::string> aSeq;
    for (i = 0; i < aLength; i++) aSeq.push_back(theTitles[i]);
    dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SetRowTitles(aSeq);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for(i = 0; i < aLength; i++) aSeq[i] = theTitles[i].c_str();
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetRowTitles(aSeq);
  }
  
}

std::vector<std::string> SALOMEDS_AttributeTableOfInteger::GetRowTitles()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->GetRowTitles();
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetRowTitles();
    aLength = aSeq->length();
    for(i = 0; i<aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfInteger::SetColumnTitle(int theIndex, const std::string& theTitle)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SetColumnTitle(theIndex, theTitle);
  }
  else SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetColumnTitle(theIndex, theTitle.c_str());
}

std::string SALOMEDS_AttributeTableOfInteger::GetColumnTitle(int theIndex)
{
  std::string aTitle;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      aTitle = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->GetColumnTitle(theIndex);
    }
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectIndex();
    }
  }
  else {
    aTitle = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetColumnTitle(theIndex);
  }
  return aTitle;
}

void SALOMEDS_AttributeTableOfInteger::SetColumnTitles(const std::vector<std::string>& theTitles)
{
  int aLength = theTitles.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    std::vector<std::string> aSeq;
    for (i = 0; i < aLength; i++) aSeq.push_back(theTitles[i]);
    dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SetColumnTitles(aSeq);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = theTitles[i].c_str();
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetColumnTitles(aSeq);
  }
}

std::vector<std::string> SALOMEDS_AttributeTableOfInteger::GetColumnTitles()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->GetColumnTitles();
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetColumnTitles();
    aLength = aSeq->length();
    for (i = 0; i<aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfInteger::SetRowUnit(int theIndex, const std::string& theUnit)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SetRowUnit(theIndex, theUnit);
  }
  else SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetRowUnit(theIndex, theUnit.c_str());
}

std::string SALOMEDS_AttributeTableOfInteger::GetRowUnit(int theIndex)
{
  std::string aTitle;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      aTitle = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->GetRowUnit(theIndex);
    }
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectIndex();
    }
  }
  else {
    aTitle = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetRowUnit(theIndex);
  }
  return aTitle;
}

void SALOMEDS_AttributeTableOfInteger::SetRowUnits(const std::vector<std::string>& theUnits)
{
  int aLength = theUnits.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SetRowUnits(theUnits);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = theUnits[i].c_str();
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetRowUnits(aSeq);
  }
}

std::vector<std::string> SALOMEDS_AttributeTableOfInteger::GetRowUnits()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->GetRowUnits();
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetRowUnits();
    aLength = aSeq->length();
    for (i = 0; i < aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

int SALOMEDS_AttributeTableOfInteger::GetNbRows()
{
  int aNb;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aNb = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->GetNbRows();
  }
  else aNb = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetNbRows();
  return aNb;
}

int SALOMEDS_AttributeTableOfInteger::GetNbColumns()
{  
  int aNb;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aNb = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->GetNbColumns();
  }
  else aNb = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetNbColumns();
  return aNb;
  
}

void SALOMEDS_AttributeTableOfInteger::AddRow(const std::vector<int>& theData)
{
  int aLength = theData.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTableOfInteger* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl);
    try {
      aTable->SetRowData(aTable->GetNbRows() + 1, theData);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::LongSeq_var aSeq = new SALOMEDS::LongSeq();
    aSeq->length(aLength);
    for(i = 0; i < aLength; i++) aSeq[i] = theData[i];
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->AddRow(aSeq);
  }
}

void SALOMEDS_AttributeTableOfInteger::SetRow(int theRow, const std::vector<int>& theData)
{
  int aLength = theData.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTableOfInteger* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl);
    try {
      aTable->SetRowData(theRow, theData);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::LongSeq_var aSeq = new SALOMEDS::LongSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = theData[i];
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetRow(theRow, aSeq);
  }
}

std::vector<int> SALOMEDS_AttributeTableOfInteger::GetRow(int theRow)
{
  std::vector<int> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->GetRowData(theRow);
  }
  else {
    SALOMEDS::LongSeq_var aRow = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetRow(theRow);
    aLength = aRow->length();
    for (i = 0; i < aLength; i++) aVector.push_back(aRow[i]);
  }

  return aVector;
}

void SALOMEDS_AttributeTableOfInteger::AddColumn(const std::vector<int>& theData)
{
  int aLength = theData.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTableOfInteger* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl);
    try {
      aTable->SetColumnData(aTable->GetNbColumns() + 1, theData);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::LongSeq_var aColumn = new SALOMEDS::LongSeq();
    aColumn->length(aLength);
    for(i = 0; i < aLength; i++) aColumn[i] = theData[i];
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->AddColumn(aColumn);
  }
}

void SALOMEDS_AttributeTableOfInteger::SetColumn(int theColumn, const std::vector<int>& theData)
{
  int aLength = theData.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeTableOfInteger* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl);
    try {
      aTable->SetColumnData(theColumn, theData);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::LongSeq_var aColumn = new SALOMEDS::LongSeq();
    aColumn->length(aLength);
    for (i = 0; i < aLength; i++) aColumn[i] = theData[i];
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetColumn(theColumn, aColumn);
  }
}

std::vector<int> SALOMEDS_AttributeTableOfInteger::GetColumn(int theColumn)
{
  std::vector<int> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->GetColumnData(theColumn);
  }
  else {
    SALOMEDS::LongSeq_var aColumn = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetColumn(theColumn);
    aLength = aColumn->length();
    for (i = 0; i < aLength; i++) aVector.push_back(aColumn[i]);
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfInteger::PutValue(int theValue, int theRow, int theColumn)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    try {
      dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->PutValue(theValue, theRow, theColumn);
    }
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectIndex();
    }
  }
  else {
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->PutValue(theValue, theRow, theColumn);
  }
}

bool SALOMEDS_AttributeTableOfInteger::HasValue(int theRow, int theColumn)
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->HasValue(theRow, theColumn);
  }
  else ret = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->HasValue(theRow, theColumn);
  return ret;
}

int SALOMEDS_AttributeTableOfInteger::GetValue(int theRow, int theColumn)
{
  int aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      aValue = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->GetValue(theRow, theColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectIndex();
    }
  }
  else {
    aValue = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetValue(theRow, theColumn);
  }
  return aValue;
}

void SALOMEDS_AttributeTableOfInteger::RemoveValue(int theRow, int theColumn)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->RemoveValue(theRow, theColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectIndex();
    }
  }
  else {
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->RemoveValue(theRow, theColumn);
  }
}

std::vector<int> SALOMEDS_AttributeTableOfInteger::GetRowSetIndices(int theRow)
{
  std::vector<int> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->GetSetRowIndices(theRow);
  }
  else {
    SALOMEDS::LongSeq_var aSet =
      SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetRowSetIndices(theRow);
    aLength = aSet->length();  
    for (i = 0; i < aLength; i++) aVector.push_back(aSet[i]);
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfInteger::SetNbColumns(int theNbColumns)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SetNbColumns(theNbColumns);
  }
  else SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetNbColumns(theNbColumns);
}

std::vector<int> SALOMEDS_AttributeTableOfInteger::SortRow(int theRow, SortOrder theOrder, SortPolicy thePolicy)
{
  std::vector<int> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SortRow(theRow, 
                                                                                          (SALOMEDSImpl_AttributeTable::SortOrder)theOrder, 
                                                                                          (SALOMEDSImpl_AttributeTable::SortPolicy)thePolicy);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectIndex();
    }
  }
  else {
    SALOMEDS::LongSeq_var aSet =
      SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SortRow(theRow, 
                                                                       (SALOMEDS::AttributeTable::SortOrder)theOrder, 
                                                                       (SALOMEDS::AttributeTable::SortPolicy)thePolicy);
    aLength = aSet->length();  
    for (i = 0; i < aLength; i++) aVector.push_back(aSet[i]);
  }
  return aVector;
}

std::vector<int> SALOMEDS_AttributeTableOfInteger::SortColumn(int theColumn, SortOrder theOrder, SortPolicy thePolicy)
{
  std::vector<int> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SortColumn(theColumn, 
                                                                                             (SALOMEDSImpl_AttributeTable::SortOrder)theOrder, 
                                                                                             (SALOMEDSImpl_AttributeTable::SortPolicy)thePolicy);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectIndex();
    }
  }
  else {
    SALOMEDS::LongSeq_var aSet =
      SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SortColumn(theColumn, 
                                                                          (SALOMEDS::AttributeTable::SortOrder)theOrder, 
                                                                          (SALOMEDS::AttributeTable::SortPolicy)thePolicy);
    aLength = aSet->length();  
    for (i = 0; i < aLength; i++) aVector.push_back(aSet[i]);
  }
  return aVector;
}

std::vector<int> SALOMEDS_AttributeTableOfInteger::SortByRow(int theRow, SortOrder theOrder, SortPolicy thePolicy)
{
  std::vector<int> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SortByRow(theRow, 
                                                                                            (SALOMEDSImpl_AttributeTable::SortOrder)theOrder, 
                                                                                            (SALOMEDSImpl_AttributeTable::SortPolicy)thePolicy);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectIndex();
    }
  }
  else {
    SALOMEDS::LongSeq_var aSet =
      SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SortByRow(theRow, 
                                                                         (SALOMEDS::AttributeTable::SortOrder)theOrder, 
                                                                         (SALOMEDS::AttributeTable::SortPolicy)thePolicy);
    aLength = aSet->length();  
    for (i = 0; i < aLength; i++) aVector.push_back(aSet[i]);
  }
  return aVector;
}

std::vector<int> SALOMEDS_AttributeTableOfInteger::SortByColumn(int theColumn, SortOrder theOrder, SortPolicy thePolicy)
{
  std::vector<int> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      aVector = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SortByColumn(theColumn, 
                                                                                               (SALOMEDSImpl_AttributeTable::SortOrder)theOrder, 
                                                                                               (SALOMEDSImpl_AttributeTable::SortPolicy)thePolicy);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectIndex();
    }
  }
  else {
    SALOMEDS::LongSeq_var aSet =
      SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SortByColumn(theColumn, 
                                                                            (SALOMEDS::AttributeTable::SortOrder)theOrder, 
                                                                            (SALOMEDS::AttributeTable::SortPolicy)thePolicy);
    aLength = aSet->length();  
    for (i = 0; i < aLength; i++) aVector.push_back(aSet[i]);
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfInteger::SwapCells(int theRow1, int theColumn1, int theRow2, int theColumn2)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SwapCells(theRow1, theColumn1, theRow2, theColumn2);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectIndex();
    }
  }
  else {
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SwapCells(theRow1, theColumn1, theRow2, theColumn2);
  }
}

void SALOMEDS_AttributeTableOfInteger::SwapRows(int theRow1, int theRow2)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SwapRows(theRow1, theRow2);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectIndex();
    }
  }
  else {
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SwapRows(theRow1, theRow2);
  }
}

void SALOMEDS_AttributeTableOfInteger::SwapColumns(int theColumn1, int theColumn2)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(_local_impl)->SwapColumns(theColumn1, theColumn2);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTable::IncorrectIndex();
    }
  }
  else {
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SwapColumns(theColumn1, theColumn2);
  }
}
