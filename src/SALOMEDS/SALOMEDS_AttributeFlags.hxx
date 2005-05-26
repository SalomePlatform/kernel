//  File   : SALOMEDS_AttributeFlags.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeFlags_HeaderFile
#define SALOMEDS_AttributeFlags_HeaderFile

#include "SALOMEDSClient_AttributeFlags.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeFlags.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributeFlags: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeFlags
{
public:  
  SALOMEDS_AttributeFlags(const Handle(SALOMEDSImpl_AttributeFlags)& theAttr);
  SALOMEDS_AttributeFlags(SALOMEDS::AttributeFlags_ptr theAttr);
  ~SALOMEDS_AttributeFlags();

  virtual int   GetFlags();
  virtual void  SetFlags(int theFlags);

  virtual bool  Get(int theFlag);
  virtual void  Set(int theFlag, bool theValue);
};

#endif
