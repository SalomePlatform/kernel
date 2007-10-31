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
//  File   : SALOMEDSImpl_AttributeStudyProperties.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include <SALOMEDSImpl_AttributeStudyProperties.hxx>

using namespace std;


const std::string& SALOMEDSImpl_AttributeStudyProperties::GetID()
{
  static std::string SALOMEDSImpl_AttributeStudyPropertiesID ("128371A2-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeStudyPropertiesID;
}

SALOMEDSImpl_AttributeStudyProperties* SALOMEDSImpl_AttributeStudyProperties::Set(const DF_Label& label)
{
  SALOMEDSImpl_AttributeStudyProperties* A = NULL;
  if (!(A=(SALOMEDSImpl_AttributeStudyProperties*)label.FindAttribute(SALOMEDSImpl_AttributeStudyProperties::GetID()))) {
    A = new SALOMEDSImpl_AttributeStudyProperties();
    label.AddAttribute(A);
  }
  return A;
}

SALOMEDSImpl_AttributeStudyProperties::SALOMEDSImpl_AttributeStudyProperties()
:SALOMEDSImpl_GenericAttribute("AttributeStudyProperties")
{
  myLocked = false;
  myLockChanged = false;
  Init();
}

void SALOMEDSImpl_AttributeStudyProperties::Init()
{
  myUserName.clear();
  myMinute.clear();
  myHour.clear();
  myDay.clear();
  myMonth.clear();
  myYear.clear();
  myMode = 0; // none
}

void SALOMEDSImpl_AttributeStudyProperties::SetModification(const std::string& theUserName,
                                                            const int            theMinute,
                                                            const int            theHour,
                                                            const int            theDay,
                                                            const int            theMonth,
                                                            const int            theYear)
{
  if (theMinute<0 || theMinute>60 || theHour<0 || theHour>24 ||
      theDay<0 || theDay>31 || theMonth<0 || theMonth>12)
    return;

  CheckLocked();
  Backup();

  myUserName.push_back(theUserName);
  myMinute.push_back(theMinute);
  myHour.push_back(theHour);
  myDay.push_back(theDay);
  myMonth.push_back(theMonth);
  myYear.push_back(theYear);
}

void SALOMEDSImpl_AttributeStudyProperties::GetModifications
                  (vector<string>& theUserNames,
                   vector<int>&    theMinutes,
                   vector<int>&    theHours,
                   vector<int>&    theDays,
                   vector<int>&    theMonths,
                   vector<int>&    theYears) const
{
  theUserNames = myUserName;
  theMinutes = myMinute;
  theHours = myHour;
  theDays = myDay;
  theMonths = myMonth;
  theYears = myYear;
}

std::string SALOMEDSImpl_AttributeStudyProperties::GetCreatorName() const
{
  if (myUserName.size() == 0)
    return std::string("");
  return myUserName[0];
}

bool SALOMEDSImpl_AttributeStudyProperties::GetCreationDate
                              (int&           theMinute,
                               int&           theHour,
                               int&           theDay,
                               int&           theMonth,
                               int&           theYear) const
{
  if (myMinute.size() != 0) {
    theMinute = myMinute[0];
    theHour = myHour[0];
    theDay = myDay[0];
    theMonth = myMonth[0];
    theYear = myYear[0];
    return true;
  }
  return false;
}

void SALOMEDSImpl_AttributeStudyProperties::ChangeCreatorName(const std::string& theName)
{
  if (myUserName.size() > 0) {
    CheckLocked();
    Backup();
    myUserName[0] = theName;
  }
}

void SALOMEDSImpl_AttributeStudyProperties::SetCreationMode(const int theMode)
{
  CheckLocked();
  Backup();
  myMode = theMode;
}

int SALOMEDSImpl_AttributeStudyProperties::GetCreationMode() const
{
  return myMode;
}

void SALOMEDSImpl_AttributeStudyProperties::SetModified(const int theModified)
{
  myModified = theModified;
}

bool SALOMEDSImpl_AttributeStudyProperties::IsModified() const
{
  return (myModified != 0);
}

int SALOMEDSImpl_AttributeStudyProperties::GetModified() const
{
  return myModified;
}

void SALOMEDSImpl_AttributeStudyProperties::SetLocked(const bool theLocked)
{
//  Backup();
  if (myLocked != theLocked) {
    myLockChanged = true;
    myLocked = theLocked;
  }
}

bool SALOMEDSImpl_AttributeStudyProperties::IsLocked() const
{
  return myLocked;
}

bool SALOMEDSImpl_AttributeStudyProperties::IsLockChanged(const bool theErase) {
  if (!myLockChanged) return false;
  if (theErase) myLockChanged = false;
  return true;
}

const std::string& SALOMEDSImpl_AttributeStudyProperties::ID() const
{
  return GetID();
}

void SALOMEDSImpl_AttributeStudyProperties::Restore(DF_Attribute* with)
{
  SALOMEDSImpl_AttributeStudyProperties* aProp =
    dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(with);

  Init();
  vector<string> aNames;
  vector<int> aMinutes, aHours, aDays, aMonths, aYears;
  aProp->GetModifications(aNames, aMinutes, aHours, aDays, aMonths, aYears);
  for (int i = 0, len = aNames.size(); i < len; i++) {
    myUserName.push_back(aNames[i]);
    myMinute.push_back(aMinutes[i]);
    myHour.push_back(aHours[i]);
    myDay.push_back(aDays[i]);
    myMonth.push_back(aMonths[i]);
    myYear.push_back(aYears[i]);
  }
  myMode = aProp->GetCreationMode();
//  myModified = aProp->GetModified();
//  myLocked = aProp->IsLocked();
}

DF_Attribute* SALOMEDSImpl_AttributeStudyProperties::NewEmpty() const
{
  return new SALOMEDSImpl_AttributeStudyProperties();
}

void SALOMEDSImpl_AttributeStudyProperties::Paste(DF_Attribute* into)
{
  SALOMEDSImpl_AttributeStudyProperties* aProp =
    dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(into);
  aProp->Init();

  int i;
  for(i = 0; i < myUserName.size(); i++) {
    aProp->SetModification(myUserName[i],
                           myMinute[i], myHour[i],
                           myDay[i], myMonth[i], myYear[i]);
  }

  aProp->SetCreationMode(myMode);
//  aProp->SetModified(myModified);
//  aProp->SetLocked(myLocked);
}


string SALOMEDSImpl_AttributeStudyProperties::Save()
{
  vector<string> aNames;
  vector<int> aMinutes, aHours, aDays, aMonths, aYears;
  GetModifications(aNames, aMinutes, aHours, aDays, aMonths, aYears);

  int aLength, anIndex;
  for (aLength = 0, anIndex = aNames.size()-1; anIndex >= 0; anIndex--)
    aLength += aNames[anIndex].size() + 1;

  char* aProperty = new char[3 + aLength + 12 * aNames.size()];

  char crMode = (char)GetCreationMode();

  sprintf(aProperty,"%c%c", crMode, IsLocked()?'l':'u');

  aLength = aNames.size();
  int a = 2;
  for (anIndex = 0; anIndex  < aLength; anIndex++) {
    sprintf(&(aProperty[a]),"%2d%2d%2d%2d%4d%s",
	    (int)(aMinutes[anIndex]),
	    (int)(aHours[anIndex]),
	    (int)(aDays[anIndex]),
	    (int)(aMonths[anIndex]),
	    (int)(aYears[anIndex]),
	    (char*)(aNames[anIndex].c_str()));
    a = strlen(aProperty);
    aProperty[a++] = 1;
  }
  aProperty[a] = 0;
  string prop(aProperty);
  delete aProperty;

  return prop;
}

void SALOMEDSImpl_AttributeStudyProperties::Load(const string& value)
{
  char* aCopy = (char*)value.c_str();

  int crMode = (int)aCopy[0];
  SetCreationMode(crMode);

  int anIndex;
  for (anIndex = 2; anIndex + 2 < value.size() ;) {
    char str[10];
    int aMinute, aHour, aDay, aMonth, aYear;
    str[0] = aCopy[anIndex++];
    str[1] = aCopy[anIndex++];
    str[2] = 0;
    aMinute = atoi(str);
    str[0] = aCopy[anIndex++];
    str[1] = aCopy[anIndex++];
    aHour =  atoi(str);
    str[0] = aCopy[anIndex++];
    str[1] = aCopy[anIndex++];
    aDay =  atoi(str);
    str[0] = aCopy[anIndex++];
    str[1] = aCopy[anIndex++];
    aMonth =  atoi(str);
    str[0] = aCopy[anIndex++];
    str[1] = aCopy[anIndex++];
    str[2] = aCopy[anIndex++];
    str[3] = aCopy[anIndex++];
    str[4] = 0;
    aYear = atoi(str);

    int aNameSize;
    for(aNameSize = 0; aCopy[anIndex+aNameSize]!=1; aNameSize++);
    char *aName = new char[aNameSize+1];
    strncpy(aName, &(aCopy[anIndex]), aNameSize);
    aName[aNameSize] = 0;
    SetModification(aName,aMinute,aHour,aDay,aMonth,aYear);
    delete(aName);
    anIndex += aNameSize + 1;
  }
  if (aCopy[1] == 'l') {
    SetLocked(true);
  }
  SetModified(0);
}
