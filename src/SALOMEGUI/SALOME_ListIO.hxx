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
//  File   : SALOME_ListIO.hxx
//  Module : SALOME

#ifndef _SALOME_ListIO_HeaderFile
#define _SALOME_ListIO_HeaderFile

#ifndef _Standard_Address_HeaderFile
#include <Standard_Address.hxx>
#endif
#ifndef _Handle_SALOME_InteractiveObject_HeaderFile
#include <Handle_SALOME_InteractiveObject.hxx>
#endif
#ifndef _Handle_SALOME_ListNodeOfListIO_HeaderFile
#include <Handle_SALOME_ListNodeOfListIO.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class Standard_NoSuchObject;
class SALOME_ListIteratorOfListIO;
class SALOME_InteractiveObject;
class SALOME_ListNodeOfListIO;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

class SALOME_ListIO  {

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
Standard_EXPORT SALOME_ListIO();
Standard_EXPORT   void Assign(const SALOME_ListIO& Other) ;
  void operator=(const SALOME_ListIO& Other) 
{
  Assign(Other);
}

Standard_EXPORT   Standard_Integer Extent() const;
Standard_EXPORT   void Clear() ;
~SALOME_ListIO()
{
  Clear();
}

Standard_EXPORT   Standard_Boolean IsEmpty() const;
Standard_EXPORT   void Prepend(const Handle(SALOME_InteractiveObject)& I) ;
Standard_EXPORT   void Prepend(SALOME_ListIO& Other) ;
Standard_EXPORT   void Prepend(const Handle(SALOME_InteractiveObject)& I, SALOME_ListIteratorOfListIO& It) ;
Standard_EXPORT   void Append(const Handle(SALOME_InteractiveObject)& I) ;
Standard_EXPORT   void Append(SALOME_ListIO& Other) ;
Standard_EXPORT   void Append(const Handle(SALOME_InteractiveObject)& I, SALOME_ListIteratorOfListIO& It) ;
Standard_EXPORT   Handle_SALOME_InteractiveObject& First() const;
Standard_EXPORT   Handle_SALOME_InteractiveObject& Last() const;
Standard_EXPORT   void RemoveFirst() ;
Standard_EXPORT   void Remove(SALOME_ListIteratorOfListIO& It) ;
Standard_EXPORT   void InsertBefore(const Handle(SALOME_InteractiveObject)& I,SALOME_ListIteratorOfListIO& It) ;
Standard_EXPORT   void InsertBefore(SALOME_ListIO& Other,SALOME_ListIteratorOfListIO& It) ;
Standard_EXPORT   void InsertAfter(const Handle(SALOME_InteractiveObject)& I,SALOME_ListIteratorOfListIO& It) ;
Standard_EXPORT   void InsertAfter(SALOME_ListIO& Other,SALOME_ListIteratorOfListIO& It) ;


friend class SALOME_ListIteratorOfListIO;



protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 
Standard_EXPORT SALOME_ListIO(const SALOME_ListIO& Other);


 // Fields PRIVATE
 //
Standard_Address myFirst;
Standard_Address myLast;


};

#define Item Handle_SALOME_InteractiveObject
#define Item_hxx "SALOME_InteractiveObject.hxx"
#define TCollection_ListNode SALOME_ListNodeOfListIO
#define TCollection_ListNode_hxx "SALOME_ListNodeOfListIO.hxx"
#define TCollection_ListIterator SALOME_ListIteratorOfListIO
#define TCollection_ListIterator_hxx "SALOME_ListIteratorOfListIO.hxx"
#define Handle_TCollection_ListNode Handle_SALOME_ListNodeOfListIO
#define TCollection_ListNode_Type_() SALOME_ListNodeOfListIO_Type_()
#define TCollection_List SALOME_ListIO
#define TCollection_List_hxx "SALOME_ListIO.hxx"

#include <TCollection_List.lxx>

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


// other Inline functions and methods (like "C++: function call" methods)
//


#endif
