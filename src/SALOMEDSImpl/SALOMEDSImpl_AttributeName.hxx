//  File   : SALOMEDSImpl_AttributeIOR.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeName_HeaderFile
#define _SALOMEDSImpl_AttributeName_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TDF_Label.hxx> 
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeName, SALOMEDSImpl_GenericAttribute )


class SALOMEDSImpl_AttributeName : public SALOMEDSImpl_GenericAttribute 
{

private:

 TCollection_ExtendedString myString;

public:
Standard_EXPORT static const Standard_GUID& GetID() ;

Standard_EXPORT  SALOMEDSImpl_AttributeName():SALOMEDSImpl_GenericAttribute("AttributeName") {}
Standard_EXPORT  static Handle(SALOMEDSImpl_AttributeName) Set (const TDF_Label& L, const TCollection_ExtendedString& Val);
Standard_EXPORT  void SetValue(const TCollection_ExtendedString& theVal);
Standard_EXPORT  TCollection_ExtendedString Value() const { return myString; }  

Standard_EXPORT  virtual TCollection_AsciiString Save() { return myString; }
Standard_EXPORT  virtual void Load(const TCollection_AsciiString& theValue) { myString = theValue; } 

Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT  void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT  Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT  void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;

Standard_EXPORT ~SALOMEDSImpl_AttributeName() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeName )
};

#endif
