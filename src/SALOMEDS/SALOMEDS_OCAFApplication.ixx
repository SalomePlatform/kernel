//  File      : SALOMEDS_OCAFApplication.ixx
//  Created   : Thu Nov 29 21:01:58 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$
#include "SALOMEDS_OCAFApplication.jxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SALOMEDS_OCAFApplication::~SALOMEDS_OCAFApplication() {}
 


Standard_EXPORT Handle_Standard_Type& SALOMEDS_OCAFApplication_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TDocStd_Application);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TDocStd_Application);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(CDF_Application);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(CDF_Application);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(CDM_Application);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(CDM_Application);
  static Handle_Standard_Type aType4 = STANDARD_TYPE(Standard_Transient);
  if ( aType4.IsNull()) aType4 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,aType4,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SALOMEDS_OCAFApplication",
			                                 sizeof(SALOMEDS_OCAFApplication),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOMEDS_OCAFApplication) Handle(SALOMEDS_OCAFApplication)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOMEDS_OCAFApplication) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOMEDS_OCAFApplication))) {
       _anOtherObject = Handle(SALOMEDS_OCAFApplication)((Handle(SALOMEDS_OCAFApplication)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOMEDS_OCAFApplication::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOMEDS_OCAFApplication) ; 
}
Standard_Boolean SALOMEDS_OCAFApplication::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOMEDS_OCAFApplication) == AType || TDocStd_Application::IsKind(AType)); 
}
Handle_SALOMEDS_OCAFApplication::~Handle_SALOMEDS_OCAFApplication() {}

