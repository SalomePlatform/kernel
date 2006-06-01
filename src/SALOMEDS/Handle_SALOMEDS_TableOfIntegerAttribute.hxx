//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : Handle_SALOMEDS_TableOfIntegerAttribute.hxx
//  Author : Michael Ponikarov
//  Module : SALOME
//  $Header$

#ifndef _Handle_SALOMEDS_TableOfIntegerAttribute_HeaderFile
#define _Handle_SALOMEDS_TableOfIntegerAttribute_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_TDF_Attribute_HeaderFile
#include <Handle_TDF_Attribute.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(TDF_Attribute);
class SALOMEDS_TableOfIntegerAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_TableOfIntegerAttribute);

class Handle(SALOMEDS_TableOfIntegerAttribute) : public Handle(TDF_Attribute) {
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
    Handle(SALOMEDS_TableOfIntegerAttribute)():Handle(TDF_Attribute)() {} 
    Handle(SALOMEDS_TableOfIntegerAttribute)(const Handle(SALOMEDS_TableOfIntegerAttribute)& aHandle) : Handle(TDF_Attribute)(aHandle) 
     {
     }

    Handle(SALOMEDS_TableOfIntegerAttribute)(const SALOMEDS_TableOfIntegerAttribute* anItem) : Handle(TDF_Attribute)((TDF_Attribute *)anItem) 
     {
     }

    Handle(SALOMEDS_TableOfIntegerAttribute)& operator=(const Handle(SALOMEDS_TableOfIntegerAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_TableOfIntegerAttribute)& operator=(const SALOMEDS_TableOfIntegerAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_TableOfIntegerAttribute* operator->() 
     {
      return (SALOMEDS_TableOfIntegerAttribute *)ControlAccess();
     }

    SALOMEDS_TableOfIntegerAttribute* operator->() const 
     {
      return (SALOMEDS_TableOfIntegerAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_TableOfIntegerAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_TableOfIntegerAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
