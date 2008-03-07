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
//  File   : SALOMEDS_AttributeStudyProperties.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeStudyProperties.hxx"
#include "SALOMEDS.hxx"

using namespace std;

SALOMEDS_AttributeStudyProperties::SALOMEDS_AttributeStudyProperties
                                   (SALOMEDSImpl_AttributeStudyProperties* theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeStudyProperties::SALOMEDS_AttributeStudyProperties
                                   (SALOMEDS::AttributeStudyProperties_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeStudyProperties::~SALOMEDS_AttributeStudyProperties()
{
}

void SALOMEDS_AttributeStudyProperties::SetUserName(const std::string& theName)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeStudyProperties* anImpl =
      dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_local_impl);
    anImpl->ChangeCreatorName(theName);
  } else
    SALOMEDS::AttributeStudyProperties::_narrow(_corba_impl)->SetUserName(theName.c_str());
}

std::string SALOMEDS_AttributeStudyProperties::GetUserName()
{
  std::string aName;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    aName = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_local_impl)->GetCreatorName();
  }
#ifndef WNT
  else aName = SALOMEDS::AttributeStudyProperties::_narrow(_corba_impl)->GetUserName();
#else
  else aName = SALOMEDS::AttributeStudyProperties::_narrow(_corba_impl)->GetUserNameA();
#endif
  return aName;
}

void SALOMEDS_AttributeStudyProperties::SetCreationDate
              (int theMinute, int theHour, int theDay, int theMonth, int theYear)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeStudyProperties* anImpl =
      dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_local_impl);
    int aTmp;
    if (anImpl->GetCreationDate(aTmp, aTmp, aTmp, aTmp, aTmp)) return;
    string S;
    anImpl->SetModification(S, theMinute, theHour, theDay, theMonth, theYear);
  } else {
    SALOMEDS::AttributeStudyProperties::_narrow(_corba_impl)->SetCreationDate(theMinute,
									      theHour,
									      theDay,
									      theMonth,
									      theYear);
  }
}

bool SALOMEDS_AttributeStudyProperties::GetCreationDate(int& theMinute,
							int& theHour,
							int& theDay,
							int& theMonth,
							int& theYear)
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>
      (_local_impl)->GetCreationDate(theMinute, theHour, theDay, theMonth, theYear);
  } else {
    CORBA::Long aMinute, anHour, aDay, aMonth, anYear;
    ret = SALOMEDS::AttributeStudyProperties::_narrow(_corba_impl)->GetCreationDate(aMinute,
										    anHour,
										    aDay,
										    aMonth,
										    anYear);
    theMinute = (int)aMinute;
    theHour = (int)anHour;
    theDay = (int)aDay;
    theMonth = (int)aMonth;
    theYear = (int)anYear;
  }
  return ret;
}

void SALOMEDS_AttributeStudyProperties::SetCreationMode(const std::string& theMode)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    if (theMode == "from scratch")
      dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_local_impl)->SetCreationMode(1);
    else if (theMode == "copy from")
      dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_local_impl)->SetCreationMode(2);
    else //Not defined
      dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_local_impl)->SetCreationMode(0);
  }
  else SALOMEDS::AttributeStudyProperties::_narrow(_corba_impl)->SetCreationMode(theMode.c_str());
}

std::string SALOMEDS_AttributeStudyProperties::GetCreationMode()
{
  std::string aMode;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    int mode  = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_local_impl)->GetCreationMode();
    if (mode == 1) aMode = "from scratch";
    if (mode == 2) aMode = "copy from";
  }
  else
    aMode = SALOMEDS::AttributeStudyProperties::_narrow(_corba_impl)->GetCreationMode();
  return aMode;
}

void SALOMEDS_AttributeStudyProperties::SetModified(int theModified)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_local_impl)->SetModified(theModified);
  }
  else
    SALOMEDS::AttributeStudyProperties::_narrow(_corba_impl)->SetModified(theModified);
}

