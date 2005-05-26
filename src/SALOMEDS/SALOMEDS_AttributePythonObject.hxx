//  File   : SALOMEDS_AttributePythonObject.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributePythonObject_HeaderFile
#define SALOMEDS_AttributePythonObject_HeaderFile

#include "SALOMEDSClient_AttributePythonObject.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributePythonObject.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributePythonObject: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributePythonObject
{
public:  
  SALOMEDS_AttributePythonObject(const Handle(SALOMEDSImpl_AttributePythonObject)& theAttr);
  SALOMEDS_AttributePythonObject(SALOMEDS::AttributePythonObject_ptr theAttr);
  ~SALOMEDS_AttributePythonObject();

  virtual void SetObject(const std::string& theSequence, bool IsScript);
  virtual std::string GetObject();
  virtual bool IsScript();     

};

#endif
