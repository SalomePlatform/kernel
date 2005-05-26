//  File   : SALOMEDS_AttributePersistentRef.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributePersistentRef_HeaderFile
#define SALOMEDS_AttributePersistentRef_HeaderFile

#include "SALOMEDSClient_AttributePersistentRef.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributePersistentRef.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributePersistentRef: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributePersistentRef
{
public:  
  SALOMEDS_AttributePersistentRef(const Handle(SALOMEDSImpl_AttributePersistentRef)& theAttr);
  SALOMEDS_AttributePersistentRef(SALOMEDS::AttributePersistentRef_ptr theAttr);
  ~SALOMEDS_AttributePersistentRef();

  virtual std::string Value();
  virtual void SetValue(const std::string& value);
};

#endif
