//  File   : SALOMEDS_AttributeName.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeName_HeaderFile
#define SALOMEDS_AttributeName_HeaderFile

#include "SALOMEDSClient_AttributeName.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeName.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeName: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeName
{
public:  
  SALOMEDS_AttributeName(const Handle(SALOMEDSImpl_AttributeName)& theAttr);
  SALOMEDS_AttributeName(SALOMEDS::AttributeName_ptr theAttr);
  ~SALOMEDS_AttributeName();

  virtual std::string Value();
  virtual void SetValue(const std::string& value);
};

#endif
