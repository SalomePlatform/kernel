//  File      : Handle_SALOMEDS_TargetAttribute.hxx
//  Created   : Fri Aug 16 10:20:05 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 

#ifndef _Handle_SALOMEDS_TargetAttribute_HeaderFile
#define _Handle_SALOMEDS_TargetAttribute_HeaderFile

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
class SALOMEDS_TargetAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_TargetAttribute);

class Handle(SALOMEDS_TargetAttribute) : public Handle(TDataStd_Integer) {
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
    Handle(SALOMEDS_TargetAttribute)():Handle(TDataStd_Integer)() {} 
    Handle(SALOMEDS_TargetAttribute)(const Handle(SALOMEDS_TargetAttribute)& aHandle) : Handle(TDataStd_Integer)(aHandle) 
     {
     }

    Handle(SALOMEDS_TargetAttribute)(const SALOMEDS_TargetAttribute* anItem) : Handle(TDataStd_Integer)((TDataStd_Integer *)anItem) 
     {
     }

    Handle(SALOMEDS_TargetAttribute)& operator=(const Handle(SALOMEDS_TargetAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_TargetAttribute)& operator=(const SALOMEDS_TargetAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_TargetAttribute* operator->() 
     {
      return (SALOMEDS_TargetAttribute *)ControlAccess();
     }

    SALOMEDS_TargetAttribute* operator->() const 
     {
      return (SALOMEDS_TargetAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_TargetAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_TargetAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
