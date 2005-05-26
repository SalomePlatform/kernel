//  File   : SALOMEDSImpl_ChildIterator.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDSImpl_ChildIterator.hxx"
#include "SALOMEDSImpl_Study.hxx"

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_ChildIterator, MMgt_TShared )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_ChildIterator, MMgt_TShared )

SALOMEDSImpl_ChildIterator::SALOMEDSImpl_ChildIterator(const Handle(SALOMEDSImpl_SObject)& theSO)
{
  _so  = theSO;
  _it = TDF_ChildIterator(_so->GetLabel());
}

SALOMEDSImpl_ChildIterator::SALOMEDSImpl_ChildIterator(const TDF_Label& theLabel)
{
  _so  = SALOMEDSImpl_Study::SObject(theLabel);
  _it = TDF_ChildIterator(theLabel);
}

void SALOMEDSImpl_ChildIterator::Init()
{
  _it.Initialize(_so->GetLabel(), Standard_False);
}

void SALOMEDSImpl_ChildIterator::Init(const TDF_Label& theLabel)
{
  _it.Initialize(theLabel, Standard_False);
}

void SALOMEDSImpl_ChildIterator::InitEx(bool theAllLevels)
{
  _it.Initialize(_so->GetLabel(), theAllLevels);
}

void SALOMEDSImpl_ChildIterator::InitEx(const TDF_Label& theLabel, bool theAllLevels)
{
  _it.Initialize(theLabel, theAllLevels);
}

bool SALOMEDSImpl_ChildIterator::More()
{
  return _it.More();
}

void SALOMEDSImpl_ChildIterator::Next() 
{
  return _it.Next();
}

Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_ChildIterator::Value()
{
  return SALOMEDSImpl_Study::SObject(_it.Value());
}

TDF_Label SALOMEDSImpl_ChildIterator::Label()
{
  return _it.Value();
}
