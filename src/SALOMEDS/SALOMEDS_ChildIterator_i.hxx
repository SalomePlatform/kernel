//  File      : SALOMEDS_ChildIterator_i.hxx
//  Created   : Wed Nov 28 16:20:12 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#ifndef __SALOMEDS_CHILDITERATOR_I_H__
#define __SALOMEDS_CHILDITERATOR_I_H__

// std C++ headers
#include <iostream.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)



// Cascade headers
#include <TDF_ChildIterator.hxx>
#include <TDF_Label.hxx>
#include <stdio.h>

class SALOMEDS_ChildIterator_i: public POA_SALOMEDS::ChildIterator,
				public PortableServer::RefCountServantBase {
private:
  CORBA::ORB_ptr           _orb;
  TDF_Label                _lab;
  TDF_ChildIterator        _it;
public:

  //! standard constructor  
  SALOMEDS_ChildIterator_i(TDF_Label lab,CORBA::ORB_ptr);
  
  //! standard destructor
  ~SALOMEDS_ChildIterator_i();
  
  virtual void Init();
  virtual void InitEx(CORBA::Boolean);
  virtual CORBA::Boolean More();
  virtual void Next();
  virtual SALOMEDS::SObject_ptr Value();
};
#endif
