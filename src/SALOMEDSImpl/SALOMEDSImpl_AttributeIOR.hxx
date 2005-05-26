//  File   : SALOMEDSImpl_AttributeIOR.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeIOR_HeaderFile
#define _SALOMEDSImpl_AttributeIOR_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TDF_Label.hxx>       
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeIOR, SALOMEDSImpl_GenericAttribute )

class SALOMEDSImpl_AttributeIOR : public SALOMEDSImpl_GenericAttribute 
{
private:

 TCollection_ExtendedString myString;

public:

Standard_EXPORT static const Standard_GUID& GetID() ;
Standard_EXPORT static Handle_SALOMEDSImpl_AttributeIOR Set(const TDF_Label& label,
							    const TCollection_ExtendedString& string);
Standard_EXPORT SALOMEDSImpl_AttributeIOR();
Standard_EXPORT const Standard_GUID& ID() const;
Standard_EXPORT void SetValue(const TCollection_ExtendedString& theValue);
Standard_EXPORT TCollection_ExtendedString Value() const ;
Standard_EXPORT void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDSImpl_AttributeIOR() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeIOR )
};

#endif
