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
//  File   : SALOMEDS_AttributeTableOfInteger.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeTableOfInteger.hxx"

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TColStd_HSequenceOfInteger.hxx>
#include <TColStd_HSequenceOfExtendedString.hxx>


SALOMEDS_AttributeTableOfInteger
::SALOMEDS_AttributeTableOfInteger(const Handle(SALOMEDSImpl_AttributeTableOfInteger)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTableOfInteger::SALOMEDS_AttributeTableOfInteger(SALOMEDS::AttributeTableOfInteger_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTableOfInteger::~SALOMEDS_AttributeTableOfInteger()
{}


void SALOMEDS_AttributeTableOfInteger::SetTitle(const std::string& theTitle)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->SetTitle((char*)theTitle.c_str());
  else SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetTitle(theTitle.c_str());
}

std::string SALOMEDS_AttributeTableOfInteger::GetTitle()
{
  std::string aStr;
  if(_isLocal) 
    aStr = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->GetTitle()).ToCString();
  else aStr = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetTitle();
  return aStr;
}

void SALOMEDS_AttributeTableOfInteger::SetRowTitle(int theIndex, const std::string& theTitle)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->SetRowTitle(theIndex, 
												(char*)theTitle.c_str());
  else SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetRowTitle(theIndex, theTitle.c_str());
}

void SALOMEDS_AttributeTableOfInteger::SetRowTitles(const std::vector<std::string>& theTitles)
{
  CheckLocked();
  int aLength = theTitles.size(), i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfExtendedString) aSeq = new TColStd_HSequenceOfExtendedString;
    for(i = 0; i < aLength; i++) aSeq->Append((char*)theTitles[i].c_str());
    Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->SetRowTitles(aSeq);
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
  if(_isLocal) {
    Handle(TColStd_HSequenceOfExtendedString) aSeq;
    aSeq = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->GetRowTitles();
    aLength = aSeq->Length();
    for(i = 1; i<= aLength; i++) aVector.push_back(TCollection_AsciiString((aSeq->Value(i))).ToCString());
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
  CheckLocked();
  if(_isLocal) 
    Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->SetColumnTitle(theIndex, 
											(char*)theTitle.c_str());
  else SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetColumnTitle(theIndex, theTitle.c_str());
}

void SALOMEDS_AttributeTableOfInteger::SetColumnTitles(const std::vector<std::string>& theTitles)
{
  CheckLocked();
  int aLength = theTitles.size(), i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfExtendedString) aSeq = new TColStd_HSequenceOfExtendedString;
    for(i = 0; i < aLength; i++) aSeq->Append((char*)theTitles[i].c_str());
    Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->SetColumnTitles(aSeq);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for(i = 0; i < aLength; i++) aSeq[i] = theTitles[i].c_str();
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetColumnTitles(aSeq);
  }
}

std::vector<std::string> SALOMEDS_AttributeTableOfInteger::GetColumnTitles()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfExtendedString) aSeq;
    aSeq = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->GetColumnTitles();
    aLength = aSeq->Length();
    for(i = 1; i<= aLength; i++) aVector.push_back(TCollection_AsciiString(aSeq->Value(i)).ToCString());
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetColumnTitles();
    aLength = aSeq->length();
    for(i = 0; i<aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfInteger::SetRowUnit(int theIndex, const std::string& theUnit)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->SetRowUnit(theIndex, 
											       (char*)theUnit.c_str()); 
  else SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetRowUnit(theIndex, theUnit.c_str());
}

void SALOMEDS_AttributeTableOfInteger::SetRowUnits(const std::vector<std::string>& theUnits)
{
  CheckLocked();
  int aLength = theUnits.size(), i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfExtendedString) aSeq = new TColStd_HSequenceOfExtendedString;
    for(i = 0; i < aLength; i++) aSeq->Append((char*)theUnits[i].c_str());
    Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->SetRowUnits(aSeq);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for(i = 0; i < aLength; i++) aSeq[i] = (char*)theUnits[i].c_str();
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetRowUnits(aSeq);
  }
}

