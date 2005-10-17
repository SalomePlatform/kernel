//  File   : SALOMEDSImpl_AttributePersistentRef.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributePersistentRef_HeaderFile
#define _SALOMEDSImpl_AttributePersistentRef_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TCollection_AsciiString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TDF_Label.hxx>       
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributePersistentRef, SALOMEDSImpl_GenericAttribute )

class SALOMEDSImpl_AttributePersistentRef : public SALOMEDSImpl_GenericAttribute 
{
private:
 TCollection_ExtendedString myString;

public:

Standard_EXPORT virtual TCollection_AsciiString Save() { return myString; }
Standard_EXPORT virtual void Load(const TCollection_AsciiString& theValue) { myString = theValue; } 

Standard_EXPORT static const Standard_GUID& GetID() ;
Standard_EXPORT static  Handle_SALOMEDSImpl_AttributePersistentRef Set(const TDF_Label& label,
								       const TCollection_ExtendedString& string) ;
Standard_EXPORT SALOMEDSImpl_AttributePersistentRef();
Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT void SetValue(const TCollection_ExtendedString& value); 
Standard_EXPORT TCollection_ExtendedString Value() const { return myString; }
Standard_EXPORT   void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT   Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT   void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDSImpl_AttributePersistentRef() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributePersistentRef )
};

#endif
