//  File   : SALOMEDS_AttributeIOR_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeIOR_i_HeaderFile
#define SALOMEDS_AttributeIOR_i_HeaderFile

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeIOR.hxx"

class SALOMEDS_AttributeIOR_i: public virtual POA_SALOMEDS::AttributeIOR,
			       public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeIOR_i(const Handle(SALOMEDSImpl_AttributeIOR)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {};

  ~SALOMEDS_AttributeIOR_i() {};

  char* Value();
  void SetValue(const char* value);
  
};


#endif
