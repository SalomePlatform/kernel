//  File   : SALOMEDSImpl_AttributeSelectable.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDSImpl_AttributeSelectable.hxx"
#include <Standard_GUID.hxx>

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeSelectable, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeSelectable, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeSelectable::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeSelectableID ("12837188-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeSelectableID;
}



//=======================================================================
//function : Set
//purpose  : 
//=======================================================================

Handle(SALOMEDSImpl_AttributeSelectable) SALOMEDSImpl_AttributeSelectable::Set (const TDF_Label& L,
										const Standard_Integer value) 
{
  Handle(SALOMEDSImpl_AttributeSelectable) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeSelectable::GetID(),A)) {
    A = new  SALOMEDSImpl_AttributeSelectable(); 
    L.AddAttribute(A);
  }
  
  A->SetSelectable (value); 
  return A;
}


//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeSelectable::SALOMEDSImpl_AttributeSelectable()
:SALOMEDSImpl_GenericAttribute("AttributeSelectable")
{
  myValue = 0;
}

//=======================================================================
//function : SetSelectable
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeSelectable::SetSelectable(const Standard_Integer theValue)
{
  Backup();

  (theValue!=0)?myValue=1:myValue=0;
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeSelectable::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeSelectable::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeSelectable(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeSelectable::Restore(const Handle(TDF_Attribute)& with) 
{
  myValue = Handle(SALOMEDSImpl_AttributeSelectable)::DownCast (with)->IsSelectable ();
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeSelectable::Paste (const Handle(TDF_Attribute)& into,
                                              const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeSelectable)::DownCast (into)->SetSelectable (myValue);
}

