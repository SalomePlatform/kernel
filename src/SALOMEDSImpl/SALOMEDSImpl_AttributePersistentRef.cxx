//  File   : SALOMEDSImpl_AttributePersistentRef.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDSImpl_AttributePersistentRef.hxx"
#include <Standard_GUID.hxx>

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributePersistentRef, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributePersistentRef, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributePersistentRef::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributePersistentRefID ("92888E06-7074-11d5-A690-0800369C8A03");
  return SALOMEDSImpl_AttributePersistentRefID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributePersistentRef) SALOMEDSImpl_AttributePersistentRef::Set (const TDF_Label& L,
										      const TCollection_ExtendedString& S)
{
  Handle(SALOMEDSImpl_AttributePersistentRef) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributePersistentRef::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributePersistentRef(); 
    L.AddAttribute(A);
  }
  
  A->SetValue (S); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributePersistentRef::SALOMEDSImpl_AttributePersistentRef()
:SALOMEDSImpl_GenericAttribute("AttributePersistentRef")
{
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributePersistentRef::ID () const { return GetID(); }


//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributePersistentRef::SetValue (const TCollection_ExtendedString& S)
{
  CheckLocked();

  if(myString == S) return;

  Backup();

  myString = S;
}

//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributePersistentRef::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributePersistentRef(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributePersistentRef::Restore(const Handle(TDF_Attribute)& with) 
{
  myString = Handle(SALOMEDSImpl_AttributePersistentRef)::DownCast (with)->Value ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributePersistentRef::Paste (const Handle(TDF_Attribute)& into,
						 const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributePersistentRef)::DownCast (into)->SetValue(myString);
}

