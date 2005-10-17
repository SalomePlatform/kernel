//  File   : SALOMEDS_AttributeDrawable.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeDrawable_HeaderFile
#define SALOMEDS_AttributeDrawable_HeaderFile

#include "SALOMEDSClient_AttributeDrawable.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeDrawable.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeDrawable: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeDrawable
{
public:  
  SALOMEDS_AttributeDrawable(const Handle(SALOMEDSImpl_AttributeDrawable)& theAttr);
  SALOMEDS_AttributeDrawable(SALOMEDS::AttributeDrawable_ptr theAttr);
  ~SALOMEDS_AttributeDrawable();

  virtual bool IsDrawable();
  virtual void SetDrawable(bool value);
};

#endif
