//  File   : SALOMEDSClient_AttributeExternalFileDef.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributeExternalFileDef_HeaderFile
#define SALOMEDSClient_AttributeExternalFileDef_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 
#include <string>

class SALOMEDSClient_AttributeExternalFileDef: public virtual SALOMEDSClient_GenericAttribute
{
public:
  virtual std::string Value() = 0;
  virtual void SetValue(const std::string& value) = 0;
};



#endif
