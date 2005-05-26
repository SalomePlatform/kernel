//  File   : SALOMEDSImpl_UseCaseIterator.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_USECASEITERATOR_H__
#define __SALOMEDSIMPL_USECASEITERATOR_H__

//Handle definition
#include <Handle_MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_UseCaseIterator, MMgt_TShared )

#include "SALOMEDSImpl_SObject.hxx"

// Cascade headers
#include <TDF_ChildIterator.hxx>
#include <SALOMEDSImpl_ChildNodeIterator.hxx>
#include <Standard_GUID.hxx>

class SALOMEDSImpl_UseCaseIterator : public MMgt_TShared 
{

private:
  Standard_GUID                              _guid;
  bool                                       _levels;
  Handle(SALOMEDSImpl_AttributeTreeNode)     _node;
  SALOMEDSImpl_ChildNodeIterator             _it;

public:

  //! standard constructor  
  SALOMEDSImpl_UseCaseIterator(const TDF_Label& theLabel, 
			       const Standard_GUID& theGUID, 
			       const bool allLevels);
  
  //! standard destructor
  ~SALOMEDSImpl_UseCaseIterator();
  
  virtual void Init(bool);
  virtual bool More();
  virtual void Next();
  virtual Handle(SALOMEDSImpl_SObject) Value();

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_UseCaseIterator )
};
#endif