bool SALOMEDS_AttributeStudyProperties::IsModified()
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_local_impl)->IsModified();
  }
  else
    ret = SALOMEDS::AttributeStudyProperties::_narrow(_corba_impl)->IsModified();
  return ret;
}

int SALOMEDS_AttributeStudyProperties::GetModified()
{
  int isModified;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    isModified = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_local_impl)->GetModified();
  }
  else
    isModified = SALOMEDS::AttributeStudyProperties::_narrow(_corba_impl)->GetModified();
  return isModified;
}

void SALOMEDS_AttributeStudyProperties::SetLocked(bool theLocked)
{
  if (_isLocal) {
    SALOMEDS::Locker lock;
    dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_local_impl)->SetLocked(theLocked);
  }
  else
    SALOMEDS::AttributeStudyProperties::_narrow(_corba_impl)->SetLocked(theLocked);
}

bool SALOMEDS_AttributeStudyProperties::IsLocked()
{
  bool ret;
  if (_isLocal) {
    SALOMEDS::Locker lock;
    ret = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_local_impl)->IsLocked();
  }
  else
    ret = SALOMEDS::AttributeStudyProperties::_narrow(_corba_impl)->IsLocked();
  return ret;
}

void SALOMEDS_AttributeStudyProperties::SetModification(const std::string& theName,
							int theMinute,
							int theHour,
							int theDay,
							int theMonth,
							int theYear)
{
  if (_isLocal) {
    CheckLocked();
    SALOMEDS::Locker lock;
    SALOMEDSImpl_AttributeStudyProperties* anImpl = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_local_impl);
    anImpl->SetModification(theName, theMinute, theHour, theDay, theMonth, theYear);
  } else
    SALOMEDS::AttributeStudyProperties::_narrow(_corba_impl)->SetModification(theName.c_str(),
                                                                              theMinute,
                                                                              theHour,
                                                                              theDay,
                                                                              theMonth,
                                                                              theYear);
}

void SALOMEDS_AttributeStudyProperties::GetModificationsList(std::vector<std::string>& theNames,
							     std::vector<int>& theMinutes,
							     std::vector<int>& theHours,
							     std::vector<int>& theDays,
							     std::vector<int>& theMonths,
							     std::vector<int>& theYears,
							     bool theWithCreator)
{
  int i, aLength;

  if (_isLocal) {
    SALOMEDS::Locker lock;
    vector<string> aNames;
    vector<int> aMinutes, aHours, aDays, aMonths, aYears;
    SALOMEDSImpl_AttributeStudyProperties* anImpl = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_local_impl);
    anImpl->GetModifications(aNames, aMinutes, aHours, aDays, aMonths, aYears);
    aLength = aNames.size();
    i = ((theWithCreator) ? 1 : 2);
    for (; i <= aLength; i++) {
      theNames.push_back(aNames[i-1]);
      theMinutes.push_back(aMinutes[i-1]);
      theHours.push_back(aHours[i-1]);
      theDays.push_back(aDays[i-1]);
      theMonths.push_back(aMonths[i-1]);
      theYears.push_back(aYears[i-1]);
    }
  } else {
    SALOMEDS::StringSeq_var aNames;
    SALOMEDS::LongSeq_var aMinutes, aHours, aDays, aMonths, aYears;
    SALOMEDS::AttributeStudyProperties::_narrow(_corba_impl)->GetModificationsList(aNames.out(),
										   aMinutes.out(),
										   aHours.out(),
										   aDays.out(),
										   aMonths.out(),
										   aYears.out(),
										   theWithCreator);
    aLength = aNames->length();
    for (i = 0; i<aLength; i++) {
      theNames.push_back(aNames[i].in());
      theMinutes.push_back(aMinutes[i]);
      theHours.push_back(aHours[i]);
      theDays.push_back(aDays[i]);
      theMonths.push_back(aMonths[i]);
      theYears.push_back(aYears[i]);
    }
  }
}
