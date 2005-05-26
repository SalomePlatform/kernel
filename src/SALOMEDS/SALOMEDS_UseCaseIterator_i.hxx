//  File   : SALOMEDS_UseCaseIterator_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_USECASEITERATOR_I_H__
#define __SALOMEDS_USECASEITERATOR_I_H__

// std C++ headers
#include <iostream.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include <SALOME_GenericObj_i.hh>

#include "SALOMEDSImpl_UseCaseIterator.hxx"

class SALOMEDS_UseCaseIterator_i: public virtual POA_SALOMEDS::UseCaseIterator,
				  public virtual PortableServer::RefCountServantBase, 
				  public virtual SALOME::GenericObj_i
{
private:
  CORBA::ORB_ptr                        _orb;
  Handle(SALOMEDSImpl_UseCaseIterator) _impl;

public:

  //! standard constructor  
  SALOMEDS_UseCaseIterator_i(const Handle(SALOMEDSImpl_UseCaseIterator)& theImpl, 
			     CORBA::ORB_ptr);
  
  //! standard destructor
  ~SALOMEDS_UseCaseIterator_i();
  
  virtual void Init(CORBA::Boolean);
  virtual CORBA::Boolean More();
  virtual void Next();
  virtual SALOMEDS::SObject_ptr Value();
};
#endif
