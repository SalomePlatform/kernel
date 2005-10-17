//  File   : SALOMEDS_AttributeComment.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeComment_HeaderFile
#define SALOMEDS_AttributeComment_HeaderFile

#include "SALOMEDSClient_AttributeComment.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeComment.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeComment: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeComment
{
public:  
  SALOMEDS_AttributeComment(const Handle(SALOMEDSImpl_AttributeComment)& theAttr);
  SALOMEDS_AttributeComment(SALOMEDS::AttributeComment_ptr theAttr);
  ~SALOMEDS_AttributeComment();

  virtual std::string Value();
  virtual void SetValue(const std::string& value);
};

#endif
