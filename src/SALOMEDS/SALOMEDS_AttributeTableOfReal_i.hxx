//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_AttributeTableOfReal_i.hxx
//  Author : Michael Ponikarov
//  Module : SALOME
//  $Header$

#ifndef SALOMEDS_AttributeTableOfReal_i_HeaderFile
#define SALOMEDS_AttributeTableOfReal_i_HeaderFile

#include "SALOMEDS_TableOfRealAttribute.hxx"
#include "SALOMEDS_GenericAttribute_i.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

DEFINE_DERIVED_ATTR(AttributeTableOfReal,SALOMEDS_TableOfRealAttribute,true);

class SALOMEDS_AttributeTableOfReal_i: 
  public virtual POA_SALOMEDS::AttributeTableOfReal,
  public virtual SALOMEDS_TAttributeTableOfReal_i 
{
  DEFINE_DERIVED_ATTR_METH_DEFAULT(AttributeTableOfReal,SALOMEDS_TableOfRealAttribute);
public:
  virtual void SetTitle(const char* theTitle);
  virtual char* GetTitle();
  virtual void SetRowTitle(CORBA::Long theIndex, const char* theTitle)
    throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex);
  virtual void SetRowTitles(const SALOMEDS::StringSeq& theTitles)
    throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength);
  virtual SALOMEDS::StringSeq* GetRowTitles();
  virtual void SetColumnTitle(CORBA::Long theIndex, const char* theTitle)
    throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex);
  virtual void SetColumnTitles(const SALOMEDS::StringSeq& theTitles)
    throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength);
  virtual SALOMEDS::StringSeq* GetColumnTitles();

  virtual void SetRowUnit(CORBA::Long theIndex, const char* theUnit)
    throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex);
  virtual void SetRowUnits(const SALOMEDS::StringSeq& theUnits)
    throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength);
  virtual SALOMEDS::StringSeq* GetRowUnits();

  virtual CORBA::Long GetNbRows();
  virtual CORBA::Long GetNbColumns();
  virtual void AddRow(const SALOMEDS::DoubleSeq& theData)
    throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength);
  virtual void SetRow(CORBA::Long theRow, const SALOMEDS::DoubleSeq& theData)
    throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength, SALOMEDS::AttributeTableOfReal::IncorrectIndex);
  virtual SALOMEDS::DoubleSeq* GetRow(CORBA::Long theRow)
    throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex);
  virtual void AddColumn(const SALOMEDS::DoubleSeq& theData)
    throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength);
  virtual void SetColumn(CORBA::Long theColumn, const SALOMEDS::DoubleSeq& theData)
    throw (SALOMEDS::AttributeTableOfReal::IncorrectArgumentLength, SALOMEDS::AttributeTableOfReal::IncorrectIndex);
  virtual SALOMEDS::DoubleSeq* GetColumn(CORBA::Long theColumn)
    throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex);
  virtual void PutValue(CORBA::Double theValue, CORBA::Long theRow, CORBA::Long theColumn)
    throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex);
  virtual CORBA::Boolean HasValue(CORBA::Long theRow, CORBA::Long theColumn);
  virtual CORBA::Double GetValue(CORBA::Long theRow, CORBA::Long theColumn)
    throw (SALOMEDS::AttributeTableOfReal::IncorrectIndex);

  virtual SALOMEDS::LongSeq* GetRowSetIndices(CORBA::Long theRow);
  virtual void SetNbColumns(CORBA::Long theNbColumns);

  virtual bool ReadFromFile(const SALOMEDS::TMPFile& theStream);
  virtual SALOMEDS::TMPFile* SaveToFile();

  virtual char* Store();
  virtual void Restore(const char*);

};

#endif
