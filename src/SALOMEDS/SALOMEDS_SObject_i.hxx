//  File      : SALOMEDS_SObject_i.hxx
//  Created   : Wed Nov 28 16:25:50 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#ifndef __SALOMEDS_SOBJECT_I_H__
#define __SALOMEDS_SOBJECT_I_H__

// std C++ headers
#include <iostream.h>
#include <vector>
#include <string>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

// Cascade headers
#include <TDF_Label.hxx>
#include <stdio.h>
#include <TDF_Tool.hxx>
#include <TDF_Reference.hxx>
#include <Standard_GUID.hxx>
#include <Standard_NoSuchObject.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_Comment.hxx>
#include <TDataStd_Integer.hxx>
#include <TDataStd_Real.hxx>

#include <TCollection_AsciiString.hxx>

class SALOMEDS_SObject_i: public POA_SALOMEDS::SObject,
			  public PortableServer::RefCountServantBase {
protected:
  CORBA::ORB_ptr           _orb;
  TDF_Label                _lab;
  char*                    _name;  
  char*                    _value;
  char*                    _type;
  vector<string>           _liste_ba_type;

public:
  
  SALOMEDS_SObject_i(const TDF_Label, CORBA::ORB_ptr);
  
  ~SALOMEDS_SObject_i();
  
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


  virtual CORBA::Short Tag();

  static char* AttributeIDToType(Standard_GUID);
  
  static Standard_GUID           ReturnGUIDForAttribute(const char* aTypeOfAttribute);

};
#endif
