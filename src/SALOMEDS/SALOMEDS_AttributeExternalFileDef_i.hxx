//  File   : SALOMEDS_AttributeExternalFileDef_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeExternalFileDef_i_HeaderFile
#define SALOMEDS_AttributeExternalFileDef_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeExternalFileDef.hxx"

class SALOMEDS_AttributeExternalFileDef_i: public virtual POA_SALOMEDS::AttributeExternalFileDef,
				           public virtual SALOMEDS_GenericAttribute_i 
{
public:
  
  SALOMEDS_AttributeExternalFileDef_i(const Handle(SALOMEDSImpl_AttributeExternalFileDef)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {} 
  ~SALOMEDS_AttributeExternalFileDef_i() {};

  char* Value();
  void SetValue(const char* value);
  
};



#endif
