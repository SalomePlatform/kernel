using namespace std;
//  File      : SALOMEDS_PersRefAttribute.cxx
//  Created   : Wed Nov 28 16:23:19 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#include "SALOMEDS_PersRefAttribute.ixx"
#include <TDataStd_Comment.hxx>
#include <TCollection_ExtendedString.hxx>

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_PersRefAttribute::GetID () 
{
  static Standard_GUID SALOMEDS_PersRefAttributeID ("92888E06-7074-11d5-A690-0800369C8A03");
  return SALOMEDS_PersRefAttributeID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDS_PersRefAttribute) SALOMEDS_PersRefAttribute::Set (const TDF_Label& L,
							  const TCollection_ExtendedString& S) 
{
  Handle(SALOMEDS_PersRefAttribute) A;
  if (!L.FindAttribute(SALOMEDS_PersRefAttribute::GetID(),A)) {
    A = new  SALOMEDS_PersRefAttribute(); 
    L.AddAttribute(A);
  }
  
  (Handle(TDataStd_Comment)::DownCast(A))->Set (S); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDS_PersRefAttribute::SALOMEDS_PersRefAttribute()
{}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_PersRefAttribute::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDS_PersRefAttribute::NewEmpty () const
{  
  return new SALOMEDS_PersRefAttribute(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDS_PersRefAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  TCollection_ExtendedString s = Handle(TDataStd_Comment)::DownCast (with)->Get ();
  TDataStd_Comment::Set(s);
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDS_PersRefAttribute::Paste (const Handle(TDF_Attribute)& into,
		           const Handle(TDF_RelocationTable)& RT) const
{
  Handle(TDataStd_Comment)::DownCast (into)->Set (Get());
}

