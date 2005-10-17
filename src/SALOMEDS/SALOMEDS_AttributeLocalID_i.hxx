//  File   : SALOMEDS_AttributeLocalID_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDS_AttributeLocalID_i_HeaderFile
#define SALOMEDS_AttributeLocalID_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeLocalID.hxx"

class SALOMEDS_AttributeLocalID_i: public virtual POA_SALOMEDS::AttributeLocalID,
                                   public virtual SALOMEDS_GenericAttribute_i 
{
public:
  
  SALOMEDS_AttributeLocalID_i(const Handle(SALOMEDSImpl_AttributeLocalID)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributeLocalID_i() {};

  CORBA::Long Value();
  void SetValue(CORBA::Long value);

};



#endif
