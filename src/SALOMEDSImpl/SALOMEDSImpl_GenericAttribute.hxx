//  File   : SALOMEDSImpl_GenericAttribute.hxx
//  Author : SERGEY_RUIN
//  Module : SALOME

#ifndef _GENERICIMPL_ATTRIBUTE_HXX_
#define _GENERICIMPL_ATTRIBUTE_HXX_

#include <TDF_Label.hxx>
#include <TCollection_AsciiString.hxx>
#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>     

#include "SALOMEDSImpl_SObject.hxx"

class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_GenericAttribute, TDF_Attribute )

class SALOMEDSImpl_GenericAttribute: public TDF_Attribute
{
protected:

  TCollection_AsciiString _type; //This field must be filled in each attribute that inherits from this class.

public:

Standard_EXPORT SALOMEDSImpl_GenericAttribute(const TCollection_AsciiString& theType) 
:_type(theType)
{}

Standard_EXPORT virtual TCollection_AsciiString Save() { return ""; } 
Standard_EXPORT virtual void Load(const TCollection_AsciiString&) {}
Standard_EXPORT virtual TCollection_AsciiString Type();
Standard_EXPORT virtual void CheckLocked();
Standard_EXPORT TCollection_AsciiString GetClassType() { return _type; }
Standard_EXPORT Handle(SALOMEDSImpl_SObject) GetSObject();
Standard_EXPORT void SetModifyFlag();

Standard_EXPORT static char* Impl_GetType(const Handle(TDF_Attribute)& theAttr); 
Standard_EXPORT static char* Impl_GetClassType(const Handle(TDF_Attribute)& theAttr);
Standard_EXPORT static void Impl_CheckLocked(const Handle(TDF_Attribute)& theAttr);

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_GenericAttribute )

};


#endif
