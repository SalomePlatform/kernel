using namespace std;
//  File      : SALOMEDS_DataMapIteratorOfDataMapStringLabel_0.cxx
//  Created   : Tue Aug 13 14:05:03 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 

#include "SALOMEDS_DataMapIteratorOfDataMapStringLabel.hxx"

#ifndef _Standard_NoSuchObject_HeaderFile
#include <Standard_NoSuchObject.hxx>
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
#ifndef _SALOMEDS_DataMapNodeOfDataMapStringLabel_HeaderFile
#include "SALOMEDS_DataMapNodeOfDataMapStringLabel.hxx"
#endif
 

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
#include <TCollection_DataMapIterator.gxx>

