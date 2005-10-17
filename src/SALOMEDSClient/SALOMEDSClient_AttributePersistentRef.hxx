//  File   : SALOMEDSClient_AttributePersistentRef.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributePersistentRef_HeaderFile
#define SALOMEDSClient_AttributePersistentRef_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 
#include <string>

class SALOMEDSClient_AttributePersistentRef: public virtual SALOMEDSClient_GenericAttribute
{
public:
  
  virtual std::string Value() = 0;
  virtual void SetValue(const std::string& value) = 0;

};



#endif
