//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_AttributeTableOfInteger_i.hxx
//  Author : Michael Ponikarov
//  Module : SALOME
//  $Header$

#ifndef SALOMEDS_AttributeTableOfInteger_i_HeaderFile
#define SALOMEDS_AttributeTableOfInteger_i_HeaderFile

// IDL headers
#include "SALOMEDS_TableOfIntegerAttribute.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeTableOfInteger_i: public virtual POA_SALOMEDS::AttributeTableOfInteger,
  public virtual SALOMEDS_GenericAttribute_i {
    
public:
  
  SALOMEDS_AttributeTableOfInteger_i(const Handle(SALOMEDS_TableOfIntegerAttribute)& theTableOfIntegerAttr, 
				     CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theTableOfIntegerAttr;
  };
  ~SALOMEDS_AttributeTableOfInteger_i() {};

  virtual void SetTitle(const char* theTitle);
  virtual char* GetTitle();
  virtual void SetRowTitle(CORBA::Long theIndex, const char* theTitle)
    throw (SALOMEDS::AttributeTableOfInteger::IncorrectIndex);
  virtual void SetRowTitles(const SALOMEDS::StringSeq& theTitles)
    throw (SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength);
  virtual SALOMEDS::StringSeq* GetRowTitles();
  virtual void SetColumnTitle(CORBA::Long theIndex, const char* theTitle)
    throw (SALOMEDS::AttributeTableOfInteger::IncorrectIndex);
  virtual void SetColumnTitles(const SALOMEDS::StringSeq& theTitles)
    throw (SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength);
  virtual SALOMEDS::StringSeq* GetColumnTitles();

  virtual void SetRowUnit(CORBA::Long theIndex, const char* theUnit)
    throw (SALOMEDS::AttributeTableOfInteger::IncorrectIndex);
  virtual void SetRowUnits(const SALOMEDS::StringSeq& theUnits)
    throw (SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength);
  virtual SALOMEDS::StringSeq* GetRowUnits();

  virtual CORBA::Long GetNbRows();
  virtual CORBA::Long GetNbColumns();
  virtual void AddRow(const SALOMEDS::LongSeq& theData)
    throw (SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength);
  virtual void SetRow(CORBA::Long theRow, const SALOMEDS::LongSeq& theData)
    throw (SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength, SALOMEDS::AttributeTableOfInteger::IncorrectIndex);
  virtual SALOMEDS::LongSeq* GetRow(CORBA::Long theRow)
    throw (SALOMEDS::AttributeTableOfInteger::IncorrectIndex);
  virtual void AddColumn(const SALOMEDS::LongSeq& theData)
    throw (SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength);
  virtual void SetColumn(CORBA::Long theColumn, const SALOMEDS::LongSeq& theData)
    throw (SALOMEDS::AttributeTableOfInteger::IncorrectArgumentLength, SALOMEDS::AttributeTableOfInteger::IncorrectIndex);
  virtual SALOMEDS::LongSeq* GetColumn(CORBA::Long theColumn)
    throw (SALOMEDS::AttributeTableOfInteger::IncorrectIndex);
  virtual void PutValue(CORBA::Long theValue, CORBA::Long theRow, CORBA::Long theColumn)
    throw (SALOMEDS::AttributeTableOfInteger::IncorrectIndex);
  virtual CORBA::Boolean HasValue(CORBA::Long theRow, CORBA::Long theColumn);
  virtual CORBA::Long GetValue(CORBA::Long theRow, CORBA::Long theColumn)
    throw (SALOMEDS::AttributeTableOfInteger::IncorrectIndex);

  virtual SALOMEDS::LongSeq* GetRowSetIndices(CORBA::Long theRow);
  virtual void SetNbColumns(CORBA::Long theNbColumns);

  virtual bool ReadFromFile(const SALOMEDS::TMPFile& theStream);
  virtual SALOMEDS::TMPFile* SaveToFile();

  virtual char* Store();
  virtual void Restore(const char*);

};

#endif
