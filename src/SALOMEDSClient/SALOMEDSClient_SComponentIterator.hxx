//  File   : SALOMEDSClient_SComponentIterator.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSClient_SCOMPONENTITERATOR_H__
#define __SALOMEDSClient_SCOMPONENTITERATOR_H__

#include "SALOMEDSClient_definitions.hxx"
#include "SALOMEDSClient_SComponent.hxx"

class SALOMEDSClient_SComponentIterator
{
public:
  virtual ~SALOMEDSClient_SComponentIterator() {}

  virtual void Init() = 0;
  virtual bool More() = 0;
  virtual void Next() = 0;
  virtual _PTR(SComponent) Value() = 0;  
};

#endif
