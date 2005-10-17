//  File   : SALOMEDSImpl_AttributeTarget.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDSImpl_AttributeTarget.hxx"
#include "SALOMEDSImpl_AttributeReference.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include <TDF_RelocationTable.hxx>
#include <TDF_ListIteratorOfAttributeList.hxx>
#include <Standard_GUID.hxx>

using namespace std;


IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeTarget, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeTarget, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeTarget::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeTargetID ("12837197-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeTargetID;
}


//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributeTarget) SALOMEDSImpl_AttributeTarget::Set (const TDF_Label& L) 
{
  Handle(SALOMEDSImpl_AttributeTarget) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeTarget::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeTarget(); 
    L.AddAttribute(A);
  }
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeTarget::SALOMEDSImpl_AttributeTarget()
:SALOMEDSImpl_GenericAttribute("AttributeTarget")
{
}

void SALOMEDSImpl_AttributeTarget::SetRelation(const TCollection_ExtendedString& theRelation)
{
  CheckLocked();
  if(myRelation == theRelation) return;

  Backup();
  myRelation = theRelation; 
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//function : Add
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeTarget::Add(const Handle(SALOMEDSImpl_SObject)& theSO) 
{
  Backup();
  TDF_Label aRefLabel = theSO->GetLabel();
  Handle(SALOMEDSImpl_AttributeReference) aReference;
  if (aRefLabel.FindAttribute(SALOMEDSImpl_AttributeReference::GetID(),aReference)) {
    TDF_ListIteratorOfAttributeList anIter(GetVariables());
    for(;anIter.More();anIter.Next()) if(anIter.Value()->Label() == aRefLabel) return; //BugID: PAL6192    
    GetVariables().Append(aReference);
  } 
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//function : Get
//purpose  : 
//=======================================================================
Handle(TColStd_HSequenceOfTransient) SALOMEDSImpl_AttributeTarget::Get() 
{
  Handle(TColStd_HSequenceOfTransient) aSeq = new TColStd_HSequenceOfTransient;
  
  TDF_ListIteratorOfAttributeList anIter(GetVariables());
  for(;anIter.More();anIter.Next()) {
    const TDF_Label& aLabel = anIter.Value()->Label();
   aSeq->Append( SALOMEDSImpl_Study::SObject(aLabel));
  }
  return aSeq;
}

//=======================================================================
//function : Remove
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeTarget::Remove(const Handle(SALOMEDSImpl_SObject)& theSO) 
{
  Backup();
  TDF_Label aRefLabel = theSO->GetLabel();
  TDF_ListIteratorOfAttributeList anIter(GetVariables());
  for(;anIter.More();anIter.Next()) {
    if (anIter.Value()->Label() == aRefLabel) {
      GetVariables().Remove(anIter);
      return;
    }
  }  
  
  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeTarget::ID () const { return GetID(); }

//=======================================================================
//function : Restore
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeTarget::Restore(const Handle(TDF_Attribute)& With)
{
  Handle(SALOMEDSImpl_AttributeTarget) REL = Handle(SALOMEDSImpl_AttributeTarget)::DownCast (With);
  myRelation = REL->GetRelation();
  Handle(SALOMEDSImpl_AttributeReference) V;
  myVariables.Clear();
  for (TDF_ListIteratorOfAttributeList it (REL->GetVariables()); it.More(); it.Next()) {
    V = Handle(SALOMEDSImpl_AttributeReference)::DownCast(it.Value());
    myVariables.Append(V);
  }
}

//=======================================================================
//function : NewEmpty
//purpose  :
//=======================================================================
Handle(TDF_Attribute) SALOMEDSImpl_AttributeTarget::NewEmpty() const
{
  return new SALOMEDSImpl_AttributeTarget();
}

//=======================================================================
//function : Paste
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeTarget::Paste(const Handle(TDF_Attribute)& Into,
					 const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeTarget) REL = Handle(SALOMEDSImpl_AttributeTarget)::DownCast (Into);
  REL->SetRelation(myRelation);
  Handle(SALOMEDSImpl_AttributeReference) V1,V2;
  for (TDF_ListIteratorOfAttributeList it (myVariables); it.More(); it.Next()) {
    V1 = Handle(SALOMEDSImpl_AttributeReference)::DownCast(it.Value());
    RT->HasRelocation (V1,V2);
    REL->GetVariables().Append(V2);
  }
}   
