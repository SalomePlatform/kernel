//  File      : Handle_SALOMEDS_LocalIDAttribute.hxx
//  Created   : Tue Aug 13 14:05:03 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 

#ifndef _Handle_SALOMEDS_LocalIDAttribute_HeaderFile
#define _Handle_SALOMEDS_LocalIDAttribute_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_TDataStd_Integer_HeaderFile
#include <Handle_TDataStd_Integer.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(TDataStd_Integer);
class SALOMEDS_LocalIDAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_LocalIDAttribute);

class Handle(SALOMEDS_LocalIDAttribute) : public Handle(TDataStd_Integer) {
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
    Handle(SALOMEDS_LocalIDAttribute)():Handle(TDataStd_Integer)() {} 
    Handle(SALOMEDS_LocalIDAttribute)(const Handle(SALOMEDS_LocalIDAttribute)& aHandle) : Handle(TDataStd_Integer)(aHandle) 
     {
     }

    Handle(SALOMEDS_LocalIDAttribute)(const SALOMEDS_LocalIDAttribute* anItem) : Handle(TDataStd_Integer)((TDataStd_Integer *)anItem) 
     {
     }

    Handle(SALOMEDS_LocalIDAttribute)& operator=(const Handle(SALOMEDS_LocalIDAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_LocalIDAttribute)& operator=(const SALOMEDS_LocalIDAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_LocalIDAttribute* operator->() 
     {
      return (SALOMEDS_LocalIDAttribute *)ControlAccess();
     }

    SALOMEDS_LocalIDAttribute* operator->() const 
     {
      return (SALOMEDS_LocalIDAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_LocalIDAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_LocalIDAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
