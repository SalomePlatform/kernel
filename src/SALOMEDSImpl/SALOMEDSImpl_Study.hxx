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

//  File   : SALOMEDSImpl_Study.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDSIMPL_STUDY_I_H__
#define __SALOMEDSIMPL_STUDY_I_H__

// std C++ headers
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "DF_Document.hxx"
#include "DF_Label.hxx"
#include <stdio.h>

//SALOMEDSImpl headers
#include "SALOMEDSImpl_Defines.hxx"
#include "SALOMEDSImpl_SComponentIterator.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"
#include "SALOMEDSImpl_UseCaseBuilder.hxx"
#include "SALOMEDSImpl_AttributeStudyProperties.hxx"
#include "SALOMEDSImpl_AttributeIOR.hxx"
#include "SALOMEDSImpl_AttributeParameter.hxx"
#include "SALOMEDSImpl_Callback.hxx"
#include "SALOMEDSImpl_Driver.hxx" 
#include "SALOMEDSImpl_ChildIterator.hxx" 
#include "SALOMEDSImpl_GenericVariable.hxx"

class SALOMEDSImpl_StudyManager;
class SALOMEDSImpl_GenericAttribute;


class SALOMEDSIMPL_EXPORT SALOMEDSImpl_Study
{
private:
  std::string              _name;  
  DF_Document*             _doc;  // Document
  bool                     _Saved; // True if the Study is saved
  std::string              _URL; //URL of the persistent reference of the study
  int                      _StudyId; 
  DF_Label                 _current;
  bool                     _autoFill; 
  std::string              _errorCode;
  std::vector<std::string> _lockers;
  SALOMEDSImpl_Callback*   _cb;
  SALOMEDSImpl_StudyBuilder*   _builder;
  SALOMEDSImpl_UseCaseBuilder* _useCaseBuilder;
  SALOMEDSImpl_AbstractCallback* _notifier;
  SALOMEDSImpl_AbstractCallback* _genObjRegister;

  std::map<std::string, SALOMEDSImpl_SObject> _mapOfSO;
  std::map<std::string, SALOMEDSImpl_SComponent> _mapOfSCO;
  std::map<std::string, DF_Label> myIORLabels;
  std::vector<SALOMEDSImpl_GenericVariable*> myNoteBookVars;

  SALOMEDSImpl_SObject   _FindObject(const SALOMEDSImpl_SObject& SO,
    const std::string& anObjectName,
    bool& _find);

  SALOMEDSImpl_SObject   _FindObjectIOR(const SALOMEDSImpl_SObject& SO,
    const std::string& anObjectIOR,
    bool& _find);

  std::string _GetStudyVariablesScript();
  std::string _GetNoteBookAccessor();
  std::string _GetNoteBookAccess(const std::string& theStudyVar);

public:

  static SALOMEDSImpl_Study* GetStudy(const DF_Label& theLabel);
  static SALOMEDSImpl_SObject SObject(const DF_Label& theLabel);
  static SALOMEDSImpl_SComponent SComponent(const DF_Label& theLabel);
  static void IORUpdated(const SALOMEDSImpl_AttributeIOR* theAttribute);

   //! standard constructor
   SALOMEDSImpl_Study(const DF_Document*, const std::string& study_name);
  
  //! standard destructor
  virtual ~SALOMEDSImpl_Study(); 
  
  //! method to Get persistent reference of study (idem URL())
  virtual std::string GetPersistentReference();

  //! method to Get transient reference of study
  virtual std::string GetTransientReference();

  virtual void SetTransientReference(const std::string& theIOR);

  //! method to detect if a study is empty
  virtual bool IsEmpty();

  //! method to Find a Component with ComponentDataType = aComponentName
  virtual SALOMEDSImpl_SComponent FindComponent (const std::string& aComponentName);

  //! method to Find a Component Find a Component from it's ID
  virtual SALOMEDSImpl_SComponent FindComponentID(const std::string& aComponentID);

  //! method to  Find an Object with SALOMEDSImpl::Name = anObjectName 
  virtual SALOMEDSImpl_SObject FindObject(const std::string& anObjectName);


