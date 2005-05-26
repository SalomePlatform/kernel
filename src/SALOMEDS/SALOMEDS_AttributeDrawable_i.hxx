//  File   : SALOMEDS_AttributeDrawable_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeDrawable_i_HeaderFile
#define SALOMEDS_AttributeDrawable_i_HeaderFile

// IDL headers

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeDrawable.hxx"

class SALOMEDS_AttributeDrawable_i: public virtual POA_SALOMEDS::AttributeDrawable,
                                    public virtual SALOMEDS_GenericAttribute_i 
{
public:
  
  SALOMEDS_AttributeDrawable_i(const Handle(SALOMEDSImpl_AttributeDrawable)& theAttr, CORBA::ORB_ptr orb)
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  virtual ~SALOMEDS_AttributeDrawable_i() {};
  CORBA::Boolean IsDrawable();
  void SetDrawable(CORBA::Boolean value);
};



#endif
