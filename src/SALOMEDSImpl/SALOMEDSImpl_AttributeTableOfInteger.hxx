//  SALOME SALOMEDSImpl : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDSImpl_AttributeTableOfInteger.hxx
//  Author : Michael Ponikarov
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeTableOfInteger_HeaderFile
#define _SALOMEDSImpl_AttributeTableOfInteger_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <Standard.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_Label.hxx>       
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);

#include <TColStd_SequenceOfTransient.hxx>
#include <TCollection_ExtendedString.hxx>
#include <Handle_TColStd_HSequenceOfExtendedString.hxx>
#include <TColStd_DataMapOfIntegerInteger.hxx>
#include <TColStd_HSequenceOfInteger.hxx>
#include <strstream>
#include <TCollection_AsciiString.hxx>

DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeTableOfInteger, SALOMEDSImpl_GenericAttribute )

class SALOMEDSImpl_AttributeTableOfInteger : public SALOMEDSImpl_GenericAttribute 
{

public:
Standard_EXPORT virtual TCollection_AsciiString Save();
Standard_EXPORT virtual void Load(const TCollection_AsciiString&); 
Standard_EXPORT static const Standard_GUID& GetID() ;
Standard_EXPORT static  Handle_SALOMEDSImpl_AttributeTableOfInteger Set(const TDF_Label& label) ;
Standard_EXPORT SALOMEDSImpl_AttributeTableOfInteger();
Standard_EXPORT   void SetNbColumns(const Standard_Integer theNbColumns);
Standard_EXPORT   void SetTitle(const TCollection_ExtendedString& theTitle) ;
Standard_EXPORT   TCollection_ExtendedString GetTitle() const;
Standard_EXPORT   void SetRowData(const Standard_Integer theRow,const Handle(TColStd_HSequenceOfInteger)& theData) ;
Standard_EXPORT   Handle_TColStd_HSequenceOfInteger GetRowData(const Standard_Integer theRow) ;
Standard_EXPORT   void SetRowTitle(const Standard_Integer theRow,const TCollection_ExtendedString& theTitle) ;
Standard_EXPORT   void SetRowUnit(const Standard_Integer theRow,const TCollection_ExtendedString& theUnit) ;
Standard_EXPORT   TCollection_ExtendedString GetRowUnit(const Standard_Integer theRow) const;
Standard_EXPORT   void SetRowUnits(const Handle(TColStd_HSequenceOfExtendedString)& theUnits) ;
Standard_EXPORT   Handle(TColStd_HSequenceOfExtendedString) GetRowUnits();
Standard_EXPORT   void SetRowTitles(const Handle(TColStd_HSequenceOfExtendedString)& theTitles) ;
Standard_EXPORT   Handle(TColStd_HSequenceOfExtendedString) GetRowTitles();
Standard_EXPORT   TCollection_ExtendedString GetRowTitle(const Standard_Integer theRow) const;
Standard_EXPORT   void SetColumnData(const Standard_Integer theColumn,const Handle(TColStd_HSequenceOfInteger)& theData) ;
Standard_EXPORT   Handle_TColStd_HSequenceOfInteger GetColumnData(const Standard_Integer theColumn) ;
Standard_EXPORT   void SetColumnTitle(const Standard_Integer theColumn,const TCollection_ExtendedString& theTitle) ;
Standard_EXPORT   TCollection_ExtendedString GetColumnTitle(const Standard_Integer theColumn) const;
Standard_EXPORT   void SetColumnTitles(const Handle(TColStd_HSequenceOfExtendedString)& theTitles);
Standard_EXPORT   Handle(TColStd_HSequenceOfExtendedString) GetColumnTitles();
Standard_EXPORT   Standard_Integer GetNbRows() const;
Standard_EXPORT   Standard_Integer GetNbColumns() const;

Standard_EXPORT   void PutValue(const Standard_Integer theValue,const Standard_Integer theRow,const Standard_Integer theColumn) ;
Standard_EXPORT   Standard_Boolean HasValue(const Standard_Integer theRow,const Standard_Integer theColumn) ;
Standard_EXPORT   Standard_Integer GetValue(const Standard_Integer theRow,const Standard_Integer theColumn) ;
Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT   void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT   Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT   void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;

Standard_EXPORT   Handle_TColStd_HSequenceOfInteger GetSetRowIndices(const Standard_Integer theRow);
Standard_EXPORT   Handle_TColStd_HSequenceOfInteger GetSetColumnIndices(const Standard_Integer theColumn);

Standard_EXPORT   void ConvertToString(ostrstream& theStream);
Standard_EXPORT   bool RestoreFromString(istrstream& theStream);

Standard_EXPORT ~SALOMEDSImpl_AttributeTableOfInteger() {}

private: 

TColStd_DataMapOfIntegerInteger myTable;
TCollection_ExtendedString myTitle;
Handle_TColStd_HSequenceOfExtendedString myRows;
Handle_TColStd_HSequenceOfExtendedString myCols;
Standard_Integer myNbRows;
Standard_Integer myNbColumns;

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeTableOfInteger )
};

#endif
