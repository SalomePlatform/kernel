//  File   : SALOMEDSImpl_AttributeFileType.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDSImpl_AttributeFileType.hxx"

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeFileType, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeFileType, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeFileType::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeFileTypeID ("0181B525-3F15-4ab2-9DE3-5E2F54B5F340");
  return SALOMEDSImpl_AttributeFileTypeID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributeFileType) SALOMEDSImpl_AttributeFileType::Set (const TDF_Label& L,
									    const TCollection_ExtendedString& S) 
{

  Handle(SALOMEDSImpl_AttributeFileType) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeFileType::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeFileType(); 
    L.AddAttribute(A);
  }
  
  A->SetValue (S); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeFileType::SALOMEDSImpl_AttributeFileType()
:SALOMEDSImpl_GenericAttribute("AttributeFileType")
{
}

//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeFileType::SetValue (const TCollection_ExtendedString& S)
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

const Standard_GUID& SALOMEDSImpl_AttributeFileType::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeFileType::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeFileType(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeFileType::Restore(const Handle(TDF_Attribute)& with) 
{
  myString = Handle(SALOMEDSImpl_AttributeFileType)::DownCast (with)->Value ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeFileType::Paste (const Handle(TDF_Attribute)& into,
					    const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeFileType)::DownCast (into)->SetValue (myString);
}

