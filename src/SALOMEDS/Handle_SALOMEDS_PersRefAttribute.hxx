//  File      : Handle_SALOMEDS_PersRefAttribute.hxx
//  Created   : Thu Nov 29 21:00:10 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#ifndef _Handle_SALOMEDS_PersRefAttribute_HeaderFile
#define _Handle_SALOMEDS_PersRefAttribute_HeaderFile

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
class SALOMEDS_PersRefAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_PersRefAttribute);

class Handle(SALOMEDS_PersRefAttribute) : public Handle(TDataStd_Comment) {
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
    Handle(SALOMEDS_PersRefAttribute)():Handle(TDataStd_Comment)() {} 
    Handle(SALOMEDS_PersRefAttribute)(const Handle(SALOMEDS_PersRefAttribute)& aHandle) : Handle(TDataStd_Comment)(aHandle) 
     {
     }

    Handle(SALOMEDS_PersRefAttribute)(const SALOMEDS_PersRefAttribute* anItem) : Handle(TDataStd_Comment)((TDataStd_Comment *)anItem) 
     {
     }

    Handle(SALOMEDS_PersRefAttribute)& operator=(const Handle(SALOMEDS_PersRefAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_PersRefAttribute)& operator=(const SALOMEDS_PersRefAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_PersRefAttribute* operator->() 
     {
      return (SALOMEDS_PersRefAttribute *)ControlAccess();
     }

    SALOMEDS_PersRefAttribute* operator->() const 
     {
      return (SALOMEDS_PersRefAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_PersRefAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_PersRefAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
