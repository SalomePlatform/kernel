//  File   : SALOMEDSClient_AttributeUserID.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributeUserID_HeaderFile
#define SALOMEDSClient_AttributeUserID_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 
#include <string> 

class SALOMEDSClient_AttributeUserID: public virtual SALOMEDSClient_GenericAttribute
{
public:
  
  virtual std::string Value() = 0;
  virtual void SetValue(const std::string& value) = 0;

};




#endif
