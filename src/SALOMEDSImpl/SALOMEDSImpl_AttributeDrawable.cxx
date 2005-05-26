//  File   : SALOMEDSImpl_AttributeDrawable.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDSImpl_AttributeDrawable.hxx"

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeDrawable, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeDrawable, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeDrawable::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeDrawableID ("12837184-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeDrawableID;
}


//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributeDrawable) SALOMEDSImpl_AttributeDrawable::Set (const TDF_Label& L,
									    const Standard_Integer value) 
{
  Handle(SALOMEDSImpl_AttributeDrawable) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeDrawable::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeDrawable(); 
    L.AddAttribute(A);
  }
  
  A->SetDrawable (value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeDrawable::SALOMEDSImpl_AttributeDrawable()
:SALOMEDSImpl_GenericAttribute("AttributeDrawable") 
{
  myValue = 0;
}

//=======================================================================
//function : SetDrawable
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeDrawable::SetDrawable(const Standard_Integer theValue)
{
  CheckLocked();

  Backup();

  (theValue!=0)?myValue=1:myValue=0;
}
           

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeDrawable::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeDrawable::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeDrawable(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeDrawable::Restore(const Handle(TDF_Attribute)& with) 
{
  myValue = Handle(SALOMEDSImpl_AttributeDrawable)::DownCast (with)->IsDrawable ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeDrawable::Paste (const Handle(TDF_Attribute)& into,
                                    const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeDrawable)::DownCast (into)->SetDrawable (myValue);
}

