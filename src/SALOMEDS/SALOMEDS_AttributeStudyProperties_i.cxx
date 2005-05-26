//  File   : SALOMEDS_AttributeStudyProperties_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeStudyProperties_i.hxx"
#include "SALOMEDS.hxx"
#include <TColStd_HSequenceOfExtendedString.hxx>
#include <TColStd_HSequenceOfInteger.hxx>
#include <TCollection_ExtendedString.hxx>

#define CREATION_MODE_NOTDEFINED 0
#define CREATION_MODE_SCRATCH 1
#define CREATION_MODE_COPY 2

void SALOMEDS_AttributeStudyProperties_i::SetUserName(const char* theName) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(_impl)->SetFirstName((char*)theName);
}

char* SALOMEDS_AttributeStudyProperties_i::GetUserName() 
{
  SALOMEDS::Locker lock;
  TCollection_ExtendedString S = Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(_impl)->GetCreatorName();
  CORBA::String_var c_s = CORBA::string_dup(TCollection_AsciiString(S).ToCString());
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
  Handle(SALOMEDSImpl_AttributeStudyProperties) aProp = Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(_impl);
  int aTmp;
  if (aProp->GetCreationDate(aTmp, aTmp, aTmp, aTmp, aTmp)) return;
  aProp->SetModificationDate(theMinute, theHour, theDay, theMonth, theYear);
}

CORBA::Boolean SALOMEDS_AttributeStudyProperties_i::GetCreationDate(CORBA::Long& theMinute,
								    CORBA::Long& theHour,
								    CORBA::Long& theDay,
								    CORBA::Long& theMonth,
								    CORBA::Long& theYear) 
{
  SALOMEDS::Locker lock;
  Standard_Integer aMinute;
  Standard_Integer aHour;
  Standard_Integer aDay;
  Standard_Integer aMonth;
  Standard_Integer aYear;
  if (Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(_impl)->GetCreationDate(aMinute, aHour, aDay,
										      aMonth, aYear)) 
    {
      theMinute = aMinute;
      theHour = aHour;
      theDay = aDay;
      theMonth = aMonth;
      theYear = aYear;
      return Standard_True;
    }
  return Standard_False;
}

void SALOMEDS_AttributeStudyProperties_i::SetCreationMode(const char* theMode) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(SALOMEDSImpl_AttributeStudyProperties) aProp = Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(_impl);
  if (strcmp(theMode,"from scratch")==0) aProp->SetCreationMode(CREATION_MODE_SCRATCH);
  else if (strcmp(theMode,"copy from")==0) aProp->SetCreationMode(CREATION_MODE_COPY);
  else aProp->SetCreationMode(CREATION_MODE_NOTDEFINED);
}

char* SALOMEDS_AttributeStudyProperties_i::GetCreationMode() 
{
  SALOMEDS::Locker lock;
  CORBA::String_var c_s;
  switch (Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(_impl)->GetCreationMode()) {
  case CREATION_MODE_SCRATCH: c_s = "from scratch"; break;
  case CREATION_MODE_COPY: c_s = "copy from"; break;
  default: c_s = "";
  }
  return c_s._retn();
}

void SALOMEDS_AttributeStudyProperties_i::SetModified(CORBA::Long theModified) 
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(_impl)->SetModified(theModified);
}

CORBA::Boolean SALOMEDS_AttributeStudyProperties_i::IsModified() 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(_impl)->IsModified();
}

CORBA::Long SALOMEDS_AttributeStudyProperties_i::GetModified() 
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(_impl)->GetModified();
}

void SALOMEDS_AttributeStudyProperties_i::SetLocked(CORBA::Boolean theLocked) 
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(_impl)->SetLocked(theLocked);
}

CORBA::Boolean SALOMEDS_AttributeStudyProperties_i::IsLocked()
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(_impl)->IsLocked();
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
  Handle(SALOMEDSImpl_AttributeStudyProperties) aProp = Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(_impl);
  aProp->SetUserName((char*)theName);
  aProp->SetModificationDate((int)theMinute, (int)theHour, (int)theDay, (int)theMonth, (int)theYear);
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
  Handle(TColStd_HSequenceOfExtendedString) aNames;
  Handle(TColStd_HSequenceOfInteger) aMinutes, aHours, aDays, aMonths, aYears;
  Handle(SALOMEDSImpl_AttributeStudyProperties) aProp = Handle(SALOMEDSImpl_AttributeStudyProperties)::DownCast(_impl);
  aNames = aProp->GetUserNames();
  aProp->GetModificationDates(aMinutes, aHours, aDays, aMonths, aYears);
  int aLength = aNames->Length()-((theWithCreator)?0:1);
  theNames = new SALOMEDS::StringSeq;
  theMinutes = new SALOMEDS::LongSeq;
  theHours = new SALOMEDS::LongSeq;
  theDays = new SALOMEDS::LongSeq;
  theMonths = new SALOMEDS::LongSeq;
  theYears = new SALOMEDS::LongSeq;
  theNames->length(aLength);
  theMinutes->length(aLength);
  theHours->length(aLength);
  theDays->length(aLength);
  theMonths->length(aLength);
  theYears->length(aLength);
  int a;
  for(a = 0; a < aLength; a++) 
    {
      (*theNames)[a]=CORBA::string_dup(TCollection_AsciiString(aNames->Value(a + 1 + ((theWithCreator)?0:1))).ToCString());
      (*theMinutes)[a] = aMinutes->Value(a + 1 + ((theWithCreator)?0:1));
      (*theHours)[a] = aHours->Value(a + 1 + ((theWithCreator)?0:1));
      (*theDays)[a] = aDays->Value(a + 1 + ((theWithCreator)?0:1));
      (*theMonths)[a] = aMonths->Value(a + 1 + ((theWithCreator)?0:1));
      (*theYears)[a] = aYears->Value(a + 1 + ((theWithCreator)?0:1));
    }
}

