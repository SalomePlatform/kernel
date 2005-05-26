//  File   : SALOMEDSImpl_AttributeOpened.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeOpened_HeaderFile
#define _SALOMEDSImpl_AttributeOpened_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TCollection_AsciiString.hxx>
#include <TDF_Label.hxx>       
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeOpened, SALOMEDSImpl_GenericAttribute )

class SALOMEDSImpl_AttributeOpened : public SALOMEDSImpl_GenericAttribute 
{
private:
Standard_Integer myValue;

public:

Standard_EXPORT virtual TCollection_AsciiString Save() { return (myValue == 0)?(char*)"0":(char*)"1"; }
Standard_EXPORT virtual void Load(const TCollection_AsciiString& theValue) { (theValue == "0")?myValue=0:myValue=1; } 

Standard_EXPORT static const Standard_GUID& GetID() ;
Standard_EXPORT static  Handle_SALOMEDSImpl_AttributeOpened Set(const TDF_Label& label,const Standard_Integer value) ;
Standard_EXPORT SALOMEDSImpl_AttributeOpened();
Standard_EXPORT void SetOpened(const Standard_Integer value); 
Standard_EXPORT int IsOpened() const { return myValue; }  
Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT   void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT   Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT   void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDSImpl_AttributeOpened() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeOpened )
};

#endif
