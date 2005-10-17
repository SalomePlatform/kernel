//  File   : SALOMEDS_AttributeInteger_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeInteger_i_HeaderFile
#define SALOMEDS_AttributeInteger_i_HeaderFile

// IDL headers


#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeInteger.hxx"

class SALOMEDS_AttributeInteger_i: public virtual POA_SALOMEDS::AttributeInteger,
				   public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeInteger_i(const Handle(SALOMEDSImpl_AttributeInteger)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributeInteger_i() {};

  CORBA::Long Value();
  void SetValue(CORBA::Long value);
};



#endif
