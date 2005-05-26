//  File   : SALOMEDSClient_AttributePixMap.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributePixMap_HeaderFile
#define SALOMEDSClient_AttributePixMap_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 
#include <string> 

class SALOMEDSClient_AttributePixMap: public virtual SALOMEDSClient_GenericAttribute
{
public:

  virtual bool HasPixMap() = 0;
  virtual std::string GetPixMap() = 0;
  virtual void SetPixMap(const std::string& value) = 0;

};


#endif
