//  File   : SALOMEDSClient_AttributeOpened.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDSClient_AttributeOpened_HeaderFile
#define SALOMEDSClient_AttributeOpened_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeOpened: public virtual SALOMEDSClient_GenericAttribute
{
public:
  
  virtual bool IsOpened() = 0;
  virtual void SetOpened(bool value) = 0;
};



#endif
