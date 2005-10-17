//  File   : SALOMEDSImpl_AttributeIOR.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef WNT
using namespace std;
#endif
#include "SALOMEDSImpl_AttributeIOR.hxx"
#include <TCollection_AsciiString.hxx>
#include "SALOMEDSImpl_Study.hxx"
#include <Standard_GUID.hxx>

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeIOR, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeIOR, SALOMEDSImpl_GenericAttribute )


//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeIOR::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeIORID ("92888E01-7074-11d5-A690-0800369C8A03");
  return SALOMEDSImpl_AttributeIORID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributeIOR) SALOMEDSImpl_AttributeIOR::Set (const TDF_Label& L,
							          const TCollection_ExtendedString& S) 
{
  Handle(SALOMEDSImpl_AttributeIOR) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeIOR(); 
    L.AddAttribute(A);
  }

  A->SetValue(S); 
  SALOMEDSImpl_Study::IORUpdated(A);
  return A;
}

//=======================================================================
//function : SetValue
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeIOR::SetValue(const TCollection_ExtendedString& theValue)
{
  CheckLocked();

  Handle(SALOMEDSImpl_Study) aStudy = SALOMEDSImpl_Study::GetStudy(Label());
  aStudy->AddCreatedPostponed(theValue);
  aStudy->AddPostponed(theValue);

  Backup();
  myString = theValue;

  SALOMEDSImpl_Study::IORUpdated(this);

  //SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}

//=======================================================================
//function : Value
//purpose  : 
//=======================================================================
TCollection_ExtendedString SALOMEDSImpl_AttributeIOR::Value() const
{
  return myString;
}

//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeIOR::SALOMEDSImpl_AttributeIOR()
:SALOMEDSImpl_GenericAttribute("AttributeIOR")
{
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeIOR::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeIOR::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeIOR(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeIOR::Restore(const Handle(TDF_Attribute)& with) 
{
  myString = Handle(SALOMEDSImpl_AttributeIOR)::DownCast (with)->Value();
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeIOR::Paste (const Handle(TDF_Attribute)& into,
				       const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeIOR)::DownCast (into)->SetValue(myString);
}

