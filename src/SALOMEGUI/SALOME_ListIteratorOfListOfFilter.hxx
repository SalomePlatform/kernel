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
//  File   : SALOME_ListIteratorOfListOfFilter.hxx
//  Module : SALOME

#ifndef _SALOME_ListIteratorOfListOfFilter_HeaderFile
#define _SALOME_ListIteratorOfListOfFilter_HeaderFile

#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
#ifndef _Handle_SALOME_Filter_HeaderFile
#include <Handle_SALOME_Filter.hxx>
#endif
#ifndef _Handle_SALOME_ListNodeOfListOfFilter_HeaderFile
#include <Handle_SALOME_ListNodeOfListOfFilter.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class Standard_NoMoreObject;
class Standard_NoSuchObject;
class SALOME_ListOfFilter;
class SALOME_Filter;
class SALOME_ListNodeOfListOfFilter;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

class SALOME_ListIteratorOfListOfFilter  {

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
Standard_EXPORT SALOME_ListIteratorOfListOfFilter();
Standard_EXPORT SALOME_ListIteratorOfListOfFilter(const SALOME_ListOfFilter& L);
Standard_EXPORT   void Initialize(const SALOME_ListOfFilter& L) ;
Standard_EXPORT inline   Standard_Boolean More() const;
Standard_EXPORT   void Next() ;
Standard_EXPORT   Handle_SALOME_Filter& Value() const;


friend class SALOME_ListOfFilter;



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
Standard_Address current;
Standard_Address previous;


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

#include <TCollection_ListIterator.lxx>

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
