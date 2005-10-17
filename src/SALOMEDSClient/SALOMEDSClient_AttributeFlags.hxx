//  File   : SALOMEDSClient_AttributeFlags.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDSClient_AttributeFlags_HeaderFile
#define SALOMEDSClient_AttributeFlags_HeaderFile

#include "SALOMEDSClient_definitions.hxx" 
#include "SALOMEDSClient_GenericAttribute.hxx" 

class SALOMEDSClient_AttributeFlags: public virtual SALOMEDSClient_GenericAttribute
{
public:

  virtual int   GetFlags() = 0;
  virtual void  SetFlags(int theFlags) = 0;

  virtual bool  Get(int theFlag) = 0;
  virtual void  Set(int theFlag, bool theValue) = 0;
  
};

#endif
