// Copyright (C) 2007-2012  CEA/DEN, EDF R&D, OPEN CASCADE
//
// Copyright (C) 2003-2007  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
// CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License.
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

//  File   : SALOMEDS_SimanStudy_i.cxx
//  Author : Mikhail PONIKAROV
//  Module : SALOME
//
#include "SALOMEDS_SimanStudy_i.hxx"
#include "SALOMEDS_Study_i.hxx"
#include "SALOMEDS_Study.hxx"
#include "SALOMEDS_Tool.hxx"
#include "SALOME_DataContainer_i.hxx"
#include "SALOME_NamingService.hxx"
#include "SALOME_LifeCycleCORBA.hxx"
#include "Basics_Utils.hxx"
#include "Basics_DirUtils.hxx"
#include "utilities.h"

#ifdef WITH_SIMANIO
#include <SimanIO_Link.hxx>
#include <SimanIO_Activity.hxx>
#endif

#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOME_Component)

//============================================================================
/*! Function : SALOMEDS_SimanStudy_i
 *  Purpose  : standard constructor
 */
//============================================================================
SALOMEDS_SimanStudy_i::SALOMEDS_SimanStudy_i(/*SALOMEDSImpl_SimanStudy* theImpl,*/ CORBA::ORB_ptr orb)
{
  _orb = CORBA::ORB::_duplicate(orb);
  _name_service = new SALOME_NamingService(_orb);
#ifdef WITH_SIMANIO
  _checkedOut = new SimanIO_Configuration;
#endif
}

//============================================================================
/*! Function : ~SALOMEDS_SimanStudy_i
 *  Purpose  : standard destructor
 */
//============================================================================
SALOMEDS_SimanStudy_i::~SALOMEDS_SimanStudy_i()
{
#ifdef WITH_SIMANIO
  if (_checkedOut) {
    SimanIO_Configuration::ActivitiesIterator actIter(*_checkedOut);
    for(; actIter.More(); actIter.Next()) {
      SimanIO_Activity::DocumentsIterator aDocIter(actIter.Activity());
      for(; aDocIter.More(); aDocIter.Next()) {
        SimanIO_Document& aDoc = aDocIter.Document();
        SimanIO_Document::FilesIterator aFileIter(aDoc);
        for(; aFileIter.More(); aFileIter.Next()) {
	  std::string aURL = aFileIter.URL();
          std::string aDir = Kernel_Utils::GetDirName(aURL);
          aDir += "/";
          std::string aFileName = aURL.substr(aDir.size());
          SALOMEDS::ListOfFileNames aTmpFiles;
          aTmpFiles.length(1);
          aTmpFiles[0] = aFileName.c_str();
          // try to remove temporary directory that contains this file if directory becomes empty
          SALOMEDS_Tool::RemoveTemporaryFiles(aDir, aTmpFiles, true);
        }
      }
    }
    delete _checkedOut;
  }
#endif
  delete _name_service;
}

//============================================================================
/*! Function : CheckOut
 *  Purpose  : Get data from SIMAN and put it to the given study
 */
