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
// See http://www.salome-platform.org/
//
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
  Standard_EXPORT SALOMEDSImpl_StudyManager();

  //! standard destructor
  Standard_EXPORT virtual  ~SALOMEDSImpl_StudyManager(); 

  //! method to Create a New Study of name study_name
  Standard_EXPORT virtual Handle(SALOMEDSImpl_Study) NewStudy(const TCollection_AsciiString& study_name);

  //! method to Open a Study from it's persistent reference
  Standard_EXPORT virtual Handle(SALOMEDSImpl_Study) Open(const TCollection_AsciiString& aStudyUrl);

  //! method to close a Study 
  Standard_EXPORT virtual void Close(const Handle(SALOMEDSImpl_Study)& aStudy);

  //! method to save a Study 
  Standard_EXPORT virtual bool Save(const Handle(SALOMEDSImpl_Study)& aStudy, SALOMEDSImpl_DriverFactory* aFactory, bool theMultiFile);

  Standard_EXPORT virtual bool SaveASCII(const Handle(SALOMEDSImpl_Study)& aStudy, 
			 SALOMEDSImpl_DriverFactory* aFactory, 
			 bool theMultiFile);

  //! method to save a Study to the persistent reference aUrl
  Standard_EXPORT virtual bool SaveAs(const TCollection_AsciiString& aUrl,  
		      const Handle(SALOMEDSImpl_Study)& aStudy, 
		      SALOMEDSImpl_DriverFactory* aFactory,
		      bool theMultiFile);

  Standard_EXPORT virtual bool SaveAsASCII(const TCollection_AsciiString& aUrl, 
			   const Handle(SALOMEDSImpl_Study)& aStudy, 
			   SALOMEDSImpl_DriverFactory* aFactory,
			   bool theMultiFile);

  //! method to Get name list of open studies in the session
  Standard_EXPORT virtual Handle(TColStd_HSequenceOfTransient) GetOpenStudies();

  //! method to get a Study from it's name
  Standard_EXPORT virtual Handle(SALOMEDSImpl_Study) GetStudyByName(const TCollection_AsciiString& aStudyName) ;

  //! method to get a Study from it's ID
  Standard_EXPORT virtual Handle(SALOMEDSImpl_Study) GetStudyByID(int aStudyID) ;


  Standard_EXPORT Handle(TDocStd_Document) GetDocumentOfStudy(const Handle(SALOMEDSImpl_Study)& theStudy);

  Standard_EXPORT Handle(TDocStd_Document) GetClipboard() { return _clipboard; }
  
  Standard_EXPORT bool CopyLabel(const Handle(SALOMEDSImpl_Study)& theSourceStudy, 
		 SALOMEDSImpl_Driver* theEngine,
		 const int theSourceStartDepth,
		 const TDF_Label& theSource,
		 const TDF_Label& theDestinationMain);

  Standard_EXPORT TDF_Label PasteLabel(const Handle(SALOMEDSImpl_Study)& theDestinationStudy,
		       SALOMEDSImpl_Driver* theEngine,
		       const TDF_Label& theSource,
		       const TDF_Label& theDestinationStart,
		       const int theCopiedStudyID,
		       const bool isFirstElement);
  
  Standard_EXPORT virtual bool CanCopy(const Handle(SALOMEDSImpl_SObject)& theObject, SALOMEDSImpl_Driver* Engine);
  Standard_EXPORT virtual bool Copy(const Handle(SALOMEDSImpl_SObject)& theObject, SALOMEDSImpl_Driver* Engine);
  Standard_EXPORT virtual bool CanPaste(const Handle(SALOMEDSImpl_SObject)& theObject, SALOMEDSImpl_Driver* Engine);
  Standard_EXPORT virtual Handle(SALOMEDSImpl_SObject) Paste(const Handle(SALOMEDSImpl_SObject)& theObject, SALOMEDSImpl_Driver* Engine);

  // _SaveAs private function called by Save and SaveAs
  Standard_EXPORT virtual bool Impl_SaveAs(const TCollection_AsciiString& aUrl,
			   const Handle(SALOMEDSImpl_Study)& aStudy,
			   SALOMEDSImpl_DriverFactory* aFactory,
			   bool theMultiFile,
			   bool theASCII);

  // _SaveObject private function called by _SaveAs
  Standard_EXPORT virtual bool Impl_SaveObject(const Handle(SALOMEDSImpl_SObject)& SC, HDFgroup *hdf_group_datatype);

  // _SubstituteSlash function called by Open and GetStudyByName
  Standard_EXPORT virtual TCollection_AsciiString Impl_SubstituteSlash(const TCollection_AsciiString& aUrl);

  Standard_EXPORT virtual bool Impl_SaveProperties(const Handle(SALOMEDSImpl_Study)& aStudy, HDFgroup *hdf_group);

  Standard_EXPORT TCollection_AsciiString GetErrorCode() { return _errorCode; }
  Standard_EXPORT virtual bool IsError() { return _errorCode != ""; }

public:
  DEFINE_STANDARD_RTTI( SALOMEDSImpl_StudyManager )
  
};

#endif 
