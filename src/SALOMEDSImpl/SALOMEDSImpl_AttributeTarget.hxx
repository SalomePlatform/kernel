//  File   : SALOMEDSImpl_AttributeTarget.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef _SALOMEDSImpl_AttributeTarget_HeaderFile
#define _SALOMEDSImpl_AttributeTarget_HeaderFile

#include <Standard_DefineHandle.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_LabelList.hxx>
#include <TDF_Label.hxx>       
#include <TDF_AttributeList.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#include "SALOMEDSImpl_GenericAttribute.hxx"
#include "SALOMEDSImpl_SObject.hxx"

class Standard_GUID;
class Handle(TDF_Attribute);
class Handle(TDF_RelocationTable);


DEFINE_STANDARD_HANDLE( SALOMEDSImpl_AttributeTarget, SALOMEDSImpl_GenericAttribute )


class SALOMEDSImpl_AttributeTarget :  public SALOMEDSImpl_GenericAttribute 
{
private:
TCollection_ExtendedString myRelation;
TDF_AttributeList myVariables;

public:
Standard_EXPORT static const Standard_GUID& GetID() ;
Standard_EXPORT static  Handle_SALOMEDSImpl_AttributeTarget Set(const TDF_Label& label) ;
Standard_EXPORT SALOMEDSImpl_AttributeTarget();
Standard_EXPORT void SALOMEDSImpl_AttributeTarget::Add(const Handle(SALOMEDSImpl_SObject)& theSO);
Standard_EXPORT Handle(TColStd_HSequenceOfTransient) SALOMEDSImpl_AttributeTarget::Get();
Standard_EXPORT void SALOMEDSImpl_AttributeTarget::Remove(const Handle(SALOMEDSImpl_SObject)& theSO);
Standard_EXPORT TCollection_ExtendedString GetRelation() { return myRelation; }
Standard_EXPORT void SetRelation(const TCollection_ExtendedString& theRelation); 
Standard_EXPORT TDF_AttributeList& GetVariables() { return myVariables; }
Standard_EXPORT  const Standard_GUID& ID() const;
Standard_EXPORT  void Restore(const Handle(TDF_Attribute)& with) ;
Standard_EXPORT  Handle_TDF_Attribute NewEmpty() const;
Standard_EXPORT  void Paste(const Handle(TDF_Attribute)& into,const Handle(TDF_RelocationTable)& RT) const;
Standard_EXPORT ~SALOMEDSImpl_AttributeTarget() {}

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_AttributeTarget )

};

#endif
