//  File      : Handle_SALOMEDS_TextHighlightColorAttribute.hxx
//  Created   : Wed Jul 10 12:52:57 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef _Handle_SALOMEDS_TextHighlightColorAttribute_HeaderFile
#define _Handle_SALOMEDS_TextHighlightColorAttribute_HeaderFile

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
class SALOMEDS_TextHighlightColorAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_TextHighlightColorAttribute);

class Handle(SALOMEDS_TextHighlightColorAttribute) : public Handle(TDataStd_RealArray) {
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
    Handle(SALOMEDS_TextHighlightColorAttribute)():Handle(TDataStd_RealArray)() {} 
    Handle(SALOMEDS_TextHighlightColorAttribute)(const Handle(SALOMEDS_TextHighlightColorAttribute)& aHandle) : Handle(TDataStd_RealArray)(aHandle) 
     {
     }

    Handle(SALOMEDS_TextHighlightColorAttribute)(const SALOMEDS_TextHighlightColorAttribute* anItem) : Handle(TDataStd_RealArray)((TDataStd_RealArray *)anItem) 
     {
     }

    Handle(SALOMEDS_TextHighlightColorAttribute)& operator=(const Handle(SALOMEDS_TextHighlightColorAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_TextHighlightColorAttribute)& operator=(const SALOMEDS_TextHighlightColorAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_TextHighlightColorAttribute* operator->() 
     {
      return (SALOMEDS_TextHighlightColorAttribute *)ControlAccess();
     }

    SALOMEDS_TextHighlightColorAttribute* operator->() const 
     {
      return (SALOMEDS_TextHighlightColorAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_TextHighlightColorAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_TextHighlightColorAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
