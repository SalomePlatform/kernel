//  File      : Handle_SALOMEDS_DrawableAttribute.hxx
//  Created   : Tue Jul 09 16:42:05 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef _Handle_SALOMEDS_DrawableAttribute_HeaderFile
#define _Handle_SALOMEDS_DrawableAttribute_HeaderFile

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
class SALOMEDS_DrawableAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_DrawableAttribute);

class Handle(SALOMEDS_DrawableAttribute) : public Handle(TDataStd_Integer) {
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
    Handle(SALOMEDS_DrawableAttribute)():Handle(TDataStd_Integer)() {} 
    Handle(SALOMEDS_DrawableAttribute)(const Handle(SALOMEDS_DrawableAttribute)& aHandle) : Handle(TDataStd_Integer)(aHandle) 
     {
     }

    Handle(SALOMEDS_DrawableAttribute)(const SALOMEDS_DrawableAttribute* anItem) : Handle(TDataStd_Integer)((TDataStd_Integer *)anItem) 
     {
     }

    Handle(SALOMEDS_DrawableAttribute)& operator=(const Handle(SALOMEDS_DrawableAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_DrawableAttribute)& operator=(const SALOMEDS_DrawableAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_DrawableAttribute* operator->() 
     {
      return (SALOMEDS_DrawableAttribute *)ControlAccess();
     }

    SALOMEDS_DrawableAttribute* operator->() const 
     {
      return (SALOMEDS_DrawableAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_DrawableAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_DrawableAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
