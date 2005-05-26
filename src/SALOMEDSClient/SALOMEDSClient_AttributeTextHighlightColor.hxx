//  File   : SALOMEDSClient_AttributeTextHighlightColor.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDSClient_AttributeTextHighlightColor_HeaderFile
#define SALOMEDSClient_AttributeTextHighlightColor_HeaderFile

#include <vector>
#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 
#include "SALOMEDSClient_definitions.hxx"

class SALOMEDSClient_AttributeTextHighlightColor: public virtual SALOMEDSClient_GenericAttribute
{
public:
  virtual STextColor TextHighlightColor() = 0;
  virtual void SetTextHighlightColor(STextColor value) = 0;
};


#endif
