//  File   : SALOMEDS_AttributePersistentRef_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributePersistentRef_i_HeaderFile
#define SALOMEDS_AttributePersistentRef_i_HeaderFile

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributePersistentRef.hxx"

class SALOMEDS_AttributePersistentRef_i: public virtual POA_SALOMEDS::AttributePersistentRef,
					 public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributePersistentRef_i(const Handle(SALOMEDSImpl_AttributePersistentRef)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributePersistentRef_i() {};

  char* Value();
  void SetValue(const char* value);

};



#endif
