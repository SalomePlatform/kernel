//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_PythonObjectAttribute.ixx
//  Author : Michael Ponikarov
//  Module : SALOME
//  $Header$

#include "SALOMEDS_PythonObjectAttribute.jxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SALOMEDS_PythonObjectAttribute::~SALOMEDS_PythonObjectAttribute() {}
 


Standard_EXPORT Handle_Standard_Type& SALOMEDS_PythonObjectAttribute_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TDF_Attribute);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TDF_Attribute);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SALOMEDS_PythonObjectAttribute",
			                                 sizeof(SALOMEDS_PythonObjectAttribute),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOMEDS_PythonObjectAttribute) Handle(SALOMEDS_PythonObjectAttribute)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOMEDS_PythonObjectAttribute) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOMEDS_PythonObjectAttribute))) {
       _anOtherObject = Handle(SALOMEDS_PythonObjectAttribute)((Handle(SALOMEDS_PythonObjectAttribute)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOMEDS_PythonObjectAttribute::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOMEDS_PythonObjectAttribute) ; 
}
Standard_Boolean SALOMEDS_PythonObjectAttribute::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOMEDS_PythonObjectAttribute) == AType || TDF_Attribute::IsKind(AType)); 
}
Handle_SALOMEDS_PythonObjectAttribute::~Handle_SALOMEDS_PythonObjectAttribute() {}

