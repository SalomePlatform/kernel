using namespace std;
//  File      : SALOMEDS_AttributeTextColor_i.cxx
//  Created   : Fri Jul 05 14:38:10 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#include "SALOMEDS_AttributeTextColor_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include <TColStd_HArray1OfReal.hxx>

SALOMEDS::Color SALOMEDS_AttributeTextColor_i::TextColor() {
  SALOMEDS::Color TextColor;
  Handle(TColStd_HArray1OfReal) anArray = Handle(SALOMEDS_TextColorAttribute)::DownCast(_myAttr)->Array();
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

void SALOMEDS_AttributeTextColor_i::SetTextColor(const SALOMEDS::Color& value) {
  CheckLocked();
  Handle(TColStd_HArray1OfReal) anArray = new TColStd_HArray1OfReal(1,3);
  anArray->SetValue(1,  value.R);
  anArray->SetValue(2, value.G);
  anArray->SetValue(3, value.B);
  Handle(SALOMEDS_TextColorAttribute)::DownCast(_myAttr)->ChangeArray(anArray);
}
