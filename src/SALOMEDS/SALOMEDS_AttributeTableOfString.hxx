//  File   : SALOMEDS_AttributeTableOfString.hxx
//  Author : Sergey RUIN
//  Module : SALOME

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
  SALOMEDS_AttributeTableOfString(const Handle(SALOMEDSImpl_AttributeTableOfString)& theAttr);
  SALOMEDS_AttributeTableOfString(SALOMEDS::AttributeTableOfString_ptr theAttr);
  ~SALOMEDS_AttributeTableOfString();

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
  virtual void AddRow(const std::vector<std::string>& theData);
  virtual void SetRow(int theRow, const std::vector<std::string>& theData);
  virtual std::vector<std::string> GetRow(int theRow);
  virtual void AddColumn(const std::vector<std::string>& theData);
  virtual void SetColumn(int theColumn, const std::vector<std::string>& theData);
  virtual std::vector<std::string> GetColumn(int theColumn);
  virtual void PutValue(const std::string& theValue, int theRow, int theColumn);
  virtual bool HasValue(int theRow, int theColumn);
  virtual std::string GetValue(int theRow, int theColumn);

  virtual std::vector<int> GetRowSetIndices(int theRow);
  virtual void SetNbColumns(int theNbColumns);    

};

#endif
