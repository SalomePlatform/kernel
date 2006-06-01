//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_TableOfIntegerAttribute.ixx
//  Author : Michael Ponikarov
//  Module : SALOME
//  $Header$

#include <SALOMEDS_TableOfIntegerAttribute.jxx>

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SALOMEDS_TableOfIntegerAttribute::~SALOMEDS_TableOfIntegerAttribute() {}
 


Standard_EXPORT Handle_Standard_Type& SALOMEDS_TableOfIntegerAttribute_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TDF_Attribute);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TDF_Attribute);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SALOMEDS_TableOfIntegerAttribute",
			                                 sizeof(SALOMEDS_TableOfIntegerAttribute),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOMEDS_TableOfIntegerAttribute) Handle(SALOMEDS_TableOfIntegerAttribute)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOMEDS_TableOfIntegerAttribute) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOMEDS_TableOfIntegerAttribute))) {
       _anOtherObject = Handle(SALOMEDS_TableOfIntegerAttribute)((Handle(SALOMEDS_TableOfIntegerAttribute)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOMEDS_TableOfIntegerAttribute::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOMEDS_TableOfIntegerAttribute) ; 
}
Standard_Boolean SALOMEDS_TableOfIntegerAttribute::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOMEDS_TableOfIntegerAttribute) == AType || TDF_Attribute::IsKind(AType)); 
}
Handle_SALOMEDS_TableOfIntegerAttribute::~Handle_SALOMEDS_TableOfIntegerAttribute() {}

