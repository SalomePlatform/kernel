//  SALOME OBJECT : implementation of interactive object visualization for OCC and VTK viewers
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOME_AISShape.ixx
//  Module : SALOME

#include "SALOME_AISShape.jxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

SALOME_AISShape::~SALOME_AISShape() {}
 


Standard_EXPORT Handle_Standard_Type& SALOME_AISShape_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(AIS_Shape);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(AIS_Shape);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(AIS_InteractiveObject);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(AIS_InteractiveObject);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(SelectMgr_SelectableObject);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(SelectMgr_SelectableObject);
  static Handle_Standard_Type aType4 = STANDARD_TYPE(PrsMgr_PresentableObject);
  if ( aType4.IsNull()) aType4 = STANDARD_TYPE(PrsMgr_PresentableObject);
  static Handle_Standard_Type aType5 = STANDARD_TYPE(MMgt_TShared);
  if ( aType5.IsNull()) aType5 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType6 = STANDARD_TYPE(Standard_Transient);
  if ( aType6.IsNull()) aType6 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,aType4,aType5,aType6,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SALOME_AISShape",
			                                 sizeof(SALOME_AISShape),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOME_AISShape) Handle(SALOME_AISShape)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOME_AISShape) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOME_AISShape))) {
       _anOtherObject = Handle(SALOME_AISShape)((Handle(SALOME_AISShape)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOME_AISShape::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOME_AISShape) ; 
}
Standard_Boolean SALOME_AISShape::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOME_AISShape) == AType || AIS_Shape::IsKind(AType)); 
}
Handle_SALOME_AISShape::~Handle_SALOME_AISShape() {}

