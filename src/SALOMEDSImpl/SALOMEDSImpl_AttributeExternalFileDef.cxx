//  File   : SALOMEDSImpl_AttributeExternalFileDef.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDSImpl_AttributeExternalFileDef.hxx"

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeExternalFileDef, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeExternalFileDef, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeExternalFileDef::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeExternalFileDefID ("7123AD4C-ACDB-4e3a-8FDC-70EA164D2CBE");
  return SALOMEDSImpl_AttributeExternalFileDefID;
}

//=======================================================================
//function : Set
//purpose  : 
//=======================================================================
Handle(SALOMEDSImpl_AttributeExternalFileDef) 
SALOMEDSImpl_AttributeExternalFileDef::Set (const TDF_Label& L, const TCollection_ExtendedString& S) 
{

  Handle(SALOMEDSImpl_AttributeExternalFileDef) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeExternalFileDef::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeExternalFileDef(); 
    L.AddAttribute(A);
  }
  
  A->SetValue (S); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeExternalFileDef::SALOMEDSImpl_AttributeExternalFileDef()
:SALOMEDSImpl_GenericAttribute("AttributeExternalFileDef")
{
}

//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeExternalFileDef::SetValue (const TCollection_ExtendedString& S)
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
const Standard_GUID& SALOMEDSImpl_AttributeExternalFileDef::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================
Handle(TDF_Attribute) SALOMEDSImpl_AttributeExternalFileDef::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeExternalFileDef(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeExternalFileDef::Restore(const Handle(TDF_Attribute)& with) 
{
  myString = Handle(SALOMEDSImpl_AttributeExternalFileDef)::DownCast (with)->Value ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeExternalFileDef::Paste (const Handle(TDF_Attribute)& into,
						   const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeExternalFileDef) anAttr = Handle(SALOMEDSImpl_AttributeExternalFileDef)::DownCast(into); 
  anAttr->SetValue(myString);
}

