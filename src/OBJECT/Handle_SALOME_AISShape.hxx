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
//  File   : Handle_SALOME_AISShape.hxx
//  Module : SALOME

#ifndef _Handle_SALOME_AISShape_HeaderFile
#define _Handle_SALOME_AISShape_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_AIS_Shape_HeaderFile
#include <Handle_AIS_Shape.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(AIS_Shape);
class SALOME_AISShape;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOME_AISShape);

class Handle(SALOME_AISShape) : public Handle(AIS_Shape) {
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
    Handle(SALOME_AISShape)():Handle(AIS_Shape)() {} 
    Handle(SALOME_AISShape)(const Handle(SALOME_AISShape)& aHandle) : Handle(AIS_Shape)(aHandle) 
     {
     }

    Handle(SALOME_AISShape)(const SALOME_AISShape* anItem) : Handle(AIS_Shape)((AIS_Shape *)anItem) 
     {
     }

    Handle(SALOME_AISShape)& operator=(const Handle(SALOME_AISShape)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOME_AISShape)& operator=(const SALOME_AISShape* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOME_AISShape* operator->() 
     {
      return (SALOME_AISShape *)ControlAccess();
     }

    SALOME_AISShape* operator->() const 
     {
      return (SALOME_AISShape *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOME_AISShape)();
 
   Standard_EXPORT static const Handle(SALOME_AISShape) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