//============================================================================
void SALOMEDS_SimanStudy_i::CheckOut(SALOMEDS::Study_ptr theTarget)
{
  // SimanStudy and Study must be located at the same place anyway
  SALOMEDS_Study aStudy(theTarget);
  _study = aStudy.GetLocalImpl();
  if (_study) {
#ifdef WITH_SIMANIO
    int aLocked = _study->GetProperties()->IsLocked();
    if (aLocked) _study->GetProperties()->SetLocked(false);
  
    SimanIO_Link aLink(_studyId.c_str(), _scenarioId.c_str(), _userId.c_str());
    if (aLink.IsConnected()) {
      // Set "C" locale temporarily to avoid possible localization problems
      Kernel_Utils::Localizer loc;
      *_checkedOut = aLink.RetrieveConf();
      SimanIO_Configuration::ActivitiesIterator actIter(*_checkedOut);
      for(; actIter.More(); actIter.Next()) {
        Engines::EngineComponent_var aComp =
          SALOME_LifeCycleCORBA(_name_service).FindOrLoad_Component("FactoryServerPy", actIter.Activity().Module());
        if (CORBA::is_nil(aComp)) // it is not python container, try to find in C++ container
          SALOME_LifeCycleCORBA(_name_service).FindOrLoad_Component("FactoryServer", actIter.Activity().Module());
        if (CORBA::is_nil(aComp)) {
          MESSAGE("Checkout: component "<<actIter.Activity().Module()<<" is nil");
        } else {
          SimanIO_Activity::DocumentsIterator aDocIter(actIter.Activity());
          for(; aDocIter.More(); aDocIter.Next()) {
            if (_filesId.find(aDocIter.DocId()) == _filesId.end())
              _filesId[aDocIter.DocId()] = std::map<std::string, int>();
            SimanIO_Document& aDoc = aDocIter.Document();
            SimanIO_Document::FilesIterator aFileIter(aDoc);
            for(; aFileIter.More(); aFileIter.Next()) {
              if (aFileIter.GetProcessing() == FILE_IMPORT) {
                // files provided by SIMAN will be removed later, on study close
                Engines::DataContainer_var aData = (new Engines_DataContainer_i(
                  aFileIter.URL(), aDoc.Name(), "", false))->_this();
                Engines::ListOfOptions anEmptyOpts;
                Engines::ListOfIdentifiers_var anIds = aComp->importData(_study->StudyId(), aData, anEmptyOpts);
                for(int anIdNum = 0; anIdNum < anIds->length(); anIdNum++) {
                  const char* anId = anIds[anIdNum];
                  _filesId[aDocIter.DocId()][anId] = aFileIter.Id();
                }
              } else {
                MESSAGE("!!! File just downloaded, not imported:"<<aFileIter.Id());
              }
            }
          }
        }
      }
    } else {
      MESSAGE("There is no connection to SIMAN!");
    }
  
    if (aLocked) _study->GetProperties()->SetLocked(true);
#endif
  }
}

//============================================================================
/*! Function : CheckIn
 *  Purpose  : Get data from SIMAN study and stores to SIMAN
 */
//============================================================================
void SALOMEDS_SimanStudy_i::CheckIn(const char* theModuleName)
{
  if (_study) {
#ifdef WITH_SIMANIO
    std::string aModuleName(theModuleName);
    SimanIO_Link aLink(_studyId.c_str(), _scenarioId.c_str(), _userId.c_str());
    if (aLink.IsConnected()) {
      // Set "C" locale temporarily to avoid possible localization problems
      Kernel_Utils::Localizer loc;
      SimanIO_Configuration aToStore; // here create and store data in this configuration to check in to SIMAN
      std::string aTmpDir = SALOMEDS_Tool::GetTmpDir(); // temporary directory for checked in files
      int aFileIndex = 0; // for unique file name generation
      std::list<std::string> aTemporaryFileNames;
      SimanIO_Configuration::ActivitiesIterator actIter(*_checkedOut);
      for(; actIter.More(); actIter.Next()) {
        int aDocId = actIter.Activity().DocumentMaxID();
        //if (aDocId < 0) continue; // no documents => no check in
        if (!aModuleName.empty() && aModuleName != actIter.Activity().Module()) {
          continue;
        }
        Engines::EngineComponent_var aComp =
          SALOME_LifeCycleCORBA(_name_service).FindOrLoad_Component("FactoryServerPy", actIter.Activity().Module());
        if (CORBA::is_nil(aComp)) // it is not python container, try to find in C++ container
          SALOME_LifeCycleCORBA(_name_service).FindOrLoad_Component("FactoryServer", actIter.Activity().Module());
        if (CORBA::is_nil(aComp)) {
          MESSAGE("Checkin: component "<<actIter.Activity().Module()<<" is nil");
        } else {
          SimanIO_Document aDoc;
          if (aDocId != -1) // get document is at least one exists in this action, "-1" is the Id of the new document otherwise
            aDoc = actIter.Activity().Document(aDocId);
          Engines::ListOfData_var aList = aComp->getModifiedData(_study->StudyId());
          int aNumData = aList->length();
          for(int aDataIndex = 0; aDataIndex < aNumData; aDataIndex++) {
            Engines::DataContainer_var aData = aList[aDataIndex];
            // store this in the configuration
            SimanIO_Activity& aStoreActivity = aToStore.GetOrCreateActivity(actIter.ActivityId());
            aStoreActivity.SetName(actIter.Activity().Name());
            aStoreActivity.SetModule(actIter.Activity().Module());
            SimanIO_Document& aStoreDoc = aStoreActivity.GetOrCreateDocument(aDocId);
            aStoreDoc.SetName(aDoc.Name());
            // prepare a file to store
            SimanIO_File aStoreFile;
  
            std::stringstream aNumStore;
            aNumStore<<"file"<<(++aFileIndex);
            std::string aFileName(aNumStore.str());
            std::string anExtension(aData->extension());
            aFileName += "." + anExtension;
            std::string aFullPath = aTmpDir + aFileName;
            Engines::TMPFile* aFileStream = aData->get();
            const char *aBuffer = (const char*)aFileStream->NP_data();
#ifdef WIN32
            std::ofstream aFile(aFullPath.c_str(), std::ios::binary);
#else
            std::ofstream aFile(aFullPath.c_str());
#endif
            aFile.write(aBuffer, aFileStream->length());
            aFile.close();
            aTemporaryFileNames.push_back(aFileName);
  
            aStoreFile.url = aFullPath;
            if (_filesId[aDocId].find(aData->identifier()) != _filesId[aDocId].end()) { // file is already exists
              aStoreFile.id = _filesId[aDocId][aData->identifier()];
              aStoreFile.result = aDoc.File(aStoreFile.id).result;
            } else {
              aStoreFile.id = -1; // to be created as new
              aStoreFile.result = true; // new is always result
            }
  
            aStoreDoc.AddFile(aStoreFile);
          }
        }
      }
      aLink.StoreConf(aToStore);
      // after files have been stored, remove them from the temporary directory
      SALOMEDS::ListOfFileNames aTmpFiles;
      aTmpFiles.length(aTemporaryFileNames.size());
      std::list<std::string>::const_iterator aFilesIter = aTemporaryFileNames.begin();
      for(int a = 0; aFilesIter != aTemporaryFileNames.end(); aFilesIter++, a++) {
        aTmpFiles[a] = aFilesIter->c_str();
      }
      SALOMEDS_Tool::RemoveTemporaryFiles(aTmpDir, aTmpFiles, true);
    } else {
      MESSAGE("There is no connection to SIMAN!")
    }
#endif
  }
}

