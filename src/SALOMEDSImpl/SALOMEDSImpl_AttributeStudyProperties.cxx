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

//  File   : SALOMEDSImpl_AttributeStudyProperties.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_AttributeStudyProperties.hxx"
#include <string.h>
#include <stdlib.h>

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
  myModified = false;
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
  myComponentVersions.clear();
}

void SALOMEDSImpl_AttributeStudyProperties::SetModification(const std::string& theUserName,
                                                            const int          theMinute,
                                                            const int          theHour,
                                                            const int          theDay,
                                                            const int          theMonth,
                                                            const int          theYear)
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
                  (std::vector<std::string>& theUserNames,
                   std::vector<int>&    theMinutes,
                   std::vector<int>&    theHours,
                   std::vector<int>&    theDays,
                   std::vector<int>&    theMonths,
                   std::vector<int>&    theYears) const
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
  return myUserName.empty() ? std::string("") : myUserName[0];
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
  if (theMode == myMode) return;
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

bool SALOMEDSImpl_AttributeStudyProperties::IsLockChanged(const bool theErase)
{
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
  std::vector<std::string> aNames;
  std::vector<int> aMinutes, aHours, aDays, aMonths, aYears;
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
  myComponentVersions = aProp->GetComponentsVersions();
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
  aProp->SetComponentsVersions( myComponentVersions );
//  aProp->SetModified(myModified);
//  aProp->SetLocked(myLocked);
}


std::string SALOMEDSImpl_AttributeStudyProperties::Save()
{
  std::vector<std::string> aNames;
  std::vector<int> aMinutes, aHours, aDays, aMonths, aYears;
  GetModifications(aNames, aMinutes, aHours, aDays, aMonths, aYears);

  int aLength, anIndex, unitsSize = 0, commentSize = 0;;
  for (aLength = 0, anIndex = aNames.size()-1; anIndex >= 0; anIndex--)
    aLength += aNames[anIndex].size() + 1;

  std::string units = GetUnits();
  std::string comment = GetComment();

  int aLength1 = 0;
  std::map<std::string, std::string> versions;
  versionMap::const_iterator it;
  for (aLength1 = 0, it = myComponentVersions.begin(); it != myComponentVersions.end(); ++it ) {
    std::string vlist = "";
    versionList vl = it->second;
    versionList::const_iterator vlit;
    for ( vlit = vl.begin(); vlit != vl.end(); ++vlit ) {
      if ( vlist != "" ) vlist += ";";
      vlist += *vlit;
    }
    versions[ it->first ] = vlist;
    aLength1 += it->first.size() + vlist.size() + 2;
  }
  
  unitsSize = units.size();
  commentSize = comment.size();

  char* aProperty = new char[3 + aLength + 12 * aNames.size() + 1 + unitsSize + 1 + commentSize + 1 + aLength1 ];

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

  //Write delimeter of the section to define end of the modifications section
  aProperty[a++] = 30;

  //Write units if need
  if(units.size() > 0) {
    sprintf(&(aProperty[a]),"%s",units.c_str());
    a = strlen(aProperty);
  }

  aProperty[a++] = 1; //delimeter of the units and comments

  //Write comments if need
  if(comment.size() > 0) {
    sprintf(&(aProperty[a]),"%s",comment.c_str());
    a = strlen(aProperty);
  }
  
  aProperty[a++] = 30; //delimeter of the component versions

  std::map<std::string, std::string>::const_iterator versionsIt;
  for ( versionsIt = versions.begin(); versionsIt != versions.end(); ++versionsIt ) {
    sprintf(&(aProperty[a]),"%s=%s",
            (char*)(versionsIt->first.c_str()),
            (char*)(versionsIt->second.c_str()));
    a = strlen(aProperty);
    aProperty[a++] = 1;
  }

  aProperty[a] = 0;
  std::string prop(aProperty);
  delete aProperty;

  return prop;
}

void SALOMEDSImpl_AttributeStudyProperties::SetUnits(const std::string& theUnits)
{
  if(myUnits == theUnits)
    return;

  CheckLocked();
  Backup();

  myUnits = theUnits;
}

std::string SALOMEDSImpl_AttributeStudyProperties::GetUnits() const
{
  return myUnits;
}

void SALOMEDSImpl_AttributeStudyProperties::SetComment(const std::string& theComment)
{
  if(myComment == theComment)
    return;

  CheckLocked();
  Backup();

  myComment = theComment;
}

std::string SALOMEDSImpl_AttributeStudyProperties::GetComment() const 
{
  return myComment;
}

void SALOMEDSImpl_AttributeStudyProperties::SetComponentVersion(const std::string& theComponent, const std::string& theVersion)
{
  if (!theComponent.empty()) {
    CheckLocked();
    Backup();
    if (myComponentVersions.find(theComponent) == myComponentVersions.end()) myComponentVersions[theComponent] = versionList();
    if (myComponentVersions[theComponent].empty() || myComponentVersions[theComponent].back() != theVersion)
      myComponentVersions[theComponent].push_back(theVersion);
  }
}

