//  File      : SALOMEDS_GenericAttribute_i.hxx
//  Created   : Fri Jul 05 15:49:37 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef _GENERIC_ATTRIBUTE_I_HXX_
#define _GENERIC_ATTRIBUTE_I_HXX_

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include <TDF_Attribute.hxx>
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_IORAttribute.hxx"

class SALOMEDS_GenericAttribute_i: public POA_SALOMEDS::GenericAttribute,
				   public PortableServer::RefCountServantBase {
protected:
  Handle(TDF_Attribute) _myAttr;
  CORBA::ORB_ptr        _myOrb;
public:
  SALOMEDS_GenericAttribute_i() {};
  
  void CheckLocked() throw (SALOMEDS::GenericAttribute::LockProtection);
  
  ~SALOMEDS_GenericAttribute_i() {};

//  CORBA::Boolean  GetPtr(SALOMEDS::SObject_out so) {
//    SALOMEDS_SObject_i* sob_impl = new  SALOMEDS_SObject_i(_myAttr->Label(), _myOrb);
//    SALOMEDS::SObject_var sob = sob_impl->_this();
//    return sob._retn();
//  };


};

#endif
