//  File   : SALOMEDSImpl_AttributeStudyProperties.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include <SALOMEDSImpl_AttributeStudyProperties.hxx>
#include <Standard_GUID.hxx>
#include <TCollection_ExtendedString.hxx>

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeStudyProperties, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeStudyProperties, SALOMEDSImpl_GenericAttribute )

const Standard_GUID& SALOMEDSImpl_AttributeStudyProperties::GetID()
{
  static Standard_GUID SALOMEDSImpl_AttributeStudyPropertiesID ("128371A2-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeStudyPropertiesID;
}

Handle(SALOMEDSImpl_AttributeStudyProperties) SALOMEDSImpl_AttributeStudyProperties::Set(const TDF_Label& label)
{
  Handle(SALOMEDSImpl_AttributeStudyProperties) anAttr;
  if (!label.FindAttribute(SALOMEDSImpl_AttributeStudyProperties::GetID(),anAttr)) {
    anAttr = new SALOMEDSImpl_AttributeStudyProperties();
    label.AddAttribute(anAttr);
  }
  return anAttr;
}

SALOMEDSImpl_AttributeStudyProperties::SALOMEDSImpl_AttributeStudyProperties()
:SALOMEDSImpl_GenericAttribute("AttributeStudyProperties")
{
  myLocked = Standard_False;
  myLockChanged = Standard_False;
  Init();
}

void SALOMEDSImpl_AttributeStudyProperties::Init()
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

void SALOMEDSImpl_AttributeStudyProperties::SetModification(const TCollection_ExtendedString& theUserName,
                                                            const Standard_Integer            theMinute,
                                                            const Standard_Integer            theHour,
                                                            const Standard_Integer            theDay,
                                                            const Standard_Integer            theMonth,
                                                            const Standard_Integer            theYear)
{
  if (theMinute<0 || theMinute>60 || theHour<0 || theHour>24 ||
      theDay<0 || theDay>31 || theMonth<0 || theMonth>12)
    return;

  CheckLocked();
  Backup();

  myUserName->Append(theUserName);
  myMinute->Append(theMinute);
  myHour->Append(theHour);
  myDay->Append(theDay);
  myMonth->Append(theMonth);
  myYear->Append(theYear);
}

void SALOMEDSImpl_AttributeStudyProperties::GetModifications
                  (Handle(TColStd_HSequenceOfExtendedString)& theUserNames,
                   Handle(TColStd_HSequenceOfInteger)&        theMinutes,
                   Handle(TColStd_HSequenceOfInteger)&        theHours,
                   Handle(TColStd_HSequenceOfInteger)&        theDays,
                   Handle(TColStd_HSequenceOfInteger)&        theMonths,
                   Handle(TColStd_HSequenceOfInteger)&        theYears) const
{
  theUserNames = myUserName;
  theMinutes = myMinute;
  theHours = myHour;
  theDays = myDay;
  theMonths = myMonth;
  theYears = myYear;
}

TCollection_ExtendedString SALOMEDSImpl_AttributeStudyProperties::GetCreatorName() const
{
  if (myUserName->Length() == 0)
    return TCollection_ExtendedString("");
  return myUserName->Value(1);
}

Standard_Boolean SALOMEDSImpl_AttributeStudyProperties::GetCreationDate
                              (Standard_Integer&           theMinute,
                               Standard_Integer&           theHour,
                               Standard_Integer&           theDay,
                               Standard_Integer&           theMonth,
                               Standard_Integer&           theYear) const
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

void SALOMEDSImpl_AttributeStudyProperties::ChangeCreatorName(const TCollection_ExtendedString& theName)
{
  if (myUserName->Length() > 0) {
    CheckLocked();
    Backup();
    myUserName->SetValue(1, theName);
  }
}

void SALOMEDSImpl_AttributeStudyProperties::SetCreationMode(const Standard_Integer theMode)
{
  CheckLocked();
  Backup();
  myMode = theMode;
}

Standard_Integer SALOMEDSImpl_AttributeStudyProperties::GetCreationMode() const
{
  return myMode;
}

void SALOMEDSImpl_AttributeStudyProperties::SetModified(const Standard_Integer theModified)
{
  myModified = theModified;
}

Standard_Boolean SALOMEDSImpl_AttributeStudyProperties::IsModified() const
{
  return (myModified != 0);
}

Standard_Integer SALOMEDSImpl_AttributeStudyProperties::GetModified() const
{
  return myModified;
}

void SALOMEDSImpl_AttributeStudyProperties::SetLocked(const Standard_Boolean theLocked)
{
//  Backup();
  if (myLocked != theLocked) {
    myLockChanged = Standard_True;
    myLocked = theLocked;
  }
}

Standard_Boolean SALOMEDSImpl_AttributeStudyProperties::IsLocked() const
{
  return myLocked;
}

Standard_Boolean SALOMEDSImpl_AttributeStudyProperties::IsLockChanged(const Standard_Boolean theErase) {
  if (!myLockChanged) return Standard_False;
  if (theErase) myLockChanged = Standard_False;
  return Standard_True;
}

const Standard_GUID& SALOMEDSImpl_AttributeStudyProperties::ID() const
{
  return GetID();
}

void SALOMEDSImpl_AttributeStudyProperties::Restore(const Handle(TDF_Attribute)& with)
{
  Handle(SALOMEDSImpl_AttributeStudyProperties) aProp =
    Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(with);
  Init();
  Standard_Integer i;
  Handle(TColStd_HSequenceOfExtendedString) aNames;
  Handle(TColStd_HSequenceOfInteger) aMinutes, aHours, aDays, aMonths, aYears;
  aProp->GetModifications(aNames, aMinutes, aHours, aDays, aMonths, aYears);
  for (i = aNames->Length(); i > 0; i--) {
    myUserName->Prepend(aNames->Value(i));
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

Handle(TDF_Attribute) SALOMEDSImpl_AttributeStudyProperties::NewEmpty() const
{
  return new SALOMEDSImpl_AttributeStudyProperties();
}

void SALOMEDSImpl_AttributeStudyProperties::Paste(const Handle(TDF_Attribute)& into,
                                                  const Handle(TDF_RelocationTable)&) const
{
  Handle(SALOMEDSImpl_AttributeStudyProperties) aProp =
    Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(into);
  aProp->Init();

  Standard_Integer i;
  for(i = 1; i <= myUserName->Length(); i++) {
    aProp->SetModification(myUserName->Value(i),
                           myMinute->Value(i), myHour->Value(i),
                           myDay->Value(i), myMonth->Value(i), myYear->Value(i));
  }

  aProp->SetCreationMode(myMode);
//  aProp->SetModified(myModified);
//  aProp->SetLocked(myLocked);
}


TCollection_AsciiString SALOMEDSImpl_AttributeStudyProperties::Save()
{
  Handle(TColStd_HSequenceOfExtendedString) aNames;
  Handle(TColStd_HSequenceOfInteger) aMinutes, aHours, aDays, aMonths, aYears;
  GetModifications(aNames, aMinutes, aHours, aDays, aMonths, aYears);

  int aLength, anIndex;
  for (aLength = 0, anIndex = aNames->Length(); anIndex > 0; anIndex--)
    aLength += aNames->Value(anIndex).Length() + 1;

  char* aProperty = new char[3 + aLength + 12 * aNames->Length()];

  char crMode = (char)GetCreationMode();

  sprintf(aProperty,"%c%c", crMode, IsLocked()?'l':'u');

  aLength = aNames->Length();
  int a = 2;
  for (anIndex = 1; anIndex  <= aLength; anIndex++) {
    sprintf(&(aProperty[a]),"%2d%2d%2d%2d%4d%s",
	    (int)(aMinutes->Value(anIndex)),
	    (int)(aHours->Value(anIndex)),
	    (int)(aDays->Value(anIndex)),
	    (int)(aMonths->Value(anIndex)),
	    (int)(aYears->Value(anIndex)),
	    (char*)(TCollection_AsciiString(aNames->Value(anIndex)).ToCString()));
    a = strlen(aProperty);
    aProperty[a++] = 1;
  }
  aProperty[a] = 0;
  TCollection_AsciiString prop(aProperty);
  delete aProperty;

  return prop;
}

void SALOMEDSImpl_AttributeStudyProperties::Load(const TCollection_AsciiString& value)
{
  char* aCopy = value.ToCString();

  int crMode = (int)aCopy[0];
  SetCreationMode(crMode);

  int anIndex;
  for (anIndex = 2; anIndex + 2 < value.Length() ;) {
    char str[10];
    Standard_Integer aMinute, aHour, aDay, aMonth, aYear;
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
    SetLocked(Standard_True);
  }
  SetModified(0);
}
