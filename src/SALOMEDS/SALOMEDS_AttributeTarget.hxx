//  File   : SALOMEDS_AttributeTarget.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeTarget_HeaderFile
#define SALOMEDS_AttributeTarget_HeaderFile

#include <vector>

#include "SALOMEDSClient.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeTarget.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeTarget: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeTarget
{
public:  
  SALOMEDS_AttributeTarget(const Handle(SALOMEDSImpl_AttributeTarget)& theAttr);
  SALOMEDS_AttributeTarget(SALOMEDS::AttributeTarget_ptr theAttr);
  ~SALOMEDS_AttributeTarget();

  virtual void Add(const _PTR(SObject)& theObject);
  virtual std::vector<_PTR(SObject)> Get();
  virtual void Remove(const _PTR(SObject)& theObject); 
};

#endif
