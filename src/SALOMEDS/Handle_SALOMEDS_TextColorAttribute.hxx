//  File      : Handle_SALOMEDS_TextColorAttribute.hxx
//  Created   : Wed Jul 10 12:52:50 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef _Handle_SALOMEDS_TextColorAttribute_HeaderFile
#define _Handle_SALOMEDS_TextColorAttribute_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_TDataStd_RealArray_HeaderFile
#include <Handle_TDataStd_RealArray.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(TDataStd_RealArray);
class SALOMEDS_TextColorAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_TextColorAttribute);

class Handle(SALOMEDS_TextColorAttribute) : public Handle(TDataStd_RealArray) {
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
    Handle(SALOMEDS_TextColorAttribute)():Handle(TDataStd_RealArray)() {} 
    Handle(SALOMEDS_TextColorAttribute)(const Handle(SALOMEDS_TextColorAttribute)& aHandle) : Handle(TDataStd_RealArray)(aHandle) 
     {
     }

    Handle(SALOMEDS_TextColorAttribute)(const SALOMEDS_TextColorAttribute* anItem) : Handle(TDataStd_RealArray)((TDataStd_RealArray *)anItem) 
     {
     }

    Handle(SALOMEDS_TextColorAttribute)& operator=(const Handle(SALOMEDS_TextColorAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_TextColorAttribute)& operator=(const SALOMEDS_TextColorAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_TextColorAttribute* operator->() 
     {
      return (SALOMEDS_TextColorAttribute *)ControlAccess();
     }

    SALOMEDS_TextColorAttribute* operator->() const 
     {
      return (SALOMEDS_TextColorAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_TextColorAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_TextColorAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
