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

//Handle definition
#include <Handle_MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_Study, MMgt_TShared )

// std C++ headers
#include <iostream>

// Cascade headers
#include <TDocStd_Document.hxx>
#include <TDF_Tool.hxx>
#include <TDF_Data.hxx>
#include <TDF_Label.hxx>
#include <stdio.h>
#include <TCollection_AsciiString.hxx>
#include <TColStd_SequenceOfInteger.hxx>
#include <TColStd_SequenceOfAsciiString.hxx>
#include <TColStd_HSequenceOfAsciiString.hxx>
#include <TColStd_HSequenceOfTransient.hxx>
#ifndef WNT
#include <NCollection_DataMap.hxx>
#else
#include <NCollection_DataMap1.hxx>
#endif

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

#ifndef WNT
typedef NCollection_DataMap <TCollection_AsciiString, Handle_Standard_Transient> DataMapOfAsciiStringTransient;
typedef NCollection_DataMap <TCollection_AsciiString, TDF_Label> DataMapAsciiStringLabel;
#else
typedef NCollection_DataMap1 <TCollection_AsciiString, Handle_Standard_Transient> DataMapOfAsciiStringTransient;
typedef NCollection_DataMap1 <TCollection_AsciiString, TDF_Label> DataMapAsciiStringLabel;
#endif

class SALOMEDSImpl_Study : public MMgt_TShared 
{
private:
  TCollection_AsciiString              _name;  
  Handle(TDocStd_Document) _doc;  // OCAF Document
  bool                     _Saved; // True if the Study is saved
  TCollection_AsciiString  _URL; //URL of the persistent reference of the study
  int                      _StudyId; 
  TDF_Label                _current;
  bool                     _autoFill; 
  TCollection_AsciiString  _errorCode;
  Handle(SALOMEDSImpl_Callback)       _cb;
  Handle(SALOMEDSImpl_StudyBuilder)   _builder;
  Handle(SALOMEDSImpl_UseCaseBuilder) _useCaseBuilder;

  DataMapOfAsciiStringTransient _mapOfSO;
  DataMapOfAsciiStringTransient _mapOfSCO;

  // data structures for postponed destroying of object functionality
  TColStd_SequenceOfAsciiString myPostponedIORs; // ordered set of IORs
  TColStd_SequenceOfInteger myNbPostponed; // number of IOR in the each transaction
  int myNbUndos; // number of current Undos, made by user
  DataMapAsciiStringLabel myIORLabels;
 

  Handle(SALOMEDSImpl_SObject)    _FindObject(const Handle(SALOMEDSImpl_SObject)& SO,
				              const TCollection_AsciiString& anObjectName,
				              bool& _find);
				       
  Handle(SALOMEDSImpl_SObject)   _FindObjectIOR(const Handle(SALOMEDSImpl_SObject)& SO,
					        const TCollection_AsciiString& anObjectIOR,
					        bool& _find);

public:

  Standard_EXPORT static Handle(SALOMEDSImpl_Study) GetStudy(const TDF_Label& theLabel);
  Standard_EXPORT static Handle(SALOMEDSImpl_SObject) SObject(const TDF_Label& theLabel);
  Standard_EXPORT static Handle(SALOMEDSImpl_SComponent) SComponent(const TDF_Label& theLabel);
  Standard_EXPORT static void IORUpdated(const Handle(SALOMEDSImpl_AttributeIOR)& theAttribute);

  //! standard constructor
  Standard_EXPORT SALOMEDSImpl_Study(const Handle(TDocStd_Document)&, const TCollection_AsciiString& study_name);
  
  //! standard destructor
  Standard_EXPORT virtual ~SALOMEDSImpl_Study(); 
  
  //! method to Get persistent reference of study (idem URL())
  Standard_EXPORT virtual TCollection_AsciiString GetPersistentReference();

  //! method to Get transient reference of study
  Standard_EXPORT virtual TCollection_AsciiString GetTransientReference();

