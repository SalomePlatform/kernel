//  File      : Handle_SALOMEDS_OCAFApplication.hxx
//  Created   : Thu Nov 29 20:59:51 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#ifndef _Handle_SALOMEDS_OCAFApplication_HeaderFile
#define _Handle_SALOMEDS_OCAFApplication_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_TDocStd_Application_HeaderFile
#include <Handle_TDocStd_Application.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(TDocStd_Application);
class SALOMEDS_OCAFApplication;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_OCAFApplication);

class Handle(SALOMEDS_OCAFApplication) : public Handle(TDocStd_Application) {
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
    Handle(SALOMEDS_OCAFApplication)():Handle(TDocStd_Application)() {} 
    Handle(SALOMEDS_OCAFApplication)(const Handle(SALOMEDS_OCAFApplication)& aHandle) : Handle(TDocStd_Application)(aHandle) 
     {
     }

    Handle(SALOMEDS_OCAFApplication)(const SALOMEDS_OCAFApplication* anItem) : Handle(TDocStd_Application)((TDocStd_Application *)anItem) 
     {
     }

    Handle(SALOMEDS_OCAFApplication)& operator=(const Handle(SALOMEDS_OCAFApplication)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_OCAFApplication)& operator=(const SALOMEDS_OCAFApplication* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_OCAFApplication* operator->() 
     {
      return (SALOMEDS_OCAFApplication *)ControlAccess();
     }

    SALOMEDS_OCAFApplication* operator->() const 
     {
      return (SALOMEDS_OCAFApplication *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_OCAFApplication)();
 
   Standard_EXPORT static const Handle(SALOMEDS_OCAFApplication) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
