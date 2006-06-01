//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_DataMapNodeOfDataMapOfIntegerString_0.cxx
//  Author : Sergey Ruin
//  Module : SALOME

#include <SALOMEDS_DataMapNodeOfDataMapOfIntegerString.hxx>

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

#ifndef _TCollection_ExtendedString_HeaderFile
#include <TCollection_ExtendedString.hxx>
#endif
#ifndef _TColStd_MapIntegerHasher_HeaderFile
#include <TColStd_MapIntegerHasher.hxx>
#endif
#ifndef _SALOMEDS_DataMapOfIntegerString_HeaderFile
#include <SALOMEDS_DataMapOfIntegerString.hxx>
#endif
#ifndef _SALOMEDS_DataMapIteratorOfDataMapOfIntegerString_HeaderFile
#include <SALOMEDS_DataMapIteratorOfDataMapOfIntegerString.hxx>
#endif
using namespace std;
SALOMEDS_DataMapNodeOfDataMapOfIntegerString::~SALOMEDS_DataMapNodeOfDataMapOfIntegerString() {}
 


Standard_EXPORT Handle_Standard_Type& SALOMEDS_DataMapNodeOfDataMapOfIntegerString_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TCollection_MapNode);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TCollection_MapNode);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SALOMEDS_DataMapNodeOfDataMapOfIntegerString",
			                                 sizeof(SALOMEDS_DataMapNodeOfDataMapOfIntegerString),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString) Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOMEDS_DataMapNodeOfDataMapOfIntegerString))) {
       _anOtherObject = Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString)((Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOMEDS_DataMapNodeOfDataMapOfIntegerString::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOMEDS_DataMapNodeOfDataMapOfIntegerString) ; 
}
Standard_Boolean SALOMEDS_DataMapNodeOfDataMapOfIntegerString::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOMEDS_DataMapNodeOfDataMapOfIntegerString) == AType || TCollection_MapNode::IsKind(AType)); 
}
Handle_SALOMEDS_DataMapNodeOfDataMapOfIntegerString::~Handle_SALOMEDS_DataMapNodeOfDataMapOfIntegerString() {}
#define TheKey Standard_Real
#define TheKey_hxx <Standard_Real.hxx>
#define TheItem TCollection_ExtendedString
#define TheItem_hxx <TCollection_ExtendedString.hxx>
#define Hasher TColStd_MapIntegerHasher
#define Hasher_hxx <TColStd_MapIntegerHasher.hxx>
#define TCollection_DataMapNode SALOMEDS_DataMapNodeOfDataMapOfIntegerString
#define TCollection_DataMapNode_hxx <SALOMEDS_DataMapNodeOfDataMapOfIntegerString.hxx>
#define TCollection_DataMapIterator SALOMEDS_DataMapIteratorOfDataMapOfIntegerString
#define TCollection_DataMapIterator_hxx <SALOMEDS_DataMapIteratorOfDataMapOfIntegerString.hxx>
#define Handle_TCollection_DataMapNode Handle_SALOMEDS_DataMapNodeOfDataMapOfIntegerString
#define TCollection_DataMapNode_Type_() SALOMEDS_DataMapNodeOfDataMapOfIntegerString_Type_()
#define TCollection_DataMap SALOMEDS_DataMapOfIntegerString
#define TCollection_DataMap_hxx <SALOMEDS_DataMapOfIntegerString.hxx>
#include <TCollection_DataMapNode.gxx>

