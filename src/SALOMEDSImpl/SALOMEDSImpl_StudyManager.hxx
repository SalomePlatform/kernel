//  File   : SALOMEDSImpl_StudyManager.hxx
//  Author : Sergey RUIN
//  Module : SALOME

#ifndef __SALOMEDSImpl_STUDYMANAGER_I_H__
#define __SALOMEDSImpl_STUDYMANAGER_I_H__

//Handle definition
#include <Handle_MMgt_TShared.hxx>
#include <Standard_DefineHandle.hxx>
DEFINE_STANDARD_HANDLE( SALOMEDSImpl_StudyManager, MMgt_TShared )

// std C++ headers
#include <strstream>

// Cascade headers
#include "SALOMEDSImpl_OCAFApplication.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_Driver.hxx"
#include <TCollection_AsciiString.hxx>
#include <TDF_Attribute.hxx>
#include <TDF_Label.hxx>
#include <TDocStd_Document.hxx>
#include <TColStd_HSequenceOfTransient.hxx>

class HDFgroup;

class SALOMEDSImpl_StudyManager : public MMgt_TShared 
{

private:

  Handle (SALOMEDSImpl_OCAFApplication) _OCAFApp;  
  int _IDcounter;
  Handle(TDocStd_Document) _clipboard;
  TCollection_AsciiString  _errorCode;

public:

  //! standard constructor
  SALOMEDSImpl_StudyManager();

  //! standard destructor
  virtual  ~SALOMEDSImpl_StudyManager(); 

  //! method to Create a New Study of name study_name
  virtual Handle(SALOMEDSImpl_Study) NewStudy(const TCollection_AsciiString& study_name);

  //! method to Open a Study from it's persistent reference
  virtual Handle(SALOMEDSImpl_Study) Open(const TCollection_AsciiString& aStudyUrl);

  //! method to close a Study 
  virtual void Close(const Handle(SALOMEDSImpl_Study)& aStudy);

  //! method to save a Study 
  virtual bool Save(const Handle(SALOMEDSImpl_Study)& aStudy, SALOMEDSImpl_DriverFactory* aFactory, bool theMultiFile);

  virtual bool SaveASCII(const Handle(SALOMEDSImpl_Study)& aStudy, 
			 SALOMEDSImpl_DriverFactory* aFactory, 
			 bool theMultiFile);

  //! method to save a Study to the persistent reference aUrl
  virtual bool SaveAs(const TCollection_AsciiString& aUrl,  
		      const Handle(SALOMEDSImpl_Study)& aStudy, 
		      SALOMEDSImpl_DriverFactory* aFactory,
		      bool theMultiFile);

  virtual bool SaveAsASCII(const TCollection_AsciiString& aUrl, 
			   const Handle(SALOMEDSImpl_Study)& aStudy, 
			   SALOMEDSImpl_DriverFactory* aFactory,
			   bool theMultiFile);

  //! method to Get name list of open studies in the session
  virtual Handle(TColStd_HSequenceOfTransient) GetOpenStudies();

  //! method to get a Study from it's name
  virtual Handle(SALOMEDSImpl_Study) GetStudyByName(const TCollection_AsciiString& aStudyName) ;

  //! method to get a Study from it's ID
  virtual Handle(SALOMEDSImpl_Study) GetStudyByID(int aStudyID) ;


  Handle(TDocStd_Document) GetDocumentOfStudy(const Handle(SALOMEDSImpl_Study)& theStudy);

  Handle(TDocStd_Document) GetClipboard() { return _clipboard; }
  
  bool CopyLabel(const Handle(SALOMEDSImpl_Study)& theSourceStudy, 
		 SALOMEDSImpl_Driver* theEngine,
		 const int theSourceStartDepth,
		 const TDF_Label& theSource,
		 const TDF_Label& theDestinationMain);

  TDF_Label PasteLabel(const Handle(SALOMEDSImpl_Study)& theDestinationStudy,
		       SALOMEDSImpl_Driver* theEngine,
		       const TDF_Label& theSource,
		       const TDF_Label& theDestinationStart,
		       const int theCopiedStudyID,
		       const bool isFirstElement);
  
  virtual bool CanCopy(const Handle(SALOMEDSImpl_SObject)& theObject, SALOMEDSImpl_Driver* Engine);
  virtual bool Copy(const Handle(SALOMEDSImpl_SObject)& theObject, SALOMEDSImpl_Driver* Engine);
  virtual bool CanPaste(const Handle(SALOMEDSImpl_SObject)& theObject, SALOMEDSImpl_Driver* Engine);
  virtual Handle(SALOMEDSImpl_SObject) Paste(const Handle(SALOMEDSImpl_SObject)& theObject, SALOMEDSImpl_Driver* Engine);

  // _SaveAs private function called by Save and SaveAs
  virtual bool Impl_SaveAs(const TCollection_AsciiString& aUrl,
			   const Handle(SALOMEDSImpl_Study)& aStudy,
			   SALOMEDSImpl_DriverFactory* aFactory,
			   bool theMultiFile,
			   bool theASCII);

  // _SaveObject private function called by _SaveAs
  virtual bool Impl_SaveObject(const Handle(SALOMEDSImpl_SObject)& SC, HDFgroup *hdf_group_datatype);

  // _SubstituteSlash function called by Open and GetStudyByName
  virtual TCollection_AsciiString Impl_SubstituteSlash(const TCollection_AsciiString& aUrl);

  virtual bool Impl_SaveProperties(const Handle(SALOMEDSImpl_Study)& aStudy, HDFgroup *hdf_group);

  TCollection_AsciiString GetErrorCode() { return _errorCode; }
  virtual bool IsError() { return _errorCode != ""; }

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_StudyManager )
  
};

#endif 