  //! method to Find Object(s) with SALOMEDSImpl::Name=anObjectName in a component with ComponentDataType = aComponentName
  virtual std::vector<SALOMEDSImpl_SObject> FindObjectByName( const std::string& anObjectName, 
                                                                       const std::string& aComponentName ) ;
  
  //! method to Find an Object with ID = anObjectID 
  virtual SALOMEDSImpl_SObject FindObjectID(const std::string& anObjectID);
  
  //! method to Create an Object with ID = anObjectID 
  virtual SALOMEDSImpl_SObject CreateObjectID(const std::string& anObjectID);

  //! method to Find an Object with ID = anObjectIOR 
  virtual SALOMEDSImpl_SObject FindObjectIOR(const std::string& anObjectIOR);

  //! method to Find an Object by its path
  virtual SALOMEDSImpl_SObject FindObjectByPath(const std::string& thePath);

  //! method to get a path of SObject
  virtual std::string GetObjectPath(const SALOMEDSImpl_SObject& theObject);

  std::string GetObjectPathByIOR(const std::string& theIOR);

  //! method to set a context: root ('/') is UserData component
  virtual bool SetContext(const std::string& thePath);

  //! method to get a context
  virtual std::string GetContext();  

  //! method to get all object names in the given context (or in the current context, if 'theContext' is empty)
  virtual std::vector<std::string> GetObjectNames(const std::string& theContext);

  //! method to get all directory names in the given context (or in the current context, if 'theContext' is empty)
  virtual std::vector<std::string> GetDirectoryNames(const std::string& theContext);

  //! method to get all file names in the given context (or in the current context, if 'theContext' is empty)
  virtual std::vector<std::string> GetFileNames(const std::string& theContext);

  //! method to get all components names
  virtual std::vector<std::string> GetComponentNames(const std::string& theContext);

  //! method to Create a ChildIterator from an SObject 
  virtual SALOMEDSImpl_ChildIterator NewChildIterator(const SALOMEDSImpl_SObject& aSO);

  //! method to Create a SComponentIterator 
  virtual SALOMEDSImpl_SComponentIterator NewComponentIterator();

  //! method to Create a StudyBuilder
  virtual SALOMEDSImpl_StudyBuilder* NewBuilder();
 
  //! method to get study name
  virtual std::string Name();

  //! method to set study name
  virtual void  Name(const std::string& name);
  
  //! method to get if study has been saved
  virtual bool IsSaved();

  //! method to set if study has been saved
  virtual void  IsSaved(bool save);

  //! method to Detect if a Study has been modified since it has been saved
  virtual bool IsModified();
  
  //! method to get URL of the study (idem GetPersistentReference) 
  virtual std::string URL();

  //! method to set URL of the study
  virtual void  URL(const std::string& url);

  virtual bool IsLocked();
  
  virtual int StudyId();

  virtual void  StudyId(int id);
  
  virtual void DeleteIORLabelMapItem(const std::string& anIOR);
  virtual void UpdateIORLabelMap(const std::string& anIOR, const std::string& aLabel);
  
  virtual std::vector<SALOMEDSImpl_SObject> FindDependances(const SALOMEDSImpl_SObject& anObject);
  
  virtual SALOMEDSImpl_AttributeStudyProperties* GetProperties();
  
  virtual std::string GetLastModificationDate();
  
  virtual std::vector<std::string> GetModificationsDate();
  
  virtual SALOMEDSImpl_UseCaseBuilder* GetUseCaseBuilder();
  
  virtual void Close();
  
  void EnableUseCaseAutoFilling(bool isEnabled);
  
  virtual std::string GetErrorCode() { return _errorCode; }
  virtual bool IsError() { return _errorCode != ""; }
  
  virtual SALOMEDSImpl_SComponent GetSComponent(const std::string& theEntry);
  virtual SALOMEDSImpl_SComponent GetSComponent(const DF_Label& theLabel);
  virtual SALOMEDSImpl_SObject GetSObject(const std::string& theEntry);
  virtual SALOMEDSImpl_SObject GetSObject(const DF_Label& theEntryLabel);
  virtual DF_Attribute* GetAttribute(const std::string& theEntry, 
                                                     const std::string& theType);

