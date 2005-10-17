//  File   : SALOMEDS_AttributeFlags_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//  $Header:

#ifndef SALOMEDS_AttributeFlags_i_HeaderFile
#define SALOMEDS_AttributeFlags_i_HeaderFile

// IDL headers

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOMEDS_GenericAttribute_i.hxx"
#include "SALOMEDSImpl_AttributeFlags.hxx"

/*
  Class       : SALOMEDS_AttributeFlags_i
  Description : This class is intended for storing different object attributes that
                have only two states (0 and 1).

                Avalable attributes:

                IS_VISIBLE - is equal to 1 if object is visible in 3D view (0 - overwise).
                             This attribute is valid for active view only.
*/

class SALOMEDS_AttributeFlags_i: public virtual POA_SALOMEDS::AttributeFlags,
                                 public virtual SALOMEDS_GenericAttribute_i
{
public:
  
                      SALOMEDS_AttributeFlags_i( const Handle(SALOMEDSImpl_AttributeFlags)& theAttr, CORBA::ORB_ptr orb )
			:SALOMEDS_GenericAttribute_i(theAttr, orb) {};

  virtual             ~SALOMEDS_AttributeFlags_i() {};

  CORBA::Long         GetFlags();
  void                SetFlags( CORBA::Long theFlags );

  CORBA::Boolean      Get( CORBA::Long theFlag );
  void                Set( CORBA::Long theFlag, CORBA::Boolean theValue );
  
};

#endif
