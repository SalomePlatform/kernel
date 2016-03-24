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

//  File   : SALOMEDS_AttributeStudyProperties_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDS_AttributeStudyProperties_i.hxx"
#include "SALOMEDS.hxx"

#define CREATION_MODE_NOTDEFINED 0
#define CREATION_MODE_SCRATCH 1
#define CREATION_MODE_COPY 2

void SALOMEDS_AttributeStudyProperties_i::SetUserName(const char* theName)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->ChangeCreatorName((char*)theName);
}

char* SALOMEDS_AttributeStudyProperties_i::GetUserName()
{
  SALOMEDS::Locker lock;
  std::string S = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->GetCreatorName();
  CORBA::String_var c_s = CORBA::string_dup(S.c_str());
  return c_s._retn();
}

void SALOMEDS_AttributeStudyProperties_i::SetCreationDate(CORBA::Long theMinute,
                                                          CORBA::Long theHour,
                                                          CORBA::Long theDay,
                                                          CORBA::Long theMonth,
                                                          CORBA::Long theYear)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_AttributeStudyProperties* aProp = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl);
  int aTmp;
  if (aProp->GetCreationDate(aTmp, aTmp, aTmp, aTmp, aTmp)) return;
  std::string S;
  aProp->SetModification(S, theMinute, theHour, theDay, theMonth, theYear);
}

CORBA::Boolean SALOMEDS_AttributeStudyProperties_i::GetCreationDate(CORBA::Long& theMinute,
                                                                    CORBA::Long& theHour,
                                                                    CORBA::Long& theDay,
                                                                    CORBA::Long& theMonth,
                                                                    CORBA::Long& theYear)
{
  SALOMEDS::Locker lock;
  int aMinute;
  int aHour;
  int aDay;
  int aMonth;
  int aYear;
  if (dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->GetCreationDate(aMinute, aHour, aDay, aMonth, aYear)) {
    theMinute = aMinute;
    theHour = aHour;
    theDay = aDay;
    theMonth = aMonth;
    theYear = aYear;
    return true;
  }
  return false;
}

void SALOMEDS_AttributeStudyProperties_i::SetCreationMode(const char* theMode)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_AttributeStudyProperties* aProp = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl);
  if (strcmp(theMode,"from scratch")==0) aProp->SetCreationMode(CREATION_MODE_SCRATCH);
  else if (strcmp(theMode,"copy from")==0) aProp->SetCreationMode(CREATION_MODE_COPY);
  else aProp->SetCreationMode(CREATION_MODE_NOTDEFINED);
}

char* SALOMEDS_AttributeStudyProperties_i::GetCreationMode()
{
  SALOMEDS::Locker lock;
  CORBA::String_var c_s;
  switch (dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->GetCreationMode()) {
  case CREATION_MODE_SCRATCH: c_s = "from scratch"; break;
  case CREATION_MODE_COPY: c_s = "copy from"; break;
  default: c_s = "";
  }
  return c_s._retn();
}

void SALOMEDS_AttributeStudyProperties_i::SetModified(CORBA::Long theModified)
{
  SALOMEDS::Locker lock;
  dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->SetModified(theModified);
}

CORBA::Boolean SALOMEDS_AttributeStudyProperties_i::IsModified()
{
  SALOMEDS::Locker lock;
  return dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->IsModified();
}

CORBA::Long SALOMEDS_AttributeStudyProperties_i::GetModified()
{
  SALOMEDS::Locker lock;
  return dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->GetModified();
}

void SALOMEDS_AttributeStudyProperties_i::SetLocked(CORBA::Boolean theLocked)
{
  SALOMEDS::Locker lock;
  dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->SetLocked(theLocked);
}

CORBA::Boolean SALOMEDS_AttributeStudyProperties_i::IsLocked()
{
  SALOMEDS::Locker lock;
  return dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->IsLocked();
}

void SALOMEDS_AttributeStudyProperties_i::SetModification(const char* theName,
                                                          CORBA::Long theMinute,
                                                          CORBA::Long theHour,
                                                          CORBA::Long theDay,
                                                          CORBA::Long theMonth,
                                                          CORBA::Long theYear)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  SALOMEDSImpl_AttributeStudyProperties* aProp = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl);
  aProp->SetModification((char*)theName, (int)theMinute, (int)theHour,
                         (int)theDay, (int)theMonth, (int)theYear);
}

