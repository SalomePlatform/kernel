//  File      : SALOMEDS_AttReal_i.hxx
//  Created   : Mon Jun 10 10:54:50 CEST 2002 
//  Author    : Estelle Deville

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : CEA 2002
//  $Header$

#ifndef __SALOMEDS_ATTREAL_I_H__
#define __SALOMEDS_ATTREAL_I_H__

// IDL headers
#include <TDataStd_Real.hxx>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_AttReal)
#include "SALOMEDS_BasicAttribute_i.hxx"

class SALOMEDS_AttReal_i: public POA_SALOMEDS::AttReal,
			  public SALOMEDS_BasicAttribute_i {
protected:
  char * _type;

public:
  
  SALOMEDS_AttReal_i() { _type = "AttReal";};  
  ~SALOMEDS_AttReal_i() {};
  void Set(CORBA::Double r);
  CORBA::Double Get();
  virtual  char* Save();
  virtual  void Load();
  CORBA::Boolean GetPtr(SALOMEDS::SObject_ptr so);
  virtual char* GetType();
};
#endif
