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

//  File   : SALOMEDS_StudyBuilder.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDS_STUDYBUILDER_H__
#define __SALOMEDS_STUDYBUILDER_H__

#include "SALOMEDS_Defines.hxx"

#include "SALOMEDSClient.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS)

class SALOMEDS_EXPORT SALOMEDS_StudyBuilder: public SALOMEDSClient_StudyBuilder
{
private:
  bool _isLocal;
  SALOMEDSImpl_StudyBuilder*        _local_impl;
  SALOMEDS::StudyBuilder_var        _corba_impl;
  CORBA::ORB_var                    _orb;

public:

  SALOMEDS_StudyBuilder(SALOMEDSImpl_StudyBuilder* theBuilder);
  SALOMEDS_StudyBuilder(SALOMEDS::StudyBuilder_ptr theBuilder);
  ~SALOMEDS_StudyBuilder();

  virtual _PTR(SComponent) NewComponent(const std::string& ComponentDataType);
  virtual void DefineComponentInstance (const _PTR(SComponent)&, const std::string& ComponentIOR);
  virtual void RemoveComponent(const _PTR(SComponent)& theSCO);
  virtual _PTR(SObject) NewObject(const _PTR(SObject)& theFatherObject);
  virtual _PTR(SObject) NewObjectToTag(const _PTR(SObject)& theFatherObject, int theTag);
  virtual void AddDirectory(const std::string& thePath);
  virtual void LoadWith(const _PTR(SComponent)& theSCO, const std::string& theIOR);
  virtual void Load(const _PTR(SObject)& theSCO);
  virtual void RemoveObject(const _PTR(SObject)& theSO);
  virtual void RemoveObjectWithChildren(const _PTR(SObject)& theSO);
  virtual _PTR(GenericAttribute) FindOrCreateAttribute(const _PTR(SObject)& theSO, 
                                                       const std::string& aTypeOfAttribute);
  virtual bool FindAttribute(const _PTR(SObject)& theSO, 
                             _PTR(GenericAttribute)& theAttribute, 
                             const std::string& aTypeOfAttribute);
  virtual void RemoveAttribute(const _PTR(SObject)& theSO, const std::string& aTypeOfAttribute);
  virtual void Addreference(const _PTR(SObject)& me, const _PTR(SObject)& thereferencedObject);
  virtual void RemoveReference(const _PTR(SObject)& me);
  virtual void SetGUID(const _PTR(SObject)& theSO, const std::string& theGUID);
  virtual bool IsGUID(const _PTR(SObject)& theSO, const std::string& theGUID);
  virtual void NewCommand();
  virtual void CommitCommand();
  virtual bool HasOpenCommand();
  virtual void AbortCommand();
  virtual void Undo();
  virtual void Redo();
  virtual bool GetAvailableUndos();
  virtual bool GetAvailableRedos();
  virtual int UndoLimit();
  virtual void UndoLimit(int theLimit);
  virtual void SetName(const _PTR(SObject)& theSO, const std::string& theValue);
  virtual void SetComment(const _PTR(SObject)& theSO, const std::string& theValue);
  virtual void SetIOR(const _PTR(SObject)& theSO, const std::string& theValue);

  SALOMEDS::StudyBuilder_ptr GetBuilder();

private:
  void CheckLocked();
  void init_orb();
};
#endif
