//  File   : SALOMEDSImpl_AttributeReference.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDSImpl_AttributeReference.hxx"
#include <TDF_Tool.hxx>
#include <TDF_Data.hxx>
#include <TDF_RelocationTable.hxx>

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeReference, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeReference, SALOMEDSImpl_GenericAttribute )


//=======================================================================
//function : GetID
//purpose  :
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeReference::GetID ()
{
  static Standard_GUID refID ("D913E0B3-0A9F-4ea6-9480-18A9B72D9D86");
  return refID;
} 

Handle(SALOMEDSImpl_AttributeReference) SALOMEDSImpl_AttributeReference::Set(const TDF_Label& theLabel, 
									     const TDF_Label& theRefLabel)
{
  Handle(SALOMEDSImpl_AttributeReference) A;
  if (!theLabel.FindAttribute(SALOMEDSImpl_AttributeReference::GetID(), A)) {
    A = new  SALOMEDSImpl_AttributeReference(); 
    theLabel.AddAttribute(A);
  }

  A->Set(theRefLabel);  
  return A;  
}

//=======================================================================
//function : Set
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeReference::Set(const TDF_Label& Origin)
{
  CheckLocked();

  if(myLabel == Origin) return;

  Backup();
  myLabel = Origin;

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}  

//=======================================================================
//function : ID
//purpose  :
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeReference::ID () const { return GetID(); } 


TCollection_AsciiString SALOMEDSImpl_AttributeReference::Save() 
{
  TCollection_AsciiString anEntry;
  TDF_Tool::Entry(myLabel, anEntry);
  return anEntry;
}

void SALOMEDSImpl_AttributeReference::Load(const TCollection_AsciiString& value) 
{
  TDF_Label aLabel;
  TDF_Tool::Label(Label().Data(), value, aLabel);
  myLabel = aLabel;
}

//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================
Handle(TDF_Attribute) SALOMEDSImpl_AttributeReference::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeReference(); 
}

//=======================================================================
//function : Restore
//purpose  :
//=======================================================================

void SALOMEDSImpl_AttributeReference::Restore(const Handle(TDF_Attribute)& With)
{
  myLabel = Handle(SALOMEDSImpl_AttributeReference)::DownCast (With)->Get ();
}

//=======================================================================
//function : Paste
//purpose  :
//=======================================================================

void SALOMEDSImpl_AttributeReference::Paste (const Handle(TDF_Attribute)& Into,
					     const Handle(TDF_RelocationTable)& RT) const
{
  TDF_Label tLab;
  if (!myLabel.IsNull()) {
    if (!RT->HasRelocation(myLabel,tLab)) tLab = myLabel;
  }
  Handle(SALOMEDSImpl_AttributeReference)::DownCast(Into)->Set(tLab);
} 
