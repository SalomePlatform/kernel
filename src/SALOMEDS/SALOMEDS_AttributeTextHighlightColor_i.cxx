using namespace std;
//  File      : SALOMEDS_AttributeTextHighlightColor_i.cxx
//  Created   : Fri Jul 05 14:38:10 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#include "SALOMEDS_AttributeTextHighlightColor_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include <TColStd_HArray1OfReal.hxx>

SALOMEDS::Color SALOMEDS_AttributeTextHighlightColor_i::TextHighlightColor() {
  SALOMEDS::Color TextHighlightColor;
  Handle(TColStd_HArray1OfReal) anArray = Handle(SALOMEDS_TextHighlightColorAttribute)::DownCast(_myAttr)->Array();
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

void SALOMEDS_AttributeTextHighlightColor_i::SetTextHighlightColor(const SALOMEDS::Color& value) {
  CheckLocked();
  Handle(TColStd_HArray1OfReal) anArray = new TColStd_HArray1OfReal(1,3);
  anArray->SetValue(1,  value.R);
  anArray->SetValue(2, value.G);
  anArray->SetValue(3, value.B);
  Handle(SALOMEDS_TextHighlightColorAttribute)::DownCast(_myAttr)->ChangeArray(anArray);
}
