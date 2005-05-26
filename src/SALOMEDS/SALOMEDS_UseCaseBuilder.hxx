//  File   : SALOMEDS_UseCaseBuilder.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_USECaseBuilder_H__
#define __SALOMEDS_USECaseBuilder_H__

#include "SALOMEDSClient.hxx"
#include "SALOMEDSImpl_UseCaseBuilder.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class SALOMEDS_UseCaseBuilder: public SALOMEDSClient_UseCaseBuilder
{

private: 
  bool _isLocal;
  Handle(SALOMEDSImpl_UseCaseBuilder) _local_impl;
  SALOMEDS::UseCaseBuilder_var        _corba_impl;

public:
  
  SALOMEDS_UseCaseBuilder(const Handle(SALOMEDSImpl_UseCaseBuilder)& theBuilder);
  SALOMEDS_UseCaseBuilder(SALOMEDS::UseCaseBuilder_ptr theBuilder);
  ~SALOMEDS_UseCaseBuilder();

  virtual bool Append(const _PTR(SObject)& theObject);
  virtual bool Remove(const _PTR(SObject)& theObject);
  virtual bool AppendTo(const _PTR(SObject)& theFather, _PTR(SObject) theObject);
  virtual bool InsertBefore(const _PTR(SObject)& theFirst, _PTR(SObject) theNext);
  virtual bool  SetCurrentObject(const _PTR(SObject)& theObject);
  virtual bool SetRootCurrent();
  virtual bool  HasChildren(const _PTR(SObject)& theObject);
  virtual bool  IsUseCase(const _PTR(SObject)& theObject);
  virtual bool SetName(const std::string& theName);
  virtual _PTR(SObject) GetCurrentObject();
  virtual std::string GetName();
  virtual _PTR(SObject) AddUseCase(const std::string& theName);
  virtual _PTR(UseCaseIterator) GetUseCaseIterator(const _PTR(SObject)& anObject);

};
#endif
