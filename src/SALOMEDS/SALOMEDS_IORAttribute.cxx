using namespace std;
//  File      : SALOMEDS_IORAttribute.cxx
//  Created   : Wed Nov 28 16:09:35 2001
//  Author    : Yves FRICAUD
//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#include "SALOMEDS_IORAttribute.ixx"
#include <TDataStd_Comment.hxx>
#include <TCollection_ExtendedString.hxx>
#include "SALOMEDS_Study_i.hxx"

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_IORAttribute::GetID () 
{
  static Standard_GUID SALOMEDS_IORAttributeID ("92888E01-7074-11d5-A690-0800369C8A03");
  return SALOMEDS_IORAttributeID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDS_IORAttribute) SALOMEDS_IORAttribute::Set (const TDF_Label& L,
							  const TCollection_ExtendedString& S,
							  CORBA::ORB_ptr orb) 
{
  Handle(SALOMEDS_IORAttribute) A;
  if (!L.FindAttribute(SALOMEDS_IORAttribute::GetID(),A)) {
    A = new  SALOMEDS_IORAttribute(); 
    L.AddAttribute(A);
  }
  
  (Handle(TDataStd_Comment)::DownCast(A))->Set (S); 
  SALOMEDS_Study_i::IORUpdated(A,orb);
  return A;
}

//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDS_IORAttribute::SALOMEDS_IORAttribute()
{}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_IORAttribute::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDS_IORAttribute::NewEmpty () const
{  
  return new SALOMEDS_IORAttribute(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDS_IORAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  TCollection_ExtendedString s = Handle(TDataStd_Comment)::DownCast (with)->Get ();
  TDataStd_Comment::Set(s);
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDS_IORAttribute::Paste (const Handle(TDF_Attribute)& into,
		           const Handle(TDF_RelocationTable)& RT) const
{
  Handle(TDataStd_Comment)::DownCast (into)->Set (Get());
}

