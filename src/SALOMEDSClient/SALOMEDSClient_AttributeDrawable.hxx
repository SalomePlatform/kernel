//  File   : SALOMEDSClient_AttributeDrawable.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributeDrawable_HeaderFile
#define SALOMEDSClient_AttributeDrawable_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeDrawable: public virtual SALOMEDSClient_GenericAttribute
{
public: 
  virtual bool IsDrawable() = 0;
  virtual void SetDrawable(bool value) = 0;
};

#endif
