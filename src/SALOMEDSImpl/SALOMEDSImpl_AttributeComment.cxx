//  File   : SALOMEDSImpl_AttributeComment.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef WNT
using namespace std;
#endif
#include "SALOMEDSImpl_AttributeComment.hxx"

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeComment, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeComment, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  :
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeComment::GetID ()
{
  static Standard_GUID CommentID ("7AF2F7CC-1CA2-4476-BE95-8ACC996BC7B9");
  return CommentID;
}   

Handle(SALOMEDSImpl_AttributeComment) SALOMEDSImpl_AttributeComment::Set (const TDF_Label& L, 
									  const TCollection_ExtendedString& Val) 
{
  Handle(SALOMEDSImpl_AttributeComment) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeComment::GetID(), A)) {
    A = new  SALOMEDSImpl_AttributeComment(); 
    L.AddAttribute(A);
  }

  A->SetValue(Val);     
    
  return A;
}

//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeComment::SetValue (const TCollection_ExtendedString& S)
{
  CheckLocked();

  if(myString == S) return;

  Backup();

  myString = S;
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}
           

//=======================================================================
//function : ID
//purpose  :
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeComment::ID () const { return GetID(); } 

//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================
Handle(TDF_Attribute) SALOMEDSImpl_AttributeComment::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeComment(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeComment::Restore(const Handle(TDF_Attribute)& with) 
{
  myString = Handle(SALOMEDSImpl_AttributeComment)::DownCast (with)->Value ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeComment::Paste (const Handle(TDF_Attribute)& into,
					   const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeComment)::DownCast (into)->SetValue(myString);
}
