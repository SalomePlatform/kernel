//  File   : SALOMEDS_AttributeInteger.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeInteger_HeaderFile
#define SALOMEDS_AttributeInteger_HeaderFile

#include "SALOMEDSClient_AttributeInteger.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeInteger.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeInteger: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeInteger
{
public:  
  SALOMEDS_AttributeInteger(const Handle(SALOMEDSImpl_AttributeInteger)& theAttr);
  SALOMEDS_AttributeInteger(SALOMEDS::AttributeInteger_ptr theAttr);
  ~SALOMEDS_AttributeInteger();

  virtual int Value();
  virtual void SetValue(int value);
};

#endif
