//  File   : SALOMEDSImpl_StudyBuilder.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSImpl_STUDYBUILDER_H__
#define __SALOMEDSImpl_STUDYBUILDER_H__

//Handle definition
#include <Handle_MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_StudyBuilder, MMgt_TShared )

// std C++ headers
#include <iostream.h>

// Cascade header
#include <TCollection_AsciiString.hxx>
#include <TDocStd_Document.hxx>
 
#include "SALOMEDSImpl_Callback.hxx"
#include "SALOMEDSImpl_Driver.hxx"

class SALOMEDSImpl_StudyBuilder : public MMgt_TShared 
{
private:
  Handle(TDocStd_Document)        _doc;
  Handle(Standard_Transient)      _study;  
  Handle(SALOMEDSImpl_Callback)   _callbackOnAdd;
  Handle(SALOMEDSImpl_Callback)   _callbackOnRemove;
  TCollection_AsciiString         _errorCode;

public:
    
  SALOMEDSImpl_StudyBuilder(const Handle(Standard_Transient)& theOwner);

  ~SALOMEDSImpl_StudyBuilder();

  virtual Handle(SALOMEDSImpl_SComponent) NewComponent(const TCollection_AsciiString& ComponentDataType);

  virtual bool DefineComponentInstance (const Handle(SALOMEDSImpl_SComponent)&, const TCollection_AsciiString& ComponentIOR);
  
  virtual bool RemoveComponent(const Handle(SALOMEDSImpl_SComponent)& aComponent);

  virtual Handle(SALOMEDSImpl_SObject) NewObject(const Handle(SALOMEDSImpl_SObject)& theFatherObject);

  virtual Handle(SALOMEDSImpl_SObject) NewObjectToTag(const Handle(SALOMEDSImpl_SObject)& theFatherObject, 
                                                      const int theTag);

  //! The methods adds a new subdirectory, the path can be absolute or relative (then the current context is used)
  virtual bool AddDirectory(const TCollection_AsciiString& thePath);

  virtual bool LoadWith(const Handle(SALOMEDSImpl_SComponent)& sco, SALOMEDSImpl_Driver* Engine);
  virtual bool Load(const Handle(SALOMEDSImpl_SObject)& sco);

  virtual bool RemoveObject(const Handle(SALOMEDSImpl_SObject)& anObject);
  virtual bool RemoveObjectWithChildren(const Handle(SALOMEDSImpl_SObject)& anObject);

  virtual Handle(TDF_Attribute) FindOrCreateAttribute(const Handle(SALOMEDSImpl_SObject)& anObject, 
                                                      const TCollection_AsciiString& aTypeOfAttribute);
  virtual bool FindAttribute(const Handle(SALOMEDSImpl_SObject)& anObject, 
                             Handle(TDF_Attribute)& anAttribute, 
                             const TCollection_AsciiString& aTypeOfAttribute);

  virtual bool RemoveAttribute(const Handle(SALOMEDSImpl_SObject)& anObject, const TCollection_AsciiString& aTypeOfAttribute);

  virtual bool Addreference(const Handle(SALOMEDSImpl_SObject)& me, 
                            const Handle(SALOMEDSImpl_SObject)& thereferencedObject);

  virtual bool RemoveReference(const Handle(SALOMEDSImpl_SObject)& me);

  virtual bool SetGUID(const Handle(SALOMEDSImpl_SObject)& anObject, const TCollection_AsciiString& theGUID);
  virtual bool IsGUID(const Handle(SALOMEDSImpl_SObject)& anObject, const TCollection_AsciiString& theGUID);

  virtual void NewCommand();
  virtual void CommitCommand();
  virtual bool HasOpenCommand();
  virtual void AbortCommand();
  virtual void Undo();
  virtual void Redo();
  bool GetAvailableUndos();
  bool GetAvailableRedos();
  bool IsSaved();
  bool IsModified();
  virtual int UndoLimit();
  virtual void UndoLimit(const int);

  void CheckLocked();

  virtual Handle(SALOMEDSImpl_Callback) SetOnAddSObject(const Handle(SALOMEDSImpl_Callback)& theCallback);
  virtual Handle(SALOMEDSImpl_Callback) SetOnRemoveSObject(const Handle(SALOMEDSImpl_Callback)& theCallback);

  virtual bool SetName(const Handle(SALOMEDSImpl_SObject)& theSO, const TCollection_AsciiString& theValue);

  virtual bool SetComment(const Handle(SALOMEDSImpl_SObject)& theSO, const TCollection_AsciiString& theValue);

  virtual bool SetIOR(const Handle(SALOMEDSImpl_SObject)& theSO, const TCollection_AsciiString& theValue);

  virtual TCollection_AsciiString GetErrorCode() { return _errorCode; }
  virtual bool IsError() { return _errorCode != ""; }

  virtual Handle(Standard_Transient) GetOwner() { return _study; }

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_StudyBuilder )
};
#endif
