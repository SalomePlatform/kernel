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
//  File   : Handle_SALOME_AISObject.hxx
//  Module : SALOME

#ifndef _Handle_SALOME_AISObject_HeaderFile
#define _Handle_SALOME_AISObject_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_AIS_InteractiveObject_HeaderFile
#include <Handle_AIS_InteractiveObject.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(AIS_InteractiveObject);
class SALOME_AISObject;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOME_AISObject);

class Handle(SALOME_AISObject) : public Handle(AIS_InteractiveObject) {
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
    Handle(SALOME_AISObject)():Handle(AIS_InteractiveObject)() {} 
    Handle(SALOME_AISObject)(const Handle(SALOME_AISObject)& aHandle) : Handle(AIS_InteractiveObject)(aHandle) 
     {
     }

    Handle(SALOME_AISObject)(const SALOME_AISObject* anItem) : Handle(AIS_InteractiveObject)((AIS_InteractiveObject *)anItem) 
     {
     }

    Handle(SALOME_AISObject)& operator=(const Handle(SALOME_AISObject)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOME_AISObject)& operator=(const SALOME_AISObject* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOME_AISObject* operator->() 
     {
      return (SALOME_AISObject *)ControlAccess();
     }

    SALOME_AISObject* operator->() const 
     {
      return (SALOME_AISObject *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOME_AISObject)();
 
   Standard_EXPORT static const Handle(SALOME_AISObject) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
