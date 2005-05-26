//  File   : SALOMEDSImpl_AttributeExpandable.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDSImpl_AttributeExpandable.hxx"

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeExpandable, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeExpandable, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeExpandable::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeExpandableID ("12837185-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeExpandableID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================
Handle(SALOMEDSImpl_AttributeExpandable) SALOMEDSImpl_AttributeExpandable::Set (const TDF_Label& L,
										const Standard_Integer value) 
{
  Handle(SALOMEDSImpl_AttributeExpandable) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeExpandable::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeExpandable(); 
    L.AddAttribute(A);
  }
  
  A->SetExpandable(value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeExpandable::SALOMEDSImpl_AttributeExpandable()
:SALOMEDSImpl_GenericAttribute("AttributeExpandable")
{
  myValue = 0;
}

//=======================================================================
//function : SetExpandable
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeExpandable::SetExpandable(const Standard_Integer theValue)
{
  CheckLocked();

  Backup();

  (theValue!=0)?myValue=1:myValue=0;
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeExpandable::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeExpandable::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeExpandable(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeExpandable::Restore(const Handle(TDF_Attribute)& with) 
{
  myValue = Handle(SALOMEDSImpl_AttributeExpandable)::DownCast (with)->IsExpandable ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeExpandable::Paste (const Handle(TDF_Attribute)& into,
                                    const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeExpandable)::DownCast (into)->SetExpandable (myValue);
}

