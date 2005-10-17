//  File   : SALOMEDS_ChildIterator.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_CHILDITERATOR_H__
#define __SALOMEDS_CHILDITERATOR_H__

#include "SALOMEDSClient.hxx"
#include "SALOMEDSImpl_ChildIterator.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class SALOMEDS_ChildIterator: public SALOMEDSClient_ChildIterator
{
private:
  bool                               _isLocal;
  Handle(SALOMEDSImpl_ChildIterator) _local_impl;
  SALOMEDS::ChildIterator_var        _corba_impl;

public:

  SALOMEDS_ChildIterator(const Handle(SALOMEDSImpl_ChildIterator)& theIterator);
  SALOMEDS_ChildIterator(SALOMEDS::ChildIterator_ptr theIterator);
  ~SALOMEDS_ChildIterator();

  virtual void Init();
  virtual void InitEx(bool theAllLevels);
  virtual bool More();
  virtual void Next();
  virtual _PTR(SObject) Value();
};
#endif
