// File:	SALOMEDS_AttributePixMap_i.hxx
// Created:	Fri Jul  5 10:21:15 2002
// Author:	Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 


#ifndef SALOMEDS_AttributePixMap_i_HeaderFile
#define SALOMEDS_AttributePixMap_i_HeaderFile

// IDL headers
#include "SALOMEDS_PixMapAttribute.hxx"
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"

class SALOMEDS_AttributePixMap_i: public virtual POA_SALOMEDS::AttributePixMap,
				public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributePixMap_i(const Handle(SALOMEDS_PixMapAttribute)& thePixMapAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = thePixMapAttr;
  };
  ~SALOMEDS_AttributePixMap_i() {};
  CORBA::Boolean HasPixMap();
  char* GetPixMap();
  void SetPixMap(const char* value);
};


#endif
