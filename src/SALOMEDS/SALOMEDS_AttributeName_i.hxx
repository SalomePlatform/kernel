// File:	SALOMEDS_AttributeName_i.hxx
// Created:	Fri Jul  5 10:21:51 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header:

#ifndef SALOMEDS_AttributeName_i_HeaderFile
#define SALOMEDS_AttributeName_i_HeaderFile

// IDL headers

#include <TDataStd_Name.hxx>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeName_i: public virtual POA_SALOMEDS::AttributeName,
				public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeName_i(const Handle(TDataStd_Name)& theNameAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theNameAttr;
  };
  ~SALOMEDS_AttributeName_i() {};

  char* Value();
  void SetValue(const char* value);
};




#endif
