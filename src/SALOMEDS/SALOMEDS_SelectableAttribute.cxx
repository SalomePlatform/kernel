using namespace std;
//  File      : SALOMEDS_SelectableAttribute.cxx
//  Created   : Wed Nov 28 16:09:35 2001
//  Author    : Yves FRICAUD
//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header:  

#include "SALOMEDS_SelectableAttribute.ixx"
#include <TDataStd_Integer.hxx>

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_SelectableAttribute::GetID () 
{
  static Standard_GUID SALOMEDS_SelectableAttributeID ("12837188-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_SelectableAttributeID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDS_SelectableAttribute) SALOMEDS_SelectableAttribute::Set (const TDF_Label& L,
                                                                  const Standard_Integer value) 
{
  Handle(SALOMEDS_SelectableAttribute) A;
  if (!L.FindAttribute(SALOMEDS_SelectableAttribute::GetID(),A)) {
    A = new  SALOMEDS_SelectableAttribute(); 
    L.AddAttribute(A);
  }
  
  (Handle(TDataStd_Integer)::DownCast(A))->Set (value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDS_SelectableAttribute::SALOMEDS_SelectableAttribute()
{
  TDataStd_Integer::Set(0);
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_SelectableAttribute::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDS_SelectableAttribute::NewEmpty () const
{  
  return new SALOMEDS_SelectableAttribute(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDS_SelectableAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  Standard_Integer s = Handle(TDataStd_Integer)::DownCast (with)->Get ();
  TDataStd_Integer::Set(s);
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDS_SelectableAttribute::Paste (const Handle(TDF_Attribute)& into,
                                    const Handle(TDF_RelocationTable)& RT) const
{
  Handle(TDataStd_Integer)::DownCast (into)->Set (Get());
}

