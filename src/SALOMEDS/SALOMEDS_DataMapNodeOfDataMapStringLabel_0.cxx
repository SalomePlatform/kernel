using namespace std;
//  File      : Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel_0.cxx
//  Created   : Tue Aug 13 14:05:03 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 

#include "SALOMEDS_DataMapNodeOfDataMapStringLabel.hxx"

#ifndef _Standard_TypeMismatch_HeaderFile
#include <Standard_TypeMismatch.hxx>
#endif

#ifndef _TCollection_ExtendedString_HeaderFile
#include <TCollection_ExtendedString.hxx>
#endif
#ifndef _TDF_Label_HeaderFile
#include <TDF_Label.hxx>
#endif
#ifndef _SALOMEDS_DataMapStringLabel_HeaderFile
#include "SALOMEDS_DataMapStringLabel.hxx"
#endif
#ifndef _SALOMEDS_DataMapIteratorOfDataMapStringLabel_HeaderFile
#include "SALOMEDS_DataMapIteratorOfDataMapStringLabel.hxx"
#endif
SALOMEDS_DataMapNodeOfDataMapStringLabel::~SALOMEDS_DataMapNodeOfDataMapStringLabel() {}
 


Standard_EXPORT Handle_Standard_Type& SALOMEDS_DataMapNodeOfDataMapStringLabel_Type_()
{

    static Handle_Standard_Type aType1 = STANDARD_TYPE(TCollection_MapNode);
  if ( aType1.IsNull()) aType1 = STANDARD_TYPE(TCollection_MapNode);
  static Handle_Standard_Type aType2 = STANDARD_TYPE(MMgt_TShared);
  if ( aType2.IsNull()) aType2 = STANDARD_TYPE(MMgt_TShared);
  static Handle_Standard_Type aType3 = STANDARD_TYPE(Standard_Transient);
  if ( aType3.IsNull()) aType3 = STANDARD_TYPE(Standard_Transient);
 

  static Handle_Standard_Transient _Ancestors[]= {aType1,aType2,aType3,NULL};
  static Handle_Standard_Type _aType = new Standard_Type("SALOMEDS_DataMapNodeOfDataMapStringLabel",
			                                 sizeof(SALOMEDS_DataMapNodeOfDataMapStringLabel),
			                                 1,
			                                 (Standard_Address)_Ancestors,
			                                 (Standard_Address)NULL);

  return _aType;
}


// DownCast method
//   allow safe downcasting
//
const Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel) Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel)::DownCast(const Handle(Standard_Transient)& AnObject) 
{
  Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel) _anOtherObject;

  if (!AnObject.IsNull()) {
     if (AnObject->IsKind(STANDARD_TYPE(SALOMEDS_DataMapNodeOfDataMapStringLabel))) {
       _anOtherObject = Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel)((Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel)&)AnObject);
     }
  }

  return _anOtherObject ;
}
const Handle(Standard_Type)& SALOMEDS_DataMapNodeOfDataMapStringLabel::DynamicType() const 
{ 
  return STANDARD_TYPE(SALOMEDS_DataMapNodeOfDataMapStringLabel) ; 
}
Standard_Boolean SALOMEDS_DataMapNodeOfDataMapStringLabel::IsKind(const Handle(Standard_Type)& AType) const 
{ 
  return (STANDARD_TYPE(SALOMEDS_DataMapNodeOfDataMapStringLabel) == AType || TCollection_MapNode::IsKind(AType)); 
}
Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel::~Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel() {}
#define TheKey TCollection_ExtendedString
#define TheKey_hxx <TCollection_ExtendedString.hxx>
#define TheItem TDF_Label
#define TheItem_hxx <TDF_Label.hxx>
#define Hasher TCollection_ExtendedString
#define Hasher_hxx <TCollection_ExtendedString.hxx>
#define TCollection_DataMapNode SALOMEDS_DataMapNodeOfDataMapStringLabel
#define TCollection_DataMapNode_hxx "SALOMEDS_DataMapNodeOfDataMapStringLabel.hxx"
#define TCollection_DataMapIterator SALOMEDS_DataMapIteratorOfDataMapStringLabel
#define TCollection_DataMapIterator_hxx "SALOMEDS_DataMapIteratorOfDataMapStringLabel.hxx"
#define Handle_TCollection_DataMapNode Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel
#define TCollection_DataMapNode_Type_() SALOMEDS_DataMapNodeOfDataMapStringLabel_Type_()
#define TCollection_DataMap SALOMEDS_DataMapStringLabel
#define TCollection_DataMap_hxx "SALOMEDS_DataMapStringLabel.hxx"
#include <TCollection_DataMapNode.gxx>

