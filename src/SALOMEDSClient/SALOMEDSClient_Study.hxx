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

//  File   : SALOMEDSClient_Study.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDSClient_STUDY_H__
#define __SALOMEDSClient_STUDY_H__

#include <vector>
#include <string>

#include "SALOMEDSClient_definitions.hxx"
#include "SALOMEDSClient_SComponentIterator.hxx"
#include "SALOMEDSClient_StudyBuilder.hxx"
#include "SALOMEDSClient_AttributeParameter.hxx"
#include "SALOMEDSClient_SObject.hxx"
#include "SALOMEDSClient_SComponent.hxx"
#include "SALOMEDSClient_UseCaseBuilder.hxx"
#include "SALOMEDSClient_AttributeStudyProperties.hxx"
#include "SALOMEDSClient_ChildIterator.hxx"
#include "SALOMEDSClient_Observer.hxx"
#include <SALOMEconfig.h>
#include CORBA_CLIENT_HEADER(SALOMEDS)

class SALOMEDSClient_Study
{

public:
  virtual ~SALOMEDSClient_Study() {}

  virtual std::string GetPersistentReference() = 0;
  virtual std::string GetTransientReference() = 0;
  virtual bool IsEmpty() = 0;
  virtual _PTR(SComponent) FindComponent (const std::string& aComponentName) = 0;
  virtual _PTR(SComponent) FindComponentID(const std::string& aComponentID) = 0;
  virtual _PTR(SObject) FindObject(const std::string& anObjectName) = 0;
  virtual std::vector<_PTR(SObject)> FindObjectByName( const std::string& anObjectName, const std::string& aComponentName ) = 0;  
  virtual _PTR(SObject) FindObjectID(const std::string& anObjectID) = 0;
  virtual _PTR(SObject) CreateObjectID(const std::string& anObjectID) = 0;
  virtual _PTR(SObject) FindObjectIOR(const std::string& anObjectIOR) = 0;
  virtual _PTR(SObject) FindObjectByPath(const std::string& thePath) = 0;
  virtual std::string GetObjectPath(const _PTR(SObject)& theSO) = 0;
  virtual void SetContext(const std::string& thePath) = 0;
  virtual std::string GetContext() = 0;  
  virtual std::vector<std::string> GetObjectNames(const std::string& theContext) = 0;
  virtual std::vector<std::string> GetDirectoryNames(const std::string& theContext) = 0;
  virtual std::vector<std::string> GetFileNames(const std::string& theContext) = 0;
  virtual std::vector<std::string> GetComponentNames(const std::string& theContext) = 0;
  virtual _PTR(ChildIterator) NewChildIterator(const _PTR(SObject)& theSO) = 0;
  virtual _PTR(SComponentIterator) NewComponentIterator() = 0;
  virtual _PTR(StudyBuilder) NewBuilder() = 0;
  virtual std::string Name() = 0;
  virtual void  Name(const std::string& name) = 0;
  virtual bool IsSaved() = 0;
  virtual void  IsSaved(bool save) = 0;
  virtual bool IsModified() = 0;
  virtual void Modified() = 0;
  virtual std::string URL() = 0;
  virtual void  URL(const std::string& url) = 0;
  virtual int StudyId() = 0;
  virtual void  StudyId(int id) = 0;
  virtual std::vector<_PTR(SObject)> FindDependances(const _PTR(SObject)& theSO) = 0;
  virtual _PTR(AttributeStudyProperties) GetProperties() = 0;
  virtual std::string GetLastModificationDate() = 0;
  virtual std::vector<std::string> GetModificationsDate() = 0;
  virtual _PTR(UseCaseBuilder) GetUseCaseBuilder() = 0;
  virtual void Close() = 0;
  virtual void EnableUseCaseAutoFilling(bool isEnabled) = 0;
  virtual bool DumpStudy(const std::string& thePath, 
                         const std::string& theBaseName, 
                         bool isPublished,
                         bool isMultiFile) = 0;
  virtual _PTR(AttributeParameter) GetCommonParameters(const std::string& theID, int theSavePoint) = 0;
  virtual _PTR(AttributeParameter) GetModuleParameters(const std::string& theID, 
                                                       const std::string& theModuleName, int theSavePoint) = 0;
  virtual void SetStudyLock(const std::string& theLockerID) = 0;
  virtual bool IsStudyLocked() = 0;
  virtual void UnLockStudy(const std::string& theLockerID) = 0;
  virtual std::vector<std::string> GetLockerID() = 0;

  virtual void SetReal(const std::string& theVarName, const double theValue) = 0;
  virtual void SetInteger(const std::string& theVarName, const int theValue) = 0;
  virtual void SetBoolean(const std::string& theVarName, const bool theValue) = 0;  
  virtual void SetString(const std::string& theVarName, const std::string& theValue) = 0;
  virtual void SetStringAsDouble(const std::string& theVarName, const double theValue) = 0;

  virtual double GetReal(const std::string& theVarName) = 0;
  virtual int GetInteger(const std::string& theVarName) = 0;
  virtual bool GetBoolean(const std::string& theVarName) = 0;
  virtual std::string GetString(const std::string& theVarName) = 0;
  
  virtual bool IsReal(const std::string& theVarName) = 0;
  virtual bool IsInteger(const std::string& theVarName) = 0;
  virtual bool IsBoolean(const std::string& theVarName) = 0;
  virtual bool IsString(const std::string& theVarName) = 0;
  
  virtual bool IsVariable(const std::string& theVarName) = 0;
  virtual std::vector<std::string> GetVariableNames() = 0;

  virtual bool RemoveVariable(const std::string& theVarName) = 0;
  virtual bool RenameVariable(const std::string& theVarName,
                              const std::string& theNewVarName) = 0;
  virtual bool IsVariableUsed(const std::string& theVarName) = 0;

  virtual std::vector< std::vector<std::string> > ParseVariables(const std::string& theVars) = 0;

  virtual void attach(SALOMEDS::Observer_ptr theObserver,bool modify) = 0;
  virtual void detach(SALOMEDS::Observer_ptr theObserver) = 0;
};


#endif
