//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_UseCaseBuilder_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME

#ifndef __SALOMEDS_USECaseBuilder_I_H__
#define __SALOMEDS_USECaseBuilder_I_H__

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

// Cascade headers
#include <TDataStd_TreeNode.hxx>
#include <TDocStd_Document.hxx>

class SALOMEDS_Study_i;

class SALOMEDS_UseCaseBuilder_i: public POA_SALOMEDS::UseCaseBuilder,
				 public PortableServer::RefCountServantBase 
{
  SALOMEDS_UseCaseBuilder_i(); // Not implemented
  void operator=(const SALOMEDS_UseCaseBuilder_i&); // Not implemented

private:
  Handle(TDataStd_TreeNode)     _root;
  Handle(TDocStd_Document)      _doc;
  SALOMEDS_Study_i*             _study;

public:
  SALOMEDS_UseCaseBuilder_i(SALOMEDS_Study_i* theStudy,
			    const Handle(TDocStd_Document)& theDocument);
  
  ~SALOMEDS_UseCaseBuilder_i();
  
  CORBA::ORB_var GetORB() const;
  PortableServer::POA_var GetPOA() const;

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
