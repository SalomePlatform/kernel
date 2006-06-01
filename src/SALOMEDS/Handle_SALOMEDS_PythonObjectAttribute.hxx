//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : Handle_SALOMEDS_PythonObjectAttribute.hxx
//  Author : Michael Ponikarov
//  Module : SALOME
//  $Header$

#ifndef _Handle_SALOMEDS_PythonObjectAttribute_HeaderFile
#define _Handle_SALOMEDS_PythonObjectAttribute_HeaderFile

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
class SALOMEDS_PythonObjectAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_PythonObjectAttribute);

class Handle(SALOMEDS_PythonObjectAttribute) : public Handle(TDF_Attribute) {
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
    Handle(SALOMEDS_PythonObjectAttribute)():Handle(TDF_Attribute)() {} 
    Handle(SALOMEDS_PythonObjectAttribute)(const Handle(SALOMEDS_PythonObjectAttribute)& aHandle) : Handle(TDF_Attribute)(aHandle) 
     {
     }

    Handle(SALOMEDS_PythonObjectAttribute)(const SALOMEDS_PythonObjectAttribute* anItem) : Handle(TDF_Attribute)((TDF_Attribute *)anItem) 
     {
     }

    Handle(SALOMEDS_PythonObjectAttribute)& operator=(const Handle(SALOMEDS_PythonObjectAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_PythonObjectAttribute)& operator=(const SALOMEDS_PythonObjectAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_PythonObjectAttribute* operator->() 
     {
      return (SALOMEDS_PythonObjectAttribute *)ControlAccess();
     }

    SALOMEDS_PythonObjectAttribute* operator->() const 
     {
      return (SALOMEDS_PythonObjectAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_PythonObjectAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_PythonObjectAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
