//  File      : SALOMEDS_SequenceOfIntegerAttribute.ixx
//  Created   : Fri Jul 05 12:59:55 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#include "SALOMEDS_SequenceOfIntegerAttribute.jxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SALOMEDS_SequenceOfIntegerAttribute::~SALOMEDS_SequenceOfIntegerAttribute() {}
 


Standard_EXPORT Handle_Standard_Type& SALOMEDS_SequenceOfIntegerAttribute_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TDF_Attribute);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TDF_Attribute);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SALOMEDS_SequenceOfIntegerAttribute",
			                                 sizeof(SALOMEDS_SequenceOfIntegerAttribute),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOMEDS_SequenceOfIntegerAttribute) Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOMEDS_SequenceOfIntegerAttribute) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOMEDS_SequenceOfIntegerAttribute))) {
       _anOtherObject = Handle(SALOMEDS_SequenceOfIntegerAttribute)((Handle(SALOMEDS_SequenceOfIntegerAttribute)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOMEDS_SequenceOfIntegerAttribute::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOMEDS_SequenceOfIntegerAttribute) ; 
}
Standard_Boolean SALOMEDS_SequenceOfIntegerAttribute::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOMEDS_SequenceOfIntegerAttribute) == AType || TDF_Attribute::IsKind(AType)); 
}
Handle_SALOMEDS_SequenceOfIntegerAttribute::~Handle_SALOMEDS_SequenceOfIntegerAttribute() {}

