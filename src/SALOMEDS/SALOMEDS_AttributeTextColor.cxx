// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/
//
//  File   : SALOMEDS_AttributeTextColor.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeTextColor.hxx"

#include <TColStd_HArray1OfReal.hxx>

SALOMEDS_AttributeTextColor::SALOMEDS_AttributeTextColor(const Handle(SALOMEDSImpl_AttributeTextColor)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTextColor::SALOMEDS_AttributeTextColor(SALOMEDS::AttributeTextColor_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTextColor::~SALOMEDS_AttributeTextColor()
{}


STextColor SALOMEDS_AttributeTextColor::TextColor()
{
  STextColor aColor;
  if(_isLocal) {
    Handle(TColStd_HArray1OfReal) aSeq = Handle(SALOMEDSImpl_AttributeTextColor)::DownCast(_local_impl)->TextColor();
    aColor.R = aSeq->Value(1);
    aColor.G = aSeq->Value(2);
    aColor.B = aSeq->Value(3);	
  }
  else {
    SALOMEDS::Color anImplColor = SALOMEDS::AttributeTextColor::_narrow(_corba_impl)->TextColor();
    aColor.R = anImplColor.R;
    aColor.G = anImplColor.G;
    aColor.B = anImplColor.B;
  }
  return aColor;
}
 
void SALOMEDS_AttributeTextColor::SetTextColor(STextColor value)
{
  CheckLocked();
  if(_isLocal) {
    Handle(TColStd_HArray1OfReal) aSeq = new TColStd_HArray1OfReal(1, 3);
    aSeq->SetValue(1, value.R);
    aSeq->SetValue(2, value.G);
    aSeq->SetValue(3, value.B);
    Handle(SALOMEDSImpl_AttributeTextColor)::DownCast(_local_impl)->ChangeArray(aSeq);
  }
  else {
    SALOMEDS::Color aColor;
    aColor.R = value.R;
    aColor.G = value.G;
    aColor.B = value.B;
    SALOMEDS::AttributeTextColor::_narrow(_corba_impl)->SetTextColor(aColor);
  }
}
