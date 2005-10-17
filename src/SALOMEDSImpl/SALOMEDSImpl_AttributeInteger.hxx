//  File   : SALOMEDSImpl_AttributeInteger.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeInteger_HeaderFile
#define _SALOMEDSImpl_AttributeInteger_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TCollection_AsciiString.hxx>
#include <TDF_Label.hxx>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeInteger, SALOMEDSImpl_GenericAttribute )


class SALOMEDSImpl_AttributeInteger : public SALOMEDSImpl_GenericAttribute 
{
private:
Standard_Integer myValue;

public:
Standard_EXPORT static const Standard_GUID& GetID() ;

Standard_EXPORT  SALOMEDSImpl_AttributeInteger():SALOMEDSImpl_GenericAttribute("AttributeInteger") {} 
Standard_EXPORT  static Handle(SALOMEDSImpl_AttributeInteger) Set (const TDF_Label& L, Standard_Integer Val); 
Standard_EXPORT  void SetValue(const Standard_Integer theVal);
Standard_EXPORT  Standard_Integer Value() const { return myValue; } 
Standard_EXPORT  virtual TCollection_AsciiString Save() { return TCollection_AsciiString(myValue); }
Standard_EXPORT  virtual void Load(const TCollection_AsciiString& theValue) { myValue = theValue.IntegerValue(); } 

Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT  void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT  Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT  void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDSImpl_AttributeInteger() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeInteger )

};

#endif
