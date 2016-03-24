// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//

//  SALOME SALOMEDSImpl : data structure of SALOME and sources of Salome data server 
//  File   : SALOMEDSImpl_AttributeTableOfReal.hxx
//  Author : Michael Ponikarov
//  Module : SALOME
//
#ifndef _SALOMEDSImpl_AttributeTableOfReal_HeaderFile
#define _SALOMEDSImpl_AttributeTableOfReal_HeaderFile

#include "SALOMEDSImpl_Defines.hxx"
#include "DF_Attribute.hxx"
#include "DF_Label.hxx"      
#include "SALOMEDSImpl_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeTable.hxx"

#include <string>
#include <vector>
#include <map>

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_AttributeTableOfReal :
  public SALOMEDSImpl_GenericAttribute, public SALOMEDSImpl_AttributeTable
{

public:
  virtual std::string       Save();
  virtual void              Load(const std::string&); 

  static const std::string& GetID();
  static SALOMEDSImpl_AttributeTableOfReal* Set(const DF_Label& label);

  SALOMEDSImpl_AttributeTableOfReal();

  void                     SetNbColumns(const int theNbColumns);
  void                     SetTitle(const std::string& theTitle);
  std::string              GetTitle() const;
  void                     SetRowData(const int theRow, const std::vector<double>& theData);
  std::vector<double>      GetRowData(const int theRow);
  void                     SetRowTitle(const int theRow, const std::string& theTitle);
  void                     SetRowUnit(const int theRow, const std::string& theUnit);
  std::string              GetRowUnit(const int theRow) const;
  void                     SetRowUnits(const std::vector<std::string>& theUnits);
  std::vector<std::string> GetRowUnits();
  void                     SetRowTitles(const std::vector<std::string>& theTitles);
  std::vector<std::string> GetRowTitles();
  std::string              GetRowTitle(const int theRow) const;
  void                     SetColumnData(const int theColumn, const std::vector<double>& theData);
  std::vector<double>      GetColumnData(const int theColumn);
  void                     SetColumnTitle(const int theColumn, const std::string& theTitle);
  void                     SetColumnTitles(const std::vector<std::string>& theTitles);
  std::vector<std::string> GetColumnTitles();
  std::string              GetColumnTitle(const int theColumn) const;
  int                      GetNbRows() const;
  int                      GetNbColumns() const;

  void                     PutValue(const double& theValue, const int theRow, const int theColumn);
  bool                     HasValue(const int theRow, const int theColumn);
  double                   GetValue(const int theRow, const int theColumn);
  void                     RemoveValue(const int theRow, const int theColumn);
  const std::string&       ID() const;
  void                     Restore(DF_Attribute* with);
  DF_Attribute*            NewEmpty() const;
  void                     Paste(DF_Attribute* into);

  std::vector<int>         GetSetRowIndices(const int theRow);
  std::vector<int>         GetSetColumnIndices(const int theColumn);

  std::vector<int>         SortRow(const int theRow, SortOrder sortOrder, SortPolicy sortPolicy);
  std::vector<int>         SortColumn(const int theColumn, SortOrder sortOrder, SortPolicy sortPolicy);
  std::vector<int>         SortByRow(const int theRow, SortOrder sortOrder, SortPolicy sortPolicy);
  std::vector<int>         SortByColumn(const int theColumn, SortOrder sortOrder, SortPolicy sortPolicy);

  void                     SwapCells(const int theRow1, const int theColumn1, const int theRow2, const int theColumn2);
  void                     SwapRows(const int theRow1, const int theRow2);
  void                     SwapColumns(const int theColumn1, const int theColumn2);

  ~SALOMEDSImpl_AttributeTableOfReal() {}

private: 
  std::map<int, double>    myTable;
  std::string              myTitle;
  std::vector<std::string> myRows;
  std::vector<std::string> myCols;
  int                      myNbRows;
  int                      myNbColumns;
};

#endif
