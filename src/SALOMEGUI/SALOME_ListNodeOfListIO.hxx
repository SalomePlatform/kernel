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
//  File   : SALOME_ListNodeOfListIO.hxx
//  Module : SALOME

#ifndef _SALOME_ListNodeOfListIO_HeaderFile
#define _SALOME_ListNodeOfListIO_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_SALOME_ListNodeOfListIO_HeaderFile
#include <Handle_SALOME_ListNodeOfListIO.hxx>
#endif

#ifndef _Handle_SALOME_InteractiveObject_HeaderFile
#include <Handle_SALOME_InteractiveObject.hxx>
#endif
#ifndef _TCollection_MapNode_HeaderFile
#include <TCollection_MapNode.hxx>
#endif
#ifndef _TCollection_MapNodePtr_HeaderFile
#include <TCollection_MapNodePtr.hxx>
#endif
class SALOME_InteractiveObject;
class SALOME_ListIO;
class SALOME_ListIteratorOfListIO;


class SALOME_ListNodeOfListIO : public TCollection_MapNode {

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
SALOME_ListNodeOfListIO(const Handle(SALOME_InteractiveObject)& I,const TCollection_MapNodePtr& n);
  Handle_SALOME_InteractiveObject& Value() const;
Standard_EXPORT ~SALOME_ListNodeOfListIO();




 // Type management
 //
 Standard_EXPORT friend Handle_Standard_Type& SALOME_ListNodeOfListIO_Type_();
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
Handle_SALOME_InteractiveObject myValue;


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


// other Inline functions and methods (like "C++: function call" methods)
//


#endif
