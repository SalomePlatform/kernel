//  File   : SALOMEDS_SComponent.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDS_SCOMPONENT_H__
#define __SALOMEDS_SCOMPONENT_H__

#include "SALOMEDSClient_SComponent.hxx"
#include "SALOMEDS_SObject.hxx"
#include "SALOMEDSImpl_SComponent.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class SALOMEDS_SComponent: public SALOMEDS_SObject, public SALOMEDSClient_SComponent
{
public:

  SALOMEDS_SComponent(const Handle(SALOMEDSImpl_SComponent)& theSComponent);
  SALOMEDS_SComponent(SALOMEDS::SComponent_ptr theSComponent);
  ~SALOMEDS_SComponent();

  virtual std::string ComponentDataType();
  virtual bool ComponentIOR(std::string& theID);    

  SALOMEDS::SComponent_ptr GetSComponent();
};
#endif
