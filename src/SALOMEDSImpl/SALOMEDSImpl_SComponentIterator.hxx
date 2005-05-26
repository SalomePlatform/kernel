//  File   : SALOMEDSImpl_SComponentIterator_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_SCOMPONENTITERATOR_H__
#define __SALOMEDSIMPL_SCOMPONENTITERATOR_H__


//SALOMEDSImpl headers
#include "SALOMEDSImpl_SComponent.hxx"

// Cascade headers
#include <TDocStd_Document.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_Label.hxx>
#include <stdio.h>

class SALOMEDSImpl_SComponentIterator
{
private:

  TDF_ChildIterator        _it;
  TDF_Label                _lab;

public:
  
  SALOMEDSImpl_SComponentIterator(const SALOMEDSImpl_SComponentIterator& theIterator) 
  {
    _it = theIterator._it;
    _lab = theIterator._lab;
  } 

  SALOMEDSImpl_SComponentIterator() {};
  
  SALOMEDSImpl_SComponentIterator(const Handle(TDocStd_Document)& theDocument);
 
  ~SALOMEDSImpl_SComponentIterator() {};
  
  virtual void Init();
  virtual bool More();
  virtual void Next();
  virtual Handle(SALOMEDSImpl_SComponent) Value();  
};
#endif
