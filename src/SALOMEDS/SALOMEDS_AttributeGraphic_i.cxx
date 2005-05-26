//  File   : SALOMEDS_AttributeGraphic_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeGraphic_i.hxx"
#include "SALOMEDS.hxx" 

/*
  Class       : SALOMEDS_AttributeGraphic_i
  Description : This class is intended for storing information about
                graphic representation of objects in dirrent views
*/


//=======================================================================
// function : SALOMEDS_AttributeGraphic_i::~SetVisibility
// purpose  : Set visibility of object in given view
//=======================================================================
void SALOMEDS_AttributeGraphic_i::SetVisibility( CORBA::Long    theViewId,
                                                 CORBA::Boolean theValue )
{
  SALOMEDS::Locker lock;    
  if ( !_impl.IsNull() )
    Handle(SALOMEDSImpl_AttributeGraphic)::DownCast(_impl)->SetVisibility( theViewId, theValue );
}

//=======================================================================
// function : SALOMEDS_AttributeGraphic_i::~SALOMEDS_AttributeGraphic_i
// purpose  : Get visibility of object in given view
//=======================================================================                                     
CORBA::Boolean SALOMEDS_AttributeGraphic_i::GetVisibility( CORBA::Long theViewId )
{
  SALOMEDS::Locker lock;
  return !_impl.IsNull() ? Handle(SALOMEDSImpl_AttributeGraphic)::DownCast(_impl)->GetVisibility( theViewId ) : false;
}


















