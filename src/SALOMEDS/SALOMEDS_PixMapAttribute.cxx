using namespace std;
//  File      : SALOMEDS_PixMapAttribute.cxx
//  Created   : Wed Nov 28 16:23:19 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header: 

#include "SALOMEDS_PixMapAttribute.ixx"
#include <TDataStd_Comment.hxx>
#include <TCollection_ExtendedString.hxx>

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_PixMapAttribute::GetID () 
{
  static Standard_GUID SALOMEDS_PixMapAttributeID ("12837187-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_PixMapAttributeID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDS_PixMapAttribute) SALOMEDS_PixMapAttribute::Set (const TDF_Label& L,
							  const TCollection_ExtendedString& S) 
{
  Handle(SALOMEDS_PixMapAttribute) A;
  if (!L.FindAttribute(SALOMEDS_PixMapAttribute::GetID(),A)) {
    A = new  SALOMEDS_PixMapAttribute(); 
    L.AddAttribute(A);
  }
  
  (Handle(TDataStd_Comment)::DownCast(A))->Set (S); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDS_PixMapAttribute::SALOMEDS_PixMapAttribute()
{TDataStd_Comment::Set("None");}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_PixMapAttribute::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDS_PixMapAttribute::NewEmpty () const
{  
  return new SALOMEDS_PixMapAttribute(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDS_PixMapAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  TCollection_ExtendedString s = Handle(TDataStd_Comment)::DownCast (with)->Get ();
  TDataStd_Comment::Set(s);
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDS_PixMapAttribute::Paste (const Handle(TDF_Attribute)& into,
		           const Handle(TDF_RelocationTable)& RT) const
{
  Handle(TDataStd_Comment)::DownCast (into)->Set (Get());
}

