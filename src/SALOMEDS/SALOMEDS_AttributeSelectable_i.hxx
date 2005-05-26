//  File   : SALOMEDS_AttributeSelectable_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeSelectable_i_HeaderFile
#define SALOMEDS_AttributeSelectable_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeSelectable.hxx"

class SALOMEDS_AttributeSelectable_i: public virtual POA_SALOMEDS::AttributeSelectable,
                                      public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeSelectable_i(const Handle(SALOMEDSImpl_AttributeSelectable)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributeSelectable_i() {};

  CORBA::Boolean IsSelectable();
  void SetSelectable(CORBA::Boolean value);

};



#endif
