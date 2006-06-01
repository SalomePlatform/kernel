//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_DataMapNodeOfDataMapOfIntegerString.hxx
//  Author : Sergey Ruin
//  Module : SALOME

#ifndef _SALOMEDS_DataMapNodeOfDataMapOfIntegerString_HeaderFile
#define _SALOMEDS_DataMapNodeOfDataMapOfIntegerString_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_SALOMEDS_DataMapNodeOfDataMapOfIntegerString_HeaderFile
#include <Handle_SALOMEDS_DataMapNodeOfDataMapOfIntegerString.hxx>
#endif

#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _TCollection_ExtendedString_HeaderFile
#include <TCollection_ExtendedString.hxx>
#endif
#ifndef _TCollection_MapNode_HeaderFile
#include <TCollection_MapNode.hxx>
#endif
#ifndef _TCollection_MapNodePtr_HeaderFile
#include <TCollection_MapNodePtr.hxx>
#endif
class TCollection_ExtendedString;
class TColStd_MapIntegerHasher;
class SALOMEDS_DataMapOfIntegerString;
class SALOMEDS_DataMapIteratorOfDataMapOfIntegerString;


class SALOMEDS_DataMapNodeOfDataMapOfIntegerString : public TCollection_MapNode {

public:

    void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }
 // Methods PUBLIC
 // 
SALOMEDS_DataMapNodeOfDataMapOfIntegerString(const Standard_Real& K,const TCollection_ExtendedString& I,const TCollection_MapNodePtr& n);
  Standard_Real& Key() const;
  TCollection_ExtendedString& Value() const;
Standard_EXPORT ~SALOMEDS_DataMapNodeOfDataMapOfIntegerString();




 // Type management
 //
 Standard_EXPORT friend Handle_Standard_Type& SALOMEDS_DataMapNodeOfDataMapOfIntegerString_Type_();
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
Standard_Real myKey;
TCollection_ExtendedString myValue;


};

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


// other Inline functions and methods (like "C++: function call" methods)
//


#endif
