//  File   : SALOMEDSClient_AttributeName.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDSClient_AttributeName_HeaderFile
#define SALOMEDSClient_AttributeName_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 
#include <string>

class SALOMEDSClient_AttributeName: public virtual SALOMEDSClient_GenericAttribute
{
public:

  virtual std::string Value() = 0;
  virtual void SetValue(const std::string& value) = 0;

};




#endif
