//  File      : Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel.hxx
//  Created   : Tue Aug 13 14:05:03 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 

#ifndef _Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel_HeaderFile
#define _Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel_HeaderFile

#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif
#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif

#ifndef _Handle_TCollection_MapNode_HeaderFile
#include <Handle_TCollection_MapNode.hxx>
#endif

class Standard_Transient;
class Handle_Standard_Type;
class Handle(TCollection_MapNode);
class SALOMEDS_DataMapNodeOfDataMapStringLabel;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_DataMapNodeOfDataMapStringLabel);

class Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel) : public Handle(TCollection_MapNode) {
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
    Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel)():Handle(TCollection_MapNode)() {} 
    Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel)(const Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel)& aHandle) : Handle(TCollection_MapNode)(aHandle) 
     {
     }

    Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel)(const SALOMEDS_DataMapNodeOfDataMapStringLabel* anItem) : Handle(TCollection_MapNode)((TCollection_MapNode *)anItem) 
     {
     }

    Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel)& operator=(const Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel)& operator=(const SALOMEDS_DataMapNodeOfDataMapStringLabel* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_DataMapNodeOfDataMapStringLabel* operator->() 
     {
      return (SALOMEDS_DataMapNodeOfDataMapStringLabel *)ControlAccess();
     }

    SALOMEDS_DataMapNodeOfDataMapStringLabel* operator->() const 
     {
      return (SALOMEDS_DataMapNodeOfDataMapStringLabel *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel)();
 
   Standard_EXPORT static const Handle(SALOMEDS_DataMapNodeOfDataMapStringLabel) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
