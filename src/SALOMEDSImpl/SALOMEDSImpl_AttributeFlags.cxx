//  File   : SALOMEDSImpl_AttributeFlags.cxx
//  Author : Sergey LITONIN
//  Module : SALOME

#include "SALOMEDSImpl_AttributeFlags.hxx"
#include <Standard_GUID.hxx>

using namespace std;


/*
  Class       : SALOMEDSImpl_AttributeFlags
  Description : This class is intended for storing different object attributes that
                have only two states (0 and 1).
*/

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeFlags, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeFlags, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::GetID
//purpose  : Get GUID of this attribute
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeFlags::GetID ()
{
  static Standard_GUID SALOMEDSImpl_AttributeFlagsID( "866EEC9F-A517-4cb4-88E6-E208DB8FC96F" );
  return SALOMEDSImpl_AttributeFlagsID;                
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::Set
//purpose  : Set value of the attribute
//=======================================================================
Handle(SALOMEDSImpl_AttributeFlags) SALOMEDSImpl_AttributeFlags::Set(const TDF_Label& L,
								     const Standard_Integer value )
{
  Handle(SALOMEDSImpl_AttributeFlags) A;
  if ( !L.FindAttribute(SALOMEDSImpl_AttributeFlags::GetID(),A ) )
  {
    A = new  SALOMEDSImpl_AttributeFlags();
    L.AddAttribute( A );
  }

  A->Set( value );
  return A;
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::SALOMEDSImpl_AttributeFlags
//purpose  : Constructor
//=======================================================================
SALOMEDSImpl_AttributeFlags::SALOMEDSImpl_AttributeFlags()
:SALOMEDSImpl_GenericAttribute("AttributeFlags")
{
  myValue = 0;
}

SALOMEDSImpl_AttributeFlags::~SALOMEDSImpl_AttributeFlags()
{
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::ID
//purpose  : Get GUID of this attribute
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeFlags::ID () const
{
  return GetID();
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::NewEmpty
//purpose  : Create new empty attribute
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeFlags::NewEmpty () const
{
  return new SALOMEDSImpl_AttributeFlags();
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::Restore
//purpose  : Assign given value to the attribute
//=======================================================================
void SALOMEDSImpl_AttributeFlags::Restore( const Handle(TDF_Attribute)& with )
{
  myValue = Handle(SALOMEDSImpl_AttributeFlags)::DownCast( with )->Get();
  return;
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::Paste
//purpose  : Assign internal value to the given attribute
//=======================================================================
void SALOMEDSImpl_AttributeFlags::Paste(const Handle(TDF_Attribute)& into,
					const Handle(TDF_RelocationTable)& RT ) const
{
  Handle(SALOMEDSImpl_AttributeFlags)::DownCast( into )->Set( myValue );
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::Set
//purpose  : Set value
//=======================================================================
void SALOMEDSImpl_AttributeFlags::Set( const Standard_Integer v )
{
  Backup();
  myValue=v;
}

//=======================================================================
//function : SALOMEDSImpl_AttributeFlags::Get
//purpose  : GetValue
//=======================================================================
Standard_Integer SALOMEDSImpl_AttributeFlags::Get() const
{
  return myValue;
}




















