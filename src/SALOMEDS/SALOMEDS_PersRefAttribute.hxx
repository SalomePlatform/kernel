//  File      : SALOMEDS_PersRefAttribute.hxx
//  Created   : Thu Nov 29 21:01:34 2001
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2001
//  $Header$

#ifndef _SALOMEDS_PersRefAttribute_HeaderFile
#define _SALOMEDS_PersRefAttribute_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_SALOMEDS_PersRefAttribute_HeaderFile
#include <Handle_SALOMEDS_PersRefAttribute.hxx>
#endif

#ifndef _TDataStd_Comment_HeaderFile
#include <TDataStd_Comment.hxx>
#endif
#ifndef _Handle_TDF_Attribute_HeaderFile
#include <Handle_TDF_Attribute.hxx>
#endif
#ifndef _Handle_TDF_RelocationTable_HeaderFile
#include <Handle_TDF_RelocationTable.hxx>
#endif
class Standard_GUID;
class TDF_Label;
class TCollection_ExtendedString;
class TDF_Attribute;
class TDF_RelocationTable;


class SALOMEDS_PersRefAttribute : public TDataStd_Comment {

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
Standard_EXPORT static const Standard_GUID& GetID() ;
Standard_EXPORT static  Handle_SALOMEDS_PersRefAttribute Set(const TDF_Label& label,const TCollection_ExtendedString& string) ;
Standard_EXPORT SALOMEDS_PersRefAttribute();
Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT   void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT   Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT   void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDS_PersRefAttribute();




 // Type management
 //
 Standard_EXPORT friend Handle_Standard_Type& SALOMEDS_PersRefAttribute_Type_();
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
