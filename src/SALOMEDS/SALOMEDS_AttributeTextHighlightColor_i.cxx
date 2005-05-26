//  File   : SALOMEDS_AttributeTextHighlightColor_i.cxx
//  Author : Sergey RUIN
//  Module : SALOME

using namespace std;
#include "SALOMEDS_AttributeTextHighlightColor_i.hxx"
#include "SALOMEDS.hxx"
#include <TColStd_HArray1OfReal.hxx>

SALOMEDS::Color SALOMEDS_AttributeTextHighlightColor_i::TextHighlightColor() 
{
  SALOMEDS::Locker lock;
  SALOMEDS::Color TextHighlightColor;
  Handle(TColStd_HArray1OfReal) anArray=Handle(SALOMEDSImpl_AttributeTextHighlightColor)::DownCast(_impl)->TextHighlightColor();
  if (anArray.IsNull() || anArray->Length()!=3) { 
    TextHighlightColor.R = 0;
    TextHighlightColor.G = 0;
    TextHighlightColor.B = 0;
  }
  else {
    TextHighlightColor.R = anArray->Value(1);
    TextHighlightColor.G = anArray->Value(2);
    TextHighlightColor.B = anArray->Value(3);
  }
  return TextHighlightColor;
}

void SALOMEDS_AttributeTextHighlightColor_i::SetTextHighlightColor(const SALOMEDS::Color& value) 
{
  SALOMEDS::Locker lock;
  CheckLocked();
  Handle(TColStd_HArray1OfReal) anArray = new TColStd_HArray1OfReal(1,3);
  anArray->SetValue(1,  value.R);
  anArray->SetValue(2, value.G);
  anArray->SetValue(3, value.B);
  Handle(SALOMEDSImpl_AttributeTextHighlightColor)::DownCast(_impl)->ChangeArray(anArray);
}
