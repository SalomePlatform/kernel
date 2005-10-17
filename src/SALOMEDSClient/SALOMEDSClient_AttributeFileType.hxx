//  File   : SALOMEDSClient_AttributeFileType.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributeFileType_HeaderFile
#define SALOMEDSClient_AttributeFileType_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 
#include <string>

class SALOMEDSClient_AttributeFileType: public virtual SALOMEDSClient_GenericAttribute
{
public:
  virtual std::string Value() = 0;
  virtual void SetValue(const std::string& value) = 0;
  
};



#endif
