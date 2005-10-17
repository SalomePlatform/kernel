//  File   : SALOMEDS_AttributeComment_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeComment_i_HeaderFile
#define SALOMEDS_AttributeComment_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeComment.hxx"

class SALOMEDS_AttributeComment_i: public virtual POA_SALOMEDS::AttributeComment,
				   public virtual SALOMEDS_GenericAttribute_i 
{
public:  
  SALOMEDS_AttributeComment_i(const Handle(SALOMEDSImpl_AttributeComment)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {};

  virtual ~SALOMEDS_AttributeComment_i() {};

  char* Value();
  void SetValue(const char* value);
};

#endif
