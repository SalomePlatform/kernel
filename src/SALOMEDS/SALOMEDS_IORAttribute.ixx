//  File      : SALOMEDS_IORAttribute.ixx
//  Created   : Thu Nov 29 21:00:52 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#include "SALOMEDS_IORAttribute.jxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SALOMEDS_IORAttribute::~SALOMEDS_IORAttribute() {}
 


Standard_EXPORT Handle_Standard_Type& SALOMEDS_IORAttribute_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TDataStd_Comment);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TDataStd_Comment);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(TDF_Attribute);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(TDF_Attribute);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(MMgt_TShared);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType4 = STANDARD_TYPE(Standard_Transient);
  if ( aType4.IsNull()) aType4 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,aType4,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SALOMEDS_IORAttribute",
			                                 sizeof(SALOMEDS_IORAttribute),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOMEDS_IORAttribute) Handle(SALOMEDS_IORAttribute)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOMEDS_IORAttribute) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOMEDS_IORAttribute))) {
       _anOtherObject = Handle(SALOMEDS_IORAttribute)((Handle(SALOMEDS_IORAttribute)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOMEDS_IORAttribute::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOMEDS_IORAttribute) ; 
}
Standard_Boolean SALOMEDS_IORAttribute::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOMEDS_IORAttribute) == AType || TDataStd_Comment::IsKind(AType)); 
}
Handle_SALOMEDS_IORAttribute::~Handle_SALOMEDS_IORAttribute() {}

