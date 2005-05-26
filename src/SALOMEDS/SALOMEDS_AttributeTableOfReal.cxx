//  File   : SALOMEDS_AttributeTableOfReal.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeTableOfReal.hxx"

#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TColStd_HSequenceOfInteger.hxx>
#include <TColStd_HSequenceOfReal.hxx>
#include <TColStd_HSequenceOfExtendedString.hxx>


SALOMEDS_AttributeTableOfReal
::SALOMEDS_AttributeTableOfReal(const Handle(SALOMEDSImpl_AttributeTableOfReal)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTableOfReal::SALOMEDS_AttributeTableOfReal(SALOMEDS::AttributeTableOfReal_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTableOfReal::~SALOMEDS_AttributeTableOfReal()
{}


void SALOMEDS_AttributeTableOfReal::SetTitle(const std::string& theTitle)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->SetTitle((char*)theTitle.c_str());
  else SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetTitle(theTitle.c_str());
}

std::string SALOMEDS_AttributeTableOfReal::GetTitle()
{
  std::string aStr;
  if(_isLocal) 
    aStr = TCollection_AsciiString(Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->GetTitle()).ToCString();
  else aStr = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetTitle();
  return aStr;
}

void SALOMEDS_AttributeTableOfReal::SetRowTitle(int theIndex, const std::string& theTitle)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->SetRowTitle(theIndex, 
											     (char*)theTitle.c_str());
  else SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetRowTitle(theIndex, theTitle.c_str());
}

void SALOMEDS_AttributeTableOfReal::SetRowTitles(const std::vector<std::string>& theTitles)
{
  CheckLocked();
  int aLength = theTitles.size(), i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfExtendedString) aSeq = new TColStd_HSequenceOfExtendedString;
    for(i = 0; i < aLength; i++) aSeq->Append((char*)theTitles[i].c_str());
    Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->SetRowTitles(aSeq);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for(i = 0; i < aLength; i++) aSeq[i] = (char*)theTitles[i].c_str();
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetRowTitles(aSeq);
  }
  
}

std::vector<std::string> SALOMEDS_AttributeTableOfReal::GetRowTitles()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfExtendedString) aSeq;
    aSeq = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->GetRowTitles();
    aLength = aSeq->Length();
    for(i = 1; i<= aLength; i++) aVector.push_back(TCollection_AsciiString(aSeq->Value(i)).ToCString());
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetRowTitles();
    aLength = aSeq->length();
    for(i = 0; i<aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfReal::SetColumnTitle(int theIndex, const std::string& theTitle)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->SetColumnTitle(theIndex, 
												(char*)theTitle.c_str());
  else SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetColumnTitle(theIndex, theTitle.c_str());
}

void SALOMEDS_AttributeTableOfReal::SetColumnTitles(const std::vector<std::string>& theTitles)
{
  CheckLocked();
  int aLength = theTitles.size(), i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfExtendedString) aSeq = new TColStd_HSequenceOfExtendedString;
    for(i = 0; i < aLength; i++) aSeq->Append((char*)theTitles[i].c_str());
    Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->SetColumnTitles(aSeq);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for(i = 0; i < aLength; i++) aSeq[i] = (char*)theTitles[i].c_str();
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetColumnTitles(aSeq);
  }
}

std::vector<std::string> SALOMEDS_AttributeTableOfReal::GetColumnTitles()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfExtendedString) aSeq;
    aSeq = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->GetColumnTitles();
    aLength = aSeq->Length();
    for(i = 1; i<= aLength; i++) aVector.push_back(TCollection_AsciiString(aSeq->Value(i)).ToCString());
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetColumnTitles();
    aLength = aSeq->length();
    for(i = 0; i<aLength; i++) aVector.push_back((char*)aSeq[i].in());
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfReal::SetRowUnit(int theIndex, const std::string& theUnit)
{
  CheckLocked();
  if(_isLocal) Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->SetRowUnit(theIndex, 
											    (char*)theUnit.c_str());
  else SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetRowUnit(theIndex, theUnit.c_str());
}

