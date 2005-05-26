//  File   : SALOMEDS_SComponentIterator.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_SCOMPONENTITERATOR_H__
#define __SALOMEDS_SCOMPONENTITERATOR_H__

#include "SALOMEDSClient.hxx"
#include "SALOMEDSImpl_SComponentIterator.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class SALOMEDS_SComponentIterator: public SALOMEDSClient_SComponentIterator
{
public:
  SALOMEDS_SComponentIterator(const SALOMEDSImpl_SComponentIterator& theIterator);
  SALOMEDS_SComponentIterator(SALOMEDS::SComponentIterator_ptr theIterator);
  ~SALOMEDS_SComponentIterator();

private:
  bool                                    _isLocal;
  SALOMEDSImpl_SComponentIterator         _local_impl;
  SALOMEDS::SComponentIterator_var        _corba_impl;

public:
  virtual void Init();
  virtual bool More();
  virtual void Next();
  virtual _PTR(SComponent) Value();  
};
#endif
