//  File      : SALOMEDS_SelectableAttribute.ixx
//  Created   : Tue Jul 09 16:51:55 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#include "SALOMEDS_SelectableAttribute.jxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SALOMEDS_SelectableAttribute::~SALOMEDS_SelectableAttribute() {}
 


Standard_EXPORT Handle_Standard_Type& SALOMEDS_SelectableAttribute_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TDataStd_Integer);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TDataStd_Integer);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(TDF_Attribute);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(TDF_Attribute);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(MMgt_TShared);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType4 = STANDARD_TYPE(Standard_Transient);
  if ( aType4.IsNull()) aType4 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,aType4,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SALOMEDS_SelectableAttribute",
			                                 sizeof(SALOMEDS_SelectableAttribute),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOMEDS_SelectableAttribute) Handle(SALOMEDS_SelectableAttribute)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOMEDS_SelectableAttribute) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOMEDS_SelectableAttribute))) {
       _anOtherObject = Handle(SALOMEDS_SelectableAttribute)((Handle(SALOMEDS_SelectableAttribute)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOMEDS_SelectableAttribute::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOMEDS_SelectableAttribute) ; 
}
Standard_Boolean SALOMEDS_SelectableAttribute::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOMEDS_SelectableAttribute) == AType || TDataStd_Integer::IsKind(AType)); 
}
Handle_SALOMEDS_SelectableAttribute::~Handle_SALOMEDS_SelectableAttribute() {}

