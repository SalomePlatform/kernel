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

//  File   : SALOMEDSImpl_AttributeTableOfInteger.cxx
//  Author : Michael Ponikarov
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeTableOfInteger.hxx"

#include <sstream>
#include <algorithm>

#define SEPARATOR '\1'
typedef std::map<int, int>::const_iterator MI;

static std::string getUnit(std::string theString)
{
  std::string aString(theString);
  int aPos = aString.find(SEPARATOR);
  if(aPos <= 0 || aPos == aString.size() ) return std::string();
  return aString.substr(aPos+1, aString.size());
}

static std::string getTitle(std::string theString)
{
  std::string aString(theString);
  int aPos = aString.find(SEPARATOR);
  if(aPos < 1) return aString;
  if(aPos == 0) return std::string();
  return aString.substr(0, aPos);
}

const std::string& SALOMEDSImpl_AttributeTableOfInteger::GetID() 
{
  static std::string SALOMEDSImpl_AttributeTableOfIntegerID ("128371A0-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeTableOfIntegerID;
}

SALOMEDSImpl_AttributeTableOfInteger* SALOMEDSImpl_AttributeTableOfInteger::Set(const DF_Label& label) 
{
  SALOMEDSImpl_AttributeTableOfInteger* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeTableOfInteger*)label.FindAttribute(SALOMEDSImpl_AttributeTableOfInteger::GetID()))) {
    A = new SALOMEDSImpl_AttributeTableOfInteger();
    label.AddAttribute(A);
  }
  return A;
}

SALOMEDSImpl_AttributeTableOfInteger::SALOMEDSImpl_AttributeTableOfInteger() 
:SALOMEDSImpl_GenericAttribute("AttributeTableOfInteger")
{
  myNbRows = 0;
  myNbColumns = 0;
}

