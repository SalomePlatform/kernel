//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_DataMapIteratorOfDataMapOfIntegerString.hxx
//  Author : Sergey Ruin
//  Module : SALOME

#ifndef _SALOMEDS_DataMapIteratorOfDataMapOfIntegerString_HeaderFile
#define _SALOMEDS_DataMapIteratorOfDataMapOfIntegerString_HeaderFile

#ifndef _TCollection_BasicMapIterator_HeaderFile
#include <TCollection_BasicMapIterator.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Handle_SALOMEDS_DataMapNodeOfDataMapOfIntegerString_HeaderFile
#include <Handle_SALOMEDS_DataMapNodeOfDataMapOfIntegerString.hxx>
#endif
class Standard_NoSuchObject;
class TCollection_ExtendedString;
class TColStd_MapIntegerHasher;
class SALOMEDS_DataMapOfIntegerString;
class SALOMEDS_DataMapNodeOfDataMapOfIntegerString;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

class SALOMEDS_DataMapIteratorOfDataMapOfIntegerString  : public TCollection_BasicMapIterator {

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
 // Methods PUBLIC
 // 
Standard_EXPORT SALOMEDS_DataMapIteratorOfDataMapOfIntegerString();
Standard_EXPORT SALOMEDS_DataMapIteratorOfDataMapOfIntegerString(const SALOMEDS_DataMapOfIntegerString& aMap);
Standard_EXPORT   void Initialize(const SALOMEDS_DataMapOfIntegerString& aMap) ;
Standard_EXPORT  const Standard_Real& Key() const;
Standard_EXPORT  const TCollection_ExtendedString& Value() const;





protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 


 // Fields PRIVATE
 //


};





// other Inline functions and methods (like "C++: function call" methods)
//


#endif
