//  File      : SALOMEDS_SequenceOfIntegerAttribute.hxx
//  Created   : Fri Jul 05 12:59:44 2002
//  Author    : Yves FRICAUD

//  Project   : SALOME
//  Module    : SALOMEDS
//  Copyright : Open CASCADE 2002
//  $Header$

#ifndef _SALOMEDS_SequenceOfIntegerAttribute_HeaderFile
#define _SALOMEDS_SequenceOfIntegerAttribute_HeaderFile

#ifndef _Standard_HeaderFile
#include <Standard.hxx>
#endif
#ifndef _Handle_SALOMEDS_SequenceOfIntegerAttribute_HeaderFile
#include <Handle_SALOMEDS_SequenceOfIntegerAttribute.hxx>
#endif

#ifndef _Handle_TColStd_HSequenceOfInteger_HeaderFile
#include <Handle_TColStd_HSequenceOfInteger.hxx>
#endif
#ifndef _TDF_Attribute_HeaderFile
#include <TDF_Attribute.hxx>
#endif
#ifndef _Standard_Integer_HeaderFile
#include <Standard_Integer.hxx>
#endif
#ifndef _Handle_TDF_Attribute_HeaderFile
#include <Handle_TDF_Attribute.hxx>
#endif
#ifndef _Handle_TDF_RelocationTable_HeaderFile
#include <Handle_TDF_RelocationTable.hxx>
#endif
class TColStd_HSequenceOfInteger;
class Standard_GUID;
class TDF_Label;
class TDF_Attribute;
class TDF_RelocationTable;


class SALOMEDS_SequenceOfIntegerAttribute : public TDF_Attribute {

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
Standard_EXPORT static const Standard_GUID& GetID() ;
Standard_EXPORT static  Handle_SALOMEDS_SequenceOfIntegerAttribute Set(const TDF_Label& label) ;
Standard_EXPORT SALOMEDS_SequenceOfIntegerAttribute();
Standard_EXPORT   void Assign(const Handle(TColStd_HSequenceOfInteger)& other) ;
Standard_EXPORT   void ChangeValue(const Standard_Integer Index,const Standard_Integer Value) ;
Standard_EXPORT   void Add(const Standard_Integer value) ;
Standard_EXPORT   Standard_Integer Value(const Standard_Integer Index) ;
Standard_EXPORT   void Remove(const Standard_Integer Index) ;
Standard_EXPORT   Standard_Integer Length() ;
Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT   void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT   Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT   void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDS_SequenceOfIntegerAttribute();




 // Type management
 //
 Standard_EXPORT friend Handle_Standard_Type& SALOMEDS_SequenceOfIntegerAttribute_Type_();
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
Handle_TColStd_HSequenceOfInteger myValue;


};





// other Inline functions and methods (like "C++: function call" methods)
//


#endif
