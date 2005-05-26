//  File   : SALOMEDSImpl_AttributeInteger.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDSImpl_AttributeInteger.hxx"

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeInteger,  SALOMEDSImpl_GenericAttribute)
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeInteger, SALOMEDSImpl_GenericAttribute )


//=======================================================================
//function : GetID
//purpose  :
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeInteger::GetID ()
{
  static Standard_GUID IntegerID ("8CC3E213-C9B4-47e4-8496-DD5E62E22018");
  return IntegerID;
}   

Handle(SALOMEDSImpl_AttributeInteger) SALOMEDSImpl_AttributeInteger::Set (const TDF_Label& L, Standard_Integer Val) 
{
  Handle(SALOMEDSImpl_AttributeInteger) A;
  if (!L.FindAttribute(SALOMEDSImpl_AttributeInteger::GetID(), A)) {
    A = new  SALOMEDSImpl_AttributeInteger(); 
    L.AddAttribute(A);
  }

  A->SetValue(Val); 
  return A;
}

//=======================================================================
//function : SetValue
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeInteger::SetValue(const Standard_Integer v)
{
  if(myValue == v) return;

  Backup();
  myValue = v;
}

//=======================================================================
//function : ID
//purpose  :
//=======================================================================
const Standard_GUID& SALOMEDSImpl_AttributeInteger::ID () const { return GetID(); }


//=======================================================================
//function : NewEmpty
//purpose  :
//=======================================================================
Handle(TDF_Attribute) SALOMEDSImpl_AttributeInteger::NewEmpty () const
{
  return new SALOMEDSImpl_AttributeInteger();
}

//=======================================================================
//function : Restore
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeInteger::Restore(const Handle(TDF_Attribute)& With)
{
  myValue = Handle(SALOMEDSImpl_AttributeInteger)::DownCast (With)->Value();
}

//=======================================================================
//function : Paste
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeInteger::Paste (const Handle(TDF_Attribute)& Into,
					   const Handle(TDF_RelocationTable)& RT) const
{
  Handle(SALOMEDSImpl_AttributeInteger)::DownCast(Into)->SetValue(myValue);
}             
