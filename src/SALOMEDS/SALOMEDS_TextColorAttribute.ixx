//  File      : SALOMEDS_TextColorAttribute.ixx
//  Created   : Wed Jul 10 16:42:10 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#include "SALOMEDS_TextColorAttribute.jxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SALOMEDS_TextColorAttribute::~SALOMEDS_TextColorAttribute() {}
 


Standard_EXPORT Handle_Standard_Type& SALOMEDS_TextColorAttribute_Type_()
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
  static Handle_Standard_Type _aType = new Standard_Type("SALOMEDS_TextColorAttribute",
			                                 sizeof(SALOMEDS_TextColorAttribute),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOMEDS_TextColorAttribute) Handle(SALOMEDS_TextColorAttribute)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOMEDS_TextColorAttribute) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOMEDS_TextColorAttribute))) {
       _anOtherObject = Handle(SALOMEDS_TextColorAttribute)((Handle(SALOMEDS_TextColorAttribute)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOMEDS_TextColorAttribute::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOMEDS_TextColorAttribute) ; 
}
Standard_Boolean SALOMEDS_TextColorAttribute::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOMEDS_TextColorAttribute) == AType || TDataStd_RealArray::IsKind(AType)); 
}
Handle_SALOMEDS_TextColorAttribute::~Handle_SALOMEDS_TextColorAttribute() {}