std::vector<std::string> SALOMEDSImpl_AttributeStudyProperties::GetStoredComponents() const
{
  std::vector<std::string> components;
  versionMap::const_iterator it;  
  for (it = myComponentVersions.begin(); it != myComponentVersions.end(); ++it)
    components.push_back(it->first);
  return components;
}

std::string SALOMEDSImpl_AttributeStudyProperties::GetComponentVersion(const std::string& theComponent) const
{
  versionList versions = GetComponentVersions(theComponent);
  return versions.size() > 0 ? versions[0] : std::string("");
}

std::vector<std::string> SALOMEDSImpl_AttributeStudyProperties::GetComponentVersions(const std::string& theComponent) const
{
  versionList versions;
  versionMap::const_iterator it = myComponentVersions.find(theComponent);
  if ( it != myComponentVersions.end() ) versions = it->second;
  return versions;
}

std::map< std::string, std::vector<std::string> > SALOMEDSImpl_AttributeStudyProperties::GetComponentsVersions() const
{
  return myComponentVersions;
}

void SALOMEDSImpl_AttributeStudyProperties::SetComponentsVersions( const std::map< std::string, std::vector<std::string> >& theVersions )
{
  myComponentVersions = theVersions;
}

void SALOMEDSImpl_AttributeStudyProperties::Load(const std::string& value)
{
  char* aCopy = (char*)value.c_str();

  int crMode = (int)aCopy[0];
  SetCreationMode(crMode);

  int anIndex;
  // number 13 below is minimal size of modification data record, which has form:
  // mmhhddMMyyyyname1, where
  // - mm:   minute = 2 bytes
  // - hh:   hour   = 2 bytes
  // - dd:   day    = 2 bytes
  // - MM:   month  = 2 bytes
  // - yyyy: year   = 4 bytes
  // - name: user's name = arbitrary value, minimal length is 0 bytes
  // - 1   : records delimiter = 1 byte  
  for (anIndex = 2; anIndex + 13 < value.size() ;) {
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
    delete [] (aName);
    anIndex += aNameSize + 1;
    
    //Check end of the modifications section
    if(anIndex < value.size() && aCopy[anIndex] == 30)
      break;
  }
  
  //Case when study contains units and comment properties
  if( anIndex < value.size() ) {
    anIndex++; //skip the delimeter of the sections: char(30)
    int unitsSize;
    for(unitsSize = 0; aCopy[anIndex+unitsSize] != 1; unitsSize++);

    if(unitsSize > 0) {
      char *anUnits = new char[unitsSize+1];
      strncpy(anUnits, &(aCopy[anIndex]), unitsSize);
      anUnits[unitsSize] = 0;
      SetUnits(anUnits);
      delete [] (anUnits);
    }
    anIndex += unitsSize + 1;

    int commentSize;
    for(commentSize = 0; aCopy[anIndex+commentSize] != 0 && aCopy[anIndex+commentSize] != 30; commentSize++);

    if(commentSize > 0) {
      char *aComment = new char[commentSize+1];
      strncpy(aComment, &(aCopy[anIndex]), commentSize);
      aComment[commentSize] = 0;
      SetComment(aComment);
      delete [] (aComment);
    }
    anIndex += commentSize + 1;
  }

  //Case when study contains components versions
  if( anIndex < value.size() ) {
    while ( anIndex < value.size() && aCopy[anIndex] != 0 ) {
      int modSize;
      for(modSize = 0; aCopy[anIndex+modSize] != '='; modSize++);
      int verSize;
      for(verSize = 0; aCopy[anIndex+modSize+1+verSize] != 1; verSize++);

      if(modSize > 0) {
        char *aModule = new char[modSize+1];
        strncpy(aModule, &(aCopy[anIndex]), modSize);
        aModule[modSize] = 0;
        char *aVersions = new char[verSize+1];
        if ( verSize > 0 )
          strncpy(aVersions, &(aCopy[anIndex+modSize+1]), verSize);
        aVersions[verSize] = 0;
        
        std::string mVersions = aVersions;
        int start = 0, idx = mVersions.find( ';', start );
        while ( idx != std::string::npos ) {
          SetComponentVersion( aModule, mVersions.substr( start, idx-start ) );
          start = idx + 1;
          idx = mVersions.find( ';', start );
        }
        SetComponentVersion( aModule, mVersions.substr( start ) );

        delete [] (aModule);
        delete [] (aVersions);
        anIndex += modSize + 1 + verSize + 1;
      }
    }
  }
  
  if (aCopy[1] == 'l') {
    SetLocked(true);
  }
  SetModified(0);  
}
