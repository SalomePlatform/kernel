//  File   : SALOMEDSClient_AttributeExpandable.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDSClient_AttributeExpandable_HeaderFile
#define SALOMEDSClient_AttributeExpandable_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeExpandable: public virtual SALOMEDSClient_GenericAttribute
{
public:  
  virtual bool IsExpandable() = 0;
  virtual void SetExpandable(bool value) = 0;
};



#endif
