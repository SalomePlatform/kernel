//  File   : SALOMEDS_AttributeTextHighlightColor_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef SALOMEDS_AttributeTextHighlightColor_i_HeaderFile
#define SALOMEDS_AttributeTextHighlightColor_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeTextHighlightColor.hxx"

class SALOMEDS_AttributeTextHighlightColor_i: public virtual POA_SALOMEDS::AttributeTextHighlightColor,
				              public virtual SALOMEDS_GenericAttribute_i 
{
public:
  
  SALOMEDS_AttributeTextHighlightColor_i(const Handle(SALOMEDSImpl_AttributeTextHighlightColor)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributeTextHighlightColor_i() {};

  SALOMEDS::Color TextHighlightColor();
  void SetTextHighlightColor(const SALOMEDS::Color& value);
};


#endif