std::vector<std::string> SALOMEDS_AttributeTableOfInteger::GetRowUnits()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfExtendedString) aSeq;
    aSeq = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->GetRowUnits();
    aLength = aSeq->Length();
    for(i = 1; i<= aLength; i++) aVector.push_back(TCollection_AsciiString(aSeq->Value(i)).ToCString());
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetRowUnits();
    aLength = aSeq->length();
    for(i = 0; i<aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

int SALOMEDS_AttributeTableOfInteger::GetNbRows()
{
  int aNb;
  if(_isLocal) aNb = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->GetNbRows();
  else aNb = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetNbRows();
  return aNb;
}

int SALOMEDS_AttributeTableOfInteger::GetNbColumns()
{  
  int aNb;
  if(_isLocal) aNb = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->GetNbColumns();
  else aNb = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetNbColumns();
  return aNb;
  
}

void SALOMEDS_AttributeTableOfInteger::AddRow(const std::vector<int>& theData)
{
  CheckLocked();
  int aLength = theData.size(), i;
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable;
    aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl);
    Handle(TColStd_HSequenceOfInteger) aRow = new TColStd_HSequenceOfInteger;
    for (int i = 0; i < aLength; i++) aRow->Append(theData[i]);
    try {
      aTable->SetRowData(aTable->GetNbRows() + 1, aRow);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength();
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
  CheckLocked();
  int aLength = theData.size(), i;
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable;
    aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl);
    Handle(TColStd_HSequenceOfInteger) aRow = new TColStd_HSequenceOfInteger;
    for (int i = 0; i < aLength; i++) aRow->Append(theData[i]);
    try {
      aTable->SetRowData(theRow, aRow);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::LongSeq_var aSeq = new SALOMEDS::LongSeq();
    aSeq->length(aLength);
    for(i = 0; i < aLength; i++) aSeq[i] = theData[i];
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetRow(theRow, aSeq);
  }
}

std::vector<int> SALOMEDS_AttributeTableOfInteger::GetRow(int theRow)
{
  std::vector<int> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfInteger) aRow; 
    aRow = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->GetRowData(theRow);
    aLength = aRow->Length();
    for(i = 1; i<= aLength; i++) aVector.push_back(aRow->Value(i));
  }
  else {
    SALOMEDS::LongSeq_var aRow = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetRow(theRow);
    for(i = 0; i < aLength; i++) aVector.push_back(aRow[i]);
  }

  return aVector;
}

void SALOMEDS_AttributeTableOfInteger::AddColumn(const std::vector<int>& theData)
{
  CheckLocked();
  int aLength = theData.size(), i;
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable;
    aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl);
    Handle(TColStd_HSequenceOfInteger) aColumn = new TColStd_HSequenceOfInteger;
    for (int i = 0; i < aLength; i++) aColumn->Append(theData[i]);
    try {
      aTable->SetColumnData(aTable->GetNbColumns() + 1, aColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength();
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
  CheckLocked();
  int aLength = theData.size(), i;
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTableOfInteger) aTable;
    aTable = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl);
    Handle(TColStd_HSequenceOfInteger) aColumn = new TColStd_HSequenceOfInteger;
    for (int i = 0; i < aLength; i++) aColumn->Append(theData[i]);
    try {
      aTable->SetRowData(theColumn, aColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::LongSeq_var aColumn = new SALOMEDS::LongSeq();
    aColumn->length(aLength);
    for(i = 0; i < aLength; i++) aColumn[i] = theData[i];
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetRow(theColumn, aColumn);
  }
}

std::vector<int> SALOMEDS_AttributeTableOfInteger::GetColumn(int theColumn)
{
  std::vector<int> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfInteger) aColumn; 
    aColumn = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->GetColumnData(theColumn);
    aLength = aColumn->Length();
    for(i = 1; i<= aLength; i++) aVector.push_back(aColumn->Value(i));
  }
  else {
    SALOMEDS::LongSeq_var aColumn = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetColumn(theColumn);
    for(i = 0; i < aLength; i++) aVector.push_back(aColumn[i]);
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfInteger::PutValue(int theValue, int theRow, int theColumn)
{
  CheckLocked();
  if(_isLocal) {
    try {
      Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->PutValue(theValue, theRow, theColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfInteger::IncorrectIndex();
    }
  }
  else {
    SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->PutValue(theValue, theRow, theColumn);
  }
}

bool SALOMEDS_AttributeTableOfInteger::HasValue(int theRow, int theColumn)
{
  bool ret;
  if(_isLocal) ret = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->HasValue(theRow, theColumn);
  else ret = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->HasValue(theRow, theColumn);
  return ret;
}

int SALOMEDS_AttributeTableOfInteger::GetValue(int theRow, int theColumn)
{
  int aValue;
  if(_isLocal) {
    try {
      aValue = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->GetValue(theRow, theColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfInteger::IncorrectIndex();
    }
  }
  else {
    aValue = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetValue(theRow, theColumn);
  }
  return aValue;
}

std::vector<int> SALOMEDS_AttributeTableOfInteger::GetRowSetIndices(int theRow)
{
  std::vector<int> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfInteger) aSet; 
    aSet = Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->GetSetRowIndices(theRow);
    aLength = aSet->Length();
    for(i = 1; i<= aLength; i++) aVector.push_back(aSet->Value(i));
  }
  else {
    SALOMEDS::LongSeq_var aSet = SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->GetRowSetIndices(theRow);
    for(i = 0; i < aLength; i++) aVector.push_back(aSet[i]);
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfInteger::SetNbColumns(int theNbColumns)
{
  if(_isLocal) Handle(SALOMEDSImpl_AttributeTableOfInteger)::DownCast(_local_impl)->SetNbColumns(theNbColumns);
  else SALOMEDS::AttributeTableOfInteger::_narrow(_corba_impl)->SetNbColumns(theNbColumns);
}
