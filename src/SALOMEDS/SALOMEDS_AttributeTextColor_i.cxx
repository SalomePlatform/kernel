//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOMEDS_AttributeTextColor_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include "SALOMEDS_AttributeTextColor_i.hxx"
#include <TColStd_HArray1OfReal.hxx>

using namespace std;

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

char* SALOMEDS_AttributeTextColor_i::Store() {
  SALOMEDS::Color aColor = TextColor();
  char *Val = new char[75];
  sprintf(Val, "%f %f %f", (float)aColor.R, (float)aColor.G, (float)aColor.B);
  return Val;
}

void SALOMEDS_AttributeTextColor_i::Restore(const char* value) {
  SALOMEDS::Color aColor;
  float r, g, b;
  sscanf(value, "%f %f %f", &r, &g, &b);
  aColor.R = r;
  aColor.G = g;
  aColor.B = b;
  SetTextColor(aColor);
}
