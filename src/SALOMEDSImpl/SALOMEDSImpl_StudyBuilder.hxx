// Copyright (C) 2007-2016  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
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
//
#ifndef __SALOMEDSImpl_STUDYBUILDER_H__
#define __SALOMEDSImpl_STUDYBUILDER_H__

#include "SALOMEDSImpl_Defines.hxx"
// std C++ headers
#include <iostream>
#include <string>
#include <vector>

#include "SALOMEDSImpl_Callback.hxx"
#include "SALOMEDSImpl_Driver.hxx"

class SALOMEDSImpl_Study;

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_StudyBuilder
{
private:
  DF_Document*             _doc;
  SALOMEDSImpl_Study*      _study;  
  SALOMEDSImpl_Callback*   _callbackOnAdd;
  SALOMEDSImpl_Callback*   _callbackOnRemove;
  std::string              _errorCode;

public:

  SALOMEDSImpl_StudyBuilder(const SALOMEDSImpl_Study* theOwner);

  ~SALOMEDSImpl_StudyBuilder();

  virtual SALOMEDSImpl_SComponent NewComponent(const std::string& ComponentDataType);

  virtual bool DefineComponentInstance (const SALOMEDSImpl_SComponent&, const std::string& ComponentIOR);

  virtual bool RemoveComponent(const SALOMEDSImpl_SComponent& aComponent);

  virtual SALOMEDSImpl_SObject NewObject(const SALOMEDSImpl_SObject& theFatherObject);

  virtual SALOMEDSImpl_SObject NewObjectToTag(const SALOMEDSImpl_SObject& theFatherObject, 
    const int theTag);

  //! The methods adds a new subdirectory, the path can be absolute or relative (then the current context is used)
  virtual bool AddDirectory(const std::string& thePath);

  virtual bool LoadWith(const SALOMEDSImpl_SComponent& sco, SALOMEDSImpl_Driver* Engine);
  virtual bool Load(const SALOMEDSImpl_SObject& sco);

  virtual bool RemoveObject(const SALOMEDSImpl_SObject& anObject);
  virtual bool RemoveObjectWithChildren(const SALOMEDSImpl_SObject& anObject);

  virtual DF_Attribute* FindOrCreateAttribute(const SALOMEDSImpl_SObject& anObject, 
                                              const std::string& aTypeOfAttribute);
  virtual bool FindAttribute(const SALOMEDSImpl_SObject& anObject, 
                             DF_Attribute*& anAttribute, 
                             const std::string& aTypeOfAttribute);

  virtual bool RemoveAttribute(const SALOMEDSImpl_SObject& anObject, const std::string& aTypeOfAttribute);

  virtual bool Addreference(const SALOMEDSImpl_SObject& me, 
                            const SALOMEDSImpl_SObject& thereferencedObject);

  virtual bool RemoveReference(const SALOMEDSImpl_SObject& me);

  virtual bool SetGUID(const SALOMEDSImpl_SObject& anObject, const std::string& theGUID);
  virtual bool IsGUID(const SALOMEDSImpl_SObject& anObject, const std::string& theGUID);

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

  virtual SALOMEDSImpl_Callback* SetOnAddSObject(const SALOMEDSImpl_Callback* theCallback);
  virtual SALOMEDSImpl_Callback* SetOnRemoveSObject(const SALOMEDSImpl_Callback* theCallback);

  virtual bool SetName(const SALOMEDSImpl_SObject& theSO, const std::string& theValue);

  virtual bool SetComment(const SALOMEDSImpl_SObject& theSO, const std::string& theValue);

  virtual bool SetIOR(const SALOMEDSImpl_SObject& theSO, const std::string& theValue);

  virtual std::string GetErrorCode() { return _errorCode; }
  virtual bool IsError() { return _errorCode != ""; }

  virtual SALOMEDSImpl_Study* GetOwner() { return _study; }
};
#endif
