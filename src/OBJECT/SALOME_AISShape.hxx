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
//  File   : SALOME_AISShape.hxx
//  Module : SALOME

#ifndef _SALOME_AISShape_HeaderFile
#define _SALOME_AISShape_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_SALOME_AISShape_HeaderFile
#include <Handle_SALOME_AISShape.hxx>
#endif

#ifndef _Standard_CString_HeaderFile
#include <Standard_CString.hxx>
#endif
#ifndef _Handle_SALOME_InteractiveObject_HeaderFile
#include <Handle_SALOME_InteractiveObject.hxx>
#endif
#ifndef _AIS_Shape_HeaderFile
#include <AIS_Shape.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class SALOME_InteractiveObject;
class TopoDS_Shape;
class TColStd_IndexedMapOfInteger;


class SALOME_AISShape : public AIS_Shape {

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
Standard_EXPORT SALOME_AISShape(const TopoDS_Shape& shape);
Standard_EXPORT virtual  Handle_SALOME_InteractiveObject getIO() = 0;
Standard_EXPORT virtual  Standard_Boolean hasIO() = 0;
Standard_EXPORT virtual  Standard_CString getName() = 0;
Standard_EXPORT virtual  void setName(Standard_CString aName) = 0;
Standard_EXPORT virtual  void highlightSubShapes(const TColStd_IndexedMapOfInteger& aIndexMap, const Standard_Boolean aHighlight ) = 0;
Standard_EXPORT ~SALOME_AISShape();




 // Type management
 //
 Standard_EXPORT friend Handle_Standard_Type& SALOME_AISShape_Type_();
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


};





// other inline functions and methods (like "C++: function call" methods)
//


#endif
