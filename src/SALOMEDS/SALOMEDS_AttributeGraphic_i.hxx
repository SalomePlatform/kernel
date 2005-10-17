//  File   : SALOMEDS_AttributeFlags_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDS_AttributeGraphic_i_HeaderFile
#define SALOMEDS_AttributeGraphic_i_HeaderFile

// IDL headers


#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeGraphic.hxx"

/*
  Class       : SALOMEDS_AttributeGraphic_i
  Description : This class is intended for storing information about
                graphic representation of objects in dirrent views
*/

class SALOMEDS_AttributeGraphic_i: public virtual POA_SALOMEDS::AttributeGraphic,
                                   public virtual SALOMEDS_GenericAttribute_i
{
public:
  
  SALOMEDS_AttributeGraphic_i( const Handle(SALOMEDSImpl_AttributeGraphic)& theAttr, CORBA::ORB_ptr orb )
    :SALOMEDS_GenericAttribute_i(theAttr, orb) {}; 

  virtual             ~SALOMEDS_AttributeGraphic_i() {};

  void                SetVisibility( CORBA::Long    theViewId,
                                     CORBA::Boolean theValue );
  CORBA::Boolean      GetVisibility( CORBA::Long    theViewId );

};

#endif
