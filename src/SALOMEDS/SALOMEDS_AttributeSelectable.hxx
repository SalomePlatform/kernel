//  File   : SALOMEDS_AttributeSelectable.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeSelectable_HeaderFile
#define SALOMEDS_AttributeSelectable_HeaderFile

#include "SALOMEDSClient_AttributeSelectable.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeSelectable.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeSelectable: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeSelectable
{
public:  
  SALOMEDS_AttributeSelectable(const Handle(SALOMEDSImpl_AttributeSelectable)& theAttr);
  SALOMEDS_AttributeSelectable(SALOMEDS::AttributeSelectable_ptr theAttr);
  ~SALOMEDS_AttributeSelectable();

  virtual bool IsSelectable();
  virtual void SetSelectable(bool value);
};

#endif
