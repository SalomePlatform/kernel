//  File   : SALOMEDS_AttributeGraphic.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeGraphic_HeaderFile
#define SALOMEDS_AttributeGraphic_HeaderFile

#include "SALOMEDSClient_AttributeGraphic.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeGraphic.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeGraphic: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeGraphic
{
public:  
  SALOMEDS_AttributeGraphic(const Handle(SALOMEDSImpl_AttributeGraphic)& theAttr);
  SALOMEDS_AttributeGraphic(SALOMEDS::AttributeGraphic_ptr theAttr);
  ~SALOMEDS_AttributeGraphic();

  virtual void  SetVisibility(int theViewId, bool theValue);
  virtual bool GetVisibility(int theViewId);
};

#endif
