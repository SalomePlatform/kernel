//  File      : SALOMEDS_PersRefAttribute.ixx
//  Created   : Thu Nov 29 21:02:22 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#include "SALOMEDS_PersRefAttribute.jxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SALOMEDS_PersRefAttribute::~SALOMEDS_PersRefAttribute() {}
 


Standard_EXPORT Handle_Standard_Type& SALOMEDS_PersRefAttribute_Type_()
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
  static Handle_Standard_Type _aType = new Standard_Type("SALOMEDS_PersRefAttribute",
			                                 sizeof(SALOMEDS_PersRefAttribute),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOMEDS_PersRefAttribute) Handle(SALOMEDS_PersRefAttribute)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOMEDS_PersRefAttribute) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOMEDS_PersRefAttribute))) {
       _anOtherObject = Handle(SALOMEDS_PersRefAttribute)((Handle(SALOMEDS_PersRefAttribute)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOMEDS_PersRefAttribute::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOMEDS_PersRefAttribute) ; 
}
Standard_Boolean SALOMEDS_PersRefAttribute::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOMEDS_PersRefAttribute) == AType || TDataStd_Comment::IsKind(AType)); 
}
Handle_SALOMEDS_PersRefAttribute::~Handle_SALOMEDS_PersRefAttribute() {}

