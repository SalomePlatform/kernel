//  File   : SALOMEDS_AttributeOpened.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeOpened_HeaderFile
#define SALOMEDS_AttributeOpened_HeaderFile

#include "SALOMEDSClient_AttributeOpened.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeOpened.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeOpened: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeOpened
{
public:  
  SALOMEDS_AttributeOpened(const Handle(SALOMEDSImpl_AttributeOpened)& theAttr);
  SALOMEDS_AttributeOpened(SALOMEDS::AttributeOpened_ptr theAttr);
  ~SALOMEDS_AttributeOpened();

  virtual bool IsOpened();
  virtual void SetOpened(bool value);
};

#endif
