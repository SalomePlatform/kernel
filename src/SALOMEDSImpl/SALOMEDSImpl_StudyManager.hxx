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

//  File   : SALOMEDSImpl_StudyManager.hxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#ifndef __SALOMEDSImpl_STUDYMANAGER_I_H__
#define __SALOMEDSImpl_STUDYMANAGER_I_H__

#include "SALOMEDSImpl_Defines.hxx"

// std C++ headers
#include <sstream>
#include <string>
#include <vector>

#include "DF_Application.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_Driver.hxx"
#include "DF_Attribute.hxx"
#include "DF_Label.hxx"
#include "DF_Document.hxx"

class HDFgroup;

class SALOMEDSIMPL_EXPORT SALOMEDSImpl_StudyManager
{

private:

  DF_Application*   _appli;  
  int               _IDcounter;
  DF_Document*      _clipboard;
  std::string       _errorCode;

public:

  //! standard constructor
  SALOMEDSImpl_StudyManager();

  //! standard destructor
  virtual  ~SALOMEDSImpl_StudyManager(); 

  //! method to Create a New Study of name study_name
  virtual SALOMEDSImpl_Study* NewStudy(const std::string& study_name);

  //! method to Open a Study from it's persistent reference
  virtual SALOMEDSImpl_Study* Open(const std::string& aStudyUrl);

  //! method to close a Study 
  virtual void Close(SALOMEDSImpl_Study* aStudy);

  //! method to save a Study 
  virtual bool Save(SALOMEDSImpl_Study* aStudy, SALOMEDSImpl_DriverFactory* aFactory, bool theMultiFile);

  virtual bool SaveASCII(SALOMEDSImpl_Study* aStudy, 
    SALOMEDSImpl_DriverFactory* aFactory, 
    bool theMultiFile);

  //! method to save a Study to the persistent reference aUrl
  virtual bool SaveAs(const std::string& aUrl,  
    SALOMEDSImpl_Study* aStudy, 
    SALOMEDSImpl_DriverFactory* aFactory,
    bool theMultiFile);

  virtual bool SaveAsASCII(const std::string& aUrl, 
    SALOMEDSImpl_Study* aStudy, 
    SALOMEDSImpl_DriverFactory* aFactory,
    bool theMultiFile);

  //! method to Get name list of open studies in the session
  virtual std::vector<SALOMEDSImpl_Study*> GetOpenStudies();

  //! method to get a Study from it's name
  virtual SALOMEDSImpl_Study* GetStudyByName(const std::string& aStudyName) ;

  //! method to get a Study from it's ID
  virtual SALOMEDSImpl_Study* GetStudyByID(int aStudyID) ;


  DF_Document* GetDocumentOfStudy(SALOMEDSImpl_Study* theStudy);

  DF_Document* GetClipboard() { return _clipboard; }

  bool CopyLabel(SALOMEDSImpl_Study* theSourceStudy, 
    SALOMEDSImpl_Driver* theEngine,
    const int theSourceStartDepth,
    const DF_Label& theSource,
    const DF_Label& theDestinationMain);

  DF_Label PasteLabel(SALOMEDSImpl_Study* theDestinationStudy,
    SALOMEDSImpl_Driver* theEngine,
    const DF_Label& theSource,
    const DF_Label& theDestinationStart,
    const int theCopiedStudyID,
    const bool isFirstElement);

  virtual bool CanCopy(const SALOMEDSImpl_SObject& theObject, SALOMEDSImpl_Driver* Engine);
  virtual bool Copy(const SALOMEDSImpl_SObject& theObject, SALOMEDSImpl_Driver* Engine);
  virtual bool CanPaste(const SALOMEDSImpl_SObject& theObject, SALOMEDSImpl_Driver* Engine);
  virtual SALOMEDSImpl_SObject Paste(const SALOMEDSImpl_SObject& theObject, SALOMEDSImpl_Driver* Engine);

  // _SaveAs private function called by Save and SaveAs
  virtual bool Impl_SaveAs(const std::string& aUrl,
    SALOMEDSImpl_Study* aStudy,
    SALOMEDSImpl_DriverFactory* aFactory,
    bool theMultiFile,
    bool theASCII);

  // _SaveObject private function called by _SaveAs
  virtual bool Impl_SaveObject(const SALOMEDSImpl_SObject& SC, HDFgroup *hdf_group_datatype);

  // _SubstituteSlash function called by Open and GetStudyByName
  virtual std::string Impl_SubstituteSlash(const std::string& aUrl);

  virtual bool Impl_SaveProperties(SALOMEDSImpl_Study* aStudy, HDFgroup *hdf_group);

  std::string GetErrorCode() { return _errorCode; }
  virtual bool IsError() { return _errorCode != ""; }

};

#endif 
