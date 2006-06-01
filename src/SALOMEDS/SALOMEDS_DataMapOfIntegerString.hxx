//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  CEA/DEN, EDF R&D
//
//  See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDS_DataMapOfIntegerString.hxx
//  Author : Sergey Ruin
//  Module : SALOME

#ifndef _SALOMEDS_DataMapOfIntegerString_HeaderFile
#define _SALOMEDS_DataMapOfIntegerString_HeaderFile

#ifndef _TCollection_BasicMap_HeaderFile
#include <TCollection_BasicMap.hxx>
#endif
#ifndef _Standard_Real_HeaderFile
#include <Standard_Real.hxx>
#endif
#ifndef _Handle_SALOMEDS_DataMapNodeOfDataMapOfIntegerString_HeaderFile
#include <Handle_SALOMEDS_DataMapNodeOfDataMapOfIntegerString.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Standard_Boolean_HeaderFile
#include <Standard_Boolean.hxx>
#endif
class Standard_DomainError;
class Standard_NoSuchObject;
class TCollection_ExtendedString;
class TColStd_MapIntegerHasher;
class SALOMEDS_DataMapNodeOfDataMapOfIntegerString;
class SALOMEDS_DataMapIteratorOfDataMapOfIntegerString;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

class SALOMEDS_DataMapOfIntegerString  : public TCollection_BasicMap {

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
Standard_EXPORT SALOMEDS_DataMapOfIntegerString(const Standard_Integer NbBuckets = 1);
Standard_EXPORT   SALOMEDS_DataMapOfIntegerString& Assign(const SALOMEDS_DataMapOfIntegerString& Other) ;
  SALOMEDS_DataMapOfIntegerString& operator =(const SALOMEDS_DataMapOfIntegerString& Other) 
{
  return Assign(Other);
}

Standard_EXPORT   void ReSize(const Standard_Integer NbBuckets) ;
Standard_EXPORT   void Clear() ;
~SALOMEDS_DataMapOfIntegerString()
{
  Clear();
}

Standard_EXPORT   Standard_Boolean Bind(const Standard_Real& K,const TCollection_ExtendedString& I) ;
Standard_EXPORT   Standard_Boolean IsBound(const Standard_Real& K) const;
Standard_EXPORT   Standard_Boolean UnBind(const Standard_Real& K) ;
Standard_EXPORT  const TCollection_ExtendedString& Find(const Standard_Real& K) const;
 const TCollection_ExtendedString& operator()(const Standard_Real& K) const
{
  return Find(K);
}

Standard_EXPORT   TCollection_ExtendedString& ChangeFind(const Standard_Real& K) ;
  TCollection_ExtendedString& operator()(const Standard_Real& K) 
{
  return ChangeFind(K);
}






protected:

 // Methods PROTECTED
 // 


 // Fields PROTECTED
 //


private: 

 // Methods PRIVATE
 // 
Standard_EXPORT SALOMEDS_DataMapOfIntegerString(const SALOMEDS_DataMapOfIntegerString& Other);


 // Fields PRIVATE
 //


};





// other Inline functions and methods (like "C++: function call" methods)
//


#endif
