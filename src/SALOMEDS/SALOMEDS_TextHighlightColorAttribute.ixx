//  File      : SALOMEDS_TextHighlightColorAttribute.ixx
//  Created   : Wed Jul 10 16:44:31 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#include "SALOMEDS_TextHighlightColorAttribute.jxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SALOMEDS_TextHighlightColorAttribute::~SALOMEDS_TextHighlightColorAttribute() {}
 


Standard_EXPORT Handle_Standard_Type& SALOMEDS_TextHighlightColorAttribute_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TDataStd_RealArray);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TDataStd_RealArray);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(TDF_Attribute);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(TDF_Attribute);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(MMgt_TShared);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType4 = STANDARD_TYPE(Standard_Transient);
  if ( aType4.IsNull()) aType4 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,aType4,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SALOMEDS_TextHighlightColorAttribute",
			                                 sizeof(SALOMEDS_TextHighlightColorAttribute),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOMEDS_TextHighlightColorAttribute) Handle(SALOMEDS_TextHighlightColorAttribute)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOMEDS_TextHighlightColorAttribute) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOMEDS_TextHighlightColorAttribute))) {
       _anOtherObject = Handle(SALOMEDS_TextHighlightColorAttribute)((Handle(SALOMEDS_TextHighlightColorAttribute)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOMEDS_TextHighlightColorAttribute::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOMEDS_TextHighlightColorAttribute) ; 
}
Standard_Boolean SALOMEDS_TextHighlightColorAttribute::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOMEDS_TextHighlightColorAttribute) == AType || TDataStd_RealArray::IsKind(AType)); 
}
Handle_SALOMEDS_TextHighlightColorAttribute::~Handle_SALOMEDS_TextHighlightColorAttribute() {}

