//  File      : Handle_SALOMEDS_TableOfRealAttribute.hxx
//  Created   : Tue Oct  8 10:13:30 2002
//  Author    : Michael Ponikarov
//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE
//  $Header$

#ifndef _Handle_SALOMEDS_TableOfRealAttribute_HeaderFile
#define _Handle_SALOMEDS_TableOfRealAttribute_HeaderFile

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
class SALOMEDS_TableOfRealAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_TableOfRealAttribute);

class Handle(SALOMEDS_TableOfRealAttribute) : public Handle(TDF_Attribute) {
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
    Handle(SALOMEDS_TableOfRealAttribute)():Handle(TDF_Attribute)() {} 
    Handle(SALOMEDS_TableOfRealAttribute)(const Handle(SALOMEDS_TableOfRealAttribute)& aHandle) : Handle(TDF_Attribute)(aHandle) 
     {
     }

    Handle(SALOMEDS_TableOfRealAttribute)(const SALOMEDS_TableOfRealAttribute* anItem) : Handle(TDF_Attribute)((TDF_Attribute *)anItem) 
     {
     }

    Handle(SALOMEDS_TableOfRealAttribute)& operator=(const Handle(SALOMEDS_TableOfRealAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_TableOfRealAttribute)& operator=(const SALOMEDS_TableOfRealAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_TableOfRealAttribute* operator->() 
     {
      return (SALOMEDS_TableOfRealAttribute *)ControlAccess();
     }

    SALOMEDS_TableOfRealAttribute* operator->() const 
     {
      return (SALOMEDS_TableOfRealAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_TableOfRealAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_TableOfRealAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
