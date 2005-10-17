//  File   : SALOMEDS_AttributeTextHighlightColor.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDS_AttributeTextHighlightColor.hxx"

#include <TColStd_HArray1OfReal.hxx>

SALOMEDS_AttributeTextHighlightColor::SALOMEDS_AttributeTextHighlightColor(const Handle(SALOMEDSImpl_AttributeTextHighlightColor)& theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTextHighlightColor::SALOMEDS_AttributeTextHighlightColor(SALOMEDS::AttributeTextHighlightColor_ptr theAttr)
:SALOMEDS_GenericAttribute(theAttr)
{}

SALOMEDS_AttributeTextHighlightColor::~SALOMEDS_AttributeTextHighlightColor()
{}


STextColor SALOMEDS_AttributeTextHighlightColor::TextHighlightColor()
{
  STextColor aColor;
  if(_isLocal) {
    Handle(TColStd_HArray1OfReal) aSeq;
    aSeq = Handle(SALOMEDSImpl_AttributeTextHighlightColor)::DownCast(_local_impl)->TextHighlightColor();
    aColor.R = aSeq->Value(1);
    aColor.G = aSeq->Value(2);
    aColor.B = aSeq->Value(3);	
  }
  else {
    SALOMEDS::Color anImplColor = SALOMEDS::AttributeTextHighlightColor::_narrow(_corba_impl)->TextHighlightColor();
    aColor.R = anImplColor.R;
    aColor.G = anImplColor.G;
    aColor.B = anImplColor.B;
  }
  return aColor;
}
 
void SALOMEDS_AttributeTextHighlightColor::SetTextHighlightColor(STextColor value)
{
  if(_isLocal) {
    Handle(TColStd_HArray1OfReal) aSeq = new TColStd_HArray1OfReal(1, 3);
    aSeq->SetValue(1, value.R);
    aSeq->SetValue(2, value.G);
    aSeq->SetValue(3, value.B);
    Handle(SALOMEDSImpl_AttributeTextHighlightColor)::DownCast(_local_impl)->ChangeArray(aSeq);
  }
  else {
    SALOMEDS::Color aColor;
    aColor.R = value.R;
    aColor.G = value.G;
    aColor.B = value.B;
    SALOMEDS::AttributeTextHighlightColor::_narrow(_corba_impl)->SetTextHighlightColor(aColor);
  }
}
