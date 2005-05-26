//  File   : SALOMEDS_AttributeTableOfInteger.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeTableOfInteger_HeaderFile
#define SALOMEDS_AttributeTableOfInteger_HeaderFile

#include "SALOMEDSClient_AttributeTableOfInteger.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeTableOfInteger.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include <vector>

class SALOMEDS_AttributeTableOfInteger: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeTableOfInteger
{
public:  
  SALOMEDS_AttributeTableOfInteger(const Handle(SALOMEDSImpl_AttributeTableOfInteger)& theAttr);
  SALOMEDS_AttributeTableOfInteger(SALOMEDS::AttributeTableOfInteger_ptr theAttr);
  ~SALOMEDS_AttributeTableOfInteger();

  virtual void SetTitle(const std::string& theTitle);
  virtual std::string GetTitle();
  virtual void SetRowTitle(int theIndex, const std::string& theTitle);
  virtual void SetRowTitles(const std::vector<std::string>& theTitles);
  virtual std::vector<std::string> GetRowTitles();
  virtual void SetColumnTitle(int theIndex, const std::string& theTitle);
  virtual void SetColumnTitles(const std::vector<std::string>& theTitles);
  virtual std::vector<std::string> GetColumnTitles();

  virtual void SetRowUnit(int theIndex, const std::string& theUnit);
  virtual void SetRowUnits(const std::vector<std::string>& theUnits);
  virtual std::vector<std::string> GetRowUnits();

  virtual int GetNbRows();
  virtual int GetNbColumns();
  virtual void AddRow(const std::vector<int>& theData);
  virtual void SetRow(int theRow, const std::vector<int>& theData);
  virtual std::vector<int> GetRow(int theRow);
  virtual void AddColumn(const std::vector<int>& theData);
  virtual void SetColumn(int theColumn, const std::vector<int>& theData);
  virtual std::vector<int> GetColumn(int theColumn);
  virtual void PutValue(int theValue, int theRow, int theColumn);
  virtual bool HasValue(int theRow, int theColumn);
  virtual int GetValue(int theRow, int theColumn);

  virtual std::vector<int> GetRowSetIndices(int theRow);
  virtual void SetNbColumns(int theNbColumns);    

};

#endif
