//  File   : SALOMEDS_SObject_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_SOBJECT_I_H__
#define __SALOMEDS_SOBJECT_I_H__

// std C++ headers
#include <iostream>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include <SALOME_GenericObj_i.hh>

// Cascade headers
#include "SALOMEDSImpl_SObject.hxx"

#ifdef GetObject
#undef GetObject
#endif

class Standard_EXPORT SALOMEDS_SObject_i: public virtual POA_SALOMEDS::SObject,
			  public virtual PortableServer::RefCountServantBase,
			  public virtual SALOME::GenericObj_i
{
protected:
  CORBA::ORB_ptr                _orb;
  Handle(SALOMEDSImpl_SObject)  _impl;

public:

  static SALOMEDS::SObject_ptr New(const Handle(SALOMEDSImpl_SObject)&, CORBA::ORB_ptr); 
  
  SALOMEDS_SObject_i(const Handle(SALOMEDSImpl_SObject)&, CORBA::ORB_ptr);
  
  virtual ~SALOMEDS_SObject_i();
  
  virtual char* GetID();
  virtual SALOMEDS::SComponent_ptr GetFatherComponent();
  virtual SALOMEDS::SObject_ptr    GetFather() ;
  virtual CORBA::Boolean FindAttribute(SALOMEDS::GenericAttribute_out anAttribute, const char* aTypeOfAttribute);
  virtual CORBA::Boolean ReferencedObject(SALOMEDS::SObject_out obj) ;
  virtual CORBA::Boolean FindSubObject(long atag, SALOMEDS::SObject_out obj );

  virtual SALOMEDS::Study_ptr    GetStudy() ;
  virtual char* Name();
  virtual void  Name(const char*);
  virtual SALOMEDS::ListOfAttributes* GetAllAttributes();

  virtual CORBA::Object_ptr GetObject();

  virtual char* GetName();
  virtual char* GetComment();
  virtual char* GetIOR();

  virtual CORBA::Short Tag();
  virtual CORBA::Short Depth();

  virtual long GetLocalImpl(const char* theHostname, CORBA::Long thePID, CORBA::Boolean& isLocal);
};

#endif