  virtual bool HasCurrentContext() { return !_current.IsNull(); }

  virtual bool DumpStudy(const std::string& thePath, 
                         const std::string& theBaseName, 
                         bool isPublished,
                         bool isMultiFile,
                         SALOMEDSImpl_DriverFactory* theFactory);

  static std::string GetDumpStudyComment(const char* theComponentName = 0);
  
  virtual DF_Document* GetDocument() { return _doc; } 

  //The method dump creates a txt file that contain a dump of the study, for debug use
  void dump(const std::string& theFileName);

  //This method marks the study as being modified
  void Modify();

  SALOMEDSImpl_AttributeParameter* GetCommonParameters(const char* theID, int theSavePoint);

  SALOMEDSImpl_AttributeParameter* GetModuleParameters(const char* theID, 
                                                                              const char* theModuleName,
                                                                              int theSavePoint);

  //Locks the study, theLockerID is identificator of the of the one who locked the study for ex. IOR
  void SetStudyLock(const char* theLockerID);

  //Returns True if the study is locked
  bool IsStudyLocked();

  //Unlocks the study
  void UnLockStudy(const char* theLockerID);
  
  //Returns an ID of the study locker
  std::vector<std::string> GetLockerID();

  //Managing of variables
  void SetVariable(const std::string& theVarName,
                   const double theValue, 
                   const SALOMEDSImpl_GenericVariable::VariableTypes);

  void SetStringVariable(const std::string& theVarName,
                         const std::string& theValue, 
                         const SALOMEDSImpl_GenericVariable::VariableTypes);

  void SetStringVariableAsDouble(const std::string& theVarName,
                                 const double theValue, 
                                 const SALOMEDSImpl_GenericVariable::VariableTypes);
  
  double GetVariableValue(const std::string& theVarName);

  std::string GetStringVariableValue(const std::string& theVarName);

  bool IsTypeOf(const std::string& theVarName,
                SALOMEDSImpl_GenericVariable::VariableTypes theType) const;
        
  bool IsVariable(const std::string& theVarName) const;


  std::vector<std::string> GetVariableNames() const;

  void AddVariable(SALOMEDSImpl_GenericVariable* theVariable);

  SALOMEDSImpl_GenericVariable* GetVariable(const std::string& theName) const;

  bool RemoveVariable(const std::string& theVarName);

  bool RenameVariable(const std::string& theVarName, const std::string& theNewVarName);

  bool IsVariableUsed(const std::string& theVarName);

  bool FindVariableAttribute(SALOMEDSImpl_StudyBuilder* theStudyBuilder,
                             SALOMEDSImpl_SObject theSObject,
                             const std::string& theName);
  bool FindVariableAttribute(const std::string& theName);

  void ReplaceVariableAttribute(SALOMEDSImpl_StudyBuilder* theStudyBuilder,
                                SALOMEDSImpl_SObject theSObject,
                                const std::string& theSource,
                                const std::string& theDest);
  void ReplaceVariableAttribute(const std::string& theSource, const std::string& theDest);

  std::vector< std::vector<std::string> > ParseVariables(const std::string& theVariables) const;

  //Returns a callback 
  SALOMEDSImpl_Callback* GetCallback() { return _cb; }

  //Returns a list of IOR's stored in the study
  std::vector<std::string> GetIORs();

  // Notification mechanism
  virtual bool addSO_Notification(const SALOMEDSImpl_SObject& theSObject);
  virtual bool removeSO_Notification(const SALOMEDSImpl_SObject& theSObject);
  virtual bool modifySO_Notification(const SALOMEDSImpl_SObject& theSObject, int reason);
  virtual void setNotifier(SALOMEDSImpl_AbstractCallback* notifier);

  static void RegisterGenObj  (const std::string& theIOR, DF_Label label);
  static void UnRegisterGenObj(const std::string& theIOR, DF_Label label);
  void setGenObjRegister(SALOMEDSImpl_AbstractCallback* theRegister);

  friend class SALOMEDSImpl_StudyManager;    
  friend class SALOMEDSImpl_GenericAttribute;
  friend class SALOMEDSImpl_GenericVariable;
};
#endif
