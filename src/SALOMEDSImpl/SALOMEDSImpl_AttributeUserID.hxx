//  File   : SALOMEDSImpl_AttributeUAttribute.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeUserID_HeaderFile
#define _SALOMEDSImpl_AttributeUserID_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <Standard_GUID.hxx>
#include <TDF_Label.hxx>       
#include <TCollection_AsciiString.hxx>
#include "SALOMEDSImpl_GenericAttribute.hxx"

class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeUserID, SALOMEDSImpl_GenericAttribute )

class SALOMEDSImpl_AttributeUserID : public SALOMEDSImpl_GenericAttribute 
{
private:
Standard_GUID myID;   

public:
SALOMEDSImpl_AttributeUserID():SALOMEDSImpl_GenericAttribute("AttributeUserID") {}
static Handle(SALOMEDSImpl_AttributeUserID) Set (const TDF_Label& L, const Standard_GUID& ID);
static const Standard_GUID& DefaultID() 
{
  static Standard_GUID SALOMEDSImpl_DefaultUserAttributeID ("FFFFFFFF-D9CD-11d6-945D-1050DA506788");
  return SALOMEDSImpl_DefaultUserAttributeID;
}   

Standard_EXPORT  const Standard_GUID& Value() const { return ID(); }
Standard_EXPORT  void SetValue(const Standard_GUID& value);

Standard_EXPORT  virtual TCollection_AsciiString Type(); 

Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT  void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT  Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT  void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDSImpl_AttributeUserID() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeUserID )

};

#endif
