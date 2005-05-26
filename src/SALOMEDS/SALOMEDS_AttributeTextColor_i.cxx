//  File   : SALOMEDS_AttributeTextColor_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeTextColor_i.hxx"
#include "SALOMEDS.hxx" 
#include <TColStd_HArray1OfReal.hxx>

SALOMEDS::Color SALOMEDS_AttributeTextColor_i::TextColor() 
{
  SALOMEDS::Locker lock;
  SALOMEDS::Color TextColor;
  Handle(TColStd_HArray1OfReal) anArray = Handle(SALOMEDSImpl_AttributeTextColor)::DownCast(_impl)->TextColor();
  if (anArray.IsNull() || anArray->Length()!=3) { 
    TextColor.R = 0;
    TextColor.G = 0;
    TextColor.B = 0;
  }
  else {
    TextColor.R = anArray->Value(1);
    TextColor.G = anArray->Value(2);
    TextColor.B = anArray->Value(3);
  }
  return TextColor;
}

void SALOMEDS_AttributeTextColor_i::SetTextColor(const SALOMEDS::Color& value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(TColStd_HArray1OfReal) anArray = new TColStd_HArray1OfReal(1,3);
  anArray->SetValue(1,  value.R);
  anArray->SetValue(2, value.G);
  anArray->SetValue(3, value.B);
  Handle(SALOMEDSImpl_AttributeTextColor)::DownCast(_impl)->ChangeArray(anArray);
}

