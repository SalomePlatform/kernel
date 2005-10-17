//  File   : SALOMEDSImpl_AttributeLocalID.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeLocalID_HeaderFile
#define _SALOMEDSImpl_AttributeLocalID_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TCollection_AsciiString.hxx> 
#include <TDF_Label.hxx>       
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeLocalID, SALOMEDSImpl_GenericAttribute )



class SALOMEDSImpl_AttributeLocalID : public SALOMEDSImpl_GenericAttribute 
{
private:
Standard_Integer myValue;

public:

Standard_EXPORT  virtual TCollection_AsciiString Save() { return TCollection_AsciiString(myValue); }
Standard_EXPORT  virtual void Load(const TCollection_AsciiString& theValue) { myValue = theValue.IntegerValue(); }  

Standard_EXPORT static const Standard_GUID& GetID() ;
Standard_EXPORT static  Handle_SALOMEDSImpl_AttributeLocalID Set(const TDF_Label& label,const Standard_Integer value) ;
Standard_EXPORT  void SetValue(const Standard_Integer value);  
Standard_EXPORT Standard_Integer Value() const { return myValue; }  
Standard_EXPORT SALOMEDSImpl_AttributeLocalID();
Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT   void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT   Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT   void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDSImpl_AttributeLocalID() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeLocalID )
};

#endif
