//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_TableOfRealAttribute.hxx
//  Author : Michael Ponikarov
//  Module : SALOME
//  $Header$

#ifndef _SALOMEDS_TableOfRealAttribute_HeaderFile
#define _SALOMEDS_TableOfRealAttribute_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_SALOMEDS_TableOfRealAttribute_HeaderFile
#include <Handle_SALOMEDS_TableOfRealAttribute.hxx>
#endif

#ifndef _TColStd_SequenceOfTransient_HeaderFile
#include <TColStd_SequenceOfTransient.hxx>
#endif
#ifndef _TCollection_ExtendedString_HeaderFile
#include <TCollection_ExtendedString.hxx>
#endif
#ifndef _Handle_TColStd_HSequenceOfExtendedString_HeaderFile
#include <Handle_TColStd_HSequenceOfExtendedString.hxx>
#endif

#ifndef _TColStd_DataMapOfIntegerReal_HeaderFile
#include <TColStd_DataMapOfIntegerReal.hxx>
#endif

#ifndef _TDF_Attribute_HeaderFile
#include <TDF_Attribute.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif

#ifndef _Handle_TColStd_HSequenceOfInteger_HeaderFile
#include <Handle_TColStd_HSequenceOfInteger.hxx>
#endif 

#ifndef TColStd_HSequenceOfInteger_HeaderFile
#include <TColStd_HSequenceOfInteger.hxx>
#endif 

#ifndef _Handle_TColStd_HSequenceOfReal_HeaderFile
#include <Handle_TColStd_HSequenceOfReal.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
#ifndef _Handle_TDF_Attribute_HeaderFile
#include <Handle_TDF_Attribute.hxx>
#endif
#ifndef _Handle_TDF_RelocationTable_HeaderFile
#include <Handle_TDF_RelocationTable.hxx>
#endif
class TColStd_HSequenceOfExtendedString;
class Standard_GUID;
class TDF_Label;
class TCollection_ExtendedString;
class TColStd_HSequenceOfReal;
class TDF_Attribute;
class TDF_RelocationTable;

#include <strstream>

class SALOMEDS_TableOfRealAttribute : public TDF_Attribute {

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
Standard_EXPORT static  Handle_SALOMEDS_TableOfRealAttribute Set(const TDF_Label& label) ;
Standard_EXPORT SALOMEDS_TableOfRealAttribute();
Standard_EXPORT   void SetNbColumns(const Standard_Integer theNbColumns);
Standard_EXPORT   void SetTitle(const TCollection_ExtendedString& theTitle) ;
Standard_EXPORT   TCollection_ExtendedString GetTitle() const;
Standard_EXPORT   void SetRowData(const Standard_Integer theRow,const Handle(TColStd_HSequenceOfReal)& theData) ;
Standard_EXPORT   Handle_TColStd_HSequenceOfReal GetRowData(const Standard_Integer theRow) ;
Standard_EXPORT   void SetRowTitle(const Standard_Integer theRow,const TCollection_ExtendedString& theTitle) ;
Standard_EXPORT   TCollection_ExtendedString GetRowTitle(const Standard_Integer theRow) const;
Standard_EXPORT   void SetColumnData(const Standard_Integer theColumn,const Handle(TColStd_HSequenceOfReal)& theData) ;
Standard_EXPORT   Handle_TColStd_HSequenceOfReal GetColumnData(const Standard_Integer theColumn) ;
Standard_EXPORT   void SetColumnTitle(const Standard_Integer theColumn,const TCollection_ExtendedString& theTitle) ;
Standard_EXPORT   TCollection_ExtendedString GetColumnTitle(const Standard_Integer theColumn) const;
Standard_EXPORT   Standard_Integer GetNbRows() const;
Standard_EXPORT   Standard_Integer GetNbColumns() const;

Standard_EXPORT   void PutValue(const Standard_Real theValue,const Standard_Integer theRow,const Standard_Integer theColumn) ;
Standard_EXPORT   Standard_Boolean HasValue(const Standard_Integer theRow,const Standard_Integer theColumn) ;
Standard_EXPORT   Standard_Real GetValue(const Standard_Integer theRow,const Standard_Integer theColumn) ;
Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT   void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT   Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT   void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;

Standard_EXPORT   Handle_TColStd_HSequenceOfInteger GetSetRowIndices(const Standard_Integer theRow);
Standard_EXPORT   Handle_TColStd_HSequenceOfInteger GetSetColumnIndices(const Standard_Integer theColumn);

Standard_EXPORT   void ConvertToString(ostrstream& theStream);
Standard_EXPORT   bool RestoreFromString(istrstream& theStream);

Standard_EXPORT ~SALOMEDS_TableOfRealAttribute();




 // Type management
 //
 Standard_EXPORT friend Handle_Standard_Type& SALOMEDS_TableOfRealAttribute_Type_();
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
TColStd_DataMapOfIntegerReal myTable;
TCollection_ExtendedString myTitle;
Handle_TColStd_HSequenceOfExtendedString myRows;
Handle_TColStd_HSequenceOfExtendedString myCols;
Standard_Integer myNbRows;
Standard_Integer myNbColumns;

};





// other inline functions and methods (like "C++: function call" methods)
//


#endif
