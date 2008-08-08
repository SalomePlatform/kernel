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

#include "SALOMEDSImpl_Defines.hxx"
#include "DF_Attribute.hxx"
#include "DF_Label.hxx"      
#include "SALOMEDSImpl_GenericAttribute.hxx"

#include <string>
#include <vector>
#include <map>

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_AttributeTableOfReal :
  public SALOMEDSImpl_GenericAttribute 
{

public:
  virtual std::string Save();
  virtual void Load(const std::string&); 
  static const std::string& GetID() ;
  static SALOMEDSImpl_AttributeTableOfReal* Set(const DF_Label& label) ;
  SALOMEDSImpl_AttributeTableOfReal();
  void SetNbColumns(const int theNbColumns);
  void SetTitle(const std::string& theTitle) ;
  std::string GetTitle() const;
  void SetRowData(const int theRow,const std::vector<double>& theData) ;
  std::vector<double> GetRowData(const int theRow) ;
  void SetRowTitle(const int theRow,const std::string& theTitle) ;
  void SetRowUnit(const int theRow,const std::string& theUnit) ;
  std::string GetRowUnit(const int theRow) const;
  void SetRowUnits(const std::vector<std::string>& theUnits) ;
  std::vector<std::string> GetRowUnits();
  void SetRowTitles(const std::vector<std::string>& theTitles) ;
  std::vector<std::string> GetRowTitles();
  std::string GetRowTitle(const int theRow) const;
  void SetColumnData(const int theColumn,const std::vector<double>& theData) ;
  std::vector<double> GetColumnData(const int theColumn) ;
  void SetColumnTitle(const int theColumn,const std::string& theTitle) ;
  void SetColumnTitles(const std::vector<std::string>& theTitles);
  std::vector<std::string> GetColumnTitles();
  std::string GetColumnTitle(const int theColumn) const;
  int GetNbRows() const;
  int GetNbColumns() const;

  void PutValue(const double& theValue,const int theRow,const int theColumn) ;
  bool HasValue(const int theRow,const int theColumn) ;
  double GetValue(const int theRow,const int theColumn) ;
  const std::string& ID() const;
  void Restore(DF_Attribute* with) ;
  DF_Attribute* NewEmpty() const;
  void Paste(DF_Attribute* into);

  std::vector<int> GetSetRowIndices(const int theRow);
  std::vector<int> GetSetColumnIndices(const int theColumn);

  ~SALOMEDSImpl_AttributeTableOfReal() {}

private: 
  std::map<int, double> myTable;
  std::string myTitle;
  std::vector<std::string> myRows;
  std::vector<std::string> myCols;
  int myNbRows;
  int myNbColumns;

};

#endif
