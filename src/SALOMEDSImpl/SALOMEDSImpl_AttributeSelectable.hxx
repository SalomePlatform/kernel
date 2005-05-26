//  File   : SALOMEDSImpl_AttributeSelectable.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeSelectable_HeaderFile
#define _SALOMEDSImpl_AttributeSelectable_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_Label.hxx>       
#include <TCollection_AsciiString.hxx>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeSelectable, SALOMEDSImpl_GenericAttribute )



class SALOMEDSImpl_AttributeSelectable : public SALOMEDSImpl_GenericAttribute 
{
private:
Standard_Integer myValue;

public:

Standard_EXPORT virtual TCollection_AsciiString Save() { return (myValue == 0)?(char*)"0":(char*)"1"; }
Standard_EXPORT virtual void Load(const TCollection_AsciiString& theValue) { (theValue == "0")?myValue=0:myValue=1; }  

Standard_EXPORT static const Standard_GUID& GetID() ;
Standard_EXPORT static  Handle_SALOMEDSImpl_AttributeSelectable Set(const TDF_Label& label,const Standard_Integer value) ;
Standard_EXPORT SALOMEDSImpl_AttributeSelectable();
Standard_EXPORT void SetSelectable(const Standard_Integer value);
Standard_EXPORT Standard_Integer IsSelectable() const { return myValue; } 
Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT   void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT   Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT   void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDSImpl_AttributeSelectable() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeDrawable )

};

#endif
