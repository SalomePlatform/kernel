//  File   : SALOMEDS_AttributeLocalID.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeLocalID_HeaderFile
#define SALOMEDS_AttributeLocalID_HeaderFile

#include "SALOMEDSClient_AttributeLocalID.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeLocalID.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeLocalID: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeLocalID
{
public:  
  SALOMEDS_AttributeLocalID(const Handle(SALOMEDSImpl_AttributeLocalID)& theAttr);
  SALOMEDS_AttributeLocalID(SALOMEDS::AttributeLocalID_ptr theAttr);
  ~SALOMEDS_AttributeLocalID();

  virtual int Value();
  virtual void SetValue(int value);
};

#endif
