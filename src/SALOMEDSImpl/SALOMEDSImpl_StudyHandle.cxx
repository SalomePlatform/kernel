//  File   : SALOMEDSImpl_StudyHandle.cxx
//  Author : Sergey LITONIN
//  Module : SALOME

#include "SALOMEDSImpl_StudyHandle.hxx"
#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>

/*
  Class       : SALOMEDSImpl_StudyHandle
  Description : This class is intended for storing information about
                graphic representation of objects in dirrent views
*/

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_StudyHandle, TDF_Attribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_StudyHandle, TDF_Attribute )

//=======================================================================
//function : GetID
//purpose  : Get GUID of this attribute
//=======================================================================
const Standard_GUID& SALOMEDSImpl_StudyHandle::GetID()
{
  static Standard_GUID SALOMEDSImpl_StudyHandleID( "050C9555-4BA8-49bf-8F1C-086F0469A40B" );
  return SALOMEDSImpl_StudyHandleID;
}

//=======================================================================
//function : SALOMEDSImpl_StudyHandle
//purpose  : Empty Constructor
//=======================================================================
SALOMEDSImpl_StudyHandle::SALOMEDSImpl_StudyHandle()
{
  myHandle.Nullify();
}

//=======================================================================
//function : Set
//purpose  : 
//=======================================================================
Handle(SALOMEDSImpl_StudyHandle) SALOMEDSImpl_StudyHandle::Set(const TDF_Label& theLabel, 
							       const Handle(SALOMEDSImpl_Study)& theStudy)
{
  Handle(SALOMEDSImpl_StudyHandle) A;
  if (!theLabel.FindAttribute(GetID(), A)) {
    A = new  SALOMEDSImpl_StudyHandle(); 
    theLabel.AddAttribute(A);
  }

  A->SetHandle(theStudy);
  return A;  
}


//=======================================================================
//function : ID
//purpose  : Get GUID of this attribute
//=======================================================================
const Standard_GUID& SALOMEDSImpl_StudyHandle::ID () const
{
  return GetID();
}


//=======================================================================
//function : NewEmpty
//purpose  : Create new empty attribute
//=======================================================================
Handle(TDF_Attribute) SALOMEDSImpl_StudyHandle::NewEmpty () const
{
  return new SALOMEDSImpl_StudyHandle ();
}


//=======================================================================
//function : Restore
//purpose  : Restore value of attribute with value of theWith one
//=======================================================================
void SALOMEDSImpl_StudyHandle::Restore( const Handle(TDF_Attribute)& theWith )
{
  Handle(SALOMEDSImpl_StudyHandle) anAttr = Handle(SALOMEDSImpl_StudyHandle)::DownCast( theWith );
  if ( !anAttr.IsNull() ) SetHandle( anAttr->GetHandle() );
}

//=======================================================================
//function : Paste
//purpose  : Paste value of current attribute to the value of entry one
//=======================================================================
void SALOMEDSImpl_StudyHandle::Paste( const Handle(TDF_Attribute)& theInto,
                                       const Handle(TDF_RelocationTable)& ) const
{
  Handle(SALOMEDSImpl_StudyHandle) anAttr =  Handle(SALOMEDSImpl_StudyHandle)::DownCast( theInto );
  if ( !anAttr.IsNull() ) anAttr->SetHandle( myHandle );
}

