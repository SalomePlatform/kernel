//  File   : SALOMEDS_SComponent_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_SCOMPONENT_I_H__
#define __SALOMEDS_SCOMPONENT_I_H__

// std C++ headers
#include <iostream.h>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

//SALOMEDS headers
#include "SALOMEDS_SObject_i.hxx"

#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_SComponent.hxx"

class SALOMEDS_SComponent_i: public POA_SALOMEDS::SComponent,
			     public SALOMEDS_SObject_i
{

public:

  static SALOMEDS::SComponent_ptr New(const Handle(SALOMEDSImpl_SComponent)&, CORBA::ORB_ptr); 
  
  SALOMEDS_SComponent_i(const Handle(SALOMEDSImpl_SComponent)&, CORBA::ORB_ptr);
  
  virtual ~SALOMEDS_SComponent_i();

  virtual char* ComponentDataType();
  virtual CORBA::Boolean ComponentIOR(CORBA::String_out theID);

};
#endif
