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

// std C++ headers
#include <iostream.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

// Cascade headers
#include <TDataStd_TreeNode.hxx>
#include <TDocStd_Document.hxx>
#include <Standard_GUID.hxx>
#include <stdio.h>

class SALOMEDS_UseCaseBuilder_i: public POA_SALOMEDS::UseCaseBuilder,
				public PortableServer::RefCountServantBase {
private:

  CORBA::ORB_ptr                _orb;
  Handle(TDataStd_TreeNode)     _root;
  Handle(TDocStd_Document)      _doc;

public:

  //! standard constructor  
  SALOMEDS_UseCaseBuilder_i(const Handle(TDocStd_Document)& theDocument,
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
