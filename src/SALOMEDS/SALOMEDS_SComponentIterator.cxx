//  File   : SALOMEDS_SComponentIterator.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_SComponentIterator.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDS_SComponent.hxx"

SALOMEDS_SComponentIterator::SALOMEDS_SComponentIterator(const SALOMEDSImpl_SComponentIterator& theIterator)
:_local_impl(theIterator)
{
  _isLocal = true;
  _corba_impl = SALOMEDS::SComponentIterator::_nil();
}

SALOMEDS_SComponentIterator::SALOMEDS_SComponentIterator(SALOMEDS::SComponentIterator_ptr theIterator)
{
  _isLocal = false;
  _corba_impl = SALOMEDS::SComponentIterator::_duplicate(theIterator);
}

SALOMEDS_SComponentIterator::~SALOMEDS_SComponentIterator()
{
  if(!_isLocal) _corba_impl->Destroy(); 
}

void SALOMEDS_SComponentIterator::Init()
{
  if(_isLocal) _local_impl.Init();
  else _corba_impl->Init();
}

bool SALOMEDS_SComponentIterator::More()
{
  bool ret;
  if(_isLocal) ret = _local_impl.More();
  else ret = _corba_impl->More();
  return ret;
}
 
void SALOMEDS_SComponentIterator::Next()
{
  if(_isLocal) _local_impl.Next();
  else _corba_impl->Next();
}
 
_PTR(SComponent) SALOMEDS_SComponentIterator::Value()  
{
  SALOMEDSClient_SComponent* aSCO = NULL;
  if(_isLocal) aSCO = new SALOMEDS_SComponent(_local_impl.Value());
  else aSCO = new SALOMEDS_SComponent(_corba_impl->Value());
  return _PTR(SComponent)(aSCO);
}
