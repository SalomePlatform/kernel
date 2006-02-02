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
//  File   : SALOMEDS_AttributeTableOfString.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeTableOfString.hxx"
#include "SALOMEDS.hxx"

#include <string>
#include <TCollection_AsciiString.hxx> 
#include <TCollection_ExtendedString.hxx>
#include <TColStd_HSequenceOfInteger.hxx>
#include <TColStd_HSequenceOfReal.hxx>
#include <TColStd_HSequenceOfExtendedString.hxx>
 
SALOMEDS_AttributeTableOfString::SALOMEDS_AttributeTableOfString
                  (const Handle(SALOMEDSImpl_AttributeTableOfString)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTableOfString::SALOMEDS_AttributeTableOfString
                  (SALOMEDS::AttributeTableOfString_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTableOfString::~SALOMEDS_AttributeTableOfString()
{}


void SALOMEDS_AttributeTableOfString::SetTitle(const std::string& theTitle)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl)->SetTitle((char*)theTitle.c_str());
  }
  else SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->SetTitle(theTitle.c_str());
}

std::string SALOMEDS_AttributeTableOfString::GetTitle()
{
  std::string aStr;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aStr = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeTableOfString)::
                                   DownCast(_local_impl)->GetTitle()).ToCString();
  }
  else aStr = SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->GetTitle();
  return aStr;
}

void SALOMEDS_AttributeTableOfString::SetRowTitle(int theIndex, const std::string& theTitle)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    Handle(SALOMEDSImpl_AttributeTableOfString)::
      DownCast(_local_impl)->SetRowTitle(theIndex, (char*)theTitle.c_str());
  }
  else SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->SetRowTitle(theIndex, theTitle.c_str());
}

void SALOMEDS_AttributeTableOfString::SetRowTitles(const std::vector<std::string>& theTitles)
{
  int aLength = theTitles.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    Handle(TColStd_HSequenceOfExtendedString) aSeq = new TColStd_HSequenceOfExtendedString;
    for (i = 0; i < aLength; i++) aSeq->Append((char*)theTitles[i].c_str());
    Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl)->SetRowTitles(aSeq);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = (char*)theTitles[i].c_str();
    SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->SetRowTitles(aSeq);
  }
}

std::vector<std::string> SALOMEDS_AttributeTableOfString::GetRowTitles()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    Handle(TColStd_HSequenceOfExtendedString) aSeq;
    aSeq = Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl)->GetRowTitles();
    aLength = aSeq->Length();
    for (i = 1; i <= aLength; i++) aVector.push_back(TCollection_AsciiString(aSeq->Value(i)).ToCString());
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->GetRowTitles();
    aLength = aSeq->length();
    for (i = 0; i < aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfString::SetColumnTitle(int theIndex, const std::string& theTitle)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    Handle(SALOMEDSImpl_AttributeTableOfString)::
      DownCast(_local_impl)->SetColumnTitle(theIndex, (char*)theTitle.c_str());
  }
  else SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->SetColumnTitle(theIndex, theTitle.c_str());
}

void SALOMEDS_AttributeTableOfString::SetColumnTitles(const std::vector<std::string>& theTitles)
{
  int aLength = theTitles.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    Handle(TColStd_HSequenceOfExtendedString) aSeq = new TColStd_HSequenceOfExtendedString;
    for (i = 0; i < aLength; i++) aSeq->Append((char*)theTitles[i].c_str());
    Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl)->SetColumnTitles(aSeq);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = (char*)theTitles[i].c_str();
    SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->SetColumnTitles(aSeq);
  }
}

std::vector<std::string> SALOMEDS_AttributeTableOfString::GetColumnTitles()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    Handle(TColStd_HSequenceOfExtendedString) aSeq;
    aSeq = Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl)->GetColumnTitles();
    aLength = aSeq->Length();
    for (i = 1; i <= aLength; i++) aVector.push_back(TCollection_AsciiString(aSeq->Value(i)).ToCString());
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->GetColumnTitles();
    aLength = aSeq->length();
    for (i = 0; i < aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfString::SetRowUnit(int theIndex, const std::string& theUnit)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    Handle(SALOMEDSImpl_AttributeTableOfString)::
      DownCast(_local_impl)->SetRowUnit(theIndex, (char*)theUnit.c_str());
  }
  else SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->SetRowUnit(theIndex, theUnit.c_str());
}