//============================================================================
/*! Function : getReferencedStudy
 *  Purpose  : Returns the %Study with checked out data
 */
//============================================================================
SALOMEDS::Study_ptr SALOMEDS_SimanStudy_i::getReferencedStudy()
{
  SALOMEDS::Study_var aStudy = (new SALOMEDS_Study_i(_study, _orb))->_this();
  return aStudy._retn();
}

//============================================================================
/*! Function : StudyId
 *  Purpose  : The ID of the study in SIMAN server
 */
//============================================================================
char* SALOMEDS_SimanStudy_i::StudyId()
{
  return CORBA::string_dup(_studyId.c_str());
}

//============================================================================
/*! Function : StudyId
 *  Purpose  : The ID of the study in SIMAN server
 */
//============================================================================
void SALOMEDS_SimanStudy_i::StudyId(const char* theId)
{
  _studyId = theId;
}

//============================================================================
/*! Function : ScenarioId
 *  Purpose  : The ID of the scenario in SIMAN server
 */
//============================================================================
char* SALOMEDS_SimanStudy_i::ScenarioId()
{
  return CORBA::string_dup(_scenarioId.c_str());
}

//============================================================================
/*! Function : ScenarioId
 *  Purpose  : The ID of the scenario in SIMAN server
 */
//============================================================================
void SALOMEDS_SimanStudy_i::ScenarioId(const char* theId)
{
  _scenarioId = theId;
}

//============================================================================
/*! Function : UserId
 *  Purpose  : The ID of the user in SIMAN server
 */
//============================================================================
char* SALOMEDS_SimanStudy_i::UserId()
{
  return CORBA::string_dup(_userId.c_str());
}

//============================================================================
/*! Function : UserId
 *  Purpose  : The ID of the user in SIMAN server
 */
//============================================================================
void SALOMEDS_SimanStudy_i::UserId(const char* theId)
{
  _userId = theId;
}

SALOMEDS_SimanStudy_i* SALOMEDS_SimanStudy_i::GetSimanServant(CORBA::ORB_ptr orb)
{
  static SALOMEDS_SimanStudy_i* aServant = 0;
  if (aServant == 0) {
    aServant = new SALOMEDS_SimanStudy_i(orb);
  }
  return aServant;
}
                                  
/// PRIVATE FUNCTIONS
/*
CORBA::LongLong SALOMEDS_SimanStudy_i::GetLocalImpl(const char* theHostname, CORBA::Long thePID, CORBA::Boolean& isLocal)
{
#ifdef WIN32
  long pid = (long)_getpid();
#else
  long pid = (long)getpid();
#endif
  isLocal = (strcmp(theHostname, Kernel_Utils::GetHostname().c_str()) == 0 && pid == thePID)?1:0;
  return reinterpret_cast<CORBA::LongLong>(_impl);
}
*/
