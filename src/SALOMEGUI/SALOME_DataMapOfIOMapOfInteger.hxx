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
//  File   : SALOME_DataMapOfIOMapOfInteger.hxx
//  Module : SALOME

#ifndef _SALOME_DataMapOfIOMapOfInteger_HeaderFile
#define _SALOME_DataMapOfIOMapOfInteger_HeaderFile

#ifndef _TCollection_BasicMap_HeaderFile
#include <TCollection_BasicMap.hxx>
#endif
#ifndef _Handle_SALOME_InteractiveObject_HeaderFile
#include <Handle_SALOME_InteractiveObject.hxx>
#endif
#ifndef _Handle_SALOME_DataMapNodeOfDataMapOfIOMapOfInteger_HeaderFile
#include <Handle_SALOME_DataMapNodeOfDataMapOfIOMapOfInteger.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class Standard_DomainError;
class Standard_NoSuchObject;
class SALOME_InteractiveObject;
class TColStd_IndexedMapOfInteger;
class TColStd_MapTransientHasher;
class SALOME_DataMapNodeOfDataMapOfIOMapOfInteger;
class SALOME_DataMapIteratorOfDataMapOfIOMapOfInteger;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

class SALOME_DataMapOfIOMapOfInteger  : public TCollection_BasicMap {

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
Standard_EXPORT SALOME_DataMapOfIOMapOfInteger(const Standard_Integer NbBuckets = 1);
Standard_EXPORT   SALOME_DataMapOfIOMapOfInteger& Assign(const SALOME_DataMapOfIOMapOfInteger& Other) ;
  SALOME_DataMapOfIOMapOfInteger& operator =(const SALOME_DataMapOfIOMapOfInteger& Other) 
{
  return Assign(Other);
}

Standard_EXPORT   void ReSize(const Standard_Integer NbBuckets) ;
Standard_EXPORT   void Clear() ;
~SALOME_DataMapOfIOMapOfInteger()
{
  Clear();
}

Standard_EXPORT   Standard_Boolean Bind(const Handle(SALOME_InteractiveObject)& K,const TColStd_IndexedMapOfInteger& I) ;
Standard_EXPORT   Standard_Boolean IsBound(const Handle(SALOME_InteractiveObject)& K) const;
Standard_EXPORT   Standard_Boolean UnBind(const Handle(SALOME_InteractiveObject)& K) ;
Standard_EXPORT  const TColStd_IndexedMapOfInteger& Find(const Handle(SALOME_InteractiveObject)& K) const;
 const TColStd_IndexedMapOfInteger& operator()(const Handle(SALOME_InteractiveObject)& K) const
{
  return Find(K);
}

Standard_EXPORT   TColStd_IndexedMapOfInteger& ChangeFind(const Handle(SALOME_InteractiveObject)& K) ;
  TColStd_IndexedMapOfInteger& operator()(const Handle(SALOME_InteractiveObject)& K) 
{
  return ChangeFind(K);
}






protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 
Standard_EXPORT SALOME_DataMapOfIOMapOfInteger(const SALOME_DataMapOfIOMapOfInteger& Other);


 // Fields PRIVATE
 //


};





// other inline functions and methods (like "C++: function call" methods)
//


#endif
