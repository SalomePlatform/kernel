using namespace std;
//  File      : SALOMEDS_TextColorAttribute.cxx
//  Created   : Wed Nov 28 16:09:35 2001
//  Author    : Yves FRICAUD
//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header: 

#include "SALOMEDS_TextColorAttribute.ixx"

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_TextColorAttribute::GetID () 
{
  static Standard_GUID SALOMEDS_TextColorAttributeID ("12837189-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDS_TextColorAttributeID;
}




//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDS_TextColorAttribute::SALOMEDS_TextColorAttribute()
{Init(1,3);}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDS_TextColorAttribute::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDS_TextColorAttribute::NewEmpty () const
{  
  return new SALOMEDS_TextColorAttribute(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDS_TextColorAttribute::Restore(const Handle(TDF_Attribute)& with) 
{
  Handle(TColStd_HArray1OfReal) s = Handle(TDataStd_RealArray)::DownCast (with)->Array ();
  TDataStd_RealArray::ChangeArray(s);
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDS_TextColorAttribute::Paste (const Handle(TDF_Attribute)& into,
                                    const Handle(TDF_RelocationTable)& RT) const
{
  Handle(TDataStd_RealArray)::DownCast (into)->ChangeArray (Array());
}

