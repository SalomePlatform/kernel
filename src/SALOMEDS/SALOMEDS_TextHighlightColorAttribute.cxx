using namespace std;
//  File      : SALOMEDS_TextHighlightColorAttribute.cxx
//  Created   : Wed Nov 28 16:09:35 2001
//  Author    : Yves FRICAUD
//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header: 

#include "SALOMEDS_TextHighlightColorAttribute.ixx"

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_TextHighlightColorAttribute::GetID () 
{
  static Standard_GUID SALOMEDS_TextHighlightColorAttributeID ("12837190-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_TextHighlightColorAttributeID;
}



//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDS_TextHighlightColorAttribute::SALOMEDS_TextHighlightColorAttribute()
{Init(1,3);}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_TextHighlightColorAttribute::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDS_TextHighlightColorAttribute::NewEmpty () const
{  
  return new SALOMEDS_TextHighlightColorAttribute(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDS_TextHighlightColorAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  Handle(TColStd_HArray1OfReal) s = Handle(TDataStd_RealArray)::DownCast (with)->Array ();
  TDataStd_RealArray::ChangeArray(s);
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDS_TextHighlightColorAttribute::Paste (const Handle(TDF_Attribute)& into,
                                                  const Handle(TDF_RelocationTable)& ) const
{
  Handle(TDataStd_RealArray)::DownCast (into)->ChangeArray (Array());
}

