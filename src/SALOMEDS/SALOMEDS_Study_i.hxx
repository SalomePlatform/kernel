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

//  File   : SALOMEDS_Study_i.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDS_STUDY_I_H__
#define __SALOMEDS_STUDY_I_H__

// std C++ headers
#include <iostream>

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_GenericObj)
#include CORBA_SERVER_HEADER(SALOMEDS)

#include <stdio.h>

//SALOMEDS headers
#include "SALOMEDS_SComponentIterator_i.hxx"
#include "SALOMEDS_StudyBuilder_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_UseCaseBuilder_i.hxx"

#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_AttributeIOR.hxx"

class Standard_EXPORT SALOMEDS_Study_i: public POA_SALOMEDS::Study
{
private:
  CORBA::ORB_var                 _orb;
  SALOMEDSImpl_Study*            _impl;  
  SALOMEDS_StudyBuilder_i*       _builder;    
  static std::map<SALOMEDSImpl_Study*, SALOMEDS_Study_i*> _mapOfStudies;
  SALOMEDSImpl_AbstractCallback* _notifier;
  SALOMEDSImpl_AbstractCallback* _genObjRegister;
  bool                           _closed;

public:

  //! standard constructor
  SALOMEDS_Study_i(SALOMEDSImpl_Study*, CORBA::ORB_ptr);
  
  //! standard destructor
  virtual ~SALOMEDS_Study_i(); 
  
  //! method to Get persistent reference of study (idem URL())
  /*!
    \sa URL()
    \return char* arguments, the persistent reference of the study
  */  
  virtual char* GetPersistentReference();


  //! method to Get transient reference of study
  /*!
    \return char* arguments, the transient reference of the study
  */  
  virtual char* GetTransientReference();

  //! method to detect if a study is empty
  /*!
    \return bool arguments, true if study is empty
  */  
  virtual CORBA::Boolean IsEmpty();

  //! method to Find a Component with ComponentDataType = aComponentName
  /*!
    \param aComponentName char* arguments
    \return SComponent_ptr arguments, the component found
  */  
  virtual SALOMEDS::SComponent_ptr FindComponent (const char* aComponentName);

  //! method to Find a Component Find a Component from it's ID
  /*!
    \param aComponentID char* arguments
    \return SComponent_ptr arguments, the component found
  */  
  virtual SALOMEDS::SComponent_ptr FindComponentID(const char* aComponentID);

  //! method to  Find an Object with SALOMEDS::Name = anObjectName 
  /*!
    \param anObjectName char* arguments
    \return SObject_ptr arguments, the object found
  */  
  virtual SALOMEDS::SObject_ptr FindObject(const char* anObjectName);


  //! method to Find Object(s) with SALOMEDS::Name = anObjectName in a component with ComponentDataType = aComponentName
  /*!
    \param anObjectName char* arguments
    \param aComponentName char* arguments
    \return ListOfSObject_ptr arguments, a list of objects found
  */  
  
  virtual SALOMEDS::Study::ListOfSObject* FindObjectByName( const char* anObjectName, const char* aComponentName ) ;
  
  //! method to Find an Object with ID = anObjectID 
  /*!
    \param anObjectID char* arguments
    \return SObject_ptr arguments, the object found
  */  
  virtual SALOMEDS::SObject_ptr FindObjectID(const char* anObjectID);

  //! method to Create an Object with ID = anObjectID 
  /*!
    \param anObjectID char* arguments
    \return SObject_ptr arguments, the object found
  */  
  virtual SALOMEDS::SObject_ptr CreateObjectID(const char* anObjectID);

  //! method to Find an Object with ID = anObjectIOR 
  /*!
    \param anObjectIOR char* arguments
    \return SObject_ptr arguments, the object found
  */  
  virtual SALOMEDS::SObject_ptr FindObjectIOR(const char* anObjectIOR);

  //! method to Find an Object by its path
  /*!
    \param thePath char* arguments
    \return SObject_ptr arguments, the object found
  */
  virtual SALOMEDS::SObject_ptr FindObjectByPath(const char* thePath);

  //! method to get a path of SObject
  /*!
    \param aSO  SObject_ptr arguments
    \return char* arguments, the path of the SObject
  */
  virtual char* GetObjectPath(CORBA::Object_ptr theObject);

  //! method to set a context: root ('/') is UserData component
  /*!
  */
  virtual void SetContext(const char* thePath);

  //! method to get a context
  /*!
  */
  virtual char* GetContext();  

  //! method to get all object names in the given context (or in the current context, if 'theContext' is empty)
  /*!
  */
  virtual SALOMEDS::ListOfStrings* GetObjectNames(const char* theContext);

  //! method to get all directory names in the given context (or in the current context, if 'theContext' is empty)
  /*!
  */
  virtual SALOMEDS::ListOfStrings* GetDirectoryNames(const char* theContext);

  //! method to get all file names in the given context (or in the current context, if 'theContext' is empty)
  /*!
  */
  virtual SALOMEDS::ListOfStrings* GetFileNames(const char* theContext);

  //! method to get all components names
  /*!
  */
  virtual SALOMEDS::ListOfStrings* GetComponentNames(const char* theContext);

  //! method to Create a ChildIterator from an SObject 
  /*!
    \param aSO  SObject_ptr arguments
    \return ChildIterator_ptr arguments, the created ChildIterator
  */  
  virtual SALOMEDS::ChildIterator_ptr NewChildIterator(SALOMEDS::SObject_ptr aSO);

  //! method to Create a SComponentIterator 
  /*!
    \return SComponentIterator_ptr arguments, the created SComponentIterator
  */  
  virtual SALOMEDS::SComponentIterator_ptr NewComponentIterator();

