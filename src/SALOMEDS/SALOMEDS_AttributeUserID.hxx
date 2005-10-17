//  File   : SALOMEDS_AttributeUserID.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeUserID_HeaderFile
#define SALOMEDS_AttributeUserID_HeaderFile

#include "SALOMEDSClient_AttributeUserID.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeUserID.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeUserID: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeUserID
{
public:  
  SALOMEDS_AttributeUserID(const Handle(SALOMEDSImpl_AttributeUserID)& theAttr);
  SALOMEDS_AttributeUserID(SALOMEDS::AttributeUserID_ptr theAttr);
  ~SALOMEDS_AttributeUserID();

  virtual std::string Value();
  virtual void SetValue(const std::string& value);
};

#endif
