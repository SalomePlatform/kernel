//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//
//
//  File   : SALOMEDS_Study_i.hxx
//  Author : Yves FRICAUD
//  Module : SALOME

#ifndef __SALOMEDS_CALLBACK_I_H__
#define __SALOMEDS_CALLBACK_I_H__

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class SALOMEDS_Callback_i: public POA_SALOMEDS::Callback,
			   public PortableServer::RefCountServantBase {

private:
  CORBA::ORB_var _orb;
  SALOMEDS::UseCaseBuilder_var _builder;

public:

 SALOMEDS_Callback_i(SALOMEDS::UseCaseBuilder_ptr builder, CORBA::ORB_ptr orb) {
   _orb = CORBA::ORB::_duplicate(orb);
   _builder = SALOMEDS::UseCaseBuilder::_duplicate(builder);
 }

 virtual void OnAddSObject(SALOMEDS::SObject_ptr theObject) {
   if(!_builder->_is_nil() && !theObject->_is_nil()) _builder->Append(theObject);
 }

 virtual void OnRemoveSObject(SALOMEDS::SObject_ptr theObject) {
   if(!_builder->_is_nil() && !theObject->_is_nil()) _builder->Remove(theObject);
 }

};

#endif
