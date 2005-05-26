//  File   : SALOMEDSClient_AttributeLocalID.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDSClient_AttributeLocalID_HeaderFile
#define SALOMEDSClient_AttributeLocalID_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeLocalID: public virtual SALOMEDSClient_GenericAttribute
{
public:
  virtual int Value() = 0;
  virtual void SetValue(int value) = 0;

};



#endif
