//  File   : SALOMEDSClient_AttributeIOR.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributeIOR_HeaderFile
#define SALOMEDSClient_AttributeIOR_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 
#include <string>

class SALOMEDSClient_AttributeIOR: public virtual SALOMEDSClient_GenericAttribute
{
public:
  
  virtual std::string Value() = 0;
  virtual void SetValue(const std::string& value) = 0;
  
};


#endif
