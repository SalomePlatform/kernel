// File:	SALOMEDS_AttributeReal_i.hxx
// Created:	Fri Jul  5 10:21:15 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 


#ifndef SALOMEDS_AttributeReal_i_HeaderFile
#define SALOMEDS_AttributeReal_i_HeaderFile

// IDL headers
#include <TDataStd_Real.hxx>
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributeReal_i: public virtual POA_SALOMEDS::AttributeReal,
				public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeReal_i(const Handle(TDataStd_Real)& theRealAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theRealAttr;
  };
  ~SALOMEDS_AttributeReal_i() {};

  CORBA::Double Value();
  void SetValue(CORBA::Double value);
};


#endif
