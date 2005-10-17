//  File   : SALOMEDS_AttributePixMap.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributePixMap_HeaderFile
#define SALOMEDS_AttributePixMap_HeaderFile

#include "SALOMEDSClient_AttributePixMap.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributePixMap.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

class SALOMEDS_AttributePixMap: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributePixMap
{
public:  
  SALOMEDS_AttributePixMap(const Handle(SALOMEDSImpl_AttributePixMap)& theAttr);
  SALOMEDS_AttributePixMap(SALOMEDS::AttributePixMap_ptr theAttr);
  ~SALOMEDS_AttributePixMap();

  virtual bool HasPixMap();
  virtual std::string GetPixMap();
  virtual void SetPixMap(const std::string& value); 

};

#endif
