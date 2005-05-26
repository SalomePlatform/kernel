//  File   : SALOMEDS_AttributeExternalFileDef.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeExternalFileDef_HeaderFile
#define SALOMEDS_AttributeExternalFileDef_HeaderFile

#include "SALOMEDSClient_AttributeExternalFileDef.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeExternalFileDef.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeExternalFileDef: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeExternalFileDef
{
public:  
  SALOMEDS_AttributeExternalFileDef(const Handle(SALOMEDSImpl_AttributeExternalFileDef)& theAttr);
  SALOMEDS_AttributeExternalFileDef(SALOMEDS::AttributeExternalFileDef_ptr theAttr);
  ~SALOMEDS_AttributeExternalFileDef();

  virtual std::string Value();
  virtual void SetValue(const std::string& value);
};

#endif