void SALOMEDS_AttributeTableOfReal::SetRowUnits(const std::vector<std::string>& theUnits)
{
  CheckLocked();
  int aLength = theUnits.size(), i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfExtendedString) aSeq = new TColStd_HSequenceOfExtendedString;
    for(i = 0; i < aLength; i++) aSeq->Append((char*)theUnits[i].c_str());
    Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->SetRowUnits(aSeq);
  }
  else {
    SALOMEDS::StringSeq_var aSeq = new SALOMEDS::StringSeq();
    aSeq->length(aLength);
    for(i = 0; i < aLength; i++) aSeq[i] = (char*)theUnits[i].c_str();
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetRowUnits(aSeq);
  }
}

std::vector<std::string> SALOMEDS_AttributeTableOfReal::GetRowUnits()
{
  std::vector<std::string> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfExtendedString) aSeq;
    aSeq = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->GetRowUnits();
    aLength = aSeq->Length();
    for(i = 1; i<= aLength; i++) aVector.push_back(TCollection_AsciiString(aSeq->Value(i)).ToCString());
  }
  else {
    SALOMEDS::StringSeq_var aSeq = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetRowUnits();
    aLength = aSeq->length();
    for(i = 0; i<aLength; i++) aVector.push_back(TCollection_AsciiString((char*)aSeq[i].in()).ToCString());
  }
  return aVector;
}

int SALOMEDS_AttributeTableOfReal::GetNbRows()
{
  int aNb;
  if(_isLocal) aNb = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->GetNbRows();
  else aNb = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetNbRows();
  return aNb;
}

int SALOMEDS_AttributeTableOfReal::GetNbColumns()
{  
  int aNb;
  if(_isLocal) aNb = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->GetNbColumns();
  else aNb = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetNbColumns();
  return aNb;
  
}

void SALOMEDS_AttributeTableOfReal::AddRow(const std::vector<double>& theData)
{
  CheckLocked();
  int aLength = theData.size(), i;
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTableOfReal) aTable;
    aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl);
    Handle(TColStd_HSequenceOfReal) aRow = new TColStd_HSequenceOfReal;
    for (int i = 0; i < aLength; i++) aRow->Append(theData[i]);
    try {
      aTable->SetRowData(aTable->GetNbRows() + 1, aRow);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::DoubleSeq_var aSeq = new SALOMEDS::DoubleSeq();
    aSeq->length(aLength);
    for(i = 0; i < aLength; i++) aSeq[i] = theData[i];
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->AddRow(aSeq);
  }
}

void SALOMEDS_AttributeTableOfReal::SetRow(int theRow, const std::vector<double>& theData)
{
  CheckLocked();
  int aLength = theData.size(), i;
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTableOfReal) aTable;
    aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl);
    Handle(TColStd_HSequenceOfReal) aRow = new TColStd_HSequenceOfReal;
    for (int i = 0; i < aLength; i++) aRow->Append(theData[i]);
    try {
      aTable->SetRowData(theRow, aRow);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::DoubleSeq_var aSeq = new SALOMEDS::DoubleSeq();
    aSeq->length(aLength);
    for(i = 0; i < aLength; i++) aSeq[i] = theData[i];
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetRow(theRow, aSeq);
  }
}

std::vector<double> SALOMEDS_AttributeTableOfReal::GetRow(int theRow)
{
  std::vector<double> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfReal) aRow; 
    aRow = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->GetRowData(theRow);
    aLength = aRow->Length();
    for(i = 1; i<= aLength; i++) aVector.push_back(aRow->Value(i));
  }
  else {
    SALOMEDS::DoubleSeq_var aRow = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetRow(theRow);
    for(i = 0; i < aLength; i++) aVector.push_back(aRow[i]);
  }

  return aVector;
}

