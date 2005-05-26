//  File   : SALOMEDS_AttributeReal_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeReal_i_HeaderFile
#define SALOMEDS_AttributeReal_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeReal.hxx"

class SALOMEDS_AttributeReal_i: public virtual POA_SALOMEDS::AttributeReal,
				public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeReal_i(const Handle(SALOMEDSImpl_AttributeReal)& theAttr, CORBA::ORB_ptr orb)
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {};

  ~SALOMEDS_AttributeReal_i() {};

  CORBA::Double Value();
  void SetValue(CORBA::Double value);
};


#endif
