//=============================================================================
// File      : SALOMEDS_BasicAttribute_i.hxx
// Created   : ven mai 31 13:45:47 CEST 2002
// Author    : Estelle Deville, CEA
// Project   : SALOME
// Copyright : CEA 2002
// $Header$
//=============================================================================

#ifndef _BASIC_ATTRIBUTE_I_HXX_
#define _BASIC_ATTRIBUTE_I_HXX_

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include <TDF_Label.hxx>

class SALOMEDS_BasicAttribute_i: public POA_SALOMEDS::BasicAttribute,
				 public PortableServer::RefCountServantBase {
protected:
  TDF_Label _Lab;

public:
  SALOMEDS_BasicAttribute_i() {};

  ~SALOMEDS_BasicAttribute_i() {};

  virtual char *  Save() =0;
  virtual void    Load () =0;
  CORBA::Boolean    GetPtr(SALOMEDS::SObject so);  
  void SetLabel(const TDF_Label Lab);
  virtual char* GetType() = 0;

};

#endif
