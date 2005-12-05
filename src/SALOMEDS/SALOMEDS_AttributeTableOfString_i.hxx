// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
//  File   : SALOMEDS_AttributeTableOfString_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeTableOfString_i_HeaderFile
#define SALOMEDS_AttributeTableOfString_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeTableOfString.hxx"

class SALOMEDS_AttributeTableOfString_i: public virtual POA_SALOMEDS::AttributeTableOfString,
  public virtual SALOMEDS_GenericAttribute_i {
    
public:
  
  SALOMEDS_AttributeTableOfString_i(const Handle(SALOMEDSImpl_AttributeTableOfString)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributeTableOfString_i() {};

  virtual void SetTitle(const char* theTitle);
  virtual char* GetTitle();
  virtual void SetRowTitle(CORBA::Long theIndex, const char* theTitle)
    throw (SALOMEDS::AttributeTableOfString::IncorrectIndex);
  virtual void SetRowTitles(const SALOMEDS::StringSeq& theTitles)
    throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength);
  virtual SALOMEDS::StringSeq* GetRowTitles();
  virtual void SetColumnTitle(CORBA::Long theIndex, const char* theTitle)
    throw (SALOMEDS::AttributeTableOfString::IncorrectIndex);
  virtual void SetColumnTitles(const SALOMEDS::StringSeq& theTitles)
    throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength);
  virtual SALOMEDS::StringSeq* GetColumnTitles();

  virtual void SetRowUnit(CORBA::Long theIndex, const char* theUnit)
    throw (SALOMEDS::AttributeTableOfString::IncorrectIndex);
  virtual void SetRowUnits(const SALOMEDS::StringSeq& theUnits)
    throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength);
  virtual SALOMEDS::StringSeq* GetRowUnits();

  virtual CORBA::Long GetNbRows();
  virtual CORBA::Long GetNbColumns();
  virtual void AddRow(const SALOMEDS::StringSeq& theData)
    throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength);
  virtual void SetRow(CORBA::Long theRow, const SALOMEDS::StringSeq& theData)
    throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength, SALOMEDS::AttributeTableOfString::IncorrectIndex);
  virtual SALOMEDS::StringSeq* GetRow(CORBA::Long theRow)
    throw (SALOMEDS::AttributeTableOfString::IncorrectIndex);
  virtual void AddColumn(const SALOMEDS::StringSeq& theData)
    throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength);
  virtual void SetColumn(CORBA::Long theColumn, const SALOMEDS::StringSeq& theData)
    throw (SALOMEDS::AttributeTableOfString::IncorrectArgumentLength, SALOMEDS::AttributeTableOfString::IncorrectIndex);
  virtual SALOMEDS::StringSeq* GetColumn(CORBA::Long theColumn)
    throw (SALOMEDS::AttributeTableOfString::IncorrectIndex);
  virtual void PutValue(const char* theValue, CORBA::Long theRow, CORBA::Long theColumn)
    throw (SALOMEDS::AttributeTableOfString::IncorrectIndex);
  virtual CORBA::Boolean HasValue(CORBA::Long theRow, CORBA::Long theColumn);
  virtual char* GetValue(CORBA::Long theRow, CORBA::Long theColumn)
    throw (SALOMEDS::AttributeTableOfString::IncorrectIndex);

  virtual SALOMEDS::LongSeq* GetRowSetIndices(CORBA::Long theRow);
  virtual void SetNbColumns(CORBA::Long theNbColumns);

  virtual bool ReadFromFile(const SALOMEDS::TMPFile& theStream);
  virtual SALOMEDS::TMPFile* SaveToFile();

};

#endif
