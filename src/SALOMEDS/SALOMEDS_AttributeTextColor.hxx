//  File   : SALOMEDS_AttributeTextColor.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeTextColor_HeaderFile
#define SALOMEDS_AttributeTextColor_HeaderFile

#include "SALOMEDSClient_AttributeTextColor.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeTextColor.hxx"
#include "SALOMEDSClient_definitions.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include <vector>

class SALOMEDS_AttributeTextColor: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeTextColor
{
public:  
  SALOMEDS_AttributeTextColor(const Handle(SALOMEDSImpl_AttributeTextColor)& theAttr);
  SALOMEDS_AttributeTextColor(SALOMEDS::AttributeTextColor_ptr theAttr);
  ~SALOMEDS_AttributeTextColor();

  virtual STextColor TextColor();
  virtual void SetTextColor(STextColor value);

};

#endif
