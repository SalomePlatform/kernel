//  File      : Handle_SALOMEDS_SequenceOfIntegerAttribute.hxx
//  Created   : Fri Jul 05 10:55:49 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef _Handle_SALOMEDS_SequenceOfIntegerAttribute_HeaderFile
#define _Handle_SALOMEDS_SequenceOfIntegerAttribute_HeaderFile

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
class SALOMEDS_SequenceOfIntegerAttribute;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_SequenceOfIntegerAttribute);

class Handle(SALOMEDS_SequenceOfIntegerAttribute) : public Handle(TDF_Attribute) {
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
    Handle(SALOMEDS_SequenceOfIntegerAttribute)():Handle(TDF_Attribute)() {} 
    Handle(SALOMEDS_SequenceOfIntegerAttribute)(const Handle(SALOMEDS_SequenceOfIntegerAttribute)& aHandle) : Handle(TDF_Attribute)(aHandle) 
     {
     }

    Handle(SALOMEDS_SequenceOfIntegerAttribute)(const SALOMEDS_SequenceOfIntegerAttribute* anItem) : Handle(TDF_Attribute)((TDF_Attribute *)anItem) 
     {
     }

    Handle(SALOMEDS_SequenceOfIntegerAttribute)& operator=(const Handle(SALOMEDS_SequenceOfIntegerAttribute)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_SequenceOfIntegerAttribute)& operator=(const SALOMEDS_SequenceOfIntegerAttribute* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_SequenceOfIntegerAttribute* operator->() 
     {
      return (SALOMEDS_SequenceOfIntegerAttribute *)ControlAccess();
     }

    SALOMEDS_SequenceOfIntegerAttribute* operator->() const 
     {
      return (SALOMEDS_SequenceOfIntegerAttribute *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_SequenceOfIntegerAttribute)();
 
   Standard_EXPORT static const Handle(SALOMEDS_SequenceOfIntegerAttribute) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
