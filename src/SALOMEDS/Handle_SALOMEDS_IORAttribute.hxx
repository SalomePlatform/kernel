//  File      : Handle_SALOMEDS_IORAttribute.hxx
//  Created   : Thu Nov 29 20:59:28 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#ifndef _Handle_SALOMEDS_IORAttribute_HeaderFile
#define _Handle_SALOMEDS_IORAttribute_HeaderFile

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
class SALOMEDS_IORAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_IORAttribute);

class Handle(SALOMEDS_IORAttribute) : public Handle(TDataStd_Comment) {
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
    Handle(SALOMEDS_IORAttribute)():Handle(TDataStd_Comment)() {} 
    Handle(SALOMEDS_IORAttribute)(const Handle(SALOMEDS_IORAttribute)& aHandle) : Handle(TDataStd_Comment)(aHandle) 
     {
     }

    Handle(SALOMEDS_IORAttribute)(const SALOMEDS_IORAttribute* anItem) : Handle(TDataStd_Comment)((TDataStd_Comment *)anItem) 
     {
     }

    Handle(SALOMEDS_IORAttribute)& operator=(const Handle(SALOMEDS_IORAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_IORAttribute)& operator=(const SALOMEDS_IORAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_IORAttribute* operator->() 
     {
      return (SALOMEDS_IORAttribute *)ControlAccess();
     }

    SALOMEDS_IORAttribute* operator->() const 
     {
      return (SALOMEDS_IORAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_IORAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_IORAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
