//  File   : SALOMEDS_AttributeFileType_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeFileType_i_HeaderFile
#define SALOMEDS_AttributeFileType_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeFileType.hxx"

class SALOMEDS_AttributeFileType_i: public virtual POA_SALOMEDS::AttributeFileType,
				   public virtual SALOMEDS_GenericAttribute_i {
public:
  
  SALOMEDS_AttributeFileType_i(const Handle(SALOMEDSImpl_AttributeFileType)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 
  ~SALOMEDS_AttributeFileType_i() {};

  char* Value();
  void SetValue(const char* value);
  
};



#endif
