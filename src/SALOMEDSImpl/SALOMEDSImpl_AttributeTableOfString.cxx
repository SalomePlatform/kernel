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
//  File   : SALOMEDSImpl_AttributeTableOfString.cxx
//  Author : Sergey Ruin
//  Module : SALOME

#include <SALOMEDSImpl_AttributeTableOfString.hxx>
#include <stdio.h>
#include <stdlib.h>
#include <strstream>

using namespace std;

#define SEPARATOR '\1'

typedef map<int, string>::const_iterator MI;

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

const std::string& SALOMEDSImpl_AttributeTableOfString::GetID() 
{
  static std::string SALOMEDSImpl_AttributeTableOfStringID ("128371A4-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeTableOfStringID;
}

SALOMEDSImpl_AttributeTableOfString* SALOMEDSImpl_AttributeTableOfString::Set(const DF_Label& label) 
{
  SALOMEDSImpl_AttributeTableOfString* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeTableOfString*)label.FindAttribute(SALOMEDSImpl_AttributeTableOfString::GetID()))) {
    A = new SALOMEDSImpl_AttributeTableOfString();
    label.AddAttribute(A);
  }
  return A;
}

SALOMEDSImpl_AttributeTableOfString::SALOMEDSImpl_AttributeTableOfString() 
:SALOMEDSImpl_GenericAttribute("AttributeTableOfString")
{
  myNbRows = 0;
  myNbColumns = 0;
}

