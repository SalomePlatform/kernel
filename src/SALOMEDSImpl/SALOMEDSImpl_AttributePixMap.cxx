//  File   : SALOMEDSImpl_AttributePixMap.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDSImpl_AttributePixMap.hxx"
#include <TCollection_ExtendedString.hxx>
#include <Standard_GUID.hxx>

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributePixMap, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributePixMap, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributePixMap::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributePixMapID ("12837187-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributePixMapID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributePixMap) SALOMEDSImpl_AttributePixMap::Set (const TDF_Label& L,
									const TCollection_ExtendedString& S) 
{
  Handle(SALOMEDSImpl_AttributePixMap) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributePixMap::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributePixMap(); 
    L.AddAttribute(A);
  }
  
  A->SetPixMap (S); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributePixMap::SALOMEDSImpl_AttributePixMap()
:SALOMEDSImpl_GenericAttribute("AttributePixMap")
{
  myString = "None";
}

//=======================================================================
//function : SetPixMap
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributePixMap::SetPixMap (const TCollection_ExtendedString& S)
{
  CheckLocked();

  if(myString == S) return;

  Backup();

  myString = S;
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributePixMap::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributePixMap::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributePixMap(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributePixMap::Restore(const Handle(TDF_Attribute)& with) 
{
  myString = Handle(SALOMEDSImpl_AttributePixMap)::DownCast (with)->GetPixMap ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributePixMap::Paste (const Handle(TDF_Attribute)& into,
		           const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributePixMap)::DownCast (into)->SetPixMap (myString);
}

