//  File      : SALOMEDS_SComponentIterator_i.hxx
//  Created   : Wed Nov 28 16:24:22 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#ifndef __SALOMEDS_SCOMPONENTITERATOR_I_H__
#define __SALOMEDS_SCOMPONENTITERATOR_I_H__

// std C++ headers
#include <iostream.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

//SALOMEDS headers
#include "SALOMEDS_SComponent_i.hxx"

// Cascade headers
#include <TDocStd_Document.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_Label.hxx>
#include <stdio.h>

class SALOMEDS_SComponentIterator_i:public POA_SALOMEDS::SComponentIterator,
				    public PortableServer::RefCountServantBase {

private:

  CORBA::ORB_ptr           _orb;
  TDF_ChildIterator        _it;
  TDF_Label                _lab;

public:
  
  SALOMEDS_SComponentIterator_i(const Handle(TDocStd_Document) adoc, CORBA::ORB_ptr);
  
  ~SALOMEDS_SComponentIterator_i();
  
  virtual void Init();
  virtual CORBA::Boolean More();
  virtual void Next();
  virtual SALOMEDS::SComponent_ptr Value();  
};
#endif
