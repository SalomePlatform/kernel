//  File   : SALOMEDSImpl_AttributeTextColor.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeTextColor_HeaderFile
#define _SALOMEDSImpl_AttributeTextColor_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TColStd_HArray1OfReal.hxx>
#include <TDF_Label.hxx>       
#include <TCollection_AsciiString.hxx>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeTextColor, SALOMEDSImpl_GenericAttribute )

class SALOMEDSImpl_AttributeTextColor : public SALOMEDSImpl_GenericAttribute 
{
private:
  Handle(TColStd_HArray1OfReal) myValue;

public:
Standard_EXPORT virtual TCollection_AsciiString Save();
Standard_EXPORT virtual void Load(const TCollection_AsciiString&); 
Standard_EXPORT static const Standard_GUID& GetID() ;
Standard_EXPORT SALOMEDSImpl_AttributeTextColor();
Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT  void SetTextColor(const Standard_Real R, const Standard_Real G, const Standard_Real B); 
Standard_EXPORT  Handle(TColStd_HArray1OfReal) TextColor();  
Standard_EXPORT  void ChangeArray(const Handle(TColStd_HArray1OfReal)& newArray);
Standard_EXPORT   void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT   Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT   void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDSImpl_AttributeTextColor() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeTextColor )
};

#endif
