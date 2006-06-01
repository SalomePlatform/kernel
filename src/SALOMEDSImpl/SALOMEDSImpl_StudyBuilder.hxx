// Copyright (C) 2005  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
// 
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either 
// version 2.1 of the License.
// 
// This library is distributed in the hope that it will be useful 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public  
// License along with this library; if not, write to the Free Software 
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
//
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
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
#include <iostream>

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
    
  Standard_EXPORT SALOMEDSImpl_StudyBuilder(const Handle(Standard_Transient)& theOwner);

  Standard_EXPORT ~SALOMEDSImpl_StudyBuilder();

  Standard_EXPORT virtual Handle(SALOMEDSImpl_SComponent) NewComponent(const TCollection_AsciiString& ComponentDataType);

  Standard_EXPORT virtual bool DefineComponentInstance (const Handle(SALOMEDSImpl_SComponent)&, const TCollection_AsciiString& ComponentIOR);
  
  Standard_EXPORT virtual bool RemoveComponent(const Handle(SALOMEDSImpl_SComponent)& aComponent);

  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) NewObject(const Handle(SALOMEDSImpl_SObject)& theFatherObject);

  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) NewObjectToTag(const Handle(SALOMEDSImpl_SObject)& theFatherObject, 
                                                      const int theTag);

  //! The methods adds a new subdirectory, the path can be absolute or relative (then the current context is used)
  Standard_EXPORT virtual bool AddDirectory(const TCollection_AsciiString& thePath);

  Standard_EXPORT virtual bool LoadWith(const Handle(SALOMEDSImpl_SComponent)& sco, SALOMEDSImpl_Driver* Engine);
  Standard_EXPORT virtual bool Load(const Handle(SALOMEDSImpl_SObject)& sco);

  Standard_EXPORT virtual bool RemoveObject(const Handle(SALOMEDSImpl_SObject)& anObject);
  Standard_EXPORT virtual bool RemoveObjectWithChildren(const Handle(SALOMEDSImpl_SObject)& anObject);

  Standard_EXPORT virtual Handle(TDF_Attribute) FindOrCreateAttribute(const Handle(SALOMEDSImpl_SObject)& anObject, 
                                                      const TCollection_AsciiString& aTypeOfAttribute);
  Standard_EXPORT virtual bool FindAttribute(const Handle(SALOMEDSImpl_SObject)& anObject, 
                             Handle(TDF_Attribute)& anAttribute, 
                             const TCollection_AsciiString& aTypeOfAttribute);

  Standard_EXPORT virtual bool RemoveAttribute(const Handle(SALOMEDSImpl_SObject)& anObject, const TCollection_AsciiString& aTypeOfAttribute);

  Standard_EXPORT virtual bool Addreference(const Handle(SALOMEDSImpl_SObject)& me, 
                            const Handle(SALOMEDSImpl_SObject)& thereferencedObject);

  Standard_EXPORT virtual bool RemoveReference(const Handle(SALOMEDSImpl_SObject)& me);

  Standard_EXPORT virtual bool SetGUID(const Handle(SALOMEDSImpl_SObject)& anObject, const TCollection_AsciiString& theGUID);
  Standard_EXPORT virtual bool IsGUID(const Handle(SALOMEDSImpl_SObject)& anObject, const TCollection_AsciiString& theGUID);

  Standard_EXPORT virtual void NewCommand();
  Standard_EXPORT virtual void CommitCommand();
  Standard_EXPORT virtual bool HasOpenCommand();
  Standard_EXPORT virtual void AbortCommand();
  Standard_EXPORT virtual void Undo();
  Standard_EXPORT virtual void Redo();
  Standard_EXPORT bool GetAvailableUndos();
  Standard_EXPORT bool GetAvailableRedos();
  Standard_EXPORT bool IsSaved();
  Standard_EXPORT bool IsModified();
  Standard_EXPORT virtual int UndoLimit();
  Standard_EXPORT virtual void UndoLimit(const int);

  Standard_EXPORT void CheckLocked();

  Standard_EXPORT virtual Handle(SALOMEDSImpl_Callback) SetOnAddSObject(const Handle(SALOMEDSImpl_Callback)& theCallback);
  Standard_EXPORT virtual Handle(SALOMEDSImpl_Callback) SetOnRemoveSObject(const Handle(SALOMEDSImpl_Callback)& theCallback);

  Standard_EXPORT virtual bool SetName(const Handle(SALOMEDSImpl_SObject)& theSO, const TCollection_AsciiString& theValue);

  Standard_EXPORT virtual bool SetComment(const Handle(SALOMEDSImpl_SObject)& theSO, const TCollection_AsciiString& theValue);

  Standard_EXPORT virtual bool SetIOR(const Handle(SALOMEDSImpl_SObject)& theSO, const TCollection_AsciiString& theValue);

  Standard_EXPORT virtual TCollection_AsciiString GetErrorCode() { return _errorCode; }
  Standard_EXPORT virtual bool IsError() { return _errorCode != ""; }

  Standard_EXPORT virtual Handle(Standard_Transient) GetOwner() { return _study; }

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_StudyBuilder )
};
#endif