void SALOMEDS_AttributeTableOfString::SetRowUnits(const std::vector<std::string>& theUnits)
{
  int aLength = theUnits.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    Handle(TColStd_HSequenceOfExtendedString) aSeq = new TColStd_HSequenceOfExtendedString;
    for (i = 0; i < aLength; i++) aSeq->Append((char*)theUnits[i].c_str());
    Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl)->SetRowUnits(aSeq);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = (char*)theUnits[i].c_str();
    SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->SetRowUnits(aSeq);
  }
}

std::vector<std::string> SALOMEDS_AttributeTableOfString::GetRowUnits()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    Handle(TColStd_HSequenceOfExtendedString) aSeq;
    aSeq = Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl)->GetRowUnits();
    aLength = aSeq->Length();
    for (i = 1; i <= aLength; i++) aVector.push_back(TCollection_AsciiString(aSeq->Value(i)).ToCString());
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->GetRowUnits();
    aLength = aSeq->length();
    for (i = 0; i < aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

int SALOMEDS_AttributeTableOfString::GetNbRows()
{
  int aNb;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aNb = Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl)->GetNbRows();
  }
  else aNb = SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->GetNbRows();
  return aNb;
}

int SALOMEDS_AttributeTableOfString::GetNbColumns()
{  
  int aNb;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aNb = Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl)->GetNbColumns();
  }
  else aNb = SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->GetNbColumns();
  return aNb;
}

void SALOMEDS_AttributeTableOfString::AddRow(const std::vector<std::string>& theData)
{
  int aLength = theData.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    Handle(SALOMEDSImpl_AttributeTableOfString) aTable;
    aTable = Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl);
    Handle(TColStd_HSequenceOfExtendedString) aRow = new TColStd_HSequenceOfExtendedString;
    for (int i = 0; i < aLength; i++) aRow->Append((char*)theData[i].c_str());
    try {
      aTable->SetRowData(aTable->GetNbRows() + 1, aRow);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfString::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = (char*)theData[i].c_str();
    SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->AddRow(aSeq);
  }
}

void SALOMEDS_AttributeTableOfString::SetRow(int theRow, const std::vector<std::string>& theData)
{
  int aLength = theData.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    Handle(SALOMEDSImpl_AttributeTableOfString) aTable;
    aTable = Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl);
    Handle(TColStd_HSequenceOfExtendedString) aRow = new TColStd_HSequenceOfExtendedString;
    for (int i = 0; i < aLength; i++) aRow->Append((char*)theData[i].c_str());
    try {
      aTable->SetRowData(theRow, aRow);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfString::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for (i = 0; i < aLength; i++) aSeq[i] = (char*)theData[i].c_str();
    SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->SetRow(theRow, aSeq);
  }
}

std::vector<std::string> SALOMEDS_AttributeTableOfString::GetRow(int theRow)
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    Handle(TColStd_HSequenceOfExtendedString) aRow; 
    aRow = Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl)->GetRowData(theRow);
    aLength = aRow->Length();
    for (i = 1; i <= aLength; i++) aVector.push_back(TCollection_AsciiString(aRow->Value(i)).ToCString());
  }
  else {
    SALOMEDS::StringSeq_var aRow = SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->GetRow(theRow);
    for (i = 0; i < aLength; i++) aVector.push_back((char*)aRow[i].in());
  }

  return aVector;
}

