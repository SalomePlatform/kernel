//  File      : SALOMEDS_SequenceOfRealAttribute.ixx
//  Created   : Fri Jul 05 13:00:55 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$


#include "SALOMEDS_SequenceOfRealAttribute.jxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SALOMEDS_SequenceOfRealAttribute::~SALOMEDS_SequenceOfRealAttribute() {}
 


Standard_EXPORT Handle_Standard_Type& SALOMEDS_SequenceOfRealAttribute_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TDF_Attribute);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TDF_Attribute);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SALOMEDS_SequenceOfRealAttribute",
			                                 sizeof(SALOMEDS_SequenceOfRealAttribute),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOMEDS_SequenceOfRealAttribute) Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOMEDS_SequenceOfRealAttribute) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOMEDS_SequenceOfRealAttribute))) {
       _anOtherObject = Handle(SALOMEDS_SequenceOfRealAttribute)((Handle(SALOMEDS_SequenceOfRealAttribute)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOMEDS_SequenceOfRealAttribute::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOMEDS_SequenceOfRealAttribute) ; 
}
Standard_Boolean SALOMEDS_SequenceOfRealAttribute::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOMEDS_SequenceOfRealAttribute) == AType || TDF_Attribute::IsKind(AType)); 
}
Handle_SALOMEDS_SequenceOfRealAttribute::~Handle_SALOMEDS_SequenceOfRealAttribute() {}

