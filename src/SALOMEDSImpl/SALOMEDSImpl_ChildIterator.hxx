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

  Standard_EXPORT SALOMEDSImpl_ChildIterator(const Handle(SALOMEDSImpl_SObject)& theSO);
  Standard_EXPORT SALOMEDSImpl_ChildIterator(const TDF_Label& theLabel);
  Standard_EXPORT ~SALOMEDSImpl_ChildIterator() {};

  Standard_EXPORT virtual void Init();
  Standard_EXPORT virtual void Init(const TDF_Label& theLabel);
  Standard_EXPORT virtual void InitEx(bool theAllLevels);
  Standard_EXPORT virtual void InitEx(const TDF_Label& theLabel, bool theAllLevels);
  Standard_EXPORT virtual bool More();
  Standard_EXPORT virtual void Next();
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) Value();
  Standard_EXPORT virtual TDF_Label Label();

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_SObject )
};
#endif
