//  File   : SALOMEDS_AttributeExpandable_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDS_AttributeExpandable_i_HeaderFile
#define SALOMEDS_AttributeExpandable_i_HeaderFile

// IDL headers

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeExpandable.hxx"

class SALOMEDS_AttributeExpandable_i: public virtual POA_SALOMEDS::AttributeExpandable,
                                      public virtual SALOMEDS_GenericAttribute_i 
{
public:
  
  SALOMEDS_AttributeExpandable_i(const Handle(SALOMEDSImpl_AttributeExpandable)& theAttr, CORBA::ORB_ptr orb)
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {};  

  ~SALOMEDS_AttributeExpandable_i() {};
  CORBA::Boolean IsExpandable();
  void SetExpandable(CORBA::Boolean value);
};



#endif
