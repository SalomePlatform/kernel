//  File      : SALOMEDS_DataMapIteratorOfDataMapStringLabel.hxx
//  Created   : Tue Aug 13 14:05:03 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header: 

#ifndef _SALOMEDS_DataMapIteratorOfDataMapStringLabel_HeaderFile
#define _SALOMEDS_DataMapIteratorOfDataMapStringLabel_HeaderFile

#ifndef _TCollection_BasicMapIterator_HeaderFile
#include <TCollection_BasicMapIterator.hxx>
#endif
#ifndef _Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel_HeaderFile
#include <Handle_SALOMEDS_DataMapNodeOfDataMapStringLabel.hxx>
#endif
class Standard_NoSuchObject;
class TCollection_ExtendedString;
class TDF_Label;
class SALOMEDS_DataMapStringLabel;
class SALOMEDS_DataMapNodeOfDataMapStringLabel;


#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Standard_Macro_HeaderFile
#include <Standard_Macro.hxx>
#endif

class SALOMEDS_DataMapIteratorOfDataMapStringLabel  : public TCollection_BasicMapIterator {

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
Standard_EXPORT SALOMEDS_DataMapIteratorOfDataMapStringLabel();
Standard_EXPORT SALOMEDS_DataMapIteratorOfDataMapStringLabel(const SALOMEDS_DataMapStringLabel& aMap);
Standard_EXPORT   void Initialize(const SALOMEDS_DataMapStringLabel& aMap) ;
Standard_EXPORT  const TCollection_ExtendedString& Key() const;
Standard_EXPORT  const TDF_Label& Value() const;





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





// other inline functions and methods (like "C++: function call" methods)
//


#endif
