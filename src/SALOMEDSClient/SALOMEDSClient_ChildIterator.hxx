//  File   : SALOMEDSClient_ChildIterator.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSClient_CHILDITERATOR_H__
#define __SALOMEDSClient_CHILDITERATOR_H__

#include "SALOMEDSClient_definitions.hxx"
#include "SALOMEDSClient_SObject.hxx"

class SALOMEDSClient_ChildIterator
{
public:
  virtual ~SALOMEDSClient_ChildIterator() {}

  virtual void Init() = 0;
  virtual void InitEx(bool) = 0;
  virtual bool More() = 0;
  virtual void Next() = 0;
  virtual _PTR(SObject) Value() = 0;
};
#endif
