//  File      : SALOMEDS_AttLong_i.hxx
//  Created   : Mon Jun 10 10:54:50 CEST 2002 
//  Author    : Estelle Deville

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : CEA 2002
//  $Header$

#ifndef __SALOMEDS_ATTLONG_I_H__
#define __SALOMEDS_ATTLONG_I_H__

// IDL headers
#include <TDataStd_Integer.hxx>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_AttLong)
#include "SALOMEDS_BasicAttribute_i.hxx"

class SALOMEDS_AttLong_i: public POA_SALOMEDS::AttLong,
			  public SALOMEDS_BasicAttribute_i {
protected:
  char * _type;

public:
  
  SALOMEDS_AttLong_i() { _type = "AttLong";};  
  ~SALOMEDS_AttLong_i() {};
  void Set(CORBA::Long i);
  CORBA::Long Get();
  virtual  char* Save();
  virtual  void Load();
  CORBA::Boolean GetPtr(SALOMEDS::SObject_ptr so);
  virtual char* GetType();
};
#endif
