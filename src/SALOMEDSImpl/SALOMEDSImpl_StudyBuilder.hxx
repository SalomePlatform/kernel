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

// std C++ headers
#include <iostream>
#include <string>
#include <vector>
 
#include "SALOMEDSImpl_Callback.hxx"
#include "SALOMEDSImpl_Driver.hxx"

class SALOMEDSImpl_Study;

class SALOMEDSImpl_StudyBuilder
{
private:
  DF_Document*             _doc;
  SALOMEDSImpl_Study*      _study;  
  SALOMEDSImpl_Callback*   _callbackOnAdd;
  SALOMEDSImpl_Callback*   _callbackOnRemove;
  std::string              _errorCode;

public:
    
  Standard_EXPORT SALOMEDSImpl_StudyBuilder(const SALOMEDSImpl_Study* theOwner);

  Standard_EXPORT ~SALOMEDSImpl_StudyBuilder();

  Standard_EXPORT virtual SALOMEDSImpl_SComponent NewComponent(const std::string& ComponentDataType);

  Standard_EXPORT virtual bool DefineComponentInstance (const SALOMEDSImpl_SComponent&, const std::string& ComponentIOR);
  
  Standard_EXPORT virtual bool RemoveComponent(const SALOMEDSImpl_SComponent& aComponent);

  Standard_EXPORT virtual SALOMEDSImpl_SObject NewObject(const SALOMEDSImpl_SObject& theFatherObject);

  Standard_EXPORT virtual SALOMEDSImpl_SObject NewObjectToTag(const SALOMEDSImpl_SObject& theFatherObject, 
                                                       const int theTag);

  //! The methods adds a new subdirectory, the path can be absolute or relative (then the current context is used)
  Standard_EXPORT virtual bool AddDirectory(const std::string& thePath);

  Standard_EXPORT virtual bool LoadWith(const SALOMEDSImpl_SComponent& sco, SALOMEDSImpl_Driver* Engine);
  Standard_EXPORT virtual bool Load(const SALOMEDSImpl_SObject& sco);

  Standard_EXPORT virtual bool RemoveObject(const SALOMEDSImpl_SObject& anObject);
  Standard_EXPORT virtual bool RemoveObjectWithChildren(const SALOMEDSImpl_SObject& anObject);

  Standard_EXPORT virtual DF_Attribute* FindOrCreateAttribute(const SALOMEDSImpl_SObject& anObject, 
                                                              const std::string& aTypeOfAttribute);
  Standard_EXPORT virtual bool FindAttribute(const SALOMEDSImpl_SObject& anObject, 
                                             DF_Attribute*& anAttribute, 
                                             const std::string& aTypeOfAttribute);

  Standard_EXPORT virtual bool RemoveAttribute(const SALOMEDSImpl_SObject& anObject, const std::string& aTypeOfAttribute);

  Standard_EXPORT virtual bool Addreference(const SALOMEDSImpl_SObject& me, 
                                            const SALOMEDSImpl_SObject& thereferencedObject);

  Standard_EXPORT virtual bool RemoveReference(const SALOMEDSImpl_SObject& me);

  Standard_EXPORT virtual bool SetGUID(const SALOMEDSImpl_SObject& anObject, const std::string& theGUID);
  Standard_EXPORT virtual bool IsGUID(const SALOMEDSImpl_SObject& anObject, const std::string& theGUID);

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

  Standard_EXPORT virtual SALOMEDSImpl_Callback* SetOnAddSObject(const SALOMEDSImpl_Callback* theCallback);
  Standard_EXPORT virtual SALOMEDSImpl_Callback* SetOnRemoveSObject(const SALOMEDSImpl_Callback* theCallback);

  Standard_EXPORT virtual bool SetName(const SALOMEDSImpl_SObject& theSO, const std::string& theValue);

  Standard_EXPORT virtual bool SetComment(const SALOMEDSImpl_SObject& theSO, const std::string& theValue);

  Standard_EXPORT virtual bool SetIOR(const SALOMEDSImpl_SObject& theSO, const std::string& theValue);

  Standard_EXPORT virtual std::string GetErrorCode() { return _errorCode; }
  Standard_EXPORT virtual bool IsError() { return _errorCode != ""; }

  Standard_EXPORT virtual SALOMEDSImpl_Study* GetOwner() { return _study; }
};
#endif
