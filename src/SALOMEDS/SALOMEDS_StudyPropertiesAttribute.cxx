//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_StudyPropertiesAttribute.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include <SALOMEDS_StudyPropertiesAttribute.ixx>
using namespace std;

const Standard_GUID& SALOMEDS_StudyPropertiesAttribute::GetID() 
{
  static Standard_GUID SALOMEDS_StudyPropertiesAttributeID ("128371A2-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_StudyPropertiesAttributeID;
}

Handle(SALOMEDS_StudyPropertiesAttribute) SALOMEDS_StudyPropertiesAttribute::Set(const TDF_Label& label) 
{
  Handle(SALOMEDS_StudyPropertiesAttribute) anAttr;
  if (!label.FindAttribute(SALOMEDS_StudyPropertiesAttribute::GetID(),anAttr)) {
    anAttr = new SALOMEDS_StudyPropertiesAttribute();
    label.AddAttribute(anAttr);
  }
  return anAttr;
}

SALOMEDS_StudyPropertiesAttribute::SALOMEDS_StudyPropertiesAttribute()
{
  myLocked = Standard_False;
  myLockChanged = Standard_False;
  Init();
}

void SALOMEDS_StudyPropertiesAttribute::Init()
{
  myUserName = new TColStd_HSequenceOfExtendedString();
  myMinute = new TColStd_HSequenceOfInteger();
  myHour = new TColStd_HSequenceOfInteger();
  myDay = new TColStd_HSequenceOfInteger();
  myMonth = new TColStd_HSequenceOfInteger();
  myYear = new TColStd_HSequenceOfInteger();
//  myModified = 0;
  myMode = 0; // none
}

void SALOMEDS_StudyPropertiesAttribute::SetUserName(const TCollection_ExtendedString& theName) 
{
  Backup();
  myUserName->Append(theName);
}

void SALOMEDS_StudyPropertiesAttribute::SetFirstName(const TCollection_ExtendedString& theName)
{
  Backup();
  if (myUserName->Length() == 0) myUserName->Append(theName);
  else myUserName->SetValue(1, theName);
}

TCollection_ExtendedString SALOMEDS_StudyPropertiesAttribute::GetCreatorName() const
{
  if (myUserName->Length() == 0) return TCollection_ExtendedString("");
  return myUserName->Value(1);
}

Handle(TColStd_HSequenceOfExtendedString) SALOMEDS_StudyPropertiesAttribute::GetUserNames() const
{
  return myUserName;
}

void SALOMEDS_StudyPropertiesAttribute::SetModificationDate(const Standard_Integer theMinute,
							const Standard_Integer theHour,
							const Standard_Integer theDay,
							const Standard_Integer theMonth,
							const Standard_Integer theYear) 
{
  Backup();
  if (theMinute<0 || theMinute>60 || theHour<0 || theHour>24 || theDay<0 || theDay>31 || theMonth<0 || theMonth>12)
    return;
  myMinute->Append(theMinute);
  myHour->Append(theHour);
  myDay->Append(theDay);
  myMonth->Append(theMonth);
  myYear->Append(theYear);
}

Standard_Boolean SALOMEDS_StudyPropertiesAttribute::GetCreationDate(Standard_Integer& theMinute,
								    Standard_Integer& theHour,
								    Standard_Integer& theDay,
								    Standard_Integer& theMonth,
								    Standard_Integer& theYear) const
{
  if (myMinute->Length() != 0) {
    theMinute = myMinute->Value(1);
    theHour = myHour->Value(1);
    theDay = myDay->Value(1);
    theMonth = myMonth->Value(1);
    theYear = myYear->Value(1);
    return Standard_True;
  }
  return Standard_False;
}

void SALOMEDS_StudyPropertiesAttribute::GetModificationDates(Handle(TColStd_HSequenceOfInteger)& theMinutes,
							     Handle(TColStd_HSequenceOfInteger)& theHours,
							     Handle(TColStd_HSequenceOfInteger)& theDays,
							     Handle(TColStd_HSequenceOfInteger)& theMonths,
							     Handle(TColStd_HSequenceOfInteger)& theYears) const
{
  theMinutes = myMinute;
  theHours = myHour;
  theDays = myDay;
  theMonths = myMonth;
  theYears = myYear;
}

void SALOMEDS_StudyPropertiesAttribute::SetCreationMode(const Standard_Integer theMode) 
{
  Backup();
  myMode = theMode;
}

Standard_Integer SALOMEDS_StudyPropertiesAttribute::GetCreationMode() const
{
  return myMode;
}

void SALOMEDS_StudyPropertiesAttribute::SetModified(const Standard_Integer theModified) 
{
  myModified = theModified;
}

Standard_Boolean SALOMEDS_StudyPropertiesAttribute::IsModified() const
{
  return (myModified != 0);
}

Standard_Integer SALOMEDS_StudyPropertiesAttribute::GetModified() const
{
  return myModified;
}

void SALOMEDS_StudyPropertiesAttribute::SetLocked(const Standard_Boolean theLocked) 
{
//  Backup();
  if (myLocked != theLocked) {
    myLockChanged = Standard_True;
    myLocked = theLocked;
  }
}

Standard_Boolean SALOMEDS_StudyPropertiesAttribute::IsLocked() const
{
  return myLocked;
}

Standard_Boolean SALOMEDS_StudyPropertiesAttribute::IsLockChanged(const Standard_Boolean theErase) {
  if (!myLockChanged) return Standard_False;
  if (theErase) myLockChanged = Standard_False;
  return Standard_True;
}

const Standard_GUID& SALOMEDS_StudyPropertiesAttribute::ID() const
{
  return GetID();
}

void SALOMEDS_StudyPropertiesAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  Handle(SALOMEDS_StudyPropertiesAttribute) aProp = Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(with);
  Init();
  Standard_Integer i;
  Handle(TColStd_HSequenceOfExtendedString) aNames = aProp->GetUserNames();
  for(i = aNames->Length(); i > 0; i--) {
    myUserName->Prepend(aNames->Value(i));
  }
  Handle(TColStd_HSequenceOfInteger) aMinutes, aHours, aDays, aMonths, aYears;
  aProp->GetModificationDates(aMinutes, aHours, aDays, aMonths, aYears);
  for(i = aMinutes->Length(); i > 0; i--) {
    myMinute->Prepend(aMinutes->Value(i));
    myHour->Prepend(aHours->Value(i));
    myDay->Prepend(aDays->Value(i));
    myMonth->Prepend(aMonths->Value(i));
    myYear->Prepend(aYears->Value(i));
  }
  myMode = aProp->GetCreationMode();
//  myModified = aProp->GetModified();
//  myLocked = aProp->IsLocked();
}

Handle(TDF_Attribute) SALOMEDS_StudyPropertiesAttribute::NewEmpty() const
{
  return new SALOMEDS_StudyPropertiesAttribute();
}

void SALOMEDS_StudyPropertiesAttribute::Paste(const Handle(TDF_Attribute)& into,
					      const Handle(TDF_RelocationTable)&) const
{
  Handle(SALOMEDS_StudyPropertiesAttribute) aProp = Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(into);
  aProp->Init();

  Standard_Integer i;
  for(i = 1; i <= myUserName->Length(); i++) {
    aProp->SetUserName(myUserName->Value(i));
  }
  for(i = 1; i <= myMinute->Length(); i++) {
    aProp->SetModificationDate(myMinute->Value(i), myHour->Value(i), myDay->Value(i), myMonth->Value(i), myYear->Value(i));
  }

  aProp->SetCreationMode(myMode);
//  aProp->SetModified(myModified);
//  aProp->SetLocked(myLocked);
}

