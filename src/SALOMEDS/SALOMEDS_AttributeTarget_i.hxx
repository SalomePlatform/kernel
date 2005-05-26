//  File   : SALOMEDS_AttributeTarget_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeTarget_i_HeaderFile
#define SALOMEDS_AttributeTarget_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)
#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeTarget.hxx"

class SALOMEDS_AttributeTarget_i: public virtual POA_SALOMEDS::AttributeTarget,
                                  public virtual SALOMEDS_GenericAttribute_i 
{
public:
  
  SALOMEDS_AttributeTarget_i(const Handle(SALOMEDSImpl_AttributeTarget)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributeTarget_i() {};

  virtual void Add(SALOMEDS::SObject_ptr anObject) ;
  virtual SALOMEDS::Study::ListOfSObject* Get();
  virtual void Remove(SALOMEDS::SObject_ptr anObject);
};

#endif
