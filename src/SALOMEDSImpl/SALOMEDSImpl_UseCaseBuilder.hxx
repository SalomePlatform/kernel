//  File   : SALOMEDSImpl_UseCaseBuilder.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_USECaseBuilder_H__
#define __SALOMEDSIMPL_USECaseBuilder_H__

//Handle definition
#include <Handle_MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_UseCaseBuilder, MMgt_TShared )

// Cascade headers
#include <SALOMEDSImpl_AttributeTreeNode.hxx>
#include <TDocStd_Document.hxx>
#include <TCollection_AsciiString.hxx>
#include <Standard_GUID.hxx>

#include "SALOMEDSImpl_UseCaseIterator.hxx"

class SALOMEDSImpl_UseCaseBuilder : public MMgt_TShared
{
private:

  Handle(SALOMEDSImpl_AttributeTreeNode)     _root;
  Handle(TDocStd_Document)                   _doc;

public:

  //! standard constructor  
  Standard_EXPORT SALOMEDSImpl_UseCaseBuilder(const Handle(TDocStd_Document)& theDocument);
  
  //! standard destructor
  Standard_EXPORT ~SALOMEDSImpl_UseCaseBuilder();
  
  Standard_EXPORT virtual bool Append(const Handle(SALOMEDSImpl_SObject)& theObject);

  Standard_EXPORT virtual bool Remove(const Handle(SALOMEDSImpl_SObject)& theObject);

  Standard_EXPORT virtual bool AppendTo(const Handle(SALOMEDSImpl_SObject)& theFather, const Handle(SALOMEDSImpl_SObject)& theObject);

  Standard_EXPORT virtual bool InsertBefore(const Handle(SALOMEDSImpl_SObject)& theFirst, const Handle(SALOMEDSImpl_SObject)& theNext);

  Standard_EXPORT virtual bool  SetCurrentObject(const Handle(SALOMEDSImpl_SObject)& theObject);
  
  Standard_EXPORT virtual bool SetRootCurrent();

  Standard_EXPORT virtual bool  HasChildren(const Handle(SALOMEDSImpl_SObject)& theObject);

  Standard_EXPORT virtual bool  IsUseCase(const Handle(SALOMEDSImpl_SObject)& theObject);

  Standard_EXPORT virtual bool SetName(const TCollection_AsciiString& theName);

  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) GetCurrentObject();

  Standard_EXPORT virtual TCollection_AsciiString GetName();

  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) AddUseCase(const TCollection_AsciiString& theName);

  Standard_EXPORT virtual Handle(SALOMEDSImpl_UseCaseIterator) GetUseCaseIterator(const Handle(SALOMEDSImpl_SObject)& anObject);

  Standard_EXPORT Handle(SALOMEDSImpl_SObject) GetSObject(const TCollection_AsciiString& theEntry);    

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_UseCaseBuilder )
};
#endif
