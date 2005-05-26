//  File   : SALOMEDS_UseCaseIterator.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_USECASEITERATOR_H__
#define __SALOMEDS_USECASEITERATOR_H__

#include "SALOMEDSClient_UseCaseIterator.hxx"
#include "SALOMEDSClient_SObject.hxx"
#include "SALOMEDSImpl_UseCaseIterator.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class SALOMEDS_UseCaseIterator: public SALOMEDSClient_UseCaseIterator
{
private:
  bool _isLocal;
  Handle(SALOMEDSImpl_UseCaseIterator) _local_impl;
  SALOMEDS::UseCaseIterator_var        _corba_impl;

public:
  
  SALOMEDS_UseCaseIterator(const Handle(SALOMEDSImpl_UseCaseIterator)& theIterator);
  SALOMEDS_UseCaseIterator(SALOMEDS::UseCaseIterator_ptr theIterator);
  ~SALOMEDS_UseCaseIterator();

  virtual void Init(bool theAllLevels);
  virtual bool More();
  virtual void Next();
  virtual _PTR(SObject) Value();
};

#endif
