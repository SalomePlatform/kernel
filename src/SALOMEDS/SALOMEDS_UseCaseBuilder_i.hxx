//  File   : SALOMEDS_UseCaseBuilder_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_USECaseBuilder_I_H__
#define __SALOMEDS_USECaseBuilder_I_H__

// std C++ headers
#include <iostream.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include <SALOME_GenericObj_i.hh>

// Cascade headers
#include <Standard_GUID.hxx>
#include <stdio.h>

#include "SALOMEDSImpl_UseCaseIterator.hxx"
#include "SALOMEDSImpl_UseCaseBuilder.hxx"

class SALOMEDS_UseCaseBuilder_i: public virtual POA_SALOMEDS::UseCaseBuilder,
				 public virtual PortableServer::RefCountServantBase,
				 public virtual SALOME::GenericObj_i
{
private:

  CORBA::ORB_ptr                      _orb;
  Handle(SALOMEDSImpl_UseCaseBuilder) _impl;

public:

  //! standard constructor  
  SALOMEDS_UseCaseBuilder_i(const Handle(SALOMEDSImpl_UseCaseBuilder)& theDocument,
			    CORBA::ORB_ptr);
  
  //! standard destructor
  ~SALOMEDS_UseCaseBuilder_i();
  
  virtual CORBA::Boolean Append(SALOMEDS::SObject_ptr theObject);

  virtual CORBA::Boolean Remove(SALOMEDS::SObject_ptr theObject);

  virtual CORBA::Boolean AppendTo(SALOMEDS::SObject_ptr theFather, SALOMEDS::SObject_ptr theObject);

  virtual CORBA::Boolean InsertBefore(SALOMEDS::SObject_ptr theFirst, SALOMEDS::SObject_ptr theNext);

  virtual CORBA::Boolean  SetCurrentObject(SALOMEDS::SObject_ptr theObject);
  
  virtual CORBA::Boolean SetRootCurrent();

  virtual CORBA::Boolean  HasChildren(SALOMEDS::SObject_ptr theObject);

  virtual CORBA::Boolean  IsUseCase(SALOMEDS::SObject_ptr theObject);

  virtual CORBA::Boolean SetName(const char* theName);

  virtual SALOMEDS::SObject_ptr GetCurrentObject();

  virtual char* GetName();

  virtual SALOMEDS::SObject_ptr AddUseCase(const char* theName);

  virtual SALOMEDS::UseCaseIterator_ptr GetUseCaseIterator(SALOMEDS::SObject_ptr anObject);

};
#endif
