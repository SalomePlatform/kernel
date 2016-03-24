// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
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

//  File   : SALOMEDSClient_AttributeTable.hxx
//  Author : Vadim SANDLER, Open CASCADE S.A.S. (vadim.sandler@opencascade.com)
//
#ifndef SALOMEDSClient_AttributeTable_HeaderFile
#define SALOMEDSClient_AttributeTable_HeaderFile

#include <vector>
#include <string>
#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeTable: public virtual SALOMEDSClient_GenericAttribute
{
public:
  typedef enum { 
    AscendingOrder,
    DescendingOrder,
  } SortOrder;

  typedef enum {
    EmptyLowest,
    EmptyHighest,
    EmptyFirst,
    EmptyLast,
    EmptyIgnore,
  } SortPolicy;

  virtual void SetTitle(const std::string& theTitle) = 0;
  virtual std::string GetTitle() = 0;
  virtual void SetRowTitle(int theIndex, const std::string& theTitle) = 0;
  virtual std::string GetRowTitle(int theIndex) = 0;
  virtual void SetRowTitles(const std::vector<std::string>& theTitles) = 0;
  virtual std::vector<std::string> GetRowTitles() = 0;
  virtual void SetColumnTitle(int theIndex, const std::string& theTitle) = 0;
  virtual std::string GetColumnTitle(int theIndex) = 0;
  virtual void SetColumnTitles(const std::vector<std::string>& theTitles) = 0;
  virtual std::vector<std::string> GetColumnTitles() = 0;
  virtual void SetRowUnit(int theIndex, const std::string& theUnit) = 0;
  virtual std::string GetRowUnit(int theIndex) = 0;
  virtual void SetRowUnits(const std::vector<std::string>& theUnits) = 0;
  virtual std::vector<std::string> GetRowUnits() = 0;

  virtual int GetNbRows() = 0;
  virtual int GetNbColumns() = 0;
  virtual bool HasValue(int theRow, int theColumn) = 0;
  virtual void RemoveValue(int theRow, int theColumn) = 0;

  virtual std::vector<int> GetRowSetIndices(int theRow) = 0;
  virtual void SetNbColumns(int theNbColumns) = 0;

  virtual std::vector<int> SortRow(int theRow, SortOrder theOrder, SortPolicy thePolicy) = 0;
  virtual std::vector<int> SortColumn(int theColumn, SortOrder theOrder, SortPolicy thePolicy) = 0;
  virtual std::vector<int> SortByRow(int theRow, SortOrder theOrder, SortPolicy thePolicy) = 0;
  virtual std::vector<int> SortByColumn(int theColumn, SortOrder theOrder, SortPolicy thePolicy) = 0;
  virtual void SwapCells(int theRow1, int theColumn1, int theRow2, int theColumn2) = 0;
  virtual void SwapRows(int theRow1, int theRow2) = 0;
  virtual void SwapColumns(int theColumn1, int theColumn2) = 0;
};

#endif // SALOMEDSClient_AttributeTable_HeaderFile
