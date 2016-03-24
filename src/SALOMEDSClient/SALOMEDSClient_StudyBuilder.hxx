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

//  File   : SALOMEDSClient_StudyBuilder.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDSClient_STUDYBUILDER_H__
#define __SALOMEDSClient_STUDYBUILDER_H__

#include "SALOMEDSClient_definitions.hxx"
#include "SALOMEDSClient_SObject.hxx"
#include "SALOMEDSClient_SComponent.hxx"
#include "SALOMEDSClient_GenericAttribute.hxx"
#include <string> 

class SALOMEDSClient_StudyBuilder
{
public:
  virtual ~SALOMEDSClient_StudyBuilder() {}

  virtual _PTR(SComponent) NewComponent(const std::string& ComponentDataType) = 0;
  virtual void DefineComponentInstance (const _PTR(SComponent)&, const std::string& ComponentIOR) = 0;
  virtual void RemoveComponent(const _PTR(SComponent)& theSCO) = 0;
  virtual _PTR(SObject) NewObject(const _PTR(SObject)& theFatherObject) = 0;
  virtual _PTR(SObject) NewObjectToTag(const _PTR(SObject)& theFatherObject, int theTag) = 0;
  virtual void AddDirectory(const std::string& thePath) = 0;
  virtual void LoadWith(const _PTR(SComponent)& theSCO, const std::string& theIOR) = 0;
  virtual void Load(const _PTR(SObject)& theSCO) = 0;
  virtual void RemoveObject(const _PTR(SObject)& theSO) = 0;
  virtual void RemoveObjectWithChildren(const _PTR(SObject)& theSO) = 0;
  virtual _PTR(GenericAttribute) FindOrCreateAttribute(const _PTR(SObject)& theSO, const std::string& aTypeOfAttribute) = 0;
  virtual bool FindAttribute(const _PTR(SObject)& theSO, 
                             _PTR(GenericAttribute)& theAttribute, 
                             const std::string& aTypeOfAttribute) = 0;
  virtual void RemoveAttribute(const _PTR(SObject)& theSO, const std::string& aTypeOfAttribute) = 0;
  virtual void Addreference(const _PTR(SObject)& me, const _PTR(SObject)& thereferencedObject) = 0;
  virtual void RemoveReference(const _PTR(SObject)& me) = 0;
  virtual void SetGUID(const _PTR(SObject)& theSO, const std::string& theGUID) = 0;
  virtual bool IsGUID(const _PTR(SObject)& theSO, const std::string& theGUID) = 0;
  virtual void NewCommand() = 0;
  virtual void CommitCommand()= 0;
  virtual bool HasOpenCommand() = 0;
  virtual void AbortCommand() = 0;
  virtual void Undo() = 0;
  virtual void Redo() = 0;
  virtual bool GetAvailableUndos() = 0;
  virtual bool GetAvailableRedos() = 0;
  virtual int UndoLimit() = 0;
  virtual void UndoLimit(int theLimit) = 0;
  virtual void SetName(const _PTR(SObject)& theSO, const std::string& theValue) = 0;
  virtual void SetComment(const _PTR(SObject)& theSO, const std::string& theValue) = 0;
  virtual void SetIOR(const _PTR(SObject)& theSO, const std::string& theValue) = 0;
};

#endif
