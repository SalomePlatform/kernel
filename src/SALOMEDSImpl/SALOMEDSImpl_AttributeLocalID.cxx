//  File   : SALOMEDSImpl_AttributeLocalID.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDSImpl_AttributeLocalID.hxx"
#include <Standard_GUID.hxx>

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeLocalID, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeLocalID, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeLocalID::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeLocalID ("12837196-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeLocalID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributeLocalID) SALOMEDSImpl_AttributeLocalID::Set (const TDF_Label& L,
									  const Standard_Integer value) 
{
  Handle(SALOMEDSImpl_AttributeLocalID) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeLocalID::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeLocalID(); 
    L.AddAttribute(A);
  }
  
  A->SetValue(value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeLocalID::SALOMEDSImpl_AttributeLocalID()
:SALOMEDSImpl_GenericAttribute("AttributeLocalID")
{
  myValue = 0; 
}

//=======================================================================
//function : Set
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeLocalID::SetValue(const Standard_Integer theValue)
{
  CheckLocked();

  Backup();

  myValue = theValue;
}
    

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeLocalID::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeLocalID::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeLocalID(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeLocalID::Restore(const Handle(TDF_Attribute)& with) 
{
  myValue = Handle(SALOMEDSImpl_AttributeLocalID)::DownCast (with)->Value();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeLocalID::Paste (const Handle(TDF_Attribute)& into,
					   const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeLocalID)::DownCast (into)->SetValue(myValue);
}

