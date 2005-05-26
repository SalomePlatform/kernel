//  File   : SALOMEDS_AttributeTextColor_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header$

#ifndef SALOMEDS_AttributeTextColor_i_HeaderFile
#define SALOMEDS_AttributeTextColor_i_HeaderFile

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeTextColor.hxx"

class SALOMEDS_AttributeTextColor_i: public virtual POA_SALOMEDS::AttributeTextColor,
				     public virtual SALOMEDS_GenericAttribute_i 
{
public:
  
  SALOMEDS_AttributeTextColor_i(const Handle(SALOMEDSImpl_AttributeTextColor)& theAttr, CORBA::ORB_ptr orb) 
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  ~SALOMEDS_AttributeTextColor_i() {};

  SALOMEDS::Color TextColor();
  void SetTextColor(const SALOMEDS::Color& value);

};


#endif
