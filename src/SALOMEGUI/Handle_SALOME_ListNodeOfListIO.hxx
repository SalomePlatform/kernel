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
//  File   : Handle_SALOME_ListNodeOfListIO.hxx
//  Module : SALOME

#ifndef _Handle_SALOME_ListNodeOfListIO_HeaderFile
#define _Handle_SALOME_ListNodeOfListIO_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_TCollection_MapNode_HeaderFile
#include <Handle_TCollection_MapNode.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(TCollection_MapNode);
class SALOME_ListNodeOfListIO;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOME_ListNodeOfListIO);

class Handle(SALOME_ListNodeOfListIO) : public Handle(TCollection_MapNode) {
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
    Handle(SALOME_ListNodeOfListIO)():Handle(TCollection_MapNode)() {} 
    Handle(SALOME_ListNodeOfListIO)(const Handle(SALOME_ListNodeOfListIO)& aHandle) : Handle(TCollection_MapNode)(aHandle) 
     {
     }

    Handle(SALOME_ListNodeOfListIO)(const SALOME_ListNodeOfListIO* anItem) : Handle(TCollection_MapNode)((TCollection_MapNode *)anItem) 
     {
     }

    Handle(SALOME_ListNodeOfListIO)& operator=(const Handle(SALOME_ListNodeOfListIO)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOME_ListNodeOfListIO)& operator=(const SALOME_ListNodeOfListIO* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOME_ListNodeOfListIO* operator->() 
     {
      return (SALOME_ListNodeOfListIO *)ControlAccess();
     }

    SALOME_ListNodeOfListIO* operator->() const 
     {
      return (SALOME_ListNodeOfListIO *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOME_ListNodeOfListIO)();
 
   Standard_EXPORT static const Handle(SALOME_ListNodeOfListIO) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
