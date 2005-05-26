//  File   : SALOMEDS_AttributeFlags_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeFlags_i.hxx"
#include "SALOMEDS.hxx"  

/*
  Class       : SALOMEDS_AttributeFlags_i
  Description : This class is intended for storing different object attributes that
                have only two states (0 and 1).
                
                Avalable attributes:
                
                IS_VISIBLE - is equal to 1 if object is visible in 3D view (0 - overwise).
                             This attribute is valid for active view only.
*/


//=======================================================================
// function : SALOMEDS_AttributeFlags_i::GetFlags
// purpose  : Get all flags as integer value
//=======================================================================
CORBA::Long SALOMEDS_AttributeFlags_i::GetFlags()
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeFlags)::DownCast(_impl)->Get();
}

//=======================================================================
// function : SALOMEDS_AttributeFlags_i::SetFlags
// purpose  : Set all flags as integer value
//=======================================================================
void SALOMEDS_AttributeFlags_i::SetFlags( CORBA::Long theFlags )
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeFlags)::DownCast(_impl)->Set( theFlags );
}

//=======================================================================
// function : SALOMEDS_AttributeFlags_i::Get
// purpose  : Get specified flag
//=======================================================================
CORBA::Boolean SALOMEDS_AttributeFlags_i::Get( CORBA::Long theFlag )
{
  SALOMEDS::Locker lock;
  return Handle(SALOMEDSImpl_AttributeFlags)::DownCast(_impl)->Get() & theFlag ? true : false;
}

//=======================================================================
// function : SALOMEDS_AttributeFlags_i::Set
// purpose  : Set/Unset specified flag
//=======================================================================
void SALOMEDS_AttributeFlags_i::Set( CORBA::Long theFlag, CORBA::Boolean theValue )
{
  SALOMEDS::Locker lock;
  Handle(SALOMEDSImpl_AttributeFlags) anAttr = Handle(SALOMEDSImpl_AttributeFlags)::DownCast(_impl);
  if ( theValue )
    anAttr->Set( anAttr->Get() | theFlag );
  else
    anAttr->Set( anAttr->Get() & ~theFlag );
}


















