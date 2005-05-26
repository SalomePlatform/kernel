//  File   : SALOMEDS_ChildIterator_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_CHILDITERATOR_I_H__
#define __SALOMEDS_CHILDITERATOR_I_H__

// std C++ headers
#include <iostream.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include <SALOME_GenericObj_i.hh>

// Cascade headers
#include "SALOMEDSImpl_ChildIterator.hxx"
#include <stdio.h>

class SALOMEDS_ChildIterator_i: public virtual POA_SALOMEDS::ChildIterator,
				public virtual PortableServer::RefCountServantBase,
				public virtual SALOME::GenericObj_i
{
private:
  CORBA::ORB_ptr                     _orb;
  Handle(SALOMEDSImpl_ChildIterator) _it;
public:

  //! standard constructor  
  SALOMEDS_ChildIterator_i(const Handle(SALOMEDSImpl_ChildIterator)&, CORBA::ORB_ptr);
  
  //! standard destructor
  ~SALOMEDS_ChildIterator_i();
  
  virtual void Init();
  virtual void InitEx(CORBA::Boolean);
  virtual CORBA::Boolean More();
  virtual void Next();
  virtual SALOMEDS::SObject_ptr Value();
};
#endif
