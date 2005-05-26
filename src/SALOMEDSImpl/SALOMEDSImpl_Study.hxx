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
#include <iostream.h>

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
#include <NCollection_DataMap.hxx>

//SALOMEDSImpl headers
#include "SALOMEDSImpl_SComponentIterator.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"
#include "SALOMEDSImpl_UseCaseBuilder.hxx"
#include "SALOMEDSImpl_AttributeStudyProperties.hxx"
#include "SALOMEDSImpl_AttributeIOR.hxx"
#include "SALOMEDSImpl_Callback.hxx"
#include "SALOMEDSImpl_Driver.hxx" 
#include "SALOMEDSImpl_ChildIterator.hxx" 

class SALOMEDSImpl_StudyManager;
class SALOMEDSImpl_GenericAttribute;

typedef NCollection_DataMap <TCollection_AsciiString, Handle_Standard_Transient> DataMapOfAsciiStringTransient;
typedef NCollection_DataMap <TCollection_AsciiString, TDF_Label> DataMapAsciiStringLabel;

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

  static Handle(SALOMEDSImpl_Study) GetStudy(const TDF_Label& theLabel);
  static Handle(SALOMEDSImpl_SObject) SObject(const TDF_Label& theLabel);
  static Handle(SALOMEDSImpl_SComponent) SComponent(const TDF_Label& theLabel);
  static void IORUpdated(const Handle(SALOMEDSImpl_AttributeIOR)& theAttribute);

  //! standard constructor
  SALOMEDSImpl_Study(const Handle(TDocStd_Document)&, const TCollection_AsciiString& study_name);
  
  //! standard destructor
  virtual ~SALOMEDSImpl_Study(); 
  
  //! method to Get persistent reference of study (idem URL())
  virtual TCollection_AsciiString GetPersistentReference();

  //! method to Get transient reference of study
  virtual TCollection_AsciiString GetTransientReference();

  virtual void SetTransientReference(const TCollection_AsciiString& theIOR);

  //! method to detect if a study is empty
  virtual bool IsEmpty();

  //! method to Find a Component with ComponentDataType = aComponentName
  virtual Handle(SALOMEDSImpl_SComponent) FindComponent (const TCollection_AsciiString& aComponentName);

  //! method to Find a Component Find a Component from it's ID
  virtual Handle(SALOMEDSImpl_SComponent) FindComponentID(const TCollection_AsciiString& aComponentID);

  //! method to  Find an Object with SALOMEDSImpl::Name = anObjectName 
  virtual Handle(SALOMEDSImpl_SObject) FindObject(const TCollection_AsciiString& anObjectName);


  //! method to Find Object(s) with SALOMEDSImpl::Name=anObjectName in a component with ComponentDataType = aComponentName
  virtual Handle(TColStd_HSequenceOfTransient) FindObjectByName( const TCollection_AsciiString& anObjectName, 
                                                                 const TCollection_AsciiString& aComponentName ) ;
  
  //! method to Find an Object with ID = anObjectID 
  virtual Handle(SALOMEDSImpl_SObject) FindObjectID(const TCollection_AsciiString& anObjectID);

  //! method to Create an Object with ID = anObjectID 
  virtual Handle(SALOMEDSImpl_SObject) CreateObjectID(const TCollection_AsciiString& anObjectID);

  //! method to Find an Object with ID = anObjectIOR 
  virtual Handle(SALOMEDSImpl_SObject) FindObjectIOR(const TCollection_AsciiString& anObjectIOR);

  //! method to Find an Object by its path
  virtual Handle(SALOMEDSImpl_SObject) FindObjectByPath(const TCollection_AsciiString& thePath);

  //! method to get a path of SObject
  virtual TCollection_AsciiString GetObjectPath(const Handle(SALOMEDSImpl_SObject)& theObject);

  TCollection_AsciiString GetObjectPathByIOR(const TCollection_AsciiString& theIOR);

  //! method to set a context: root ('/') is UserData component
  virtual bool SetContext(const TCollection_AsciiString& thePath);

  //! method to get a context
  virtual TCollection_AsciiString GetContext();  

  //! method to get all object names in the given context (or in the current context, if 'theContext' is empty)
  virtual Handle(TColStd_HSequenceOfAsciiString) GetObjectNames(const TCollection_AsciiString& theContext);

  //! method to get all directory names in the given context (or in the current context, if 'theContext' is empty)
  virtual Handle(TColStd_HSequenceOfAsciiString) GetDirectoryNames(const TCollection_AsciiString& theContext);

  //! method to get all file names in the given context (or in the current context, if 'theContext' is empty)
  virtual Handle(TColStd_HSequenceOfAsciiString) GetFileNames(const TCollection_AsciiString& theContext);

  //! method to get all components names
  virtual Handle(TColStd_HSequenceOfAsciiString) GetComponentNames(const TCollection_AsciiString& theContext);

  //! method to Create a ChildIterator from an SObject 
  virtual Handle(SALOMEDSImpl_ChildIterator) NewChildIterator(const Handle(SALOMEDSImpl_SObject)& aSO);

  //! method to Create a SComponentIterator 
  virtual SALOMEDSImpl_SComponentIterator NewComponentIterator();

  //! method to Create a StudyBuilder
  virtual Handle(SALOMEDSImpl_StudyBuilder) NewBuilder();
 
  //! method to get study name
  virtual TCollection_AsciiString Name();

  //! method to set study name
  virtual void  Name(const TCollection_AsciiString& name);

  //! method to get if study has been saved
  virtual bool IsSaved();

  //! method to set if study has been saved
  virtual void  IsSaved(bool save);

  //! method to Detect if a Study has been modified since it has been saved
  virtual bool IsModified();

  //! method to get URL of the study (idem GetPersistentReference) 
  virtual TCollection_AsciiString URL();

  //! method to set URL of the study
  virtual void  URL(const TCollection_AsciiString& url);

  virtual bool IsLocked();

  virtual int StudyId();

  virtual void  StudyId(int id);

  virtual void UpdateIORLabelMap(const TCollection_AsciiString& anIOR, const TCollection_AsciiString& aLabel);
  
  virtual Handle(TColStd_HSequenceOfTransient) FindDependances(const Handle(SALOMEDSImpl_SObject)& anObject);

  virtual Handle(SALOMEDSImpl_AttributeStudyProperties) SALOMEDSImpl_Study::GetProperties();

  virtual TCollection_AsciiString GetLastModificationDate();

  virtual Handle(TColStd_HSequenceOfAsciiString) GetModificationsDate();

  virtual Handle(SALOMEDSImpl_UseCaseBuilder) GetUseCaseBuilder();

  virtual void Close();

  void EnableUseCaseAutoFilling(bool isEnabled) { _errorCode = ""; _autoFill = isEnabled; }

  // postponed destroying of object functionality
  virtual void AddPostponed(const TCollection_AsciiString& theIOR);

  virtual void AddCreatedPostponed(const TCollection_AsciiString& theIOR);

  virtual Handle(TColStd_HSequenceOfAsciiString) RemovePostponed(const int theUndoLimit); 
                                              // removes postponed IORs of old transaction
                                              // if theUndoLimit==0, removes all
  virtual void UndoPostponed(const int theWay); // theWay = 1: resurrect objects,
                                                // theWay = -1: get back to the list of postponed


  virtual TCollection_AsciiString GetErrorCode() { return _errorCode; }
  virtual bool IsError() { return _errorCode != ""; }

  virtual Handle(SALOMEDSImpl_SComponent) GetSComponent(const TCollection_AsciiString& theEntry);
  virtual Handle(SALOMEDSImpl_SComponent) GetSComponent(const TDF_Label& theLabel);
  virtual Handle(SALOMEDSImpl_SObject) GetSObject(const TCollection_AsciiString& theEntry);
  virtual Handle(SALOMEDSImpl_SObject) GetSObject(const TDF_Label& theEntryLabel);
  virtual Handle(TDF_Attribute) GetAttribute(const TCollection_AsciiString& theEntry, 
					     const TCollection_AsciiString& theType);

  virtual bool HasCurrentContext() { return !_current.IsNull(); }

  virtual bool DumpStudy(const TCollection_AsciiString& thePath, 
		         const TCollection_AsciiString& theBaseName, 
		         bool isPublished,
			 SALOMEDSImpl_DriverFactory* theFactory);

  static TCollection_AsciiString GetDumpStudyComment(const char* theComponentName = 0);

  virtual Handle(TDocStd_Document) GetDocument() { return _doc; } 

  //The method dump creates a txt file that contain a dump of the study, for debug use
  void dump(const TCollection_AsciiString& theFileName);

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_Study )

friend class SALOMEDSImpl_StudyManager;    
friend class SALOMEDSImpl_GenericAttribute;
};
#endif