void SALOMEDSImpl_AttributeTableOfInteger::SetNbColumns(const int theNbColumns)
{
  CheckLocked();  
  Backup();
  
  std::map<int, int> aMap;
  aMap = myTable;
  myTable.clear();

  for(MI p = aMap.begin(); p != aMap.end(); p++) {
    int aRow = (int)(p->first/myNbColumns) + 1;
    int aCol = (int)(p->first - myNbColumns*(aRow-1));
    if(aCol == 0) { aCol = myNbColumns; aRow--; }
    if(aCol > theNbColumns) continue;
    int aKey = (aRow-1)*theNbColumns+aCol;
    myTable[aKey] = p->second;
  }

  myNbColumns = theNbColumns;

  while (myCols.size() < myNbColumns) { // append empty columns titles
    myCols.push_back(std::string(""));
  }

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeTableOfInteger::SetTitle(const std::string& theTitle) 
{
  CheckLocked();  
  Backup();
  myTitle = theTitle;

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

std::string SALOMEDSImpl_AttributeTableOfInteger::GetTitle() const 
{
  return myTitle;
}

void SALOMEDSImpl_AttributeTableOfInteger::SetRowData(const int theRow,
                                                      const std::vector<int>& theData) 
{
  CheckLocked();  
  if(theData.size() > myNbColumns) SetNbColumns(theData.size());

  Backup();

  while (myRows.size() < theRow) { // append new row titles
    myRows.push_back(std::string(""));
  }

  int i, aShift = (theRow-1)*myNbColumns, aLength = theData.size();
  for(i = 1; i <= aLength; i++) {
    myTable[aShift + i] = theData[i-1];
  }

  if(theRow > myNbRows) myNbRows = theRow;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

std::vector<int> SALOMEDSImpl_AttributeTableOfInteger::GetRowData(const int theRow)
{
  std::vector<int> aSeq;
  int i, aShift = (theRow-1)*myNbColumns;
  for(i = 1; i <= myNbColumns; i++) {
     if(myTable.find(aShift+i) != myTable.end()) 
       aSeq.push_back(myTable[aShift+i]);
     else
       aSeq.push_back(0);
  }
  
  return aSeq;
}

void SALOMEDSImpl_AttributeTableOfInteger::SetRowTitle(const int theRow,
                                                       const std::string& theTitle) 
{
  CheckLocked();  
  Backup();
  std::string aTitle(theTitle), aUnit = GetRowUnit(theRow);
  if(aUnit.size()>0) {
    aTitle += SEPARATOR;
    aTitle += aUnit;
  }
  myRows[theRow-1] =  aTitle;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeTableOfInteger::SetRowUnit(const int theRow,
                                                      const std::string& theUnit) 
{
  CheckLocked();  
  Backup();
  std::string aTitle = GetRowTitle(theRow);
  aTitle += SEPARATOR;
  aTitle += theUnit;

  myRows[theRow-1] = aTitle;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeTableOfInteger::SetRowUnits(const std::vector<std::string>& theUnits)
{
  if (theUnits.size() != GetNbRows()) throw DFexception("Invalid number of rows");
  int aLength = theUnits.size(), i;
  for(i = 1; i <= aLength; i++) SetRowUnit(i, theUnits[i-1]);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

std::vector<std::string> SALOMEDSImpl_AttributeTableOfInteger::GetRowUnits()
{
  std::vector<std::string> aSeq;
  int aLength = myRows.size(), i;
  for(i=0; i<aLength; i++) aSeq.push_back(getUnit(myRows[i]));
  return aSeq;
}

void SALOMEDSImpl_AttributeTableOfInteger::SetRowTitles(const std::vector<std::string>& theTitles)
{
  if (theTitles.size() != GetNbRows()) throw DFexception("Invalid number of rows");
  int aLength = theTitles.size(), i;
  for(i = 1; i <= aLength; i++) SetRowTitle(i, theTitles[i-1]);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

std::vector<std::string> SALOMEDSImpl_AttributeTableOfInteger::GetRowTitles()
{
  std::vector<std::string> aSeq;
  int aLength = myRows.size(), i;
  for(i=0; i<aLength; i++) aSeq.push_back(getTitle(myRows[i]));
  return aSeq;
}

std::string SALOMEDSImpl_AttributeTableOfInteger::GetRowTitle(const int theRow) const 
{
  return getTitle(myRows[theRow-1]);
}

std::string SALOMEDSImpl_AttributeTableOfInteger::GetRowUnit(const int theRow) const 
{
  return getUnit(myRows[theRow-1]);
}

void SALOMEDSImpl_AttributeTableOfInteger::SetColumnData(const int theColumn,
                                                         const std::vector<int>& theData) 
{
  CheckLocked();  
  if(theColumn > myNbColumns) SetNbColumns(theColumn);

  Backup();

  int i, aLength = theData.size();
  for(i = 1; i <= aLength; i++) {
    myTable[myNbColumns*(i-1)+theColumn] = theData[i-1];
  }

  if(aLength > myNbRows) {
    myNbRows = aLength;
    while (myRows.size() < myNbRows) { // append empty row titles
      myRows.push_back(std::string(""));
    }
  }
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}


std::vector<int> SALOMEDSImpl_AttributeTableOfInteger::GetColumnData(const int theColumn)
{
  std::vector<int> aSeq;
  int i, anIndex;
  for(i = 1; i <= myNbRows; i++) {
    anIndex = myNbColumns*(i-1) + theColumn;
    if(myTable.find(anIndex) != myTable.end()) 
      aSeq.push_back(myTable[anIndex]);
    else
      aSeq.push_back(0);
  }
  
  return aSeq;
}

void SALOMEDSImpl_AttributeTableOfInteger::SetColumnTitle(const int theColumn,
							  const std::string& theTitle) 
{
  CheckLocked();                                                      
  Backup();
  while(myCols.size() < theColumn) myCols.push_back(std::string(""));
  myCols[theColumn-1] = theTitle;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

std::string SALOMEDSImpl_AttributeTableOfInteger::GetColumnTitle(const int theColumn) const 
{
  if(myCols.empty()) return "";
  if(myCols.size() < theColumn) return "";
  return myCols[theColumn-1];
}

void SALOMEDSImpl_AttributeTableOfInteger::SetColumnTitles(const std::vector<std::string>& theTitles)
{
  if (theTitles.size() != myNbColumns) throw DFexception("Invalid number of columns");
  int aLength = theTitles.size(), i;
  for(i = 0; i < aLength; i++)  myCols[i] = theTitles[i];
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

std::vector<std::string> SALOMEDSImpl_AttributeTableOfInteger::GetColumnTitles()
{
  std::vector<std::string> aSeq;
  int aLength = myCols.size(), i;
  for(i=0; i<aLength; i++) aSeq.push_back(myCols[i]);
  return aSeq;
}

int SALOMEDSImpl_AttributeTableOfInteger::GetNbRows() const
{
  return myNbRows;
}

int SALOMEDSImpl_AttributeTableOfInteger::GetNbColumns() const
{
  return myNbColumns;
}

void SALOMEDSImpl_AttributeTableOfInteger::PutValue(const int theValue,
                                                    const int theRow,
                                                    const int theColumn) 
{
  CheckLocked();  
  //Backup();
  if(theColumn > myNbColumns) SetNbColumns(theColumn);

  int anIndex = (theRow-1)*myNbColumns + theColumn;
  myTable[anIndex] = theValue;

  if(theRow > myNbRows) {
    while (myRows.size() < theRow) { // append empty row titles
      myRows.push_back(std::string(""));
    }
    myNbRows = theRow;
  }
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

bool SALOMEDSImpl_AttributeTableOfInteger::HasValue(const int theRow,
                                                    const int theColumn) 
{
  if(theRow > myNbRows || theRow < 1) return false;
  if(theColumn > myNbColumns || theColumn < 1) return false;
  int anIndex = (theRow-1)*myNbColumns + theColumn;
  return (myTable.find(anIndex) != myTable.end()); 
}

int SALOMEDSImpl_AttributeTableOfInteger::GetValue(const int theRow,
                                                   const int theColumn) 
{
  if(theRow > myNbRows || theRow < 1) throw DFexception("Invalid cell index");
  if(theColumn > myNbColumns || theColumn < 1) throw DFexception("Invalid cell index");

  int anIndex = (theRow-1)*myNbColumns + theColumn;
  if(myTable.find(anIndex) != myTable.end()) return myTable[anIndex];
  
  throw DFexception("Invalid cell index");
  return 0;
}

void SALOMEDSImpl_AttributeTableOfInteger::RemoveValue(const int theRow, const int theColumn)
{
  CheckLocked();  
  if(theRow > myNbRows || theRow < 1) throw DFexception("Invalid cell index");
  if(theColumn > myNbColumns || theColumn < 1) throw DFexception("Invalid cell index");

  int anIndex = (theRow-1)*myNbColumns + theColumn;
  if (myTable.find(anIndex) != myTable.end()) {
    //Backup();
    myTable.erase(anIndex);
    SetModifyFlag(); // table is modified
  }
}

const std::string& SALOMEDSImpl_AttributeTableOfInteger::ID() const
{
  return GetID();
}

void SALOMEDSImpl_AttributeTableOfInteger::Restore(DF_Attribute* with) 
{
  int anIndex;
  SALOMEDSImpl_AttributeTableOfInteger* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(with);
  if(!aTable) throw DFexception("Can't Restore from a null attribute");    

  myTable.clear();
  myCols.clear();
  myRows.clear();

  myTable = aTable->myTable;
  myNbRows = aTable->myNbRows;
  myNbColumns = aTable->myNbColumns;
  myTitle = aTable->myTitle;
  
  for(anIndex = 1; anIndex <= aTable->GetNbRows();anIndex++)
    myRows.push_back(aTable->GetRowTitle(anIndex));

  for(anIndex = 1; anIndex <= aTable->GetNbColumns(); anIndex++) 
    myCols.push_back(aTable->GetColumnTitle(anIndex));
}

DF_Attribute* SALOMEDSImpl_AttributeTableOfInteger::NewEmpty() const
{
  return new SALOMEDSImpl_AttributeTableOfInteger();
}

void SALOMEDSImpl_AttributeTableOfInteger::Paste(DF_Attribute* into)
{
  int anIndex;
  SALOMEDSImpl_AttributeTableOfInteger* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfInteger*>(into);
  if(!aTable) throw DFexception("Can't Paste into a null attribute");    

  aTable->myTable.clear();
  aTable->myCols.clear();
  aTable->myRows.clear();

  aTable->myTable = myTable;
  aTable->myTitle = myTitle;
  aTable->myNbRows = myNbRows;
  aTable->myNbColumns = myNbColumns;

  for(anIndex = 1; anIndex <= GetNbRows();anIndex++)
    aTable->myRows.push_back(GetRowTitle(anIndex));
  for(anIndex = 1; anIndex <= GetNbColumns(); anIndex++) 
    aTable->myCols.push_back(GetColumnTitle(anIndex));
}

std::vector<int> SALOMEDSImpl_AttributeTableOfInteger::GetSetRowIndices(const int theRow)
{
  std::vector<int> aSeq;

  int i, aShift = myNbColumns*(theRow-1);
  for(i = 1; i <= myNbColumns; i++) {
    if(myTable.find(aShift + i) != myTable.end()) aSeq.push_back(i);
  }
  
  return aSeq;
}

std::vector<int> SALOMEDSImpl_AttributeTableOfInteger::GetSetColumnIndices(const int theColumn)
{
  std::vector<int> aSeq;

  int i, anIndex;
  for(i = 1; i <= myNbRows; i++) {
    anIndex = myNbColumns*(i-1)+theColumn;
    if(myTable.find(anIndex) != myTable.end()) aSeq.push_back(i);
  }
  
  return aSeq;
}

std::string SALOMEDSImpl_AttributeTableOfInteger::Save() 
{
  std::string aString;
  char* buffer = new char[1024];
  int i, j, l;

  //Title
  l = myTitle.size();
  sprintf(buffer, "%d\n", l);
  aString+=buffer;
  for(i=0; i<l; i++) {
    aString += myTitle[i];
    aString +='\n';
  }
  
  //Nb rows
  sprintf(buffer, "%d\n", myNbRows);
  aString+=buffer;

  //Row titles
  for(i=0; i<myNbRows; i++) {
    l = myRows[i].size();
    sprintf(buffer, "%d\n", l);
    aString+=buffer;
    for(j=0; j<l; j++) {
      aString += myRows[i][j];
      aString += '\n';
    }
  }  

  //Nb columns
  sprintf(buffer, "%d\n", myNbColumns);
  aString+=buffer;

  //Columns titles
  for(i=0; i<myNbColumns; i++) {
    l = myCols[i].size();
    sprintf(buffer, "%d\n", l);
    aString+=buffer;
    for(j=0; j<l; j++) {
      aString += myCols[i][j];
      aString += '\n';
    }
  }

  //Store the table values
  l = myTable.size();
  sprintf(buffer, "%d\n", l);
  aString+=buffer;
  for(MI p = myTable.begin(); p != myTable.end(); p++) {
    sprintf(buffer, "%d\n%d\n", p->first, p->second);
    aString += buffer;
  }

  delete []buffer;
  return aString;
}

void SALOMEDSImpl_AttributeTableOfInteger::Load(const std::string& value) 
{
  std::vector<std::string> v;
  int i,  j, l, pos, aSize = (int)value.size(); 
  for(i = 0, pos = 0; i<aSize; i++) {
    if(value[i] == '\n') {
       v.push_back(value.substr(pos, i-pos));
       pos = i+1;
    }
  }

  Backup();

  pos = 0;
  std::string aStr;

  //Title
  l = strtol(v[pos++].c_str(), NULL, 10);

  myTitle = std::string(l, 0);
  for(i=0; i<l; i++) {
    myTitle[i] = v[pos++][0];
  }

  //Nb rows
  myNbRows = strtol(v[pos++].c_str(), NULL, 10);

  //Rows titles
  myRows.clear();  
  for(i=1; i<=myNbRows; i++) { 
    l = strtol(v[pos++].c_str(), NULL, 10);
    aStr = std::string(l,0);
    for(j=0; j<l; j++) {
      aStr[j] = v[pos++][0];
    }
    myRows.push_back(aStr);
  }

  //Nb columns
  myNbColumns = strtol(v[pos++].c_str(), NULL, 10);

  //Columns titles
  myCols.clear();
  for(i=1; i<=myNbColumns; i++) {
    l = strtol(v[pos++].c_str(), NULL, 10);
    aStr = std::string(l,0);
    for(j=0; j<l; j++) {
      aStr[j] = v[pos++][0];
    }
    myCols.push_back(aStr);
  }

  //Restore the table values
  l = strtol(v[pos++].c_str(), NULL, 10);
  myTable.clear();
  for(i=1; i<=l; i++) {
    int aKey = strtol(v[pos++].c_str(), NULL, 10);
    int aValue = strtol(v[pos++].c_str(), NULL, 10);
    myTable[aKey] = aValue;
  }
}

std::vector<int> SALOMEDSImpl_AttributeTableOfInteger::SortRow(const int theRow, SortOrder sortOrder, SortPolicy sortPolicy )
{
  CheckLocked();
  std::vector<int> result;
  if ( theRow > 0 && theRow <= myNbRows ) {
    std::vector<int> indices( myNbColumns );
    int cnt = 0;
    for ( int i = 0; i < myNbColumns; i++ ) {
      if ( sortPolicy != EmptyIgnore || HasValue(theRow, i+1) ) {
	indices[cnt++] = i+1;
      }
    }
    indices.resize(cnt);
    
    TableSorter<SALOMEDSImpl_AttributeTableOfInteger> sorter( this, sortOrder, sortPolicy, theRow, true );
    std::stable_sort( indices.begin(), indices.end(), sorter );
    
    if ( sortPolicy == EmptyIgnore ) {
      std::vector<int> other( myNbColumns );
      cnt = 0;
      for( int i = 0; i < myNbColumns; i++ )
	other[i] = HasValue(theRow, i+1) ? indices[cnt++] : i+1;
      indices = other;
    }
    result = indices;

    for ( int col = 0; col < indices.size(); col++ ) {
      int idx = indices[col];
      if ( col+1 == idx ) continue;
      SwapCells(theRow, col+1, theRow, idx);
      int idx1 = 0;
      for ( int i = col+1; i < indices.size() && idx1 == 0; i++)
	if ( indices[i] == col+1 ) idx1 = i;
      indices[idx1] = idx;
    }
    // no need for SetModifyFlag(), since it is done by SwapCells()
  }
  return result;
}

std::vector<int> SALOMEDSImpl_AttributeTableOfInteger::SortColumn(const int theColumn, SortOrder sortOrder, SortPolicy sortPolicy )
{
  CheckLocked();  
  std::vector<int> result;
  if ( theColumn > 0 && theColumn <= myNbColumns ) {
    std::vector<int> indices( myNbRows );
    int cnt = 0;
    for ( int i = 0; i < myNbRows; i++ ) {
      if ( sortPolicy != EmptyIgnore || HasValue(i+1, theColumn) ) {
	indices[cnt++] = i+1;
      }
    }
    indices.resize(cnt);
    
    TableSorter<SALOMEDSImpl_AttributeTableOfInteger> sorter( this, sortOrder, sortPolicy, theColumn, false );
    std::stable_sort( indices.begin(), indices.end(), sorter );
    
    if ( sortPolicy == EmptyIgnore ) {
      std::vector<int> other( myNbRows );
      cnt = 0;
      for( int i = 0; i < myNbRows; i++ )
	other[i] = HasValue(i+1, theColumn) ? indices[cnt++] : i+1;
      indices = other;
    }
    result = indices;

    for ( int row = 0; row < indices.size(); row++ ) {
      int idx = indices[row];
      if ( row+1 == idx ) continue;
      SwapCells(row+1, theColumn, idx, theColumn);
      int idx1 = 0;
      for ( int i = row+1; i < indices.size() && idx1 == 0; i++)
	if ( indices[i] == row+1 ) idx1 = i;
      indices[idx1] = idx;
    }
    // no need for SetModifyFlag(), since it is done by SwapCells()
  }
  return result;
}

std::vector<int> SALOMEDSImpl_AttributeTableOfInteger::SortByRow(const int theRow, SortOrder sortOrder, SortPolicy sortPolicy )
{
  CheckLocked();  
  std::vector<int> result;
  if ( theRow > 0 && theRow <= myNbRows ) {
    std::vector<int> indices( myNbColumns );
    int cnt = 0;
    for ( int i = 0; i < myNbColumns; i++ ) {
      if ( sortPolicy != EmptyIgnore || HasValue(theRow, i+1) ) {
	indices[cnt++] = i+1;
      }
    }
    indices.resize(cnt);
    
    TableSorter<SALOMEDSImpl_AttributeTableOfInteger> sorter( this, sortOrder, sortPolicy, theRow, true );
    std::stable_sort( indices.begin(), indices.end(), sorter );

    if ( sortPolicy == EmptyIgnore ) {
      std::vector<int> other( myNbColumns );
      cnt = 0;
      for( int i = 0; i < myNbColumns; i++ )
	other[i] = HasValue(theRow, i+1) ? indices[cnt++] : i+1;
      indices = other;
    }
    result = indices;

    for ( int col = 0; col < indices.size(); col++ ) {
      int idx = indices[col];
      if ( col+1 == idx ) continue;
      SwapColumns(col+1, idx);
      int idx1 = 0;
      for ( int i = col+1; i < indices.size() && idx1 == 0; i++)
	if ( indices[i] == col+1 ) idx1 = i;
      indices[idx1] = idx;
    }
    // no need for SetModifyFlag(), since it is done by SwapColumns()
  }
  return result;
}

std::vector<int> SALOMEDSImpl_AttributeTableOfInteger::SortByColumn(const int theColumn, SortOrder sortOrder, SortPolicy sortPolicy )
{
  CheckLocked();  
  std::vector<int> result;
  if ( theColumn > 0 && theColumn <= myNbColumns ) {
    std::vector<int> indices( myNbRows );
    int cnt = 0;
    for ( int i = 0; i < myNbRows; i++ ) {
      if ( sortPolicy != EmptyIgnore || HasValue(i+1, theColumn) ) {
	indices[cnt++] = i+1;
      }
    }
    indices.resize(cnt);
    
    TableSorter<SALOMEDSImpl_AttributeTableOfInteger> sorter( this, sortOrder, sortPolicy, theColumn, false );
    std::stable_sort( indices.begin(), indices.end(), sorter );

    if ( sortPolicy == EmptyIgnore ) {
      std::vector<int> other( myNbRows );
      cnt = 0;
      for( int i = 0; i < myNbRows; i++ )
	other[i] = HasValue(i+1, theColumn) ? indices[cnt++] : i+1;
      indices = other;
    }
    result = indices;

    for ( int row = 0; row < indices.size(); row++ ) {
      int idx = indices[row];
      if ( row+1 == idx ) continue;
      SwapRows(row+1, idx);
      int idx1 = 0;
      for ( int i = row+1; i < indices.size() && idx1 == 0; i++)
	if ( indices[i] == row+1 ) idx1 = i;
      indices[idx1] = idx;
    }
    // no need for SetModifyFlag(), since it is done by SwapRows()
  }
  return result;
}

void SALOMEDSImpl_AttributeTableOfInteger::SwapCells(const int theRow1, const int theColumn1, 
						     const int theRow2, const int theColumn2)
{
  CheckLocked();  
  if (theRow1    > myNbRows    || theRow1 < 1)    throw DFexception("Invalid cell index");
  if (theRow2    > myNbRows    || theRow2 < 1)    throw DFexception("Invalid cell index");
  if (theColumn1 > myNbColumns || theColumn1 < 1) throw DFexception("Invalid cell index");
  if (theColumn2 > myNbColumns || theColumn2 < 1) throw DFexception("Invalid cell index");

  int anIndex1 = (theRow1-1)*myNbColumns + theColumn1;
  int anIndex2 = (theRow2-1)*myNbColumns + theColumn2;

  bool hasValue1 = myTable.find(anIndex1) != myTable.end();
  bool hasValue2 = myTable.find(anIndex2) != myTable.end();

  if (!hasValue1 && !hasValue2) return;                   // nothing changed

  int  value1    = hasValue1 ? myTable[anIndex1] : 0;
  int  value2    = hasValue2 ? myTable[anIndex2] : 0;

  if (hasValue1 && hasValue2 && value1 == value2) return; // nothing changed

  if (hasValue1) myTable[anIndex2] = value1;
  else           myTable.erase(anIndex2);
  if (hasValue2) myTable[anIndex1] = value2;
  else           myTable.erase(anIndex1);

  SetModifyFlag(); // table is modified
}

void SALOMEDSImpl_AttributeTableOfInteger::SwapRows(const int theRow1, const int theRow2)
{
  CheckLocked();  
  for (int i = 1; i <= myNbColumns; i++)
    SwapCells(theRow1, i, theRow2, i);
  // swap row titles
  std::string tmp = myRows[theRow1-1];
  myRows[theRow1-1] = myRows[theRow2-1];
  myRows[theRow2-1] = tmp;
  // no need for SetModifyFlag(), since it is done by SwapCells()
}

void SALOMEDSImpl_AttributeTableOfInteger::SwapColumns(const int theColumn1, const int theColumn2)
{
  CheckLocked();  
  for (int i = 1; i <= myNbRows; i++)
    SwapCells(i, theColumn1, i, theColumn2);
  // swap column titles
  std::string tmp = myCols[theColumn1-1];
  myCols[theColumn1-1] = myCols[theColumn2-1];
  myCols[theColumn2-1] = tmp;
  // no need for SetModifyFlag(), since it is done by SwapCells()
}

