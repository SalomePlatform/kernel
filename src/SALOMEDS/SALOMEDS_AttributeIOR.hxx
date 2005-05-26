//  File   : SALOMEDS_AttributeIOR.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeIOR_HeaderFile
#define SALOMEDS_AttributeIOR_HeaderFile

#include "SALOMEDSClient_AttributeIOR.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeIOR.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeIOR: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeIOR
{
public:  
  SALOMEDS_AttributeIOR(const Handle(SALOMEDSImpl_AttributeIOR)& theAttr);
  SALOMEDS_AttributeIOR(SALOMEDS::AttributeIOR_ptr theAttr);
  ~SALOMEDS_AttributeIOR();

  virtual std::string Value();
  virtual void SetValue(const std::string& value);
};

#endif
