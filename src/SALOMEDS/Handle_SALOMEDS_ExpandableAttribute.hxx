//  File      : Handle_SALOMEDS_ExpandableAttribute.hxx
//  Created   : Tue Jul 09 16:42:12 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef _Handle_SALOMEDS_ExpandableAttribute_HeaderFile
#define _Handle_SALOMEDS_ExpandableAttribute_HeaderFile

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
class SALOMEDS_ExpandableAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_ExpandableAttribute);

class Handle(SALOMEDS_ExpandableAttribute) : public Handle(TDataStd_Integer) {
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
    Handle(SALOMEDS_ExpandableAttribute)():Handle(TDataStd_Integer)() {} 
    Handle(SALOMEDS_ExpandableAttribute)(const Handle(SALOMEDS_ExpandableAttribute)& aHandle) : Handle(TDataStd_Integer)(aHandle) 
     {
     }

    Handle(SALOMEDS_ExpandableAttribute)(const SALOMEDS_ExpandableAttribute* anItem) : Handle(TDataStd_Integer)((TDataStd_Integer *)anItem) 
     {
     }

    Handle(SALOMEDS_ExpandableAttribute)& operator=(const Handle(SALOMEDS_ExpandableAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_ExpandableAttribute)& operator=(const SALOMEDS_ExpandableAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_ExpandableAttribute* operator->() 
     {
      return (SALOMEDS_ExpandableAttribute *)ControlAccess();
     }

    SALOMEDS_ExpandableAttribute* operator->() const 
     {
      return (SALOMEDS_ExpandableAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_ExpandableAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_ExpandableAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
