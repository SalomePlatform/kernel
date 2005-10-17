//  File   : SALOMEDSClient_AttributeReal.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributeReal_HeaderFile
#define SALOMEDSClient_AttributeReal_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeReal: public virtual SALOMEDSClient_GenericAttribute
{
public:
  
  virtual double Value() = 0;
  virtual void SetValue(double value) = 0;
};


#endif
