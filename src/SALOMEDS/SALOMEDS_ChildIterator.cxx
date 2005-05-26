//  File   : SALOMEDS_ChildIterator.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std; 

#include "SALOMEDS_ChildIterator.hxx"
#include "SALOMEDS_SObject.hxx"

SALOMEDS_ChildIterator::SALOMEDS_ChildIterator(const Handle(SALOMEDSImpl_ChildIterator)& theIterator)
{
  _isLocal = true;
  _local_impl = theIterator;
  _corba_impl = SALOMEDS::ChildIterator::_nil();
}

SALOMEDS_ChildIterator::SALOMEDS_ChildIterator(SALOMEDS::ChildIterator_ptr theIterator)
{
  _isLocal = false;
  _local_impl = NULL;
  _corba_impl = SALOMEDS::ChildIterator::_duplicate(theIterator);
}

SALOMEDS_ChildIterator::~SALOMEDS_ChildIterator()
{
  if(!_isLocal) _corba_impl->Destroy(); 
}

void SALOMEDS_ChildIterator::Init()
{
  if(_isLocal) _local_impl->Init();
  else _corba_impl->Init();
}

void SALOMEDS_ChildIterator::InitEx(bool theAllLevels)
{
  if(_isLocal) _local_impl->InitEx(theAllLevels);
  else _corba_impl->InitEx(theAllLevels);
}

bool SALOMEDS_ChildIterator::More()
{
  bool ret;
  if(_isLocal) ret = _local_impl->More();
  else ret = _corba_impl->More();
  return ret;
}

void SALOMEDS_ChildIterator::Next() 
{
  if(_isLocal) _local_impl->Next();
  else _corba_impl->Next();
}

_PTR(SObject) SALOMEDS_ChildIterator::Value()
{
  SALOMEDSClient_SObject* aSO;
  if(_isLocal) aSO = new SALOMEDS_SObject(_local_impl->Value());
  else aSO = new SALOMEDS_SObject(_corba_impl->Value());
  return _PTR(SObject)(aSO);
}