void SALOMEDSImpl_AttributeTableOfString::SetNbColumns(const int theNbColumns)
{
  CheckLocked();  
  Backup();
  
  map<int, string> aMap;
  aMap = myTable;
  myTable.clear();

  for(MI p = aMap.begin(); p!=aMap.end(); p++) {
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

void SALOMEDSImpl_AttributeTableOfString::SetRowTitle(const int theRow,
						      const std::string& theTitle) 
{
  CheckLocked();  
  Backup();
  std::string aTitle(theTitle), aUnit = GetRowUnit(theRow);
  if(aUnit.size()>0) {
    aTitle += SEPARATOR;
    aTitle += aUnit;
  }
  myRows[theRow-1] = aTitle;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

void SALOMEDSImpl_AttributeTableOfString::SetRowUnit(const int theRow,
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

void SALOMEDSImpl_AttributeTableOfString::SetRowUnits(const vector<string>& theUnits)
{
  if (theUnits.size() != GetNbRows()) throw DFexception("Invalid number of rows");
  int aLength = theUnits.size(), i;
  for(i = 1; i <= aLength; i++) SetRowUnit(i, theUnits[i-1]);
}

vector<string> SALOMEDSImpl_AttributeTableOfString::GetRowUnits()
{
  vector<string> aSeq;
  int aLength = myRows.size(), i;
  for(i=0; i<aLength; i++) aSeq.push_back(getUnit(myRows[i]));
  return aSeq;
}

void SALOMEDSImpl_AttributeTableOfString::SetRowTitles(const vector<string>& theTitles)
{
  if (theTitles.size() != GetNbRows()) throw DFexception("Invalid number of rows");
  int aLength = theTitles.size(), i;
  for(i = 1; i <= aLength; i++) SetRowTitle(i, theTitles[i-1]);
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

vector<string> SALOMEDSImpl_AttributeTableOfString::GetRowTitles()
{
  vector<string> aSeq;
  int aLength = myRows.size(), i;
  for(i=0; i<aLength; i++) aSeq.push_back(getTitle(myRows[i]));
  return aSeq;
}


std::string SALOMEDSImpl_AttributeTableOfString::GetRowTitle(const int theRow) const 
{
  return getTitle(myRows[theRow-1]);
}


std::string SALOMEDSImpl_AttributeTableOfString::GetRowUnit(const int theRow) const 
{
  return getUnit(myRows[theRow-1]);
}

void SALOMEDSImpl_AttributeTableOfString::SetRowData(const int theRow,
						     const vector<string>& theData) 
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

void SALOMEDSImpl_AttributeTableOfString::SetTitle(const std::string& theTitle) 
{
  CheckLocked();  
  Backup();
  myTitle = theTitle;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

std::string SALOMEDSImpl_AttributeTableOfString::GetTitle() const 
{
  return myTitle;
}

vector<string> SALOMEDSImpl_AttributeTableOfString::GetRowData(const int theRow)
{
  vector<string> aSeq;
  int i, aShift = (theRow-1)*myNbColumns;
  for(i = 1; i <= myNbColumns; i++) {
     if(myTable.find(aShift+i) != myTable.end()) 
       aSeq.push_back(myTable[aShift+i]);
     else
       aSeq.push_back("");
  }
  
  return aSeq;
}

void SALOMEDSImpl_AttributeTableOfString::SetColumnData(const int theColumn,
						        const vector<string>& theData) 
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


vector<string> SALOMEDSImpl_AttributeTableOfString::GetColumnData(const int theColumn)
{
  vector<string> aSeq;
  
  int i, anIndex;
  for(i = 1; i <= myNbRows; i++) {
    anIndex = myNbColumns*(i-1) + theColumn;
    if(myTable.find(anIndex) != myTable.end()) 
      aSeq.push_back(myTable[anIndex]);
    else
      aSeq.push_back("");
  }
  
  return aSeq;
}

void SALOMEDSImpl_AttributeTableOfString::SetColumnTitle(const int theColumn,
						         const std::string& theTitle) 
{
  CheckLocked();  
  Backup();
  while(myCols.size() < theColumn) myCols.push_back(std::string(""));
  myCols[theColumn-1] = theTitle;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

std::string SALOMEDSImpl_AttributeTableOfString::GetColumnTitle(const int theColumn) const 
{
  if(myCols.empty()) return "";
  if(myCols.size() < theColumn) return "";
  return myCols[theColumn-1];
}


void SALOMEDSImpl_AttributeTableOfString::SetColumnTitles(const vector<string>& theTitles)
{
  if (theTitles.size() != myNbColumns) throw DFexception("Invalid number of columns");
  int aLength = theTitles.size(), i;
  for(i = 0; i < aLength; i++)  myCols[i] =  theTitles[i];
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

vector<string> SALOMEDSImpl_AttributeTableOfString::GetColumnTitles()
{
  vector<string> aSeq;
  int aLength = myCols.size(), i;
  for(i=0; i<aLength; i++) aSeq.push_back(myCols[i]);
  return aSeq;
}


int SALOMEDSImpl_AttributeTableOfString::GetNbRows() const
{
  return myNbRows;
}

int SALOMEDSImpl_AttributeTableOfString::GetNbColumns() const
{
  return myNbColumns;
}

void SALOMEDSImpl_AttributeTableOfString::PutValue(const std::string& theValue,
					           const int theRow,
					           const int theColumn) 
{
  CheckLocked();  
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

bool SALOMEDSImpl_AttributeTableOfString::HasValue(const int theRow,
						   const int theColumn) 
{
  if(theRow > myNbRows || theRow < 1) return false;
  if(theColumn > myNbColumns || theColumn < 1) return false;

  int anIndex = (theRow-1)*myNbColumns + theColumn;
  return (myTable.find(anIndex) !=  myTable.end()); 
}

std::string SALOMEDSImpl_AttributeTableOfString::GetValue(const int theRow,
							  const int theColumn) 
{
  if(theRow > myNbRows || theRow < 1) throw DFexception("Invalid cell index");
  if(theColumn > myNbColumns || theColumn < 1) throw DFexception("Invalid cell index");

  int anIndex = (theRow-1)*myNbColumns + theColumn;
  if(myTable.find(anIndex) != myTable.end()) return myTable[anIndex];
  
  throw DFexception("Invalid cell index");
  return "";
}

const std::string& SALOMEDSImpl_AttributeTableOfString::ID() const
{
  return GetID();
}

void SALOMEDSImpl_AttributeTableOfString::Restore(DF_Attribute* with) 
{
  int anIndex;
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(with);

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

DF_Attribute* SALOMEDSImpl_AttributeTableOfString::NewEmpty() const
{
  return new SALOMEDSImpl_AttributeTableOfString();
}

void SALOMEDSImpl_AttributeTableOfString::Paste(DF_Attribute* into)
{
  int anIndex;
  SALOMEDSImpl_AttributeTableOfString* aTable = dynamic_cast<SALOMEDSImpl_AttributeTableOfString*>(into);

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


vector<int> SALOMEDSImpl_AttributeTableOfString::GetSetRowIndices(const int theRow)
{
  vector<int> aSeq;

  int i, aShift = myNbColumns*(theRow-1);
  for(i = 1; i <= myNbColumns; i++) {
    if(myTable.find(aShift + i) != myTable.end()) aSeq.push_back(i);
  }
  
  return aSeq;
}

vector<int> SALOMEDSImpl_AttributeTableOfString::GetSetColumnIndices(const int theColumn)
{
  vector<int> aSeq;

  int i, anIndex;
  for(i = 1; i <= myNbRows; i++) {
    anIndex = myNbColumns*(i-1)+theColumn;
    if(myTable.find(anIndex) != myTable.end()) aSeq.push_back(i);
  }
  
  return aSeq;
}



string SALOMEDSImpl_AttributeTableOfString::Save() 
{
  string aString;
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
  for(MI p = myTable.begin(); p!=myTable.end(); p++) {
    if (p->second.size()) { // check empty string in the value table
      sprintf(buffer, "%d\n", p->first);
      aString += buffer;
      unsigned long aValueSize = p->second.size();
      sprintf(buffer, "%ld\n", aValueSize);
      aString +=buffer;
      aString += p->second;
      aString += '\n';
    } else { // write index only of kind: "0key"; "05", for an example
      sprintf(buffer, "0%d\n", p->first);
      aString+=buffer;
    }
  }

  delete []buffer;
  return aString;
}

void SALOMEDSImpl_AttributeTableOfString::Load(const string& value) 
{
  vector<string> v;
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
    aStr = v[pos++]; //Ket as a string 
    int aKey = strtol(aStr.c_str(), NULL, 10);
    string aValue;
    if(aStr[0] == '0') //If the first character of the key is 0, then empty value
      aValue = "";
    else {
      strtol(v[pos++].c_str(), NULL, 10);
      aValue = v[pos++];
    }
    myTable[aKey] = aValue;
  }
}
