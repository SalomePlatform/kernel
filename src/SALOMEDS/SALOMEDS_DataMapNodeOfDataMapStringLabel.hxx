//  File      : SALOMEDS_DataMapNodeOfDataMapStringLabel.hxx
//  Created   : Tue Aug 13 14:05:03 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 

#ifndef _SALOMEDS_DataMapNodeOfDataMapStringLabel_HeaderFile
#define _SALOMEDS_DataMapNodeOfDataMapStringLabel_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel_HeaderFile
#include <Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel.hxx>
#endif

#ifndef _TCollection_ExtendedString_HeaderFile
#include <TCollection_ExtendedString.hxx>
#endif
#ifndef _TDF_Label_HeaderFile
#include <TDF_Label.hxx>
#endif
#ifndef _TCollection_MapNode_HeaderFile
#include <TCollection_MapNode.hxx>
#endif
#ifndef _TCollection_MapNodePtr_HeaderFile
#include <TCollection_MapNodePtr.hxx>
#endif
class TCollection_ExtendedString;
class TDF_Label;
class SALOMEDS_DataMapStringLabel;
class SALOMEDS_DataMapIteratorOfDataMapStringLabel;


class SALOMEDS_DataMapNodeOfDataMapStringLabel : public TCollection_MapNode {

public:

    inline void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    inline void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    inline void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }
//    inline void  operator delete(void *anAddress, size_t size) 
//      { 
//        if (anAddress) Standard::Free((Standard_Address&)anAddress,size); 
//      }
 // Methods PUBLIC
 // 
Standard_EXPORT inline SALOMEDS_DataMapNodeOfDataMapStringLabel(const TCollection_ExtendedString& K,const TDF_Label& I,const TCollection_MapNodePtr& n);
Standard_EXPORT inline   TCollection_ExtendedString& Key() const;
Standard_EXPORT inline   TDF_Label& Value() const;
Standard_EXPORT ~SALOMEDS_DataMapNodeOfDataMapStringLabel();




 // Type management
 //
 Standard_EXPORT friend Handle_Standard_Type& SALOMEDS_DataMapNodeOfDataMapStringLabel_Type_();
 Standard_EXPORT const Handle(Standard_Type)& DynamicType() const;
 Standard_EXPORT Standard_Boolean	       IsKind(const Handle(Standard_Type)&) const;

protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 


 // Fields PRIVATE
 //
TCollection_ExtendedString myKey;
TDF_Label myValue;


};

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

#include <TCollection_DataMapNode.lxx>

#undef TheKey
#undef TheKey_hxx
#undef TheItem
#undef TheItem_hxx
#undef Hasher
#undef Hasher_hxx
#undef TCollection_DataMapNode
#undef TCollection_DataMapNode_hxx
#undef TCollection_DataMapIterator
#undef TCollection_DataMapIterator_hxx
#undef Handle_TCollection_DataMapNode
#undef TCollection_DataMapNode_Type_
#undef TCollection_DataMap
#undef TCollection_DataMap_hxx


// other inline functions and methods (like "C++: function call" methods)
//


#endif
