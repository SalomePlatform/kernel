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
//  File   : SALOME_ListNodeOfListOfFilter.hxx
//  Module : SALOME

#ifndef _SALOME_ListNodeOfListOfFilter_HeaderFile
#define _SALOME_ListNodeOfListOfFilter_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_SALOME_ListNodeOfListOfFilter_HeaderFile
#include <Handle_SALOME_ListNodeOfListOfFilter.hxx>
#endif

#ifndef _Handle_SALOME_Filter_HeaderFile
#include <Handle_SALOME_Filter.hxx>
#endif
#ifndef _TCollection_MapNode_HeaderFile
#include <TCollection_MapNode.hxx>
#endif
#ifndef _TCollection_MapNodePtr_HeaderFile
#include <TCollection_MapNodePtr.hxx>
#endif
class SALOME_Filter;
class SALOME_ListOfFilter;
class SALOME_ListIteratorOfListOfFilter;


class SALOME_ListNodeOfListOfFilter : public TCollection_MapNode {

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
Standard_EXPORT inline SALOME_ListNodeOfListOfFilter(const Handle(SALOME_Filter)& I,const TCollection_MapNodePtr& n);
Standard_EXPORT inline   Handle_SALOME_Filter& Value() const;
Standard_EXPORT ~SALOME_ListNodeOfListOfFilter();




 // Type management
 //
 Standard_EXPORT friend Handle_Standard_Type& SALOME_ListNodeOfListOfFilter_Type_();
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
Handle_SALOME_Filter myValue;


};

#define Item Handle_SALOME_Filter
#define Item_hxx "SALOME_Filter.hxx"
#define TCollection_ListNode SALOME_ListNodeOfListOfFilter
#define TCollection_ListNode_hxx "SALOME_ListNodeOfListOfFilter.hxx"
#define TCollection_ListIterator SALOME_ListIteratorOfListOfFilter
#define TCollection_ListIterator_hxx "SALOME_ListIteratorOfListOfFilter.hxx"
#define Handle_TCollection_ListNode Handle_SALOME_ListNodeOfListOfFilter
#define TCollection_ListNode_Type_() SALOME_ListNodeOfListOfFilter_Type_()
#define TCollection_List SALOME_ListOfFilter
#define TCollection_List_hxx "SALOME_ListOfFilter.hxx"

#include <TCollection_ListNode.lxx>

#undef Item
#undef Item_hxx
#undef TCollection_ListNode
#undef TCollection_ListNode_hxx
#undef TCollection_ListIterator
#undef TCollection_ListIterator_hxx
#undef Handle_TCollection_ListNode
#undef TCollection_ListNode_Type_
#undef TCollection_List
#undef TCollection_List_hxx


// other inline functions and methods (like "C++: function call" methods)
//


#endif
