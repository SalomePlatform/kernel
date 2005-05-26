//  File   : SALOMEDS_AttributeTableOfReal.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeTableOfReal_HeaderFile
#define SALOMEDS_AttributeTableOfReal_HeaderFile

#include "SALOMEDSClient_AttributeTableOfReal.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeTableOfReal.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include <vector>

class SALOMEDS_AttributeTableOfReal: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeTableOfReal
{
public:  
  SALOMEDS_AttributeTableOfReal(const Handle(SALOMEDSImpl_AttributeTableOfReal)& theAttr);
  SALOMEDS_AttributeTableOfReal(SALOMEDS::AttributeTableOfReal_ptr theAttr);
  ~SALOMEDS_AttributeTableOfReal();

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
  virtual void AddRow(const std::vector<double>& theData);
  virtual void SetRow(int theRow, const std::vector<double>& theData);
  virtual std::vector<double> GetRow(int theRow);
  virtual void AddColumn(const std::vector<double>& theData);
  virtual void SetColumn(int theColumn, const std::vector<double>& theData);
  virtual std::vector<double> GetColumn(int theColumn);
  virtual void PutValue(double theValue, int theRow, int theColumn);
  virtual bool HasValue(int theRow, int theColumn);
  virtual double GetValue(int theRow, int theColumn);

  virtual std::vector<int> GetRowSetIndices(int theRow);
  virtual void SetNbColumns(int theNbColumns);    

};

#endif
