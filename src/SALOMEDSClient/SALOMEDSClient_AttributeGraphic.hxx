//  File   : SALOMEDSClient_AttributeFlags.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDSClient_AttributeGraphic_HeaderFile
#define SALOMEDSClient_AttributeGraphic_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeGraphic: public virtual SALOMEDSClient_GenericAttribute
{
public:
  
  virtual void  SetVisibility(int theViewId, bool theValue) = 0;
  virtual bool GetVisibility(int theViewId) = 0;

};

#endif
