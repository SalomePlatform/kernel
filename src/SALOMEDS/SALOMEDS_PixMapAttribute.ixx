//  File      : SALOMEDS_PixMapAttribute.ixx
//  Created   : Tue Jul 09 16:50:05 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#include "SALOMEDS_PixMapAttribute.jxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SALOMEDS_PixMapAttribute::~SALOMEDS_PixMapAttribute() {}
 


Standard_EXPORT Handle_Standard_Type& SALOMEDS_PixMapAttribute_Type_()
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
  static Handle_Standard_Type _aType = new Standard_Type("SALOMEDS_PixMapAttribute",
			                                 sizeof(SALOMEDS_PixMapAttribute),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOMEDS_PixMapAttribute) Handle(SALOMEDS_PixMapAttribute)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOMEDS_PixMapAttribute) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOMEDS_PixMapAttribute))) {
       _anOtherObject = Handle(SALOMEDS_PixMapAttribute)((Handle(SALOMEDS_PixMapAttribute)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOMEDS_PixMapAttribute::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOMEDS_PixMapAttribute) ; 
}
Standard_Boolean SALOMEDS_PixMapAttribute::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOMEDS_PixMapAttribute) == AType || TDataStd_Comment::IsKind(AType)); 
}
Handle_SALOMEDS_PixMapAttribute::~Handle_SALOMEDS_PixMapAttribute() {}

