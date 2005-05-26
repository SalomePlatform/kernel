//  File   : SALOMEDSImpl_AttributeReal.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeReal_HeaderFile
#define _SALOMEDSImpl_AttributeReal_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TCollection_AsciiString.hxx>
#include <TDF_Label.hxx>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeReal, SALOMEDSImpl_GenericAttribute )


class SALOMEDSImpl_AttributeReal : public SALOMEDSImpl_GenericAttribute 
{
private:
  Standard_Real myValue;
public:

Standard_EXPORT  Standard_EXPORT static const Standard_GUID& GetID() ;

Standard_EXPORT  SALOMEDSImpl_AttributeReal():SALOMEDSImpl_GenericAttribute("AttributeReal") {}
Standard_EXPORT  static Handle(SALOMEDSImpl_AttributeReal) Set (const TDF_Label& L, const Standard_Real Val);
Standard_EXPORT  void SetValue(const Standard_Real theVal);
Standard_EXPORT  Standard_Real Value() const { return myValue; }  
Standard_EXPORT  virtual TCollection_AsciiString Save() { return TCollection_AsciiString(myValue); }
Standard_EXPORT  virtual void Load(const TCollection_AsciiString& theValue) { myValue = theValue.RealValue(); } 

Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT  void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT  Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT  void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDSImpl_AttributeReal() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeReal )

};

#endif
