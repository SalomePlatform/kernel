//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//  
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_StudyPropertiesAttribute.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef _SALOMEDS_StudyPropertiesAttribute_HeaderFile
#define _SALOMEDS_StudyPropertiesAttribute_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_SALOMEDS_StudyPropertiesAttribute_HeaderFile
#include <Handle_SALOMEDS_StudyPropertiesAttribute.hxx>
#endif

#ifndef _Handle_TColStd_HSequenceOfExtendedString_HeaderFile
#include <Handle_TColStd_HSequenceOfExtendedString.hxx>
#endif
#ifndef _Handle_TColStd_HSequenceOfInteger_HeaderFile
#include <Handle_TColStd_HSequenceOfInteger.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _TDF_Attribute_HeaderFile
#include <TDF_Attribute.hxx>
#endif
#ifndef _Handle_TDF_Attribute_HeaderFile
#include <Handle_TDF_Attribute.hxx>
#endif
#ifndef _Handle_TDF_RelocationTable_HeaderFile
#include <Handle_TDF_RelocationTable.hxx>
#endif
class TColStd_HSequenceOfExtendedString;
class TColStd_HSequenceOfInteger;
class Standard_GUID;
class TDF_Label;
class TCollection_ExtendedString;
class TDF_Attribute;
class TDF_RelocationTable;


class SALOMEDS_StudyPropertiesAttribute : public TDF_Attribute {

public:

    inline void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    inline void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    inline void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }
//    inline void  operator delete(void *anAddress, size_t size) 
//      { 
//        if (anAddress) Standard::Free((Standard_Address&)anAddress,size); 
//      }
 // Methods PUBLIC
 // 
Standard_EXPORT static const Standard_GUID& GetID() ;
Standard_EXPORT static  Handle_SALOMEDS_StudyPropertiesAttribute Set(const TDF_Label& label) ;
Standard_EXPORT SALOMEDS_StudyPropertiesAttribute();
Standard_EXPORT   void Init() ;
Standard_EXPORT   void SetUserName(const TCollection_ExtendedString& theName) ;
Standard_EXPORT   void SetFirstName(const TCollection_ExtendedString& theName) ;
Standard_EXPORT   TCollection_ExtendedString GetCreatorName() const;
Standard_EXPORT   Handle_TColStd_HSequenceOfExtendedString GetUserNames() const;
Standard_EXPORT   void SetModificationDate(const Standard_Integer theMinute,const Standard_Integer theHour,const Standard_Integer theDay,const Standard_Integer theMonth,const Standard_Integer theYear) ;
Standard_EXPORT   Standard_Boolean GetCreationDate(Standard_Integer& theMinute,Standard_Integer& theHour,Standard_Integer& theDay,Standard_Integer& theMonth,Standard_Integer& theYear) const;
Standard_EXPORT   void GetModificationDates(Handle(TColStd_HSequenceOfInteger)& theMinutes,Handle(TColStd_HSequenceOfInteger)& theHours,Handle(TColStd_HSequenceOfInteger)& theDays,Handle(TColStd_HSequenceOfInteger)& theMonths,Handle(TColStd_HSequenceOfInteger)& theYears) const;
Standard_EXPORT   void SetCreationMode(const Standard_Integer theMode) ;
Standard_EXPORT   Standard_Integer GetCreationMode() const;
Standard_EXPORT   void SetModified(const Standard_Integer theModified) ;
Standard_EXPORT   Standard_Boolean IsModified() const;
Standard_EXPORT   Standard_Integer GetModified() const;
Standard_EXPORT   void SetLocked(const Standard_Boolean theLocked) ;
Standard_EXPORT   Standard_Boolean IsLocked() const;
Standard_EXPORT   Standard_Boolean IsLockChanged(const Standard_Boolean theErase) ;
Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT   void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT   Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT   void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDS_StudyPropertiesAttribute();




 // Type management
 //
 Standard_EXPORT friend Handle_Standard_Type& SALOMEDS_StudyPropertiesAttribute_Type_();
 Standard_EXPORT const Handle(Standard_Type)& DynamicType() const;
 Standard_EXPORT Standard_Boolean	       IsKind(const Handle(Standard_Type)&) const;

protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 


 // Fields PRIVATE
 //
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


};





// other inline functions and methods (like "C++: function call" methods)
//


#endif
