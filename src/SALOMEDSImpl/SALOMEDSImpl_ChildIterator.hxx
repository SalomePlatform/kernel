//  File   : SALOMEDSImpl_ChildIterator.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSImpl_CHILDITERATOR_H__
#define __SALOMEDSImpl_CHILDITERATOR_H__

//Handle definition
#include <Handle_MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_ChildIterator, MMgt_TShared )

#include "SALOMEDSImpl_SObject.hxx"
#include "TDF_ChildIterator.hxx"
#include "TDF_Label.hxx"

class SALOMEDSImpl_ChildIterator: public MMgt_TShared 
{
private:
  TDF_ChildIterator _it;
  Handle(SALOMEDSImpl_SObject) _so;

public:

  SALOMEDSImpl_ChildIterator(const Handle(SALOMEDSImpl_SObject)& theSO);
  SALOMEDSImpl_ChildIterator(const TDF_Label& theLabel);
  ~SALOMEDSImpl_ChildIterator() {};

  virtual void Init();
  virtual void Init(const TDF_Label& theLabel);
  virtual void InitEx(bool theAllLevels);
  virtual void InitEx(const TDF_Label& theLabel, bool theAllLevels);
  virtual bool More();
  virtual void Next();
  virtual Handle(SALOMEDSImpl_SObject) Value();
  virtual TDF_Label Label();

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_SObject )
};
#endif
