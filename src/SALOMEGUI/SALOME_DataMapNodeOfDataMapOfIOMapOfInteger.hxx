//  SALOME SALOMEGUI : implementation of desktop and GUI kernel
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
//  File   : SALOME_DataMapNodeOfDataMapOfIOMapOfInteger.hxx
//  Module : SALOME

#ifndef _SALOME_DataMapNodeOfDataMapOfIOMapOfInteger_HeaderFile
#define _SALOME_DataMapNodeOfDataMapOfIOMapOfInteger_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_SALOME_DataMapNodeOfDataMapOfIOMapOfInteger_HeaderFile
#include <Handle_SALOME_DataMapNodeOfDataMapOfIOMapOfInteger.hxx>
#endif

#ifndef _Handle_SALOME_InteractiveObject_HeaderFile
#include <Handle_SALOME_InteractiveObject.hxx>
#endif
#ifndef _TColStd_IndexedMapOfInteger_HeaderFile
#include <TColStd_IndexedMapOfInteger.hxx>
#endif
#ifndef _TCollection_MapNode_HeaderFile
#include <TCollection_MapNode.hxx>
#endif
#ifndef _TCollection_MapNodePtr_HeaderFile
#include <TCollection_MapNodePtr.hxx>
#endif
class SALOME_InteractiveObject;
class TColStd_IndexedMapOfInteger;
class TColStd_MapTransientHasher;
class SALOME_DataMapOfIOMapOfInteger;
class SALOME_DataMapIteratorOfDataMapOfIOMapOfInteger;


class SALOME_DataMapNodeOfDataMapOfIOMapOfInteger : public TCollection_MapNode {

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
Standard_EXPORT inline SALOME_DataMapNodeOfDataMapOfIOMapOfInteger(const Handle(SALOME_InteractiveObject)& K,const TColStd_IndexedMapOfInteger& I,const TCollection_MapNodePtr& n);
Standard_EXPORT inline   Handle_SALOME_InteractiveObject& Key() const;
Standard_EXPORT inline   TColStd_IndexedMapOfInteger& Value() const;
Standard_EXPORT ~SALOME_DataMapNodeOfDataMapOfIOMapOfInteger();




 // Type management
 //
 Standard_EXPORT friend Handle_Standard_Type& SALOME_DataMapNodeOfDataMapOfIOMapOfInteger_Type_();
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
Handle_SALOME_InteractiveObject myKey;
TColStd_IndexedMapOfInteger myValue;


};

#define TheKey Handle_SALOME_InteractiveObject
#define TheKey_hxx "SALOME_InteractiveObject.hxx"
#define TheItem TColStd_IndexedMapOfInteger
#define TheItem_hxx <TColStd_IndexedMapOfInteger.hxx>
#define Hasher TColStd_MapTransientHasher
#define Hasher_hxx <TColStd_MapTransientHasher.hxx>
#define TCollection_DataMapNode SALOME_DataMapNodeOfDataMapOfIOMapOfInteger
#define TCollection_DataMapNode_hxx "SALOME_DataMapNodeOfDataMapOfIOMapOfInteger.hxx"
#define TCollection_DataMapIterator SALOME_DataMapIteratorOfDataMapOfIOMapOfInteger
#define TCollection_DataMapIterator_hxx "SALOME_DataMapIteratorOfDataMapOfIOMapOfInteger.hxx"
#define Handle_TCollection_DataMapNode Handle_SALOME_DataMapNodeOfDataMapOfIOMapOfInteger
#define TCollection_DataMapNode_Type_() SALOME_DataMapNodeOfDataMapOfIOMapOfInteger_Type_()
#define TCollection_DataMap SALOME_DataMapOfIOMapOfInteger
#define TCollection_DataMap_hxx "SALOME_DataMapOfIOMapOfInteger.hxx"

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
