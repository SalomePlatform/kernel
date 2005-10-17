//  File   : SALOMEDSImpl_AttributeStudyProperties.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeStudyProperties_HeaderFile
#define _SALOMEDSImpl_AttributeStudyProperties_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <Standard.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_Label.hxx>
#include <TCollection_AsciiString.hxx>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeStudyProperties, SALOMEDSImpl_GenericAttribute )

#include <TColStd_HSequenceOfExtendedString.hxx>
#include <TColStd_HSequenceOfInteger.hxx>

class SALOMEDSImpl_AttributeStudyProperties : public SALOMEDSImpl_GenericAttribute
{
public:
Standard_EXPORT SALOMEDSImpl_AttributeStudyProperties();
Standard_EXPORT ~SALOMEDSImpl_AttributeStudyProperties() {}

Standard_EXPORT static const Standard_GUID& GetID();
Standard_EXPORT        const Standard_GUID&    ID() const;

Standard_EXPORT static  Handle_SALOMEDSImpl_AttributeStudyProperties Set(const TDF_Label& label);

Standard_EXPORT void Init();

Standard_EXPORT virtual TCollection_AsciiString Save();
Standard_EXPORT virtual void Load(const TCollection_AsciiString&);

Standard_EXPORT   void SetModification(const TCollection_ExtendedString& theUserName,
                                       const Standard_Integer            theMinute,
                                       const Standard_Integer            theHour,
                                       const Standard_Integer            theDay,
                                       const Standard_Integer            theMonth,
                                       const Standard_Integer            theYear);
Standard_EXPORT   void GetModifications(Handle(TColStd_HSequenceOfExtendedString)& theUserNames,
                                        Handle(TColStd_HSequenceOfInteger)&        theMinutes,
                                        Handle(TColStd_HSequenceOfInteger)&        theHours,
                                        Handle(TColStd_HSequenceOfInteger)&        theDays,
                                        Handle(TColStd_HSequenceOfInteger)&        theMonths,
                                        Handle(TColStd_HSequenceOfInteger)&        theYears) const;

Standard_EXPORT   TCollection_ExtendedString GetCreatorName() const;
Standard_EXPORT   Standard_Boolean GetCreationDate(Standard_Integer&           theMinute,
						   Standard_Integer&           theHour,
						   Standard_Integer&           theDay,
						   Standard_Integer&           theMonth,
						   Standard_Integer&           theYear) const;

Standard_EXPORT   void ChangeCreatorName(const TCollection_ExtendedString& theUserName);

Standard_EXPORT   void SetCreationMode(const Standard_Integer theMode);
Standard_EXPORT   Standard_Integer GetCreationMode() const;

Standard_EXPORT   void SetModified(const Standard_Integer theModified);
Standard_EXPORT   Standard_Boolean IsModified() const;
Standard_EXPORT   Standard_Integer GetModified() const;

Standard_EXPORT   void SetLocked(const Standard_Boolean theLocked);
Standard_EXPORT   Standard_Boolean IsLocked() const;
Standard_EXPORT   Standard_Boolean IsLockChanged(const Standard_Boolean theErase);

Standard_EXPORT   void Restore(const Handle(TDF_Attribute)& with);
Standard_EXPORT   Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT   void Paste(const Handle(TDF_Attribute)& into,
                             const Handle(TDF_RelocationTable)& RT) const;

private:

Handle_TColStd_HSequenceOfExtendedString myUserName;
Handle_TColStd_HSequenceOfInteger myMinute;
Handle_TColStd_HSequenceOfInteger myHour;
Handle_TColStd_HSequenceOfInteger myDay;
Handle_TColStd_HSequenceOfInteger myMonth;
Handle_TColStd_HSequenceOfInteger myYear;
Standard_Integer myMode;
Standard_Integer myModified;
Standard_Boolean myLocked;
Standard_Boolean myLockChanged;

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeStudyProperties )

};

#endif
