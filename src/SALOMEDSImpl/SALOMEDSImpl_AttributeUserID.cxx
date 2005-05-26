//  File   : SALOMEDSImpl_AttributeUAttribute.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDSImpl_AttributeUserID.hxx"

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeUserID, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeUserID, SALOMEDSImpl_GenericAttribute )

Handle(SALOMEDSImpl_AttributeUserID) SALOMEDSImpl_AttributeUserID::Set (const TDF_Label& L, const Standard_GUID& ID) 
{
  Handle(SALOMEDSImpl_AttributeUserID) A;
  if (!L.FindAttribute(ID, A)) {
    A = new  SALOMEDSImpl_AttributeUserID(); 
    A->SetValue(ID);
    L.AddAttribute(A);
  }
  return A;
}

//=======================================================================
//function : ID
//purpose  :
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeUserID::ID() const
{ return myID; }


//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeUserID::SetValue( const Standard_GUID&  guid)
{
  CheckLocked();
  if(myID == guid) return;

  Backup();
  myID = guid;
}        

//=======================================================================
//function : NewEmpty
//purpose  :
//=======================================================================
Handle(TDF_Attribute) SALOMEDSImpl_AttributeUserID::NewEmpty () const
{
  Handle(SALOMEDSImpl_AttributeUserID) A = new SALOMEDSImpl_AttributeUserID();
  A->SetValue(myID);
  return A;
}

//=======================================================================
//function : Restore
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeUserID::Restore(const Handle(TDF_Attribute)& with)
{
  Handle(SALOMEDSImpl_AttributeUserID) A = Handle(SALOMEDSImpl_AttributeUserID)::DownCast(with);
  SetValue( A->ID() );
}

//=======================================================================
//function : Paste
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeUserID::Paste (const Handle(TDF_Attribute)& into,
					  const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeUserID) A = Handle(SALOMEDSImpl_AttributeUserID)::DownCast(into);
  A->SetValue( myID );
}    

TCollection_AsciiString SALOMEDSImpl_AttributeUserID::Type() 
{

  char* aUAttrName = new char[60];
  char aGUID[40];
  ID().ToCString(aGUID);
  sprintf(aUAttrName, "AttributeUserID_%s",aGUID);

  TCollection_AsciiString ret(aUAttrName);
  delete aUAttrName;

  return ret;
}
