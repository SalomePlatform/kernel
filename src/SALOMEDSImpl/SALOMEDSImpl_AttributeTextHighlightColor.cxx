//  File   : SALOMEDSImpl_AttributeTextHighlightColor.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDSImpl_AttributeTextHighlightColor.hxx"
#include <Standard_GUID.hxx>

using namespace std;

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_AttributeTextHighlightColor, SALOMEDSImpl_GenericAttribute )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_AttributeTextHighlightColor, SALOMEDSImpl_GenericAttribute )

//=======================================================================
//function : GetID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeTextHighlightColor::GetID () 
{
  static Standard_GUID SALOMEDSImpl_AttributeTextHighlightColorID ("12837190-8F52-11d6-A8A3-0001021E8C7F");
  return SALOMEDSImpl_AttributeTextHighlightColorID;
}



//=======================================================================
//function : constructor
//purpose  : 
//=======================================================================
SALOMEDSImpl_AttributeTextHighlightColor::SALOMEDSImpl_AttributeTextHighlightColor()
:SALOMEDSImpl_GenericAttribute("AttributeTextHighlightColor")
{ 
  myValue = new TColStd_HArray1OfReal(1, 3, RealFirst());
}

//=======================================================================
//function : ID
//purpose  : 
//=======================================================================

const Standard_GUID& SALOMEDSImpl_AttributeTextHighlightColor::ID () const { return GetID(); }


//=======================================================================
//function : SetTextHighlightColor
//purpose  :
//=======================================================================
void SALOMEDSImpl_AttributeTextHighlightColor::SetTextHighlightColor(const Standard_Real R, 
							             const Standard_Real G, 
							             const Standard_Real B)
{
   CheckLocked();
   if(myValue.IsNull()) return;  
   Backup(); 

   myValue->SetValue(1, R);
   myValue->SetValue(2, G);
   myValue->SetValue(3, B);
   
   SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved 
}
	    
//=======================================================================
//function : TextHighlightColor
//purpose  :
//=======================================================================
Handle(TColStd_HArray1OfReal) SALOMEDSImpl_AttributeTextHighlightColor::TextHighlightColor()
{
   return myValue;
}      

//=======================================================================
//function : ChangeArray
//purpose  : 
//=======================================================================
void SALOMEDSImpl_AttributeTextHighlightColor::ChangeArray(const Handle(TColStd_HArray1OfReal)& newArray)
{
  Backup();

  for(int i = 1; i <= 3; i++)
    myValue->SetValue(i, newArray->Value(i));

  SetModifyFlag(); //SRN: Mark the study as being modified, so it could be saved  
}    

//=======================================================================
//function : NewEmpty
//purpose  : 
//=======================================================================

Handle(TDF_Attribute) SALOMEDSImpl_AttributeTextHighlightColor::NewEmpty () const
{  
  return new SALOMEDSImpl_AttributeTextHighlightColor(); 
}

//=======================================================================
//function : Restore
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeTextHighlightColor::Restore(const Handle(TDF_Attribute)& with) 
{
  Handle(TColStd_HArray1OfReal) s = Handle(SALOMEDSImpl_AttributeTextHighlightColor)::DownCast (with)->TextHighlightColor ();
  ChangeArray(s);
  return;
}

//=======================================================================
//function : Paste
//purpose  : 
//=======================================================================

void SALOMEDSImpl_AttributeTextHighlightColor::Paste (const Handle(TDF_Attribute)& into,
                                                  const Handle(TDF_RelocationTable)& ) const
{
  Handle(SALOMEDSImpl_AttributeTextHighlightColor)::DownCast (into)->ChangeArray (myValue);
}

TCollection_AsciiString SALOMEDSImpl_AttributeTextHighlightColor::Save() 
{
  char *Val = new char[75];
  sprintf(Val, "%f %f %f", (float)myValue->Value(1), 
                           (float)myValue->Value(2), 
			   (float)myValue->Value(3));
  TCollection_AsciiString ret(Val);
  delete Val;
  return ret;
}

void SALOMEDSImpl_AttributeTextHighlightColor::Load(const TCollection_AsciiString& value) 
{
  float r, g, b;
  sscanf(value.ToCString(), "%f %f %f", &r, &g, &b);
  myValue->SetValue(1, r);
  myValue->SetValue(2, g);
  myValue->SetValue(3, b);
}

