//  File   : SALOMEDSClient_AttributeSelectable.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributeSelectable_HeaderFile
#define SALOMEDSClient_AttributeSelectable_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeSelectable: public virtual SALOMEDSClient_GenericAttribute
{
public:
  
  virtual bool IsSelectable() = 0;
  virtual void SetSelectable(bool value) = 0;

};



#endif