void SALOMEDS_AttributeTableOfReal::AddColumn(const std::vector<double>& theData)
{
  CheckLocked();
  int aLength = theData.size(), i;
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTableOfReal) aTable;
    aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl);
    Handle(TColStd_HSequenceOfReal) aColumn = new TColStd_HSequenceOfReal;
    for (int i = 0; i < aLength; i++) aColumn->Append(theData[i]);
    try {
      aTable->SetColumnData(aTable->GetNbColumns() + 1, aColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::DoubleSeq_var aColumn = new SALOMEDS::DoubleSeq();
    aColumn->length(aLength);
    for(i = 0; i < aLength; i++) aColumn[i] = theData[i];
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->AddColumn(aColumn);
  }
}

void SALOMEDS_AttributeTableOfReal::SetColumn(int theColumn, const std::vector<double>& theData)
{
  CheckLocked();
  int aLength = theData.size(), i;
  if(_isLocal) {
    Handle(SALOMEDSImpl_AttributeTableOfReal) aTable;
    aTable = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl);
    Handle(TColStd_HSequenceOfReal) aColumn = new TColStd_HSequenceOfReal;
    for (int i = 0; i < aLength; i++) aColumn->Append(theData[i]);
    try {
      aTable->SetRowData(theColumn, aColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength();
    }
  }
  else {
    SALOMEDS::DoubleSeq_var aColumn = new SALOMEDS::DoubleSeq();
    aColumn->length(aLength);
    for(i = 0; i < aLength; i++) aColumn[i] = theData[i];
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetRow(theColumn, aColumn);
  }
}

std::vector<double> SALOMEDS_AttributeTableOfReal::GetColumn(int theColumn)
{
  std::vector<double> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfReal) aColumn; 
    aColumn = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->GetColumnData(theColumn);
    aLength = aColumn->Length();
    for(i = 1; i<= aLength; i++) aVector.push_back(aColumn->Value(i));
  }
  else {
    SALOMEDS::DoubleSeq_var aColumn = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetColumn(theColumn);
    for(i = 0; i < aLength; i++) aVector.push_back(aColumn[i]);
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfReal::PutValue(double theValue, int theRow, int theColumn)
{
  CheckLocked();
  if(_isLocal) {
    try {
      Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->PutValue(theValue, theRow, theColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
    }
  }
  else {
    SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->PutValue(theValue, theRow, theColumn);
  }
}

bool SALOMEDS_AttributeTableOfReal::HasValue(int theRow, int theColumn)
{
  bool ret;
  if(_isLocal) ret = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->HasValue(theRow, theColumn);
  else ret = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->HasValue(theRow, theColumn);
  return ret;
}

double SALOMEDS_AttributeTableOfReal::GetValue(int theRow, int theColumn)
{
  double aValue;
  if(_isLocal) {
    try {
      aValue = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->GetValue(theRow, theColumn);
    }   
    catch(...) {
      throw SALOMEDS::AttributeTableOfReal::IncorrectIndex();
    }
  }
  else {
    aValue = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetValue(theRow, theColumn);
  }
  return aValue;
}

std::vector<int> SALOMEDS_AttributeTableOfReal::GetRowSetIndices(int theRow)
{
  std::vector<int> aVector;
  int aLength, i;
  if(_isLocal) {
    Handle(TColStd_HSequenceOfInteger) aSet; 
    aSet = Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->GetSetRowIndices(theRow);
    aLength = aSet->Length();
    for(i = 1; i<= aLength; i++) aVector.push_back(aSet->Value(i));
  }
  else {
    SALOMEDS::LongSeq_var aSet = SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->GetRowSetIndices(theRow);
    for(i = 0; i < aLength; i++) aVector.push_back(aSet[i]);
  }
  return aVector;
}

void SALOMEDS_AttributeTableOfReal::SetNbColumns(int theNbColumns)
{
  if(_isLocal) Handle(SALOMEDSImpl_AttributeTableOfReal)::DownCast(_local_impl)->SetNbColumns(theNbColumns);
  else SALOMEDS::AttributeTableOfReal::_narrow(_corba_impl)->SetNbColumns(theNbColumns);
}
