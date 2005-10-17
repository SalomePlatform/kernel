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
