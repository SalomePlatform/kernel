//  File      : Handle_SALOMEDS_SequenceOfRealAttribute.hxx
//  Created   : Fri Jul 05 10:55:39 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef _Handle_SALOMEDS_SequenceOfRealAttribute_HeaderFile
#define _Handle_SALOMEDS_SequenceOfRealAttribute_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_TDF_Attribute_HeaderFile
#include <Handle_TDF_Attribute.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(TDF_Attribute);
class SALOMEDS_SequenceOfRealAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_SequenceOfRealAttribute);

class Handle(SALOMEDS_SequenceOfRealAttribute) : public Handle(TDF_Attribute) {
  public:
    void* operator new(size_t,void* anAddress) 
      {
        return anAddress;
      }
    void* operator new(size_t size) 
      { 
        return Standard::Allocate(size); 
      }
    void  operator delete(void *anAddress) 
      { 
        if (anAddress) Standard::Free((Standard_Address&)anAddress); 
      }
    Handle(SALOMEDS_SequenceOfRealAttribute)():Handle(TDF_Attribute)() {} 
    Handle(SALOMEDS_SequenceOfRealAttribute)(const Handle(SALOMEDS_SequenceOfRealAttribute)& aHandle) : Handle(TDF_Attribute)(aHandle) 
     {
     }

    Handle(SALOMEDS_SequenceOfRealAttribute)(const SALOMEDS_SequenceOfRealAttribute* anItem) : Handle(TDF_Attribute)((TDF_Attribute *)anItem) 
     {
     }

    Handle(SALOMEDS_SequenceOfRealAttribute)& operator=(const Handle(SALOMEDS_SequenceOfRealAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_SequenceOfRealAttribute)& operator=(const SALOMEDS_SequenceOfRealAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_SequenceOfRealAttribute* operator->() 
     {
      return (SALOMEDS_SequenceOfRealAttribute *)ControlAccess();
     }

    SALOMEDS_SequenceOfRealAttribute* operator->() const 
     {
      return (SALOMEDS_SequenceOfRealAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_SequenceOfRealAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_SequenceOfRealAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
