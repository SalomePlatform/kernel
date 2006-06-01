//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//
//
//  File   : Handle_SALOMEDS_PersRefAttribute.hxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#ifndef _Handle_SALOMEDS_PersRefAttribute_HeaderFile
#define _Handle_SALOMEDS_PersRefAttribute_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_TDataStd_Comment_HeaderFile
#include <Handle_TDataStd_Comment.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(TDataStd_Comment);
class SALOMEDS_PersRefAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_PersRefAttribute);

class Handle(SALOMEDS_PersRefAttribute) : public Handle(TDataStd_Comment) {
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
    Handle(SALOMEDS_PersRefAttribute)():Handle(TDataStd_Comment)() {} 
    Handle(SALOMEDS_PersRefAttribute)(const Handle(SALOMEDS_PersRefAttribute)& aHandle) : Handle(TDataStd_Comment)(aHandle) 
     {
     }

    Handle(SALOMEDS_PersRefAttribute)(const SALOMEDS_PersRefAttribute* anItem) : Handle(TDataStd_Comment)((TDataStd_Comment *)anItem) 
     {
     }

    Handle(SALOMEDS_PersRefAttribute)& operator=(const Handle(SALOMEDS_PersRefAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_PersRefAttribute)& operator=(const SALOMEDS_PersRefAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_PersRefAttribute* operator->() 
     {
      return (SALOMEDS_PersRefAttribute *)ControlAccess();
     }

    SALOMEDS_PersRefAttribute* operator->() const 
     {
      return (SALOMEDS_PersRefAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_PersRefAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_PersRefAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
