//  File   : SALOMEDS_AttributeFileType.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeFileType_HeaderFile
#define SALOMEDS_AttributeFileType_HeaderFile

#include "SALOMEDSClient_AttributeFileType.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeFileType.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeFileType: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeFileType
{
public:  
  SALOMEDS_AttributeFileType(const Handle(SALOMEDSImpl_AttributeFileType)& theAttr);
  SALOMEDS_AttributeFileType(SALOMEDS::AttributeFileType_ptr theAttr);
  ~SALOMEDS_AttributeFileType();

  virtual std::string Value();
  virtual void SetValue(const std::string& value);
};

#endif
