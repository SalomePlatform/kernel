using namespace std;
//  File      : SALOMEDS_LocalIDAttribute.cxx
//  Created   : Tue Aug 13 14:05:03 2002
//  Author    : Yves FRICAUD
//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header:  

#include "SALOMEDS_LocalIDAttribute.ixx"
#include <TDataStd_Integer.hxx>

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_LocalIDAttribute::GetID () 
{
  static Standard_GUID SALOMEDS_OpenedAttributeID ("12837196-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_OpenedAttributeID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDS_LocalIDAttribute) SALOMEDS_LocalIDAttribute::Set (const TDF_Label& L,
                                                                  const Standard_Integer value) 
{
  Handle(SALOMEDS_LocalIDAttribute) A;
  if (!L.FindAttribute(SALOMEDS_LocalIDAttribute::GetID(),A)) {
    A = new  SALOMEDS_LocalIDAttribute(); 
    L.AddAttribute(A);
  }
  
  (Handle(TDataStd_Integer)::DownCast(A))->Set (value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDS_LocalIDAttribute::SALOMEDS_LocalIDAttribute()
{
  TDataStd_Integer::Set(0); 
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_LocalIDAttribute::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDS_LocalIDAttribute::NewEmpty () const
{  
  return new SALOMEDS_LocalIDAttribute(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDS_LocalIDAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  Standard_Integer s = Handle(TDataStd_Integer)::DownCast (with)->Get ();
  TDataStd_Integer::Set(s);
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDS_LocalIDAttribute::Paste (const Handle(TDF_Attribute)& into,
				       const Handle(TDF_RelocationTable)& RT) const
{
  Handle(TDataStd_Integer)::DownCast (into)->Set (Get());
}