void SALOMEDS_AttributeStudyProperties_i::GetModificationsList(SALOMEDS::StringSeq_out theNames,
                                                               SALOMEDS::LongSeq_out theMinutes,
                                                               SALOMEDS::LongSeq_out theHours,
                                                               SALOMEDS::LongSeq_out theDays,
                                                               SALOMEDS::LongSeq_out theMonths,
                                                               SALOMEDS::LongSeq_out theYears,
                                                               CORBA::Boolean theWithCreator)
{
  SALOMEDS::Locker lock;
  std::vector<std::string> aNames;
  std::vector<int> aMinutes, aHours, aDays, aMonths, aYears;
  SALOMEDSImpl_AttributeStudyProperties* aProp = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl);
  aProp->GetModifications(aNames, aMinutes, aHours, aDays, aMonths, aYears);
  int aLength = aNames.size();
  int aRetLength = aLength - ((theWithCreator) ? 0 : 1);
  theNames = new SALOMEDS::StringSeq;
  theMinutes = new SALOMEDS::LongSeq;
  theHours = new SALOMEDS::LongSeq;
  theDays = new SALOMEDS::LongSeq;
  theMonths = new SALOMEDS::LongSeq;
  theYears = new SALOMEDS::LongSeq;
  theNames->length(aRetLength);
  theMinutes->length(aRetLength);
  theHours->length(aRetLength);
  theDays->length(aRetLength);
  theMonths->length(aRetLength);
  theYears->length(aRetLength);
  int a = 0, ind = ((theWithCreator) ? 1 : 2);
  for (; ind <= aLength; a++, ind++) {
    (*theNames)[a] = CORBA::string_dup(aNames[ind-1].c_str());
    (*theMinutes)[a] = aMinutes[ind-1];
    (*theHours)[a] = aHours[ind-1];
    (*theDays)[a] = aDays[ind-1];
    (*theMonths)[a] = aMonths[ind-1];
    (*theYears)[a] = aYears[ind-1];
  }
}

void SALOMEDS_AttributeStudyProperties_i::SetComment(const char* theComment)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->SetComment(std::string(theComment));
}

char* SALOMEDS_AttributeStudyProperties_i::GetComment()
{
  SALOMEDS::Locker lock;
  std::string C = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->GetComment();
  CORBA::String_var c_s = CORBA::string_dup(C.c_str());
  return c_s._retn();
}

void SALOMEDS_AttributeStudyProperties_i::SetUnits(const char* theUnits)
{
  SALOMEDS::Locker lock;
  CheckLocked();
  dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->SetUnits(std::string(theUnits));
}

char* SALOMEDS_AttributeStudyProperties_i::GetUnits()
{
  SALOMEDS::Locker lock;
  std::string U = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->GetUnits();
  CORBA::String_var c_s = CORBA::string_dup(U.c_str());
  return c_s._retn();
}

SALOMEDS::StringSeq* SALOMEDS_AttributeStudyProperties_i::GetStoredComponents()
{
  SALOMEDS::Locker lock;
  std::vector<std::string> components = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->GetStoredComponents();
  SALOMEDS::StringSeq_var c_components = new SALOMEDS::StringSeq();
  c_components->length(components.size());
  for (int i = 0; i < components.size(); i++) {
    c_components[i] = CORBA::string_dup(components[i].c_str());
  }
  return c_components._retn();
}

char* SALOMEDS_AttributeStudyProperties_i::GetComponentVersion(const char* theComponent)
{
  SALOMEDS::Locker lock;
  std::string version = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->GetComponentVersion(theComponent);
  CORBA::String_var c_version = CORBA::string_dup(version.c_str());
  return c_version._retn();
}

SALOMEDS::StringSeq* SALOMEDS_AttributeStudyProperties_i::GetComponentVersions(const char* theComponent)
{
  SALOMEDS::Locker lock;
  std::vector<std::string> versions = dynamic_cast<SALOMEDSImpl_AttributeStudyProperties*>(_impl)->GetComponentVersions(theComponent);
  SALOMEDS::StringSeq_var c_versions = new SALOMEDS::StringSeq();
  c_versions->length(versions.size());
  for (int i = 0; i < versions.size(); i++) {
    c_versions[i] = CORBA::string_dup(versions[i].c_str());
  }
  return c_versions._retn();
}
