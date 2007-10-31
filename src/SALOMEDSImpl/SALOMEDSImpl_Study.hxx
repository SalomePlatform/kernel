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
//  File   : SALOMEDSImpl_Study.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSIMPL_STUDY_I_H__
#define __SALOMEDSIMPL_STUDY_I_H__

// std C++ headers
#include <iostream>
#include <string>
#include <vector>
#include <map>

// Cascade headers
#include "DF_Document.hxx"
#include "DF_Label.hxx"
#include <stdio.h>

//SALOMEDSImpl headers
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

class SALOMEDSImpl_StudyManager;
class SALOMEDSImpl_GenericAttribute;


class SALOMEDSImpl_Study
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

  std::map<std::string, SALOMEDSImpl_SObject> _mapOfSO;
  std::map<std::string, SALOMEDSImpl_SComponent> _mapOfSCO;
  std::map<std::string, DF_Label> myIORLabels;
 

  SALOMEDSImpl_SObject   _FindObject(const SALOMEDSImpl_SObject& SO,
			      const std::string& anObjectName,
			      bool& _find);
				       
  SALOMEDSImpl_SObject   _FindObjectIOR(const SALOMEDSImpl_SObject& SO,
			         const std::string& anObjectIOR,
				 bool& _find);

public:

  Standard_EXPORT static SALOMEDSImpl_Study* GetStudy(const DF_Label& theLabel);
  Standard_EXPORT static SALOMEDSImpl_SObject SObject(const DF_Label& theLabel);
  Standard_EXPORT static SALOMEDSImpl_SComponent SComponent(const DF_Label& theLabel);
  Standard_EXPORT static void IORUpdated(const SALOMEDSImpl_AttributeIOR* theAttribute);

  //! standard constructor
  Standard_EXPORT SALOMEDSImpl_Study(const DF_Document*, const std::string& study_name);
  
  //! standard destructor
  Standard_EXPORT virtual ~SALOMEDSImpl_Study(); 
  
  //! method to Get persistent reference of study (idem URL())
  Standard_EXPORT virtual std::string GetPersistentReference();

  //! method to Get transient reference of study
  Standard_EXPORT virtual std::string GetTransientReference();

  Standard_EXPORT virtual void SetTransientReference(const std::string& theIOR);

  //! method to detect if a study is empty
  Standard_EXPORT virtual bool IsEmpty();

  //! method to Find a Component with ComponentDataType = aComponentName
  Standard_EXPORT virtual SALOMEDSImpl_SComponent FindComponent (const std::string& aComponentName);

  //! method to Find a Component Find a Component from it's ID
  Standard_EXPORT virtual SALOMEDSImpl_SComponent FindComponentID(const std::string& aComponentID);

  //! method to  Find an Object with SALOMEDSImpl::Name = anObjectName 
  Standard_EXPORT virtual SALOMEDSImpl_SObject FindObject(const std::string& anObjectName);


  //! method to Find Object(s) with SALOMEDSImpl::Name=anObjectName in a component with ComponentDataType = aComponentName
  Standard_EXPORT virtual std::vector<SALOMEDSImpl_SObject> FindObjectByName( const std::string& anObjectName, 
                                                                       const std::string& aComponentName ) ;
  
  //! method to Find an Object with ID = anObjectID 
  Standard_EXPORT virtual SALOMEDSImpl_SObject FindObjectID(const std::string& anObjectID);

  //! method to Create an Object with ID = anObjectID 
  Standard_EXPORT virtual SALOMEDSImpl_SObject CreateObjectID(const std::string& anObjectID);

  //! method to Find an Object with ID = anObjectIOR 
  Standard_EXPORT virtual SALOMEDSImpl_SObject FindObjectIOR(const std::string& anObjectIOR);

  //! method to Find an Object by its path
  Standard_EXPORT virtual SALOMEDSImpl_SObject FindObjectByPath(const std::string& thePath);

  //! method to get a path of SObject
  Standard_EXPORT virtual std::string GetObjectPath(const SALOMEDSImpl_SObject& theObject);

  Standard_EXPORT std::string GetObjectPathByIOR(const std::string& theIOR);

  //! method to set a context: root ('/') is UserData component
  Standard_EXPORT virtual bool SetContext(const std::string& thePath);

  //! method to get a context
  Standard_EXPORT virtual std::string GetContext();  

  //! method to get all object names in the given context (or in the current context, if 'theContext' is empty)
  Standard_EXPORT virtual std::vector<std::string> GetObjectNames(const std::string& theContext);

  //! method to get all directory names in the given context (or in the current context, if 'theContext' is empty)
  Standard_EXPORT virtual std::vector<std::string> GetDirectoryNames(const std::string& theContext);

  //! method to get all file names in the given context (or in the current context, if 'theContext' is empty)
  Standard_EXPORT virtual std::vector<std::string> GetFileNames(const std::string& theContext);

  //! method to get all components names
  Standard_EXPORT virtual std::vector<std::string> GetComponentNames(const std::string& theContext);

  //! method to Create a ChildIterator from an SObject 
  Standard_EXPORT virtual SALOMEDSImpl_ChildIterator NewChildIterator(const SALOMEDSImpl_SObject& aSO);

  //! method to Create a SComponentIterator 
  Standard_EXPORT virtual SALOMEDSImpl_SComponentIterator NewComponentIterator();

  //! method to Create a StudyBuilder
  Standard_EXPORT virtual SALOMEDSImpl_StudyBuilder* NewBuilder();
 
  //! method to get study name
  Standard_EXPORT virtual std::string Name();

  //! method to set study name
  Standard_EXPORT virtual void  Name(const std::string& name);

  //! method to get if study has been saved
  Standard_EXPORT virtual bool IsSaved();

  //! method to set if study has been saved
  Standard_EXPORT virtual void  IsSaved(bool save);

  //! method to Detect if a Study has been modified since it has been saved
  Standard_EXPORT virtual bool IsModified();

  //! method to get URL of the study (idem GetPersistentReference) 
  Standard_EXPORT virtual std::string URL();

  //! method to set URL of the study
  Standard_EXPORT virtual void  URL(const std::string& url);

  Standard_EXPORT virtual bool IsLocked();

  Standard_EXPORT virtual int StudyId();

  Standard_EXPORT virtual void  StudyId(int id);

  Standard_EXPORT virtual void UpdateIORLabelMap(const std::string& anIOR, const std::string& aLabel);
  
  Standard_EXPORT virtual std::vector<SALOMEDSImpl_SObject> FindDependances(const SALOMEDSImpl_SObject& anObject);

  Standard_EXPORT virtual SALOMEDSImpl_AttributeStudyProperties* GetProperties();

  Standard_EXPORT virtual std::string GetLastModificationDate();

  Standard_EXPORT virtual std::vector<std::string> GetModificationsDate();

  Standard_EXPORT virtual SALOMEDSImpl_UseCaseBuilder* GetUseCaseBuilder();

  Standard_EXPORT virtual void Close();

  Standard_EXPORT void EnableUseCaseAutoFilling(bool isEnabled);

  Standard_EXPORT virtual std::string GetErrorCode() { return _errorCode; }
  Standard_EXPORT virtual bool IsError() { return _errorCode != ""; }

  Standard_EXPORT virtual SALOMEDSImpl_SComponent GetSComponent(const std::string& theEntry);
  Standard_EXPORT virtual SALOMEDSImpl_SComponent GetSComponent(const DF_Label& theLabel);
  Standard_EXPORT virtual SALOMEDSImpl_SObject GetSObject(const std::string& theEntry);
  Standard_EXPORT virtual SALOMEDSImpl_SObject GetSObject(const DF_Label& theEntryLabel);
  Standard_EXPORT virtual DF_Attribute* GetAttribute(const std::string& theEntry, 
					             const std::string& theType);

  Standard_EXPORT virtual bool HasCurrentContext() { return !_current.IsNull(); }

  Standard_EXPORT virtual bool DumpStudy(const std::string& thePath, 
					 const std::string& theBaseName, 
					 bool isPublished,
					 SALOMEDSImpl_DriverFactory* theFactory);

  Standard_EXPORT static std::string GetDumpStudyComment(const char* theComponentName = 0);

  Standard_EXPORT virtual DF_Document* GetDocument() { return _doc; } 

  //The method dump creates a txt file that contain a dump of the study, for debug use
  Standard_EXPORT void dump(const std::string& theFileName);

  //This method marks the study as being modified
  Standard_EXPORT void Modify();

  Standard_EXPORT SALOMEDSImpl_AttributeParameter* GetCommonParameters(const char* theID, int theSavePoint);

  Standard_EXPORT SALOMEDSImpl_AttributeParameter* GetModuleParameters(const char* theID, 
									      const char* theModuleName,
									      int theSavePoint);

  //Locks the study, theLockerID is identificator of the of the one who locked the study for ex. IOR
  Standard_EXPORT void SetStudyLock(const char* theLockerID);

  //Returns True if the study is locked
  Standard_EXPORT bool IsStudyLocked();

  //Unlocks the study
  Standard_EXPORT void UnLockStudy(const char* theLockerID);
  
  //Returns an ID of the study locker
  Standard_EXPORT std::vector<std::string> GetLockerID();

  //Returns a callback 
  Standard_EXPORT SALOMEDSImpl_Callback* GetCallback() { return _cb; }

  //Returns a list of IOR's stored in the study
  Standard_EXPORT std::vector<std::string> GetIORs();


friend class SALOMEDSImpl_StudyManager;    
friend class SALOMEDSImpl_GenericAttribute;
};
#endif
