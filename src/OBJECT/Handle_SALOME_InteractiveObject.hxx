//  SALOME OBJECT : implementation of interactive object visualization for OCC and VTK viewers
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
//  File   : Handle_SALOME_InteractiveObject.hxx
//  Module : SALOME

#ifndef _Handle_SALOME_InteractiveObject_HeaderFile
#define _Handle_SALOME_InteractiveObject_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_MMgt_TShared_HeaderFile
#include <Handle_MMgt_TShared.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(MMgt_TShared);
class SALOME_InteractiveObject;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOME_InteractiveObject);

class Handle(SALOME_InteractiveObject) : public Handle(MMgt_TShared) {
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
    Handle(SALOME_InteractiveObject)():Handle(MMgt_TShared)() {} 
    Handle(SALOME_InteractiveObject)(const Handle(SALOME_InteractiveObject)& aHandle) : Handle(MMgt_TShared)(aHandle) 
     {
     }

    Handle(SALOME_InteractiveObject)(const SALOME_InteractiveObject* anItem) : Handle(MMgt_TShared)((MMgt_TShared *)anItem) 
     {
     }

    Handle(SALOME_InteractiveObject)& operator=(const Handle(SALOME_InteractiveObject)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOME_InteractiveObject)& operator=(const SALOME_InteractiveObject* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOME_InteractiveObject* operator->() 
     {
      return (SALOME_InteractiveObject *)ControlAccess();
     }

    SALOME_InteractiveObject* operator->() const 
     {
      return (SALOME_InteractiveObject *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOME_InteractiveObject)();
 
   Standard_EXPORT static const Handle(SALOME_InteractiveObject) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
