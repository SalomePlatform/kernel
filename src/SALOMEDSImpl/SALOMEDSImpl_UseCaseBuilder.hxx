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
  SALOMEDSImpl_UseCaseBuilder(const Handle(TDocStd_Document)& theDocument);
  
  //! standard destructor
  ~SALOMEDSImpl_UseCaseBuilder();
  
  virtual bool Append(const Handle(SALOMEDSImpl_SObject)& theObject);

  virtual bool Remove(const Handle(SALOMEDSImpl_SObject)& theObject);

  virtual bool AppendTo(const Handle(SALOMEDSImpl_SObject)& theFather, const Handle(SALOMEDSImpl_SObject)& theObject);

  virtual bool InsertBefore(const Handle(SALOMEDSImpl_SObject)& theFirst, const Handle(SALOMEDSImpl_SObject)& theNext);

  virtual bool  SetCurrentObject(const Handle(SALOMEDSImpl_SObject)& theObject);
  
  virtual bool SetRootCurrent();

  virtual bool  HasChildren(const Handle(SALOMEDSImpl_SObject)& theObject);

  virtual bool  IsUseCase(const Handle(SALOMEDSImpl_SObject)& theObject);

  virtual bool SetName(const TCollection_AsciiString& theName);

  virtual Handle(SALOMEDSImpl_SObject) GetCurrentObject();

  virtual TCollection_AsciiString GetName();

  virtual Handle(SALOMEDSImpl_SObject) AddUseCase(const TCollection_AsciiString& theName);

  virtual Handle(SALOMEDSImpl_UseCaseIterator) GetUseCaseIterator(const Handle(SALOMEDSImpl_SObject)& anObject);

  Handle(SALOMEDSImpl_SObject) GetSObject(const TCollection_AsciiString& theEntry);    

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_UseCaseBuilder )
};
#endif
