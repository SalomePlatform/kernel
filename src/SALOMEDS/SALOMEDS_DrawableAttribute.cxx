using namespace std;
//  File      : SALOMEDS_DrawableAttribute.cxx
//  Created   : Wed Nov 28 16:09:35 2001
//  Author    : Yves FRICAUD
//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header:  

#include "SALOMEDS_DrawableAttribute.ixx"
#include <TDataStd_Integer.hxx>

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_DrawableAttribute::GetID () 
{
  static Standard_GUID SALOMEDS_DrawableAttributeID ("12837184-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_DrawableAttributeID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDS_DrawableAttribute) SALOMEDS_DrawableAttribute::Set (const TDF_Label& L,
                                                                  const Standard_Integer value) 
{
  Handle(SALOMEDS_DrawableAttribute) A;
  if (!L.FindAttribute(SALOMEDS_DrawableAttribute::GetID(),A)) {
    A = new  SALOMEDS_DrawableAttribute(); 
    L.AddAttribute(A);
  }
  
  (Handle(TDataStd_Integer)::DownCast(A))->Set (value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDS_DrawableAttribute::SALOMEDS_DrawableAttribute()
{
  TDataStd_Integer::Set(0);
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_DrawableAttribute::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDS_DrawableAttribute::NewEmpty () const
{  
  return new SALOMEDS_DrawableAttribute(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDS_DrawableAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  Standard_Integer s = Handle(TDataStd_Integer)::DownCast (with)->Get ();
  TDataStd_Integer::Set(s);
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDS_DrawableAttribute::Paste (const Handle(TDF_Attribute)& into,
                                    const Handle(TDF_RelocationTable)& RT) const
{
  Handle(TDataStd_Integer)::DownCast (into)->Set (Get());
}

