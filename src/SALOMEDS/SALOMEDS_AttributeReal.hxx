//  File   : SALOMEDS_AttributeReal.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeReal_HeaderFile
#define SALOMEDS_AttributeReal_HeaderFile

#include "SALOMEDSClient_AttributeReal.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeReal.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeReal: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeReal
{
public:  
  SALOMEDS_AttributeReal(const Handle(SALOMEDSImpl_AttributeReal)& theAttr);
  SALOMEDS_AttributeReal(SALOMEDS::AttributeReal_ptr theAttr);
  ~SALOMEDS_AttributeReal();

  virtual double Value();
  virtual void SetValue(double value);
};

#endif
