//  File   : SALOMEDSClient_AttributeTableOfReal.hxx
//  Author : Michael Ponikarov
//  Module : SALOME

#ifndef SALOMEDSClient_AttributeTableOfReal_HeaderFile
#define SALOMEDSClient_AttributeTableOfReal_HeaderFile

#include <vector>
#include <string>
#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeTableOfReal: public virtual SALOMEDSClient_GenericAttribute
{
    
public:
  
  virtual void SetTitle(const std::string& theTitle) = 0;
  virtual std::string GetTitle() = 0;
  virtual void SetRowTitle(int theIndex, const std::string& theTitle) = 0;
  virtual void SetRowTitles(const std::vector<std::string>& theTitles) = 0;
  virtual std::vector<std::string> GetRowTitles() = 0;
  virtual void SetColumnTitle(int theIndex, const std::string& theTitle) = 0;
  virtual void SetColumnTitles(const std::vector<std::string>& theTitles) = 0;
  virtual std::vector<std::string> GetColumnTitles() = 0;
  virtual void SetRowUnit(int theIndex, const std::string& theUnit) = 0;
  virtual void SetRowUnits(const std::vector<std::string>& theUnits) = 0;
  virtual std::vector<std::string> GetRowUnits() = 0;

  virtual int GetNbRows() = 0;
  virtual int GetNbColumns() = 0;
  virtual void AddRow(const std::vector<double>& theData) = 0;
  virtual void SetRow(int theRow, const std::vector<double>& theData) = 0;
  virtual std::vector<double> GetRow(int theRow) = 0; 
  virtual void AddColumn(const std::vector<double>& theData) = 0;
  virtual void SetColumn(int theColumn, const std::vector<double>& theData) = 0;
  virtual std::vector<double> GetColumn(int theColumn) = 0;
  virtual void PutValue(double theValue, int theRow, int theColumn) = 0;
  virtual bool HasValue(int theRow, int theColumn) = 0;
  virtual double GetValue(int theRow, int theColumn) = 0;

  virtual std::vector<int> GetRowSetIndices(int theRow) = 0;
  virtual void SetNbColumns(int theNbColumns) = 0;

};

#endif
