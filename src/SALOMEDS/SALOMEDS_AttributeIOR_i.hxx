//  File      : SALOMEDS_AttributeIOR_i.hxx
//  Created   : Fri Jul 05 10:57:09 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#ifndef SALOMEDS_AttributeIOR_i_HeaderFile
#define SALOMEDS_AttributeIOR_i_HeaderFile

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDS_IORAttribute.hxx"

class SALOMEDS_AttributeIOR_i: public virtual POA_SALOMEDS::AttributeIOR,
			       public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeIOR_i(const Handle(SALOMEDS_IORAttribute)& theIORAttr, CORBA::ORB_ptr orb) 
  {
    _myOrb = CORBA::ORB::_duplicate(orb);
    _myAttr = theIORAttr;
  }
  ~SALOMEDS_AttributeIOR_i() {};

  char* Value();
  void SetValue(const char* value);
  
};


#endif
