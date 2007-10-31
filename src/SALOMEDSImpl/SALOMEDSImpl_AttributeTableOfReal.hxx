//  SALOME SALOMEDSImpl : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDSImpl_AttributeTableOfReal.hxx
//  Author : Michael Ponikarov
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeTableOfReal_HeaderFile
#define _SALOMEDSImpl_AttributeTableOfReal_HeaderFile

#include "DF_Attribute.hxx"
#include "DF_Label.hxx"      
#include "SALOMEDSImpl_GenericAttribute.hxx"

#include <string>
#include <vector>
#include <map>

class SALOMEDSImpl_AttributeTableOfReal : public SALOMEDSImpl_GenericAttribute 
{

public:
Standard_EXPORT virtual std::string Save();
Standard_EXPORT virtual void Load(const std::string&); 
Standard_EXPORT static const std::string& GetID() ;
Standard_EXPORT static SALOMEDSImpl_AttributeTableOfReal* Set(const DF_Label& label) ;
Standard_EXPORT SALOMEDSImpl_AttributeTableOfReal();
Standard_EXPORT   void SetNbColumns(const int theNbColumns);
Standard_EXPORT   void SetTitle(const std::string& theTitle) ;
Standard_EXPORT   std::string GetTitle() const;
Standard_EXPORT   void SetRowData(const int theRow,const std::vector<double>& theData) ;
Standard_EXPORT   std::vector<double> GetRowData(const int theRow) ;
Standard_EXPORT   void SetRowTitle(const int theRow,const std::string& theTitle) ;
Standard_EXPORT   void SetRowUnit(const int theRow,const std::string& theUnit) ;
Standard_EXPORT   std::string GetRowUnit(const int theRow) const;
Standard_EXPORT   void SetRowUnits(const std::vector<std::string>& theUnits) ;
Standard_EXPORT   std::vector<std::string> GetRowUnits();
Standard_EXPORT   void SetRowTitles(const std::vector<std::string>& theTitles) ;
Standard_EXPORT   std::vector<std::string> GetRowTitles();
Standard_EXPORT   std::string GetRowTitle(const int theRow) const;
Standard_EXPORT   void SetColumnData(const int theColumn,const std::vector<double>& theData) ;
Standard_EXPORT   std::vector<double> GetColumnData(const int theColumn) ;
Standard_EXPORT   void SetColumnTitle(const int theColumn,const std::string& theTitle) ;
Standard_EXPORT   void SetColumnTitles(const std::vector<std::string>& theTitles);
Standard_EXPORT   std::vector<std::string> GetColumnTitles();
Standard_EXPORT   std::string GetColumnTitle(const int theColumn) const;
Standard_EXPORT   int GetNbRows() const;
Standard_EXPORT   int GetNbColumns() const;

Standard_EXPORT   void PutValue(const double& theValue,const int theRow,const int theColumn) ;
Standard_EXPORT   bool HasValue(const int theRow,const int theColumn) ;
Standard_EXPORT   double GetValue(const int theRow,const int theColumn) ;
Standard_EXPORT  const std::string& ID() const;
Standard_EXPORT   void Restore(DF_Attribute* with) ;
Standard_EXPORT   DF_Attribute* NewEmpty() const;
Standard_EXPORT   void Paste(DF_Attribute* into);

Standard_EXPORT   std::vector<int> GetSetRowIndices(const int theRow);
Standard_EXPORT   std::vector<int> GetSetColumnIndices(const int theColumn);

Standard_EXPORT ~SALOMEDSImpl_AttributeTableOfReal() {}

private: 
std::map<int, double> myTable;
std::string myTitle;
std::vector<std::string> myRows;
std::vector<std::string> myCols;
int myNbRows;
int myNbColumns;

};

#endif
