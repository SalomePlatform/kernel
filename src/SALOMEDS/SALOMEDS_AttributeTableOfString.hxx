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

//  File   : SALOMEDS_AttributeTableOfString.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef SALOMEDS_AttributeTableOfString_HeaderFile
#define SALOMEDS_AttributeTableOfString_HeaderFile

#include "SALOMEDSClient_AttributeTableOfString.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeTableOfString.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include <vector>

class SALOMEDS_AttributeTableOfString: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeTableOfString
{
public:  
  SALOMEDS_AttributeTableOfString(SALOMEDSImpl_AttributeTableOfString* theAttr);
  SALOMEDS_AttributeTableOfString(SALOMEDS::AttributeTableOfString_ptr theAttr);
  ~SALOMEDS_AttributeTableOfString();

  virtual void SetTitle(const std::string& theTitle);
  virtual std::string GetTitle();
  virtual void SetRowTitle(int theIndex, const std::string& theTitle);
  virtual std::string GetRowTitle(int theIndex);
  virtual void SetRowTitles(const std::vector<std::string>& theTitles);
  virtual std::vector<std::string> GetRowTitles();
  virtual void SetColumnTitle(int theIndex, const std::string& theTitle);
  virtual std::string GetColumnTitle(int theIndex);
  virtual void SetColumnTitles(const std::vector<std::string>& theTitles);
  virtual std::vector<std::string> GetColumnTitles();

  virtual void SetRowUnit(int theIndex, const std::string& theUnit);
  virtual std::string GetRowUnit(int theIndex);
  virtual void SetRowUnits(const std::vector<std::string>& theUnits);
  virtual std::vector<std::string> GetRowUnits();

  virtual int GetNbRows();
  virtual int GetNbColumns();
  virtual void AddRow(const std::vector<std::string>& theData);
  virtual void SetRow(int theRow, const std::vector<std::string>& theData);
  virtual std::vector<std::string> GetRow(int theRow);
  virtual void AddColumn(const std::vector<std::string>& theData);
  virtual void SetColumn(int theColumn, const std::vector<std::string>& theData);
  virtual std::vector<std::string> GetColumn(int theColumn);
  virtual void PutValue(const std::string& theValue, int theRow, int theColumn);
  virtual bool HasValue(int theRow, int theColumn);
  virtual std::string GetValue(int theRow, int theColumn);
  virtual void RemoveValue(int theRow, int theColumn);

  virtual std::vector<int> GetRowSetIndices(int theRow);
  virtual void SetNbColumns(int theNbColumns);    

  virtual std::vector<int> SortRow(int theRow, SortOrder theOrder, SortPolicy thePolicy);
  virtual std::vector<int> SortColumn(int theColumn, SortOrder theOrder, SortPolicy thePolicy);
  virtual std::vector<int> SortByRow(int theRow, SortOrder theOrder, SortPolicy thePolicy);
  virtual std::vector<int> SortByColumn(int theColumn, SortOrder theOrder, SortPolicy thePolicy);
  virtual void SwapCells(int theRow1, int theColumn1, int theRow2, int theColumn2);
  virtual void SwapRows(int theRow1, int theRow2);
  virtual void SwapColumns(int theColumn1, int theColumn2);
};

#endif
