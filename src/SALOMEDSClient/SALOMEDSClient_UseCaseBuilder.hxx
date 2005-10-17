//  File   : SALOMEDSClient_UseCaseBuilder.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSClient_USECaseBuilder_H__
#define __SALOMEDSClient_USECaseBuilder_H__

#include <string>
#include "SALOMEDSClient_definitions.hxx"
#include "SALOMEDSClient_UseCaseIterator.hxx"
#include "SALOMEDSClient_SObject.hxx"

class SALOMEDSClient_UseCaseBuilder
{
public:
  virtual ~SALOMEDSClient_UseCaseBuilder() {}
  
  virtual bool Append(const _PTR(SObject)& theObject) = 0;
  virtual bool Remove(const _PTR(SObject)& theObject) = 0;
  virtual bool AppendTo(const _PTR(SObject)& theFather, _PTR(SObject) theObject) = 0;
  virtual bool InsertBefore(const _PTR(SObject)& theFirst, _PTR(SObject) theNext) = 0;
  virtual bool SetCurrentObject(const _PTR(SObject)& theObject) = 0;
  virtual bool SetRootCurrent() = 0;
  virtual bool HasChildren(const _PTR(SObject)& theObject) = 0;
  virtual bool IsUseCase(const _PTR(SObject)& theObject) = 0;
  virtual bool SetName(const std::string& theName) = 0;
  virtual _PTR(SObject) GetCurrentObject() = 0;
  virtual std::string GetName() = 0;
  virtual _PTR(SObject) AddUseCase(const std::string& theName) = 0;
  virtual _PTR(UseCaseIterator) GetUseCaseIterator(const _PTR(SObject)& anObject) = 0;

};


#endif