  //! method to Create a StudyBuilder
  /*!
    \return StudyBuilder_ptr arguments, the created StudyBuilder
  */  
  virtual SALOMEDS::StudyBuilder_ptr NewBuilder();
 
  //! method to get study name
  /*!
    \return char* arguments, the study name
  */
  virtual char* Name();

  //! method to set study name
  /*!
    \param name char* arguments, the study name
  */
  virtual void  Name(const char* name);

  //! method to get if study has been saved
  /*!
    \return bool arguments
  */
  virtual CORBA::Boolean IsSaved();

 //! method to set if study has been saved
  /*!
    \param save bool arguments
  */
  virtual void  IsSaved(CORBA::Boolean save);

 //! method to Detect if a Study has been modified since it has been saved
  /*!
    \return bool arguments
  */
  virtual CORBA::Boolean IsModified();

 //! method to set Modified flag of a Study to True
  virtual void Modified();

  //! method to get URL of the study (idem GetPersistentReference) 
  /*!
    \return char* arguments, the study URL 
  */
  virtual char* URL();

 //! method to set URL of the study
  /*!
    \param url char* arguments, the study URL
  */
  virtual void  URL(const char* url);

  virtual CORBA::Short StudyId();
  virtual void  StudyId(CORBA::Short id);

  static SALOMEDS::Study_ptr GetStudy(const DF_Label& theLabel, CORBA::ORB_ptr orb);
  static SALOMEDS_Study_i* GetStudyServant(SALOMEDSImpl_Study*, CORBA::ORB_ptr orb);

  static void IORUpdated(SALOMEDSImpl_AttributeIOR* theAttribute);

  virtual void UpdateIORLabelMap(const char* anIOR, const char* aLabel);
  
  virtual SALOMEDS::Study::ListOfSObject* FindDependances(SALOMEDS::SObject_ptr anObject);

  virtual SALOMEDS::AttributeStudyProperties_ptr GetProperties();

  virtual char* GetLastModificationDate();

  virtual SALOMEDS::ListOfDates* GetModificationsDate();

  virtual char* ConvertObjectToIOR(CORBA::Object_ptr theObject) {return _orb->object_to_string(theObject); }
  virtual CORBA::Object_ptr ConvertIORToObject(const char* theIOR) { return _orb->string_to_object(theIOR); };

  virtual SALOMEDS::UseCaseBuilder_ptr GetUseCaseBuilder();

  virtual void Close();

  void EnableUseCaseAutoFilling(CORBA::Boolean isEnabled); 

  // postponed destroying of CORBA object functionality
  virtual void AddPostponed(const char* theIOR);

  virtual void AddCreatedPostponed(const char* theIOR);

  virtual void RemovePostponed(CORBA::Long theUndoLimit); // removes postponed IORs of old transaction
                                                        // if theUndoLimit==0, removes all
  virtual void UndoPostponed(CORBA::Long theWay); // theWay = 1: resurrect objects,
                                                // theWay = -1: get back to the list of postponed

  virtual SALOMEDS::AttributeParameter_ptr GetCommonParameters(const char* theID, CORBA::Long theSavePoint);
  virtual SALOMEDS::AttributeParameter_ptr GetModuleParameters(const char* theID, 
                                                               const char* theModuleName, 
                                                               CORBA::Long theSavePoint);

  virtual void SetStudyLock(const char* theLockerID);

  virtual bool IsStudyLocked();

  virtual void UnLockStudy(const char* theLockerID);

  virtual SALOMEDS::ListOfStrings* GetLockerID();

  virtual void SetReal(const char* theVarName, CORBA::Double theValue);
  
  virtual void SetInteger(const char* theVarName, CORBA::Long theValue);

  virtual void SetBoolean(const char* theVarName, CORBA::Boolean theValue);

  virtual void SetString(const char* theVarName, const char* theValue);

  virtual void SetStringAsDouble(const char* theVarName, CORBA::Double theValue);

  virtual CORBA::Double GetReal(const char* theVarName);
  
  virtual CORBA::Long GetInteger(const char* theVarName);

  virtual CORBA::Boolean GetBoolean(const char* theVarName);

  virtual char* GetString(const char* theVarName);

  virtual CORBA::Boolean IsReal(const char* theVarName);
  
  virtual CORBA::Boolean IsInteger(const char* theVarName);

  virtual CORBA::Boolean IsBoolean(const char* theVarName);

  virtual CORBA::Boolean IsString(const char* theVarName);

  virtual CORBA::Boolean IsVariable(const char* theVarName);

  virtual SALOMEDS::ListOfStrings* GetVariableNames();

  virtual CORBA::Boolean RemoveVariable(const char* theVarName);

  virtual CORBA::Boolean RenameVariable(const char* theVarName, const char* theNewVarName);

  virtual CORBA::Boolean IsVariableUsed(const char* theVarName);
  
  virtual SALOMEDS::ListOfListOfStrings* ParseVariables(const char* theVars);

  virtual char* GetDefaultScript(const char* theModuleName, const char* theShift);

  virtual CORBA::Boolean DumpStudy(const char* thePath,
                                   const char* theBaseName,
                                   CORBA::Boolean isPublished,
                                   CORBA::Boolean isMultiFile);

  virtual SALOMEDSImpl_Study* GetImpl() { return _impl; }

  virtual CORBA::LongLong GetLocalImpl(const char* theHostname, CORBA::Long thePID, CORBA::Boolean& isLocal);

  virtual void attach(SALOMEDS::Observer_ptr theObs, CORBA::Boolean modify);
  virtual void detach(SALOMEDS::Observer_ptr theObs);
};
#endif
