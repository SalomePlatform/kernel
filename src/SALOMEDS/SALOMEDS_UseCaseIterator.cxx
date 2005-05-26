//  File   : SALOMEDS_UseCaseIterator.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std; 

#include "SALOMEDS_UseCaseIterator.hxx"
#include "SALOMEDS_SObject.hxx"

SALOMEDS_UseCaseIterator::SALOMEDS_UseCaseIterator(const Handle(SALOMEDSImpl_UseCaseIterator)& theIterator)
{
  _isLocal = true;
  _local_impl = theIterator;
  _corba_impl = SALOMEDS::UseCaseIterator::_nil();
}

SALOMEDS_UseCaseIterator::SALOMEDS_UseCaseIterator(SALOMEDS::UseCaseIterator_ptr theIterator)
{
  _isLocal = false;
  _local_impl = NULL;
  _corba_impl = SALOMEDS::UseCaseIterator::_duplicate(theIterator);
}

SALOMEDS_UseCaseIterator::~SALOMEDS_UseCaseIterator()
{
  if(!_isLocal) _corba_impl->Destroy();    
}

void SALOMEDS_UseCaseIterator::Init(bool theAllLevels)
{
  if(_isLocal) _local_impl->Init(theAllLevels);
  else _corba_impl->Init(theAllLevels);
}

bool SALOMEDS_UseCaseIterator::More()
{
  bool ret;
  if(_isLocal) ret = _local_impl->More();
  else ret = _corba_impl->More();
  return ret;
}

void SALOMEDS_UseCaseIterator::Next()
{
  if(_isLocal) _local_impl->Next();
  else _corba_impl->Next();
}

_PTR(SObject) SALOMEDS_UseCaseIterator::Value()
{
  SALOMEDS_SObject* aSO;
  if(_isLocal) aSO = new SALOMEDS_SObject(_local_impl->Value());
  else aSO = new SALOMEDS_SObject(_corba_impl->Value());
  return _PTR(SObject)(aSO);
}
