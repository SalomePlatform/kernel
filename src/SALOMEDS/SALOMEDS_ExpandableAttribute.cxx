using namespace std;
//  File      : SALOMEDS_ExpandableAttribute.cxx
//  Created   : Wed Nov 28 16:09:35 2001
//  Author    : Yves FRICAUD
//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header:  

#include "SALOMEDS_ExpandableAttribute.ixx"
#include <TDataStd_Integer.hxx>

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_ExpandableAttribute::GetID () 
{
  static Standard_GUID SALOMEDS_ExpandableAttributeID ("12837185-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_ExpandableAttributeID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDS_ExpandableAttribute) SALOMEDS_ExpandableAttribute::Set (const TDF_Label& L,
                                                                  const Standard_Integer value) 
{
  Handle(SALOMEDS_ExpandableAttribute) A;
  if (!L.FindAttribute(SALOMEDS_ExpandableAttribute::GetID(),A)) {
    A = new  SALOMEDS_ExpandableAttribute(); 
    L.AddAttribute(A);
  }
  
  (Handle(TDataStd_Integer)::DownCast(A))->Set (value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDS_ExpandableAttribute::SALOMEDS_ExpandableAttribute()
{
  TDataStd_Integer::Set(0);
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_ExpandableAttribute::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDS_ExpandableAttribute::NewEmpty () const
{  
  return new SALOMEDS_ExpandableAttribute(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDS_ExpandableAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  Standard_Integer s = Handle(TDataStd_Integer)::DownCast (with)->Get ();
  TDataStd_Integer::Set(s);
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDS_ExpandableAttribute::Paste (const Handle(TDF_Attribute)& into,
                                    const Handle(TDF_RelocationTable)& RT) const
{
  Handle(TDataStd_Integer)::DownCast (into)->Set (Get());
}

