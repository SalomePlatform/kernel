//  File      : SALOMEDS_DataMapStringLabel.hxx
//  Created   : Tue Aug 13 14:05:03 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 

#ifndef _SALOMEDS_DataMapStringLabel_HeaderFile
#define _SALOMEDS_DataMapStringLabel_HeaderFile

#ifndef _TCollection_BasicMap_HeaderFile
#include <TCollection_BasicMap.hxx>
#endif
#ifndef _Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel_HeaderFile
#include <Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel.hxx>
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
class TDF_Label;
class SALOMEDS_DataMapNodeOfDataMapStringLabel;
class SALOMEDS_DataMapIteratorOfDataMapStringLabel;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

class SALOMEDS_DataMapStringLabel  : public TCollection_BasicMap {

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
 // Methods PUBLIC
 // 
Standard_EXPORT SALOMEDS_DataMapStringLabel(const Standard_Integer NbBuckets = 1);
Standard_EXPORT   SALOMEDS_DataMapStringLabel& Assign(const SALOMEDS_DataMapStringLabel& Other) ;
  SALOMEDS_DataMapStringLabel& operator =(const SALOMEDS_DataMapStringLabel& Other) 
{
  return Assign(Other);
}

Standard_EXPORT   void ReSize(const Standard_Integer NbBuckets) ;
Standard_EXPORT   void Clear() ;
~SALOMEDS_DataMapStringLabel()
{
  Clear();
}

Standard_EXPORT   Standard_Boolean Bind(const TCollection_ExtendedString& K,const TDF_Label& I) ;
Standard_EXPORT   Standard_Boolean IsBound(const TCollection_ExtendedString& K) const;
Standard_EXPORT   Standard_Boolean UnBind(const TCollection_ExtendedString& K) ;
Standard_EXPORT  const TDF_Label& Find(const TCollection_ExtendedString& K) const;
 const TDF_Label& operator()(const TCollection_ExtendedString& K) const
{
  return Find(K);
}

Standard_EXPORT   TDF_Label& ChangeFind(const TCollection_ExtendedString& K) ;
  TDF_Label& operator()(const TCollection_ExtendedString& K) 
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
Standard_EXPORT SALOMEDS_DataMapStringLabel(const SALOMEDS_DataMapStringLabel& Other);


 // Fields PRIVATE
 //


};





// other inline functions and methods (like "C++: function call" methods)
//


#endif
