//  File   : SALOMEDS_AttributeUserID_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeUserID_i_HeaderFile
#define SALOMEDS_AttributeUserID_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeUserID.hxx"

class SALOMEDS_AttributeUserID_i: public virtual POA_SALOMEDS::AttributeUserID,
                                  public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeUserID_i(const Handle(SALOMEDSImpl_AttributeUserID)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributeUserID_i() {};

  char* Value();
  void SetValue(const char* value);

  static const Standard_GUID& DefaultID() {
    return SALOMEDSImpl_AttributeUserID::DefaultID();
  }

};




#endif
