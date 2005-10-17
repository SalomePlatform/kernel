//  File   : SALOMEDS_AttributeOpened_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDS_AttributeOpened_i_HeaderFile
#define SALOMEDS_AttributeOpened_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeOpened.hxx"

class SALOMEDS_AttributeOpened_i: public virtual POA_SALOMEDS::AttributeOpened,
                                  public virtual SALOMEDS_GenericAttribute_i 
{
public:
  
  SALOMEDS_AttributeOpened_i(const Handle(SALOMEDSImpl_AttributeOpened)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {};
  ~SALOMEDS_AttributeOpened_i() {};

  CORBA::Boolean IsOpened();
  void SetOpened(CORBA::Boolean value);
};



#endif
