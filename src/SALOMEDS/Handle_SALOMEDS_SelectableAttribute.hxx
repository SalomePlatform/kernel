//  File      : Handle_SALOMEDS_SelectableAttribute.hxx
//  Created   : Tue Jul 09 16:42:58 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef _Handle_SALOMEDS_SelectableAttribute_HeaderFile
#define _Handle_SALOMEDS_SelectableAttribute_HeaderFile

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
class SALOMEDS_SelectableAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_SelectableAttribute);

class Handle(SALOMEDS_SelectableAttribute) : public Handle(TDataStd_Integer) {
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
    Handle(SALOMEDS_SelectableAttribute)():Handle(TDataStd_Integer)() {} 
    Handle(SALOMEDS_SelectableAttribute)(const Handle(SALOMEDS_SelectableAttribute)& aHandle) : Handle(TDataStd_Integer)(aHandle) 
     {
     }

    Handle(SALOMEDS_SelectableAttribute)(const SALOMEDS_SelectableAttribute* anItem) : Handle(TDataStd_Integer)((TDataStd_Integer *)anItem) 
     {
     }

    Handle(SALOMEDS_SelectableAttribute)& operator=(const Handle(SALOMEDS_SelectableAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_SelectableAttribute)& operator=(const SALOMEDS_SelectableAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_SelectableAttribute* operator->() 
     {
      return (SALOMEDS_SelectableAttribute *)ControlAccess();
     }

    SALOMEDS_SelectableAttribute* operator->() const 
     {
      return (SALOMEDS_SelectableAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_SelectableAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_SelectableAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
