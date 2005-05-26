//  File   : SALOMEDS_GenericAttribute.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _GENERIC_ATTRIBUTE_HXX_
#define _GENERIC_ATTRIBUTE_HXX_

#include <exception>
#include "SALOMEDSClient.hxx"
#include "SALOMEDSImpl_GenericAttribute.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)


class SALOMEDS_GenericAttribute: public virtual SALOMEDSClient_GenericAttribute
{
protected:
  bool                                  _isLocal;
  Handle(SALOMEDSImpl_GenericAttribute) _local_impl;
  SALOMEDS::GenericAttribute_var        _corba_impl;

public:
  SALOMEDS_GenericAttribute(const Handle(SALOMEDSImpl_GenericAttribute)& theGA);
  SALOMEDS_GenericAttribute(SALOMEDS::GenericAttribute_ptr theGA);
  virtual ~SALOMEDS_GenericAttribute();

  void CheckLocked();
  std::string Type();
  std::string GetClassType();
  _PTR(SObject) GetSObject();
  
  static SALOMEDS_GenericAttribute* CreateAttribute(const Handle(SALOMEDSImpl_GenericAttribute)& theGA);
  static SALOMEDS_GenericAttribute* CreateAttribute(SALOMEDS::GenericAttribute_ptr theGA);
};

#endif
