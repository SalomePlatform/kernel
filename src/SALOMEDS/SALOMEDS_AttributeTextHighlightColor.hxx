//  File   : SALOMEDS_AttributeTextHighlightColor.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeTextHighlightColor_HeaderFile
#define SALOMEDS_AttributeTextHighlightColor_HeaderFile

#include "SALOMEDSClient_AttributeTextHighlightColor.hxx"
#include "SALOMEDS_GenericAttribute.hxx"
#include "SALOMEDSImpl_AttributeTextHighlightColor.hxx"
#include "SALOMEDSClient_definitions.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include <vector>

class SALOMEDS_AttributeTextHighlightColor: public SALOMEDS_GenericAttribute, public SALOMEDSClient_AttributeTextHighlightColor
{
public:  
  SALOMEDS_AttributeTextHighlightColor(const Handle(SALOMEDSImpl_AttributeTextHighlightColor)& theAttr);
  SALOMEDS_AttributeTextHighlightColor(SALOMEDS::AttributeTextHighlightColor_ptr theAttr);
  ~SALOMEDS_AttributeTextHighlightColor();

  virtual STextColor TextHighlightColor();
  virtual void SetTextHighlightColor(STextColor value);

};

#endif
