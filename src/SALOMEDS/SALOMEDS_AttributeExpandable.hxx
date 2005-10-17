//  File   : SALOMEDS_AttributeExpandable.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeExpandable_HeaderFile
#define SALOMEDS_AttributeExpandable_HeaderFile

#include "SALOMEDSClient_AttributeExpandable.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeExpandable.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeExpandable: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeExpandable
{
public:  
  SALOMEDS_AttributeExpandable(const Handle(SALOMEDSImpl_AttributeExpandable)& theAttr);
  SALOMEDS_AttributeExpandable(SALOMEDS::AttributeExpandable_ptr theAttr);
  ~SALOMEDS_AttributeExpandable();

  virtual bool IsExpandable();
  virtual void SetExpandable(bool value);
};

#endif