  Standard_EXPORT virtual void SetTransientReference(const TCollection_AsciiString& theIOR);

  //! method to detect if a study is empty
  Standard_EXPORT virtual bool IsEmpty();

  //! method to Find a Component with ComponentDataType = aComponentName
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SComponent) FindComponent (const TCollection_AsciiString& aComponentName);

  //! method to Find a Component Find a Component from it's ID
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SComponent) FindComponentID(const TCollection_AsciiString& aComponentID);

  //! method to  Find an Object with SALOMEDSImpl::Name = anObjectName 
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) FindObject(const TCollection_AsciiString& anObjectName);


  //! method to Find Object(s) with SALOMEDSImpl::Name=anObjectName in a component with ComponentDataType = aComponentName
  Standard_EXPORT virtual Handle(TColStd_HSequenceOfTransient) FindObjectByName( const TCollection_AsciiString& anObjectName, 
                                                                 const TCollection_AsciiString& aComponentName ) ;
  
  //! method to Find an Object with ID = anObjectID 
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) FindObjectID(const TCollection_AsciiString& anObjectID);

  //! method to Create an Object with ID = anObjectID 
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) CreateObjectID(const TCollection_AsciiString& anObjectID);

  //! method to Find an Object with ID = anObjectIOR 
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) FindObjectIOR(const TCollection_AsciiString& anObjectIOR);

  //! method to Find an Object by its path
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) FindObjectByPath(const TCollection_AsciiString& thePath);

  //! method to get a path of SObject
  Standard_EXPORT virtual TCollection_AsciiString GetObjectPath(const Handle(SALOMEDSImpl_SObject)& theObject);

  Standard_EXPORT TCollection_AsciiString GetObjectPathByIOR(const TCollection_AsciiString& theIOR);

  //! method to set a context: root ('/') is UserData component
  Standard_EXPORT virtual bool SetContext(const TCollection_AsciiString& thePath);

  //! method to get a context
  Standard_EXPORT virtual TCollection_AsciiString GetContext();  

  //! method to get all object names in the given context (or in the current context, if 'theContext' is empty)
  Standard_EXPORT virtual Handle(TColStd_HSequenceOfAsciiString) GetObjectNames(const TCollection_AsciiString& theContext);

  //! method to get all directory names in the given context (or in the current context, if 'theContext' is empty)
  Standard_EXPORT virtual Handle(TColStd_HSequenceOfAsciiString) GetDirectoryNames(const TCollection_AsciiString& theContext);

  //! method to get all file names in the given context (or in the current context, if 'theContext' is empty)
  Standard_EXPORT virtual Handle(TColStd_HSequenceOfAsciiString) GetFileNames(const TCollection_AsciiString& theContext);

  //! method to get all components names
  Standard_EXPORT virtual Handle(TColStd_HSequenceOfAsciiString) GetComponentNames(const TCollection_AsciiString& theContext);

  //! method to Create a ChildIterator from an SObject 
  Standard_EXPORT virtual Handle(SALOMEDSImpl_ChildIterator) NewChildIterator(const Handle(SALOMEDSImpl_SObject)& aSO);

  //! method to Create a SComponentIterator 
  Standard_EXPORT virtual SALOMEDSImpl_SComponentIterator NewComponentIterator();

  //! method to Create a StudyBuilder
  Standard_EXPORT virtual Handle(SALOMEDSImpl_StudyBuilder) NewBuilder();
 
  //! method to get study name
  Standard_EXPORT virtual TCollection_AsciiString Name();

  //! method to set study name
  Standard_EXPORT virtual void  Name(const TCollection_AsciiString& name);

  //! method to get if study has been saved
  Standard_EXPORT virtual bool IsSaved();

  //! method to set if study has been saved
  Standard_EXPORT virtual void  IsSaved(bool save);

  //! method to Detect if a Study has been modified since it has been saved
  Standard_EXPORT virtual bool IsModified();

  //! method to get URL of the study (idem GetPersistentReference) 
  Standard_EXPORT virtual TCollection_AsciiString URL();

  //! method to set URL of the study
  Standard_EXPORT virtual void  URL(const TCollection_AsciiString& url);

  Standard_EXPORT virtual bool IsLocked();

  Standard_EXPORT virtual int StudyId();

  Standard_EXPORT virtual void  StudyId(int id);

  Standard_EXPORT virtual void UpdateIORLabelMap(const TCollection_AsciiString& anIOR, const TCollection_AsciiString& aLabel);
  
  Standard_EXPORT virtual Handle(TColStd_HSequenceOfTransient) FindDependances(const Handle(SALOMEDSImpl_SObject)& anObject);

  Standard_EXPORT virtual Handle(SALOMEDSImpl_AttributeStudyProperties) SALOMEDSImpl_Study::GetProperties();

  Standard_EXPORT virtual TCollection_AsciiString GetLastModificationDate();

  Standard_EXPORT virtual Handle(TColStd_HSequenceOfAsciiString) GetModificationsDate();

  Standard_EXPORT virtual Handle(SALOMEDSImpl_UseCaseBuilder) GetUseCaseBuilder();

  Standard_EXPORT virtual void Close();

  Standard_EXPORT void EnableUseCaseAutoFilling(bool isEnabled) { _errorCode = ""; _autoFill = isEnabled; }

  // postponed destroying of object functionality
  Standard_EXPORT virtual void AddPostponed(const TCollection_AsciiString& theIOR);

  Standard_EXPORT virtual void AddCreatedPostponed(const TCollection_AsciiString& theIOR);

  Standard_EXPORT virtual Handle(TColStd_HSequenceOfAsciiString) RemovePostponed(const int theUndoLimit); 
                                              // removes postponed IORs of old transaction
                                              // if theUndoLimit==0, removes all
  Standard_EXPORT virtual void UndoPostponed(const int theWay); // theWay = 1: resurrect objects,
                                                // theWay = -1: get back to the list of postponed


  Standard_EXPORT virtual TCollection_AsciiString GetErrorCode() { return _errorCode; }
  Standard_EXPORT virtual bool IsError() { return _errorCode != ""; }

  Standard_EXPORT virtual Handle(SALOMEDSImpl_SComponent) GetSComponent(const TCollection_AsciiString& theEntry);
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SComponent) GetSComponent(const TDF_Label& theLabel);
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) GetSObject(const TCollection_AsciiString& theEntry);
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) GetSObject(const TDF_Label& theEntryLabel);
  Standard_EXPORT virtual Handle(TDF_Attribute) GetAttribute(const TCollection_AsciiString& theEntry, 
					     const TCollection_AsciiString& theType);

  Standard_EXPORT virtual bool HasCurrentContext() { return !_current.IsNull(); }

  Standard_EXPORT virtual bool DumpStudy(const TCollection_AsciiString& thePath, 
					 const TCollection_AsciiString& theBaseName, 
					 bool isPublished,
					 SALOMEDSImpl_DriverFactory* theFactory);

  Standard_EXPORT static TCollection_AsciiString GetDumpStudyComment(const char* theComponentName = 0);

  Standard_EXPORT virtual Handle(TDocStd_Document) GetDocument() { return _doc; } 

  //The method dump creates a txt file that contain a dump of the study, for debug use
  Standard_EXPORT void dump(const TCollection_AsciiString& theFileName);

  //This method marks the study as being modified
  Standard_EXPORT void Modify();

  Standard_EXPORT Handle(SALOMEDSImpl_AttributeParameter) GetCommonParameters(const char* theID, int theSavePoint);

  Standard_EXPORT Handle(SALOMEDSImpl_AttributeParameter) GetModuleParameters(const char* theID, 
									      const char* theModuleName,
									      int theSavePoint);

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_Study )

friend class SALOMEDSImpl_StudyManager;    
friend class SALOMEDSImpl_GenericAttribute;
};
#endif
