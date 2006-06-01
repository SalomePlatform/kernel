//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : Handle_SALOMEDS_DataMapNodeOfDataMapOfIntegerString.hxx
//  Author : Sergey Ruin
//  Module : SALOME

#ifndef _Handle_SALOMEDS_DataMapNodeOfDataMapOfIntegerString_HeaderFile
#define _Handle_SALOMEDS_DataMapNodeOfDataMapOfIntegerString_HeaderFile

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
class SALOMEDS_DataMapNodeOfDataMapOfIntegerString;
Standard_EXPORT Handle_Standard_Type& STANDARD_TYPE(SALOMEDS_DataMapNodeOfDataMapOfIntegerString);

class Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString) : public Handle(TCollection_MapNode) {
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
    Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString)():Handle(TCollection_MapNode)() {} 
    Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString)(const Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString)& aHandle) : Handle(TCollection_MapNode)(aHandle) 
     {
     }

    Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString)(const SALOMEDS_DataMapNodeOfDataMapOfIntegerString* anItem) : Handle(TCollection_MapNode)((TCollection_MapNode *)anItem) 
     {
     }

    Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString)& operator=(const Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString)& aHandle)
     {
      Assign(aHandle.Access());
      return *this;
     }

    Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString)& operator=(const SALOMEDS_DataMapNodeOfDataMapOfIntegerString* anItem)
     {
      Assign((Standard_Transient *)anItem);
      return *this;
     }

    SALOMEDS_DataMapNodeOfDataMapOfIntegerString* operator->() 
     {
      return (SALOMEDS_DataMapNodeOfDataMapOfIntegerString *)ControlAccess();
     }

    SALOMEDS_DataMapNodeOfDataMapOfIntegerString* operator->() const 
     {
      return (SALOMEDS_DataMapNodeOfDataMapOfIntegerString *)ControlAccess();
     }

   Standard_EXPORT ~Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString)();
 
   Standard_EXPORT static const Handle(SALOMEDS_DataMapNodeOfDataMapOfIntegerString) DownCast(const Handle(Standard_Transient)& AnObject);
};
#endif
