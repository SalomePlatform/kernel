//  File      : SALOMEDS_OCAFApplication.hxx
//  Created   : Thu Nov 29 21:05:19 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#ifndef _SALOMEDS_OCAFApplication_HeaderFile
#define _SALOMEDS_OCAFApplication_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_SALOMEDS_OCAFApplication_HeaderFile
#include "Handle_SALOMEDS_OCAFApplication.hxx"
#endif

#ifndef _TDocStd_Application_HeaderFile
#include <TDocStd_Application.hxx>
#endif
#ifndef _Standard_CString_HeaderFile
#include <Standard_CString.hxx>
#endif
class TColStd_SequenceOfExtendedString;


class SALOMEDS_OCAFApplication : public TDocStd_Application {

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
Standard_EXPORT SALOMEDS_OCAFApplication();
Standard_EXPORT virtual  void Formats(TColStd_SequenceOfExtendedString& Formats) ;
Standard_EXPORT   Standard_CString ResourcesName() ;
Standard_EXPORT ~SALOMEDS_OCAFApplication();




 // Type management
 //
 Standard_EXPORT friend Handle_Standard_Type& SALOMEDS_OCAFApplication_Type_();
 Standard_EXPORT const Handle(Standard_Type)& DynamicType() const;
 Standard_EXPORT Standard_Boolean	       IsKind(const Handle(Standard_Type)&) const;

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
