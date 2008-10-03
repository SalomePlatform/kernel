//  SALOME SALOMEDSImpl : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDSImpl_AttributeTableOfString.hxx
//  Author : Sergey Ruin
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeTableOfString_HeaderFile
#define _SALOMEDSImpl_AttributeTableOfString_HeaderFile

#include "SALOMEDSImpl_Defines.hxx"
#include "DF_Label.hxx"       
#include "DF_Attribute.hxx"
#include "SALOMEDSImpl_GenericAttribute.hxx"

#include <string>
#include <vector>
#include <map>

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_AttributeTableOfString : 
  public SALOMEDSImpl_GenericAttribute 
{

public:
  virtual std::string Save();
  virtual void Load(const std::string&); 
  static const std::string& GetID() ;
  static SALOMEDSImpl_AttributeTableOfString* Set(const DF_Label& label) ;
  SALOMEDSImpl_AttributeTableOfString();
  void SetNbColumns(const int theNbColumns);
  void SetTitle(const std::string& theTitle) ;
  std::string GetTitle() const;
  void SetRowData(const int theRow,const std::vector<std::string>& theData) ;
  std::vector<std::string> GetRowData(const int theRow) ;
  void SetRowTitle(const int theRow,const std::string& theTitle) ;
  void SetRowUnit(const int theRow,const std::string& theUnit) ;
  std::string GetRowUnit(const int theRow) const;
  void SetRowUnits(const std::vector<std::string>& theUnits) ;
  std::vector<std::string> GetRowUnits();
  void SetRowTitles(const std::vector<std::string>& theTitles) ;
  std::vector<std::string> GetRowTitles();
  std::string GetRowTitle(const int theRow) const;
  void SetColumnData(const int theColumn,const std::vector<std::string>& theData) ;
  std::vector<std::string> GetColumnData(const int theColumn) ;
  void SetColumnTitle(const int theColumn,const std::string& theTitle) ;
  std::string GetColumnTitle(const int theColumn) const;
  void SetColumnTitles(const std::vector<std::string>& theTitles);
  std::vector<std::string> GetColumnTitles();
  int GetNbRows() const;
  int GetNbColumns() const;

  void PutValue(const std::string& theValue,const int theRow,const int theColumn) ;
  bool HasValue(const int theRow,const int theColumn) ;
  std::string GetValue(const int theRow,const int theColumn) ;
  const std::string& ID() const;
  void Restore(DF_Attribute* with) ;
  DF_Attribute* NewEmpty() const;
  void Paste(DF_Attribute* into);

  std::vector<int> GetSetRowIndices(const int theRow);
  std::vector<int> GetSetColumnIndices(const int theColumn);

  ~SALOMEDSImpl_AttributeTableOfString() {}

private: 

  std::map<int, std::string> myTable;
  std::string myTitle;
  std::vector<std::string> myRows;
  std::vector<std::string> myCols;
  int myNbRows;
  int myNbColumns;

};

#endif
