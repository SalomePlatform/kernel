//  File   : SALOMEDSClient_AttributeInteger.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributeInteger_HeaderFile
#define SALOMEDSClient_AttributeInteger_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeInteger: public virtual SALOMEDSClient_GenericAttribute
{
public:
  
  virtual int Value() = 0;
  virtual void SetValue(int value) = 0;
};



#endif
