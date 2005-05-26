//  File   : SALOMEDSClient_AttributeTextColor.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributeTextColor_HeaderFile
#define SALOMEDSClient_AttributeTextColor_HeaderFile

#include <vector>
#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 
#include "SALOMEDSClient_definitions.hxx"

class SALOMEDSClient_AttributeTextColor: public virtual SALOMEDSClient_GenericAttribute
{
public:

  virtual STextColor TextColor() = 0;
  virtual void SetTextColor(STextColor value) = 0;

};


#endif
