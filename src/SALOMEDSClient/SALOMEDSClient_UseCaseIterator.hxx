//  File   : SALOMEDSClient_UseCaseIterator.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSClient_USECASEITERATOR_H__
#define __SALOMEDSClient_USECASEITERATOR_H__

#include "SALOMEDSClient_definitions.hxx"
#include "SALOMEDSClient_SObject.hxx"

class SALOMEDSClient_UseCaseIterator
{

public:
  virtual ~SALOMEDSClient_UseCaseIterator() {}

  virtual void Init(bool) = 0;
  virtual bool More() = 0;
  virtual void Next() = 0;
  virtual _PTR(SObject) Value() = 0;
};

#endif