void SALOMEDS_AttributeTableOfString::AddColumn(const std::vector<std::string>& theData)
{
  int aLength = theData.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    Handle(SALOMEDSImpl_AttributeTableOfString) aTable;
    aTable = Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl);
    Handle(TColStd_HSequenceOfExtendedString) aColumn = new TColStd_HSequenceOfExtendedString;
    for (int i = 0; i < aLength; i++) aColumn->Append((char*)theData[i].c_str());
    try {
      aTable->SetColumnData(aTable->GetNbColumns() + 1, aColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfString::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::StringSeq_var aColumn = new SALOMEDS::StringSeq();
    aColumn->length(aLength);
    for (i = 0; i < aLength; i++) aColumn[i] = (char*)theData[i].c_str();
    SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->AddColumn(aColumn);
  }
}

void SALOMEDS_AttributeTableOfString::SetColumn(int theColumn, const std::vector<std::string>& theData)
{
  int aLength = theData.size(), i;
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    Handle(SALOMEDSImpl_AttributeTableOfString) aTable;
    aTable = Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl);
    Handle(TColStd_HSequenceOfExtendedString) aColumn = new TColStd_HSequenceOfExtendedString;
    for (int i = 0; i < aLength; i++) aColumn->Append((char*)theData[i].c_str());
    try {
      aTable->SetRowData(theColumn, aColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfString::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::StringSeq_var aColumn = new SALOMEDS::StringSeq();
    aColumn->length(aLength);
    for (i = 0; i < aLength; i++) aColumn[i] = (char*)theData[i].c_str();
    SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->SetRow(theColumn, aColumn);
  }
}

std::vector<std::string> SALOMEDS_AttributeTableOfString::GetColumn(int theColumn)
{
  std::vector<std::string> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    Handle(TColStd_HSequenceOfExtendedString) aColumn; 
    aColumn = Handle(SALOMEDSImpl_AttributeTableOfString)::
      DownCast(_local_impl)->GetColumnData(theColumn);
    aLength = aColumn->Length();
    for (i = 1; i <= aLength; i++)
      aVector.push_back(TCollection_AsciiString(aColumn->Value(i)).ToCString());
  }
  else {
    SALOMEDS::StringSeq_var aColumn =
      SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->GetColumn(theColumn);
    for (i = 0; i < aLength; i++) aVector.push_back(aColumn[i].in());
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfString::PutValue(const std::string& theValue, int theRow, int theColumn)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    try {
      Handle(SALOMEDSImpl_AttributeTableOfString)::
        DownCast(_local_impl)->PutValue((char*)theValue.c_str(), theRow, theColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfString::IncorrectIndex();
    }
  }
  else {
    SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->PutValue(theValue.c_str(), theRow, theColumn);
  }
}

bool SALOMEDS_AttributeTableOfString::HasValue(int theRow, int theColumn)
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl)->HasValue(theRow, theColumn);
  }
  else ret = SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->HasValue(theRow, theColumn);
  return ret;
}

std::string SALOMEDS_AttributeTableOfString::GetValue(int theRow, int theColumn)
{
  std::string aValue;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    try {
      aValue = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeTableOfString)::
                                       DownCast(_local_impl)->GetValue(theRow, theColumn)).ToCString();
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfString::IncorrectIndex();
    }
  }
  else {
    aValue = SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->GetValue(theRow, theColumn);
  }
  return aValue;
}

std::vector<int> SALOMEDS_AttributeTableOfString::GetRowSetIndices(int theRow)
{
  std::vector<int> aVector;
  int aLength, i;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    Handle(TColStd_HSequenceOfInteger) aSet; 
    aSet = Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl)->GetSetRowIndices(theRow);
    aLength = aSet->Length();
    for (i = 1; i <= aLength; i++) aVector.push_back(aSet->Value(i));
  }
  else {
    SALOMEDS::LongSeq_var aSet =
      SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->GetRowSetIndices(theRow);
    for (i = 0; i < aLength; i++) aVector.push_back(aSet[i]);
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfString::SetNbColumns(int theNbColumns)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    Handle(SALOMEDSImpl_AttributeTableOfString)::DownCast(_local_impl)->SetNbColumns(theNbColumns);
  }
  else SALOMEDS::AttributeTableOfString::_narrow(_corba_impl)->SetNbColumns(theNbColumns);
}
