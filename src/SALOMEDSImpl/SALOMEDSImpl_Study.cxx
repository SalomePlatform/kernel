// Copyright (C) 2007-2019  CEA/DEN, EDF R&D, OPEN CASCADE
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

//  File   : SALOMEDSImpl_Study.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_Study.hxx"
#include <string.h>

#include <KERNEL_version.h>
#include <Basics_Utils.hxx>
#include <Basics_DirUtils.hxx>

#include "HDFexplorer.hxx"

#include "DF_Application.hxx"
#include "DF_ChildIterator.hxx"

#include "SALOMEDSImpl_ChildNodeIterator.hxx"
#include "SALOMEDSImpl_Attributes.hxx"
#include "SALOMEDSImpl_UseCaseIterator.hxx"
#include "SALOMEDSImpl_AttributeReference.hxx"
#include "SALOMEDSImpl_StudyHandle.hxx"
#include "SALOMEDSImpl_Tool.hxx"
#include "SALOMEDSImpl_IParameters.hxx"
#include "SALOMEDSImpl_ScalarVariable.hxx"
#include "SALOMEDSImpl_SComponent.hxx"

#include "HDFOI.hxx"
#include <fstream>
#include <sstream>
#include <algorithm>

// comment out the following define to enable \t symbols in in the python dump files
#define WITHOUT_TABS

#define DIRECTORYID       16661
#define FILELOCALID       26662
#define FILEID            "FILE: "
#define VARIABLE_SEPARATOR  ':'
#define OPERATION_SEPARATOR '|'
#define USE_CASE_LABEL_ID "0:2"

static void SaveAttributes(const SALOMEDSImpl_SObject& SO, HDFgroup *hdf_group_sobject);
static void ReadAttributes(SALOMEDSImpl_Study*, const SALOMEDSImpl_SObject&, HDFdataset* );
static void BuildTree (SALOMEDSImpl_Study*, HDFgroup*);
static void Translate_IOR_to_persistentID (const SALOMEDSImpl_SObject&,
                                           SALOMEDSImpl_Driver*, bool isMultiFile, bool isASCII);
static void ReadNoteBookVariables(SALOMEDSImpl_Study* theStudy, HDFgroup* theGroup);

namespace {
  class StudyUnlocker
  {
  public:
    StudyUnlocker( SALOMEDSImpl_Study* study ): myStudy( study ), myLocked( false )
    {
      myPrevLocked = myStudy->GetProperties()->IsLocked();
      resume();
    }
    ~StudyUnlocker()
    {
      suspend();
    }
    void suspend()
    {
      if (myLocked) {
        myStudy->GetProperties()->SetLocked(true);
        myPrevLocked = myLocked;
        myLocked = false;
      }
    }
    void resume()
    {
      if (myPrevLocked) {
        myStudy->GetProperties()->SetLocked(false);
        myLocked = myPrevLocked;
        myPrevLocked = false;
      }
    }
  private:
    SALOMEDSImpl_Study* myStudy;
    bool myLocked;
    bool myPrevLocked;
  };
}

//============================================================================
/*! Function : SALOMEDSImpl_Study
 *  Purpose  : SALOMEDSImpl_Study constructor
 */
//============================================================================
SALOMEDSImpl_Study::SALOMEDSImpl_Study() : _doc(NULL)
{
  _appli = new DF_Application();
  _clipboard = _appli->NewDocument("SALOME_STUDY");

  Init();
}

//============================================================================
/*! Function : ~SALOMEDSImpl_Study
 *  Purpose  : SALOMEDSImpl_Study destructor
 */
//============================================================================
SALOMEDSImpl_Study::~SALOMEDSImpl_Study()
{
  Clear();
  _appli->Close(_clipboard);
  // Destroy application
  delete _appli;
}

//============================================================================
/*! Function : Init
 *  Purpose  : Initialize study components
 */
//============================================================================
void SALOMEDSImpl_Study::Init()
{
  if (_doc)
    return; // noop: already initialized

  static int _id = 0;
  std::stringstream sstrm;
  sstrm << ++_id;
  _name = "Study" + std::string(sstrm.str());
  _doc = _appli->NewDocument("SALOME_STUDY");
  _Saved = false;
  _URL = "";
  _autoFill = false;
  _errorCode = "";
  _useCaseBuilder = new SALOMEDSImpl_UseCaseBuilder(_doc);
  _builder = new SALOMEDSImpl_StudyBuilder(this);
  _cb = new SALOMEDSImpl_Callback(_useCaseBuilder);
  _notifier=0;
  _genObjRegister=0;
  //Put on the root label a StudyHandle attribute to store the address of this object
  //It will be used to retrieve the study object by DF_Label that belongs to the study
  SALOMEDSImpl_StudyHandle::Set(_doc->Main().Root(), this);

  // set Study properties
  SALOMEDSImpl_AttributeStudyProperties* aProp = GetProperties();

  int month=0,day=0,year=0,hh=0,mn=0,ss=0;
  SALOMEDSImpl_Tool::GetSystemDate(year, month, day, hh, mn, ss);
  aProp->SetModification(SALOMEDSImpl_Tool::GetUserName(),
                         mn, hh, day, month, year);
  aProp->SetCreationMode(1);  //"from scratch"
}

//============================================================================
/*! Function : Clear
 *  Purpose  : Clear study components
 */
//============================================================================
void SALOMEDSImpl_Study::Clear()
{
  if (_builder) delete _builder;
  _builder = NULL;
  if (_cb) delete _cb;
  _cb = NULL;
  if (_useCaseBuilder) delete _useCaseBuilder;
  _useCaseBuilder = NULL;
  URL("");
  _appli->Close(_doc);
  _doc = NULL;
  _mapOfSO.clear();
  _mapOfSCO.clear();
}

//============================================================================
/*! Function : Open
 *  Purpose  : Open a Study from it's persistent reference
 */
//============================================================================
bool SALOMEDSImpl_Study::Open(const std::string& aUrl)
{
  // Set "C" locale temporarily to avoid possible localization problems
  Kernel_Utils::Localizer loc;

  _errorCode = "";

  // open the HDFFile
  HDFfile *hdf_file =0;
  HDFgroup *hdf_group_study_structure =0;
  HDFgroup *hdf_notebook_vars = 0;

  char* aC_HDFUrl;
  std::string aHDFUrl;
  bool isASCII = false;
  if (HDFascii::isASCII(aUrl.c_str())) {
    isASCII = true;
    char* aResultPath = HDFascii::ConvertFromASCIIToHDF(aUrl.c_str());
    if ( !aResultPath )
      return NULL;
    aC_HDFUrl = new char[strlen(aResultPath) + 19];
    sprintf(aC_HDFUrl, "%shdf_from_ascii.hdf", aResultPath);
    delete [] (aResultPath);
    aHDFUrl = aC_HDFUrl;
    delete [] aC_HDFUrl;
  }
  else {
    aHDFUrl = aUrl;
  }

  hdf_file = new HDFfile((char*)aHDFUrl.c_str());
  try {
    hdf_file->OpenOnDisk(HDF_RDONLY);// mpv: was RDWR, but opened file can be write-protected too
  }
  catch (HDFexception)
  {
    char *eStr;
    eStr = new char[strlen(aUrl.c_str())+17];
    sprintf(eStr,"Can't open file %s",aUrl.c_str());
    delete [] eStr;
    _errorCode = std::string(eStr);
    return NULL;
  }

  // Assign the value of the URL in the study object
  URL(aUrl);

  SALOMEDSImpl_AttributePersistentRef::Set(_doc->Main(), aUrl);

  if (!hdf_file->ExistInternalObject("STUDY_STRUCTURE")) {
     _errorCode = "Study is empty";
    return false;
  }

  //Create  the Structure of the Document
  hdf_group_study_structure = new HDFgroup("STUDY_STRUCTURE",hdf_file);

  try {
    BuildTree (this, hdf_group_study_structure);
  }
  catch (HDFexception)
  {
    char *eStr = new char [strlen(aUrl.c_str())+17];
    sprintf(eStr,"Can't open file %s", aUrl.c_str());
    _errorCode = std::string(eStr);
    return false;
  }

  //Read and create notebook variables
  if(hdf_file->ExistInternalObject("NOTEBOOK_VARIABLES")) {
    hdf_notebook_vars  = new HDFgroup("NOTEBOOK_VARIABLES",hdf_file);
    ReadNoteBookVariables(this, hdf_notebook_vars);
    hdf_notebook_vars =0; //will be deleted by hdf_sco_group destructor
  }

  hdf_file->CloseOnDisk();
  hdf_group_study_structure = new HDFgroup("STUDY_STRUCTURE",hdf_file);

  if (isASCII) {
    std::vector<std::string> aFilesToRemove;
    aFilesToRemove.push_back("hdf_from_ascii.hdf");
    SALOMEDSImpl_Tool::RemoveTemporaryFiles(SALOMEDSImpl_Tool::GetDirFromPath(aHDFUrl), aFilesToRemove, true);
  }

  delete hdf_file; // all related hdf objects will be deleted

  // unlock study if it is locked, to set components versions
  StudyUnlocker unlock(this);

  //For old studies we have to add "unknown" version tag for all stored components
  SALOMEDSImpl_SComponentIterator itcomponent = NewComponentIterator();
  for (; itcomponent.More(); itcomponent.Next())
  {
    SALOMEDSImpl_SComponent sco = itcomponent.Value();
    std::string aCompType = sco.GetComment();
    if ( aCompType == SALOMEDSImpl_IParameters::getDefaultVisualComponent() ) continue;
    if ( GetProperties()->GetComponentVersions( aCompType ).empty() )
      GetProperties()->SetComponentVersion( aCompType, "" ); // empty version means "unknown"
  }

  return true;
}

//============================================================================
/*! Function : Save
 *  Purpose  : Save a Study to it's persistent reference
 */
//============================================================================
bool SALOMEDSImpl_Study::Save(SALOMEDSImpl_DriverFactory* aFactory,
                              bool theMultiFile,
                              bool theASCII)
{
  _errorCode = "";

  std::string url = URL();
  if (url.empty()) {
    _errorCode = "No path specified to save the study. Nothing done";
    return false;
  }
  else {
    return Impl_SaveAs(url, aFactory, theMultiFile, theASCII);
  }

  return false;
}

//=============================================================================
/*! Function : SaveAs
 *  Purpose  : Save a study to the persistent reference aUrl
 */
//============================================================================
bool SALOMEDSImpl_Study::SaveAs(const std::string& aUrl,
                                SALOMEDSImpl_DriverFactory* aFactory,
                                bool theMultiFile,
                                bool theASCII)
{
  _errorCode = "";
  return Impl_SaveAs(aUrl, aFactory, theMultiFile, theASCII);
}

//=============================================================================
/*! Function : _SaveProperties
 *  Purpose  : save the study properties in HDF file
 */
//============================================================================
bool SALOMEDSImpl_Study::Impl_SaveProperties(HDFgroup *hdf_group)
{
  _errorCode = "";

  HDFdataset *hdf_dataset = 0;
  hdf_size size[1];
  hdf_int32 name_len;

  // add modifications list (user and date of save)
  SALOMEDSImpl_AttributeStudyProperties* aProp = GetProperties();

  // unlock study if it is locked, to set modification date
  StudyUnlocker unlock(this);

  int month=0,day=0,year=0,hh=0,mn=0,ss=0;
  SALOMEDSImpl_Tool::GetSystemDate(year, month, day, hh, mn, ss);
  aProp->SetModification(SALOMEDSImpl_Tool::GetUserName(),
                         mn, hh, day, month, year);

  // lock study back if it was locked initially, to write correct value of Locked flag
  unlock.suspend();

  std::vector<std::string> aNames;
  std::vector<int> aMinutes, aHours, aDays, aMonths, aYears;

  aProp->GetModifications(aNames, aMinutes, aHours, aDays, aMonths, aYears);

  std::string units = aProp->GetUnits();
  std::string comment = aProp->GetComment();

  std::map< std::string, std::vector<std::string> > allVersions = aProp->GetComponentsVersions();
  std::map<std::string, std::string> versions;

  int aLength = 0, aLength1 = 0, anIndex, i, unitsSize = 0, commentSize = 0;

  for(i=1; i<=aNames.size(); i++)
    aLength += aNames[i-1].size() + 1;

  std::map< std::string, std::vector<std::string> >::const_iterator it;
  for (it = allVersions.begin(); it != allVersions.end(); ++it ) {
    std::string vlist = "";
    std::vector<std::string> vl = it->second;
    std::vector<std::string>::const_iterator vlit;
    for ( vlit = vl.begin(); vlit != vl.end(); ++vlit ) {
      if ( vlist != "" ) vlist += ";";
      vlist += *vlit;
    }
    versions[ it->first ] = vlist;
    aLength1 += it->first.size() + vlist.size() + 2;
  }

  unitsSize = units.size();
  commentSize = comment.size();

  //string format:
  //locked flag, modified flag,
  //minutes, hours, day, months, year, user name, char(1),
  //minutes, hours, day, months, year, user name, char(1),
  //.....................................................,
  //.....................................................,
  //.....................................................,
  //minutes, hours, day, months, year, user name, char(1), char(30) <- !!!! used to define end of section with modifications !!!!
  //units, char(1), comment, char(30) <- !!!! used to define start of section with components' versions !!!!
  //component=versions, char(1),
  //component=versions, char(1),
  //...........................,
  //component=versions, char(1), char(0)

  //string length: 1 byte = locked flag, 1 byte = modified flag, (12 + name length + 1) for each name and date, 1 byte (char(30) section delimiter)
  // unit length + 1, comment length, "zero" byte

  char* aProperty = new char[3 + aLength + 12 * aNames.size() + 1 + unitsSize + 1 + commentSize + 1 + aLength1 ];

  sprintf(aProperty,"%c%c", (char)aProp->GetCreationMode(),  (aProp->IsLocked())?'l':'u');

  aLength = aNames.size();
  int a = 2;
  for(anIndex = 0; anIndex<aLength; anIndex++) {
    sprintf(&(aProperty[a]),"%2d%2d%2d%2d%4d%s",
            (int)(aMinutes[anIndex]),
            (int)(aHours[anIndex]),
            (int)(aDays[anIndex]),
            (int)(aMonths[anIndex]),
            (int)(aYears[anIndex]),
            aNames[anIndex].c_str());
    a = strlen(aProperty);
    aProperty[a++] = 1;
  }

  //Write delimiter of the section to define end of the modifications section
  aProperty[a++] = 30;

  //Write units if need
  if(units.size() > 0) {
    sprintf(&(aProperty[a]),"%s",units.c_str());
    a = strlen(aProperty);
  }

  aProperty[a++] = 1;

  //Write comments if need
  if(comment.size() > 0) {
    sprintf(&(aProperty[a]),"%s",comment.c_str());
    a = strlen(aProperty);
  }

  aProperty[a++] = 30; //delimiter of the component versions

  std::map<std::string, std::string>::const_iterator versionsIt;
  for ( versionsIt = versions.begin(); versionsIt != versions.end(); ++versionsIt ) {
    sprintf(&(aProperty[a]),"%s=%s",
            (char*)(versionsIt->first.c_str()),
            (char*)(versionsIt->second.c_str()));
    a = a + versionsIt->first.size() + versionsIt->second.size() + 1;
    aProperty[a++] = 1;
  }

  aProperty[a] = 0;

  name_len = (hdf_int32) a;
  size[0] = name_len + 1 ;
  hdf_dataset = new HDFdataset("AttributeStudyProperties",hdf_group,HDF_STRING,size,1);
  hdf_dataset->CreateOnDisk();
  hdf_dataset->WriteOnDisk(aProperty);
  hdf_dataset->CloseOnDisk();
  hdf_dataset=0; //will be deleted by hdf_sco_group destructor
  delete [] aProperty;

  aProp->SetModified(0);
  return true;
}

//=============================================================================
/*! Function : _SaveAs
 *  Purpose  : save the study in HDF file
 */
//============================================================================
bool SALOMEDSImpl_Study::Impl_SaveAs(const std::string& aStudyUrl,
                                     SALOMEDSImpl_DriverFactory* aFactory,
                                     bool theMultiFile,
                                     bool theASCII)
{
  // Set "C" locale temporarily to avoid possible localization problems
  Kernel_Utils::Localizer loc;

  // HDF File will be composed of different part :
  // * For each ComponentDataType, all data created by the component
  //   Information in data group hdf_group_datacomponent
  // * Study Structure -> Exactly what is contained in Document
  //   Information in data group hdf_group_study_structure

  _errorCode = "";

  HDFfile *hdf_file=0;
  HDFgroup *hdf_group_study_structure =0;
  HDFgroup *hdf_sco_group =0;
  HDFgroup *hdf_sco_group2 =0;
  HDFgroup *hdf_notebook_vars =0;
  HDFgroup *hdf_notebook_var  = 0;

  HDFgroup *hdf_group_datacomponent =0;
  HDFdataset *hdf_dataset =0;
  hdf_size size[1];
  hdf_int32 name_len = 0;
  std::string component_name;

  // Store previous URL
  std::string anOldName = URL();

  // Map to store components' versions
  std::map<std::string, std::string> componentVersions;

  //Create a temporary url to which the study is saved
  std::string aUrl = SALOMEDSImpl_Tool::GetTmpDir() + SALOMEDSImpl_Tool::GetNameFromPath(aStudyUrl);

  // unlock study if it is locked, as some attributes need to be modified
  StudyUnlocker unlock(this);

  SALOMEDSImpl_StudyBuilder* SB= NewBuilder();
  std::map<std::string, SALOMEDSImpl_Driver*> aMapTypeDriver;

  try {
    // mpv 15.12.2003: for saving components we have to load all data from all modules
    SALOMEDSImpl_SComponentIterator itcomponent = NewComponentIterator();
    for (; itcomponent.More(); itcomponent.Next()) {
      SALOMEDSImpl_SComponent sco = itcomponent.Value();
      // if there is an associated Engine call its method for saving
      std::string IOREngine;
      try {
        SALOMEDSImpl_Driver* aDriver = NULL;
        std::string aCompType = sco.GetComment();
        if (!sco.ComponentIOR(IOREngine)) {
          if (!aCompType.empty()) {
            aDriver = aFactory->GetDriverByType(aCompType);
            if (aDriver != NULL) {
              if (!SB->LoadWith(sco, aDriver)) {
                _errorCode = SB->GetErrorCode();
                return false;
              }
            }
          }
        }
        else {
          aDriver = aFactory->GetDriverByIOR(IOREngine);
        }
        aMapTypeDriver[aCompType] = aDriver;
      }
      catch(...) {
        _errorCode = "Can not restore information to resave it";
        return false;
      }
    }

    // VSR: set URL to new file name
    // VSR: remember to set previous name if save operation fails
    URL(aStudyUrl);

    // To change for Save
    // Do not have to do a new file but just a Open??? Rewrite all information after erasing everything??
    hdf_file = new HDFfile((char*)aUrl.c_str());
    hdf_file->CreateOnDisk();

    //-----------------------------------------------------------------------
    // 1 - Create a groupe for each SComponent and Update the PersistanceRef
    //-----------------------------------------------------------------------
    hdf_group_datacomponent = new HDFgroup("DATACOMPONENT",hdf_file);
    hdf_group_datacomponent->CreateOnDisk();

    for (itcomponent.Init(); itcomponent.More(); itcomponent.Next()) {
      SALOMEDSImpl_SComponent sco = itcomponent.Value();

      std::string scoid = sco.GetID();
      hdf_sco_group = new HDFgroup((char*)scoid.c_str(), hdf_group_datacomponent);
      hdf_sco_group->CreateOnDisk();

      std::string componentDataType = sco.ComponentDataType();
      std::string IOREngine;
      if (sco.ComponentIOR(IOREngine)) {
        // Engine should be already in the map as it was to added before
        SALOMEDSImpl_Driver* Engine = aMapTypeDriver[componentDataType];
        if (Engine != NULL) {
          SALOMEDSImpl_TMPFile* aStream = NULL;
          long length = 0;

          componentVersions[ componentDataType ] = Engine->Version();

          if (theASCII) aStream = Engine->SaveASCII(sco,
                                                    SALOMEDSImpl_Tool::GetDirFromPath(aUrl),
                                                    length,
                                                    theMultiFile);
          else aStream = Engine->Save(sco,
                                      SALOMEDSImpl_Tool::GetDirFromPath(aUrl),
                                      length,
                                      theMultiFile);
          HDFdataset *hdf_dataset;
          hdf_size aHDFSize[1];
          if (length > 0) {  //The component saved some auxiliary files, then put them into HDF file
            aHDFSize[0] = length;

            HDFdataset *hdf_dataset = new HDFdataset("FILE_STREAM", hdf_sco_group, HDF_STRING, aHDFSize, 1);
            hdf_dataset->CreateOnDisk();
            hdf_dataset->WriteOnDisk(aStream->Data());  //Save the stream in the HDF file
            hdf_dataset->CloseOnDisk();
          }

          if (aStream) delete aStream;

          // store multifile state
          aHDFSize[0] = 2;
          hdf_dataset = new HDFdataset("MULTIFILE_STATE", hdf_sco_group, HDF_STRING, aHDFSize, 1);
          hdf_dataset->CreateOnDisk();
          hdf_dataset->WriteOnDisk((void*)(theMultiFile?"M":"S")); // save: multi or single
          hdf_dataset->CloseOnDisk();
          hdf_dataset=0; //will be deleted by hdf_sco_AuxFiles destructor
          // store ASCII state
          aHDFSize[0] = 2;
          hdf_dataset = new HDFdataset("ASCII_STATE", hdf_sco_group, HDF_STRING, aHDFSize, 1);
          hdf_dataset->CreateOnDisk();
          hdf_dataset->WriteOnDisk((void*)(theASCII?"A":"B")); // save: ASCII or BINARY
          hdf_dataset->CloseOnDisk();
          hdf_dataset=0; //will be deleted by hdf_sco_AuxFiles destructor
          // Creation of the persistence reference  attribute
          Translate_IOR_to_persistentID (sco, Engine, theMultiFile, theASCII);
        }
      }
      hdf_sco_group->CloseOnDisk();
      hdf_sco_group=0; // will be deleted by hdf_group_datacomponent destructor
    }
    hdf_group_datacomponent->CloseOnDisk();
    hdf_group_datacomponent =0;  // will be deleted by hdf_file destructor

    //-----------------------------------------------------------------------
    //3 - Write the Study Structure
    //-----------------------------------------------------------------------
    hdf_group_study_structure = new HDFgroup("STUDY_STRUCTURE",hdf_file);
    hdf_group_study_structure->CreateOnDisk();
    // save component attributes
    for (itcomponent.Init(); itcomponent.More(); itcomponent.Next()) {
      SALOMEDSImpl_SComponent SC = itcomponent.Value();
      std::string scid = SC.GetID();
      hdf_sco_group2 = new HDFgroup((char*)scid.c_str(), hdf_group_study_structure);
      hdf_sco_group2->CreateOnDisk();
      SaveAttributes(SC, hdf_sco_group2);
      // ComponentDataType treatment
      component_name = SC.ComponentDataType();
      name_len = (hdf_int32)component_name.length();
      size[0] = name_len +1 ;
      hdf_dataset = new HDFdataset("COMPONENTDATATYPE",hdf_sco_group2,HDF_STRING,size,1);
      hdf_dataset->CreateOnDisk();
      hdf_dataset->WriteOnDisk((char*)component_name.c_str());
      hdf_dataset->CloseOnDisk();
      hdf_dataset=0; //will be deleted by hdf_sco_group destructor
      Impl_SaveObject(SC, hdf_sco_group2);
      hdf_sco_group2->CloseOnDisk();
      hdf_sco_group2=0; // will be deleted by hdf_group_study_structure destructor
    }
    //-----------------------------------------------------------------------
    //4 - Write the Study UseCases Structure
    //-----------------------------------------------------------------------
    SALOMEDSImpl_SObject aSO = FindObjectID(USE_CASE_LABEL_ID);
    if (aSO) {
      HDFgroup *hdf_soo_group = new HDFgroup(USE_CASE_LABEL_ID,hdf_group_study_structure);
      hdf_soo_group->CreateOnDisk();
      SaveAttributes(aSO, hdf_soo_group);
      Impl_SaveObject(aSO, hdf_soo_group);
      hdf_soo_group->CloseOnDisk();
      hdf_soo_group=0; // will be deleted by hdf_group_study_structure destructor
    }
    //-----------------------------------------------------------------------
    //5 - Write the NoteBook Variables
    //-----------------------------------------------------------------------

    //5.1 Create group to store all note book variables
    hdf_notebook_vars = new HDFgroup("NOTEBOOK_VARIABLES",hdf_file);
    hdf_notebook_vars->CreateOnDisk();

    std::string varValue;
    std::string varType;
    std::string varIndex;

    for (int i=0 ;i < myNoteBookVars.size(); i++ ) {
      // For each variable create HDF group
      hdf_notebook_var = new HDFgroup((char*)myNoteBookVars[i]->Name().c_str(),hdf_notebook_vars);
      hdf_notebook_var->CreateOnDisk();

      // Save Variable type
      varType = myNoteBookVars[i]->SaveType();
      name_len = (hdf_int32) varType.length();
      size[0] = name_len +1 ;
      hdf_dataset = new HDFdataset("VARIABLE_TYPE",hdf_notebook_var,HDF_STRING,size,1);
      hdf_dataset->CreateOnDisk();
      hdf_dataset->WriteOnDisk((char*)varType.c_str());
      hdf_dataset->CloseOnDisk();
      hdf_dataset=0; //will be deleted by hdf_sco_group destructor

      char buffer[256];
      sprintf(buffer,"%d",i);
      varIndex= std::string(buffer);
      name_len = (hdf_int32) varIndex.length();
      size[0] = name_len +1 ;
      hdf_dataset = new HDFdataset("VARIABLE_INDEX",hdf_notebook_var,HDF_STRING,size,1);
      hdf_dataset->CreateOnDisk();
      hdf_dataset->WriteOnDisk((char*)varIndex.c_str());
      hdf_dataset->CloseOnDisk();
      hdf_dataset=0; //will be deleted by hdf_sco_group destructor

      // Save Variable value
      varValue = myNoteBookVars[i]->Save();
      name_len = (hdf_int32) varValue.length();
      size[0] = name_len +1 ;
      hdf_dataset = new HDFdataset("VARIABLE_VALUE",hdf_notebook_var,HDF_STRING,size,1);
      hdf_dataset->CreateOnDisk();
      hdf_dataset->WriteOnDisk((char*)varValue.c_str());
      hdf_dataset->CloseOnDisk();
      hdf_dataset=0; //will be deleted by hdf_sco_group destructor
      hdf_notebook_var->CloseOnDisk();
      hdf_notebook_var = 0; //will be deleted by hdf_sco_group destructor
    }
    hdf_notebook_vars->CloseOnDisk();
    hdf_notebook_vars = 0; //will be deleted by hdf_sco_group destructor

    // record component versions
    std::map<std::string, std::string>::const_iterator itVersions;
    for ( itVersions = componentVersions.begin(); itVersions != componentVersions.end(); ++itVersions )
      GetProperties()->SetComponentVersion( itVersions->first, itVersions->second );

    // lock study back if it was locked initially, to write correct value of Locked flag
    unlock.suspend();

    //-----------------------------------------------------------------------
    //6 - Write the Study Properties
    //-----------------------------------------------------------------------
    std::string study_name = Name();
    name_len = (hdf_int32) study_name.size();
    size[0] = name_len +1 ;
    hdf_dataset = new HDFdataset("STUDY_NAME",hdf_group_study_structure,HDF_STRING,size,1);
    hdf_dataset->CreateOnDisk();
    hdf_dataset->WriteOnDisk((char*)study_name.c_str());
    hdf_dataset->CloseOnDisk();
    hdf_dataset=0; // will be deleted by hdf_group_study_structure destructor

    Impl_SaveProperties(hdf_group_study_structure);
    hdf_group_study_structure->CloseOnDisk();
    hdf_file->CloseOnDisk();

    hdf_group_study_structure =0; // will be deleted by hdf_file destructor
    delete hdf_file; // recursively deletes all hdf objects...
  }
  catch (HDFexception) {
    _errorCode = "HDFexception ! ";
    URL( anOldName ); // VSR: restore previous url if operation is failed
    return false;
  }
  catch (std::exception& exc) {
    _errorCode = const_cast<char*>(exc.what());
    URL( anOldName ); // VSR: restore previous url if operation is failed
    return false;
  }
  catch (...) {
    _errorCode = "Unknown exception ! ";
    URL( anOldName ); // VSR: restore previous url if operation is failed
    return false;
  }
  if (theASCII) { // save file in ASCII format
    HDFascii::ConvertFromHDFToASCII(aUrl.c_str(), true);
  }

  // Now it's necessary to copy files from the temporary directory to the user defined directory.
  // The easiest way to get a list of file in the temporary directory

  std::string aCmd, aTmpFileDir = SALOMEDSImpl_Tool::GetTmpDir();
  std::string aTmpFile = aTmpFileDir +"files";
  std::string aStudyTmpDir = SALOMEDSImpl_Tool::GetDirFromPath(aUrl);

#ifdef WIN32
  aCmd = "dir /B \"" + aStudyTmpDir +"\" > \"" + aTmpFile + "\"";
#else
  aCmd ="ls -1 \"" + aStudyTmpDir +"\" > " + aTmpFile;
#endif
#if defined(WIN32) && defined(UNICODE)
  std::wstring awCmd = Kernel_Utils::utf8_decode_s(aCmd);
  _wsystem( awCmd.c_str() );
#else  
  system(aCmd.c_str());
#endif

  // Iterate and move files in the temporary directory
#if defined(WIN32) && defined(UNICODE)
  std::wstring awTmpFile = Kernel_Utils::utf8_decode_s(aTmpFile);
  FILE* fp = _wfopen(awTmpFile.c_str(), L"rb");
#else
  FILE* fp = fopen(aTmpFile.c_str(), "rb");
#endif
  if (!fp) {
    URL( anOldName ); // VSR: restore previous url if operation is failed
    return false;
  }
  char* buffer = new char[2047];
  int errors = 0;
  while (!feof(fp) && !errors) {
    if ((fgets(buffer, 2046, fp)) == NULL) break;
    size_t aLen = strlen(buffer);
    if (buffer[aLen-1] == '\n') buffer[aLen-1] = char(0);
#ifdef WIN32
    aCmd = "move /Y \"" + aStudyTmpDir + std::string(buffer) + "\" \"" + SALOMEDSImpl_Tool::GetDirFromPath(aStudyUrl) +"\" 2>NUL";
#else
    aCmd = "mv -f \"" + aStudyTmpDir + std::string(buffer) + "\" \"" + SALOMEDSImpl_Tool::GetDirFromPath(aStudyUrl)+"\"";
#endif
#if defined(WIN32) && defined(UNICODE)
	std::wstring awCmd = Kernel_Utils::utf8_decode_s(aCmd);
    errors = _wsystem(awCmd.c_str());
#else  
    errors = system(aCmd.c_str());
#endif
  }

  delete []buffer;
  fclose(fp);

  // Perform cleanup
#ifdef WIN32
#ifdef UNICODE
  std::wstring aTmpFileToDelete = Kernel_Utils::utf8_decode_s(aTmpFile);
 
#else
  std::string aTmpFileToDelete = aTmpFile;
#endif
  DeleteFile(aTmpFileToDelete.c_str());
#else
  unlink(aTmpFile.c_str());
#endif

#ifdef WIN32
#ifdef UNICODE
  std::wstring aTmpFileDirToDelete = Kernel_Utils::utf8_decode_s( aTmpFileDir );
  std::wstring aStudyTmpDirToDelete = Kernel_Utils::utf8_decode_s( aStudyTmpDir );
#else
  std::string aTmpFileDirToDelete = aTmpFileDir;
  std::string aStudyTmpDirToDelete = aStudyTmpDir;
#endif  
  RemoveDirectory( aTmpFileDirToDelete.c_str() );
  RemoveDirectory( aStudyTmpDirToDelete.c_str() );
#else
  rmdir(aTmpFileDir.c_str());
  rmdir(aStudyTmpDir.c_str());
#endif

  if ( !errors ) {
    // VSR: finally, if all is done without errors, mark study as Saved
    IsSaved(true);
  }

  std::map<std::string, SALOMEDSImpl_Driver*>::iterator n2dr = aMapTypeDriver.begin();
  for ( ; n2dr != aMapTypeDriver.end(); ++n2dr )
    delete n2dr->second;

  return !errors;
}

//============================================================================
/*! Function : Impl_SaveObject
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::Impl_SaveObject(const SALOMEDSImpl_SObject& SC,
                                         HDFgroup *hdf_group_datatype)
{
  _errorCode = "";

  // Write in group hdf_group_datatype all information of SObject SC
  // Iterative function to parse all SObjects under a SComponent

  HDFgroup *hdf_group_sobject = 0;

  DF_ChildIterator itchild(SC.GetLabel());
  for (; itchild.More(); itchild.Next()) {
    // mpv: don't save empty labels
    std::vector<DF_Attribute*> attr = itchild.Value().GetAttributes();
    if (attr.size() == 0) {  //No attributes on the label
      DF_ChildIterator subchild(itchild.Value());
      if (!subchild.More()) {
        continue;
      }
      subchild.Init(itchild.Value(), true);
      bool anEmpty = true;
      for (; subchild.More() && anEmpty; subchild.Next()) {
        std::vector<DF_Attribute*> attr2 = subchild.Value().GetAttributes();
        if (attr2.size()) {
          anEmpty = false;  //There are attributes on the child label
          break;
        }
      }
      if (anEmpty) continue;
    }

    SALOMEDSImpl_SObject SO = SALOMEDSImpl_Study::SObject(itchild.Value());

    std::string scoid = SO.GetID();
    hdf_group_sobject = new HDFgroup(scoid.c_str(), hdf_group_datatype);
    hdf_group_sobject->CreateOnDisk();
    SaveAttributes(SO, hdf_group_sobject);
    Impl_SaveObject(SO, hdf_group_sobject);
    hdf_group_sobject->CloseOnDisk();
    hdf_group_sobject =0; // will be deleted by father hdf object destructor
  }
  return true;
}

//============================================================================
/*! Function : CanCopy
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::CanCopy(const SALOMEDSImpl_SObject& theObject,
                                 SALOMEDSImpl_Driver* theEngine)
{
  _errorCode = "";
  SALOMEDSImpl_SComponent aComponent = theObject.GetFatherComponent();
  if (!aComponent) return false;
  if (aComponent.GetLabel() == theObject.GetLabel()) return false;
  std::string IOREngine;
  if (!aComponent.ComponentIOR(IOREngine)) return false;
  if (theEngine == NULL) return false;
  return theEngine->CanCopy(theObject);
}

//============================================================================
/*! Function : CopyLabel
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::CopyLabel(SALOMEDSImpl_Driver* theEngine,
                                   const int theSourceStartDepth,
                                   const DF_Label& theSource,
                                   const DF_Label& theDestinationMain)
{
  _errorCode = "";

  int a;
  DF_Label aTargetLabel = theDestinationMain;
  DF_Label aAuxTargetLabel = theDestinationMain.Father().FindChild(2);
  for(a = theSource.Depth() - theSourceStartDepth; a > 0 ; a--) {
    DF_Label aSourceLabel = theSource;
    for(int aNbFather = 1; aNbFather < a; aNbFather++) aSourceLabel = aSourceLabel.Father();
    aTargetLabel = aTargetLabel.FindChild(aSourceLabel.Tag());
    aAuxTargetLabel = aAuxTargetLabel.FindChild(aSourceLabel.Tag());
  }
  // iterate attributes
  std::vector<DF_Attribute*> attrList = theSource.GetAttributes();
  for(int i = 0, len = attrList.size(); i<len; i++) {
    DF_Attribute* anAttr = attrList[i];
    std::string type = SALOMEDSImpl_GenericAttribute::Impl_GetType(anAttr);
    if (type.substr(0, 17) == std::string("AttributeTreeNode")) continue; // never copy tree node attribute
    if (type == std::string("AttributeTarget")) continue; // and target attribute

    if (type == std::string("AttributeReference")) { // reference copied as Comment in aux tree
      DF_Label aReferenced = dynamic_cast<SALOMEDSImpl_AttributeReference*>(anAttr)->Get();
      std::string anEntry = aReferenced.Entry();
      // store the value of name attribute of referenced label
      SALOMEDSImpl_AttributeName* aNameAttribute;
      if ((aNameAttribute=(SALOMEDSImpl_AttributeName*)aReferenced.FindAttribute(SALOMEDSImpl_AttributeName::GetID()))) {
        anEntry += " ";
        anEntry += aNameAttribute->Value();
      }
      SALOMEDSImpl_AttributeComment::Set(aAuxTargetLabel, anEntry);
      continue;
    }

    if (type == std::string("AttributeIOR")) { // IOR => ID and TMPFile of Engine
      std::string anEntry = theSource.Entry();
      SALOMEDSImpl_SObject aSO = FindObjectID(anEntry);
      int anObjID;
      long aLen;
      SALOMEDSImpl_TMPFile* aStream = theEngine->CopyFrom(aSO, anObjID, aLen);
      std::string aResStr("");
      for(a = 0; a < aLen; a++) {
        aResStr += (char)(aStream->Get(a));
      }

      if(aStream) delete aStream;

      SALOMEDSImpl_AttributeInteger::Set(aAuxTargetLabel, anObjID);
      SALOMEDSImpl_AttributeName::Set(aAuxTargetLabel, aResStr);
      continue;
    }
    DF_Attribute* aNewAttribute = anAttr->NewEmpty();
    aTargetLabel.AddAttribute(aNewAttribute);
    anAttr->Paste(aNewAttribute);
  }

  return true;
}

//============================================================================
/*! Function : Copy
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::Copy(const SALOMEDSImpl_SObject& theObject,
                                    SALOMEDSImpl_Driver* theEngine)
{
  _errorCode = "";

  // adoptation for alliances datamodel copy: without IOR attributes !!!
  bool aStructureOnly; // copy only SObjects and attributes without component help
  aStructureOnly = !theObject.GetLabel().IsAttribute(SALOMEDSImpl_AttributeIOR::GetID());

  if (!_doc) {
    _errorCode = "Document is null";
    return false;
  }

  //Clear the clipboard
  _clipboard->Main().Root().ForgetAllAttributes(true);
  _appli->Close(_clipboard);
  _clipboard = _appli->NewDocument("SALOME_STUDY");

  // set component data type to the name attribute of root label
  if (!aStructureOnly) {
    SALOMEDSImpl_AttributeComment::Set(_clipboard->Main().Root(),
                                       theEngine->ComponentDataType());
  }
  // iterate all theObject's label children
  DF_Label aStartLabel = theObject.GetLabel();
  int aSourceStartDepth = aStartLabel.Depth();

  // copy main source label
  CopyLabel(theEngine, aSourceStartDepth, aStartLabel, _clipboard->Main());

  // copy all subchildren of the main source label (all levels)
  DF_ChildIterator anIterator(aStartLabel, true);
  for(; anIterator.More(); anIterator.Next()) {
    CopyLabel(theEngine, aSourceStartDepth, anIterator.Value(), _clipboard->Main());
  }

  return true;
}

//============================================================================
/*! Function : CanPaste
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::CanPaste(const SALOMEDSImpl_SObject& theObject,
                                         SALOMEDSImpl_Driver* theEngine)
{
  _errorCode = "";

  if (!_clipboard) {
    _errorCode = "Clipboard is null";
    return false;
  }

  SALOMEDSImpl_AttributeComment* aCompName = NULL;
  if (!(aCompName=(SALOMEDSImpl_AttributeComment*)_clipboard->Main().Root().FindAttribute(SALOMEDSImpl_AttributeComment::GetID()))) {
    _errorCode = "Clipboard has no component type";
    return false;
  }
  SALOMEDSImpl_AttributeInteger* anObjID;
  if (!(anObjID=(SALOMEDSImpl_AttributeInteger*)_clipboard->Main().Father().FindChild(2).FindAttribute(SALOMEDSImpl_AttributeInteger::GetID()))) {
    _errorCode = "Clipboard has no object id";
    return false;
  }
  SALOMEDSImpl_SComponent aComponent = theObject.GetFatherComponent();
  if (!aComponent) {
    _errorCode = "Object doesn't belong to component";
    return false;
  }

  std::string IOREngine;
  if (!aComponent.ComponentIOR(IOREngine)) {
    _errorCode = "component has no IOR";
    return false;
  }
  return theEngine->CanPaste(aCompName->Value(), anObjID->Value());
}

//============================================================================
/*! Function : PasteLabel
 *  Purpose  :
 */
//============================================================================
DF_Label SALOMEDSImpl_Study::PasteLabel(SALOMEDSImpl_Driver* theEngine,
                                        const DF_Label& theSource,
                                        const DF_Label& theDestinationStart,
                                        const bool isFirstElement)
{
  _errorCode = "";

  // get corresponding source, target and auxiliary labels
  DF_Label aTargetLabel = theDestinationStart;

  DF_Label aAuxSourceLabel = theSource.Root().FindChild(2);
  int a;
  if (!isFirstElement) {
    for(a = theSource.Depth() - 1; a > 0 ; a--) {
      DF_Label aSourceLabel = theSource;
      for(int aNbFather = 1; aNbFather < a; aNbFather++) aSourceLabel = aSourceLabel.Father();
      aTargetLabel = aTargetLabel.FindChild(aSourceLabel.Tag());
      aAuxSourceLabel = aAuxSourceLabel.FindChild(aSourceLabel.Tag());
    }
    SALOMEDSImpl_SObject so = GetSObject(aTargetLabel);
    addSO_Notification(so);
  }

  // check auxiliary label for TMPFile => IOR
  SALOMEDSImpl_AttributeName* aNameAttribute = NULL;
  if ((aNameAttribute=(SALOMEDSImpl_AttributeName*)aAuxSourceLabel.FindAttribute(SALOMEDSImpl_AttributeName::GetID()))) {
    SALOMEDSImpl_AttributeInteger* anObjID = (SALOMEDSImpl_AttributeInteger*)aAuxSourceLabel.FindAttribute(SALOMEDSImpl_AttributeInteger::GetID());
    SALOMEDSImpl_AttributeComment* aComponentName = (SALOMEDSImpl_AttributeComment*)theSource.Root().FindAttribute(SALOMEDSImpl_AttributeComment::GetID());
    std::string aCompName = aComponentName->Value();

    if (theEngine->CanPaste(aCompName, anObjID->Value())) {
      std::string aTMPStr = aNameAttribute->Value();
      int aLen = aTMPStr.size();
      unsigned char* aStream = NULL;
      if(aLen > 0) {
        aStream = new unsigned char[aLen+10];
        for(a = 0; a < aLen; a++) {
          aStream[a] = aTMPStr[a];
        }
      }

      std::string anEntry = aTargetLabel.Entry();
      SALOMEDSImpl_SObject aPastedSO = FindObjectID(anEntry);

      if (isFirstElement) {
        std::string aDestEntry = theEngine->PasteInto(aStream,
                                                      aLen,
                                                      anObjID->Value(),
                                                      aPastedSO.GetFatherComponent());
        aTargetLabel = DF_Label::Label(theDestinationStart, aDestEntry);
      } else
        theEngine->PasteInto(aStream, aLen, anObjID->Value(), aPastedSO);

      if(aStream != NULL) delete []aStream;
    }
  }

  // iterate attributes
  std::vector<DF_Attribute*> attrList = theSource.GetAttributes();
  for(int i = 0, len = attrList.size(); i<len; i++) {
    DF_Attribute* anAttr = attrList[i];
    if (aTargetLabel.FindAttribute(anAttr->ID())) {
      aTargetLabel.ForgetAttribute(anAttr->ID());
    }
    DF_Attribute* aNewAttribute = anAttr->NewEmpty();
    aTargetLabel.AddAttribute(aNewAttribute);
    anAttr->Paste(aNewAttribute);
  }

  // check auxiliary label for Comment => reference or name attribute of the referenced object
  SALOMEDSImpl_AttributeComment* aCommentAttribute = NULL;
  if ((aCommentAttribute=(SALOMEDSImpl_AttributeComment*)aAuxSourceLabel.FindAttribute(SALOMEDSImpl_AttributeComment::GetID()))) {
    char * anEntry = new char[aCommentAttribute->Value().size() + 1];
    strcpy(anEntry, std::string(aCommentAttribute->Value()).c_str());
    char* aNameStart = strchr(anEntry, ' ');
    if (aNameStart) {
      *aNameStart = '\0';
      aNameStart++;
    }
    // copy to the same study, reanimate reference
    DF_Label aRefLabel = DF_Label::Label(aTargetLabel, anEntry);
    SALOMEDSImpl_AttributeReference::Set(aTargetLabel, aRefLabel);
    // target attributes structure support
    SALOMEDSImpl_AttributeTarget::Set(aRefLabel)->Add(SALOMEDSImpl_Study::SObject(aTargetLabel));

    delete [] anEntry;
  }

  return aTargetLabel;
}

//============================================================================
/*! Function : Paste
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_SObject SALOMEDSImpl_Study::Paste(const SALOMEDSImpl_SObject& theObject,
                                               SALOMEDSImpl_Driver* theEngine)
{
  _errorCode = "";

  SALOMEDSImpl_SObject so;

  // if study is locked, then paste can't be done
  if (GetProperties()->IsLocked()) {
    _errorCode = "LockProtection";
    throw LockProtection("LockProtection");
  }

  // if there is no component name, then paste only SObjects and attributes: without component help
  SALOMEDSImpl_AttributeComment* aComponentName = NULL;
  bool aStructureOnly = !(aComponentName=(SALOMEDSImpl_AttributeComment*)_clipboard->Main().Root().FindAttribute(SALOMEDSImpl_AttributeComment::GetID()));

  // CAF document of current study usage
  if (!_doc) {
    _errorCode = "Document is null";
    return so;
  }

  SALOMEDSImpl_SComponent aComponent = theObject.GetFatherComponent();

  // fill root inserted SObject
  DF_Label aStartLabel;
  if (aStructureOnly) {
    DF_Label anObjectLabel = DF_Label::Label(_doc->Main(), theObject.GetID());
    aStartLabel = PasteLabel(theEngine, _clipboard->Main(), anObjectLabel, false);
  } else {
    DF_Label aComponentLabel = DF_Label::Label(_doc->Main(), aComponent.GetID());
    aStartLabel = PasteLabel(theEngine, _clipboard->Main(), aComponentLabel, true);
  }

  // paste all sublebels
  DF_ChildIterator anIterator(_clipboard->Main(), true);
  for(; anIterator.More(); anIterator.Next()) {
    PasteLabel(theEngine, anIterator.Value(), aStartLabel, false);
  }

  return SALOMEDSImpl_Study::SObject(aStartLabel);
}

//============================================================================
/*! Function : GetPersistentReference
 *  Purpose  : Get persistent reference of study (idem URL())
 */
//============================================================================
std::string SALOMEDSImpl_Study::GetPersistentReference()
{
  _errorCode = "";
  return URL();
}

//============================================================================
/*! Function : IsEmpty
 *  Purpose  : Detect if study is empty
 */
//============================================================================
bool SALOMEDSImpl_Study::IsEmpty()
{
  _errorCode = "";
  if (!_doc) return true;
  return _doc->IsEmpty();
}

//============================================================================
/*! Function : FindComponent
 *  Purpose  : Find a Component with ComponentDataType = aComponentName
 */
//============================================================================
SALOMEDSImpl_SComponent SALOMEDSImpl_Study::FindComponent (const std::string& aComponentName)
{
  _errorCode = "";
  bool _find = false;
  std::string name;
  SALOMEDSImpl_SComponentIterator itcomp = NewComponentIterator();
  SALOMEDSImpl_SComponent compo;

  for (; itcomp.More(); itcomp.Next()) {
    SALOMEDSImpl_SComponent SC = itcomp.Value();
    name = SC.ComponentDataType();
    if(aComponentName == name) {
      _find = true;
      return SC;
    }
  }

  if(!_find)
    {
      _errorCode = "No component was found";
      return compo;
    }
  return compo;
}

//============================================================================
/*! Function : FindComponentID
 *  Purpose  : Find a Component from it's ID
 */
//============================================================================
SALOMEDSImpl_SComponent SALOMEDSImpl_Study::FindComponentID(const std::string& aComponentID)
{
  _errorCode = "";

  // Iterate on each components defined in the study
  // Get the component ID and compare with aComponentID
  bool _find = false;
  std::string ID;
  SALOMEDSImpl_SComponent compo;

  SALOMEDSImpl_SComponentIterator itcomp = NewComponentIterator();
  for (; itcomp.More(); itcomp.Next()) {
    SALOMEDSImpl_SComponent SC = itcomp.Value();
    ID = SC.GetID();
    if(aComponentID == ID)
      {
        // ComponentID found
        _find = true;
        compo = SC;
      }
  }
  if(!_find)
    {
      _errorCode = "No component was found";
      compo = compo;
    }

  return compo;
}

//============================================================================
/*! Function : FindObject
 *  Purpose  : Find an Object with SALOMEDSImpl_Name = anObjectName
 */
//============================================================================
SALOMEDSImpl_SObject SALOMEDSImpl_Study::FindObject(const std::string& anObjectName)
{
  _errorCode = "";

  // Iterate to all components defined in the study
  // After testing the component name, iterate in all objects defined under
  // components (function _FindObject)
  bool _find = false;
  SALOMEDSImpl_SObject RefSO;

  SALOMEDSImpl_SComponentIterator it = NewComponentIterator();
  for (; it.More();it.Next()){
    if(!_find)
      {
        SALOMEDSImpl_SComponent SC = it.Value();
        if (SC.GetName() == anObjectName)
        {
            _find = true;
            RefSO = SC;

        }
        if (!_find) RefSO =  _FindObject(SC, anObjectName, _find);
      }
  }
  if(!RefSO) _errorCode = "No object was found";
  return RefSO;
}

//============================================================================
/*! Function : FindObjectID
 *  Purpose  : Find an Object with ID = anObjectID
 */
//============================================================================
SALOMEDSImpl_SObject SALOMEDSImpl_Study::FindObjectID(const std::string& anObjectID)
{
  _errorCode = "";
  SALOMEDSImpl_SObject so;

  // Convert aSO->GetID in DF_Label.
  DF_Label Lab = DF_Label::Label(_doc->Main(), anObjectID, false);

  if (Lab.IsNull()) {
    _errorCode = "No label was found by ID";
    return so;
  }
  return GetSObject(Lab);

}

//============================================================================
/*! Function : CreateObjectID
 *  Purpose  : Creates an Object with ID = anObjectID
 */
//============================================================================
SALOMEDSImpl_SObject SALOMEDSImpl_Study::CreateObjectID(const std::string& anObjectID)
{
  _errorCode = "";
  SALOMEDSImpl_SObject so;

  // Convert aSO->GetID in DF_Label.
  DF_Label Lab = DF_Label::Label(_doc->Main(), anObjectID, true);

  if (Lab.IsNull()) {
    _errorCode = "Can not create a label";
    return so;
  }
  return GetSObject(Lab);

}

//============================================================================
/*! Function : FindObjectByName
 *  Purpose  : Find Objects with SALOMEDSImpl_Name = anObjectName in a Component
 *           : with ComponentDataType = aComponentName
 */
//============================================================================
std::vector<SALOMEDSImpl_SObject> SALOMEDSImpl_Study::FindObjectByName(const std::string& anObjectName,
                                                                       const std::string& aComponentName)
{
  _errorCode = "";

  std::vector<SALOMEDSImpl_SObject> listSO;

  SALOMEDSImpl_SComponent compo = FindComponent(aComponentName) ;
  if ( !compo ) {
    _errorCode = "Can not find the component";
    return listSO;
  }

  // Iterate on each object and subobject of the component
  // If objectName is found add it to the list of SObjects
  std::string childName ;

  std::string compoId = compo.GetID();
  SALOMEDSImpl_ChildIterator it = NewChildIterator(compo);
  for ( ; it.More(); it.Next() ) {

    SALOMEDSImpl_SObject CSO = it.Value();
    if ( CSO.GetName() == anObjectName ) {
        /* add to list */
        listSO.push_back(CSO) ;
    }

    /* looks also for eventual children */
    bool found = false ;
    CSO = _FindObject( CSO, anObjectName, found ) ;
    if( found) {
      listSO.push_back(CSO) ;
    }
  }

  return listSO;
}



//============================================================================
/*! Function : FindObjectIOR
 *  Purpose  : Find an Object with IOR = anObjectIOR
 */
//============================================================================
SALOMEDSImpl_SObject SALOMEDSImpl_Study::FindObjectIOR(const std::string& anObjectIOR)
{
  _errorCode = "";

  SALOMEDSImpl_SObject aResult ;

  // searching in the datamap for optimization
  std::map<std::string, DF_Label>::iterator it=myIORLabels.find(anObjectIOR);
  if (it != myIORLabels.end()) {
    aResult = GetSObject(it->second);
    // 11 oct 2002: forbidden attributes must be checked here
    if (!aResult.GetLabel().IsAttribute(SALOMEDSImpl_AttributeIOR::GetID())) {
      myIORLabels.erase(anObjectIOR);
      aResult = SALOMEDSImpl_SObject();
    }
  }

  if(!aResult) _errorCode = "No object was found";
  return aResult;
}

//============================================================================
/*! Function : FindObjectByPath
 *  Purpose  : Find an Object by its path = thePath
 */
//============================================================================
SALOMEDSImpl_SObject SALOMEDSImpl_Study::FindObjectByPath(const std::string& thePath)
{
  _errorCode = "";

  std::string aPath(thePath), aToken;
  SALOMEDSImpl_SObject aSO;
  int aLength = aPath.size();
  bool isRelative = false;

  if(aLength == 0) {  //Empty path - return the current context
    return aSO;
  }

  if(aPath[0] != '/')  //Relative path
    isRelative = true;

  DF_ChildIterator anIterator;
  DF_Label aLabel;
  SALOMEDSImpl_AttributeName* anAttr;

  if(isRelative) {
    return aSO;
  }
  else {
    if(aPath.size() == 1 && aPath[0] == '/') {    //Root
      return GetSObject(_doc->Main());
    }
    anIterator.Init(_doc->Main(), false);
  }

  std::vector<std::string> vs = SALOMEDSImpl_Tool::splitString(aPath, '/');
  for(int i = 0, len = vs.size(); i<len; i++) {

    aToken = vs[i];
    if(aToken.size() == 0) break;

    for ( ; anIterator.More(); anIterator.Next() ) {
      aLabel = anIterator.Value();
      if((anAttr=(SALOMEDSImpl_AttributeName*)aLabel.FindAttribute(SALOMEDSImpl_AttributeName::GetID()))) {
        if(anAttr->Value() == aToken) {
          if(i == (len-1)) {  //The searched label is found (no part of the path is left)
              return GetSObject(aLabel);
          }

          anIterator.Init(aLabel, false);
          break;
        }
      }
    }

  }

  if(!aSO) _errorCode = "No object was found";
  return aSO;
}

//============================================================================
/*! Function : GetObjectPath
 *  Purpose  :
 */
//============================================================================
std::string SALOMEDSImpl_Study::GetObjectPath(const SALOMEDSImpl_SObject& theObject)
{
  _errorCode = "";

  std::string aPath("");
  if(!theObject) {
    _errorCode = "Null object";
    return aPath;
  }

  std::string aName = theObject.GetName();
  if(!aName.empty() && aName != "" ) {
    std::string aValue("/");
    aValue+=aName;
    aValue += aPath;
    aPath = aValue;
    SALOMEDSImpl_SObject aFather = theObject.GetFather();
    if(aFather) {
       aName = aFather.GetName();
       if(!aName.empty() && aName != "") {
           aValue = GetObjectPath(aFather);
          aPath = aValue + aPath;
       }
    }
  }

  return aPath;
}


//============================================================================
/*! Function : GetObjectPathByIOR
 *  Purpose  :
 */
//============================================================================
std::string SALOMEDSImpl_Study::GetObjectPathByIOR(const std::string& theIOR)
{
  _errorCode = "";

  std::string aPath;
  SALOMEDSImpl_SObject so = FindObjectIOR(theIOR);
  if(!so) {
    _errorCode = "No SObject was found by IOR";
    return aPath;
  }

  return GetObjectPath(so);
}

//============================================================================
/*! Function : NewChildIterator
 *  Purpose  : Create a ChildIterator from an SObject
 */
//============================================================================
SALOMEDSImpl_ChildIterator SALOMEDSImpl_Study::NewChildIterator(const SALOMEDSImpl_SObject& aSO)
{
  _errorCode = "";
  return SALOMEDSImpl_ChildIterator(aSO);
}


//============================================================================
/*! Function : NewComponentIterator
 *  Purpose  : Create a SComponentIterator
 */
//============================================================================
SALOMEDSImpl_SComponentIterator SALOMEDSImpl_Study::NewComponentIterator()
{
  _errorCode = "";
  return SALOMEDSImpl_SComponentIterator(_doc);
}


//============================================================================
/*! Function : NewBuilder
 *  Purpose  : Create a StudyBuilder
 */
//============================================================================
SALOMEDSImpl_StudyBuilder* SALOMEDSImpl_Study::NewBuilder()
{
  _errorCode = "";
  if(_autoFill) {
    _builder->SetOnAddSObject(_cb);
    _builder->SetOnRemoveSObject(_cb);
  }
  return _builder;

}

//============================================================================
/*! Function : Name
 *  Purpose  : get study name
 */
//============================================================================
std::string SALOMEDSImpl_Study::Name()
{
  _errorCode = "";
  return Kernel_Utils::GetBaseName( _name, false );
}

//============================================================================
/*! Function : Name
 *  Purpose  : set study name
 */
//============================================================================
void SALOMEDSImpl_Study::Name(const std::string& name)
{
  _errorCode = "";
  _name = name;
}

//============================================================================
/*! Function : IsSaved
 *  Purpose  : get if study has been saved
 */
//============================================================================
bool SALOMEDSImpl_Study::IsSaved()
{
  _errorCode = "";
  return _Saved;
}

//============================================================================
/*! Function : IsSaved
 *  Purpose  : set if study has been saved
 */
//============================================================================
void SALOMEDSImpl_Study::IsSaved(bool save)
{
  _errorCode = "";
  _Saved = save;
  if(save) _doc->SetModified(false);
}

//============================================================================
/*! Function : IsModified
 *  Purpose  : Detect if a Study has been modified since it has been saved
 */
//============================================================================
bool SALOMEDSImpl_Study::IsModified()
{
  _errorCode = "";

  // True if is modified
  if (_doc && _doc->IsModified()) return true;

  return false;
}

//============================================================================
/*! Function : URL
 *  Purpose  : get URL of the study (persistent reference of the study)
 */
//============================================================================
std::string SALOMEDSImpl_Study::URL()
{
  _errorCode = "";
  return _URL;
}

//============================================================================
/*! Function : URL
 *  Purpose  : set URL of the study (persistent reference of the study)
 */
//============================================================================
void SALOMEDSImpl_Study::URL(const std::string& url)
{
  _errorCode = "";
  _URL = url;
  _name = url;

  /*jfa: Now name of SALOMEDS study will correspond to name of SalomeApp study
  std::string tmp(_URL);

  char *aName = (char*)tmp.ToCString();
  char *adr = strtok(aName, "/");
  while (adr)
    {
      aName = adr;
      adr = strtok(NULL, "/");
    }
  Name(aName);*/
}


//============================================================================
/*! Function : _FindObject
 *  Purpose  : Find an Object with SALOMEDSImpl_Name = anObjectName
 */
//============================================================================
SALOMEDSImpl_SObject SALOMEDSImpl_Study::_FindObject(const SALOMEDSImpl_SObject& SO,
                                                     const std::string& theObjectName,
                                                     bool& _find)
{
  SALOMEDSImpl_SObject RefSO;
  if(!SO) return RefSO;

  // Iterate on each objects and subobjects of the component
  // If objectName find, stop the loop and get the object reference
  SALOMEDSImpl_AttributeName* anAttr;

  std::string soid = SO.GetID();
  DF_ChildIterator it(SO.GetLabel());
  for (; it.More(); it.Next()){
    if(!_find)
      {
        if ((anAttr=(SALOMEDSImpl_AttributeName*)it.Value().FindAttribute(SALOMEDSImpl_AttributeName::GetID())))
        {
          std::string Val(anAttr->Value());
          if (Val == theObjectName)
            {
              RefSO = GetSObject(it.Value());
              _find = true;
            }
        }
        if (!_find) RefSO = _FindObject(GetSObject(it.Value()), theObjectName, _find);
      }
  }
  return RefSO;
}

//============================================================================
/*! Function : _FindObjectIOR
 *  Purpose  : Find an Object with SALOMEDSImpl_IOR = anObjectIOR
 */
//============================================================================
SALOMEDSImpl_SObject
SALOMEDSImpl_Study::_FindObjectIOR(const SALOMEDSImpl_SObject& SO,
                                   const std::string& theObjectIOR,
                                   bool& _find)
{
  SALOMEDSImpl_SObject RefSO, aSO;
  if(!SO) return RefSO;

  // Iterate on each objects and subobjects of the component
  // If objectName find, stop the loop and get the object reference
  SALOMEDSImpl_AttributeIOR* anAttr;

  DF_ChildIterator it(SO.GetLabel());
  for (; it.More();it.Next()){
    if(!_find)
      {
        if ((anAttr=(SALOMEDSImpl_AttributeIOR*)it.Value().FindAttribute(SALOMEDSImpl_AttributeIOR::GetID())))
        {
          std::string Val(anAttr->Value());
          if (Val == theObjectIOR)
            {
              RefSO = GetSObject(it.Value());
              _find = true;
            }
        }
        aSO = GetSObject(it.Value());
        if (!_find) RefSO =  _FindObjectIOR(aSO, theObjectIOR, _find);
      }
  }
  return RefSO;
}

//============================================================================
/*! Function : _GetNoteBookAccessor
 *  Purpose  : Find an Object with SALOMEDSImpl_IOR = anObjectIOR
 */
//============================================================================
std::string SALOMEDSImpl_Study::_GetNoteBookAccessor(){
  return std::string("notebook");
}

//============================================================================
/*! Function : _GetStudyVariablesScript
 *  Purpose  :
 */
//============================================================================
std::string SALOMEDSImpl_Study::_GetStudyVariablesScript()
{
  std::string dump("");

  if(myNoteBookVars.empty())
    return dump;

  Kernel_Utils::Localizer loc;

  dump += "####################################################\n";
  dump += "##       Begin of NoteBook variables section      ##\n";
  dump += "####################################################\n";

  std::string set_method = _GetNoteBookAccessor()+".set(";
  std::string varName;
  std::string varValue;
  for(int i = 0 ; i < myNoteBookVars.size();i++ ) {
    varName = myNoteBookVars[i]->Name();
    varValue = myNoteBookVars[i]->SaveToScript();
    dump+=set_method+"\""+varName+"\", "+varValue+")\n";
  }

  dump += "####################################################\n";
  dump += "##        End of NoteBook variables section       ##\n";
  dump += "####################################################\n";

  return dump;
}

//============================================================================
/*! Function : _GetNoteBookAccess
 *  Purpose  :
 */
//============================================================================
std::string SALOMEDSImpl_Study::_GetNoteBookAccess()
{
  std::string notebook = "import salome_notebook\n";
  notebook += _GetNoteBookAccessor() + " = salome_notebook.NoteBook()" ;
  return notebook;
}

bool SALOMEDSImpl_Study::IsLocked()
{
  _errorCode = "";
  return GetProperties()->IsLocked();
}

void SALOMEDSImpl_Study::UpdateIORLabelMap(const std::string& anIOR,const std::string& anEntry)
{
  _errorCode = "";
  DF_Label aLabel = DF_Label::Label(_doc->Main(), anEntry, true);
  std::map<std::string, DF_Label>::iterator it=myIORLabels.find(anIOR);
  if (it != myIORLabels.end()) myIORLabels.erase(it);
  myIORLabels[anIOR] = aLabel;
}

void SALOMEDSImpl_Study::DeleteIORLabelMapItem(const std::string& anIOR)
{
  std::map<std::string, DF_Label>::iterator it=myIORLabels.find(anIOR);
  if (it != myIORLabels.end())
    {
      //remove the ior entry and decref the genericobj (if it's one)
      myIORLabels.erase(it);
    }
}

SALOMEDSImpl_Study* SALOMEDSImpl_Study::GetStudyImpl(const DF_Label& theLabel)
{
  SALOMEDSImpl_StudyHandle* Att;
  if ((Att=(SALOMEDSImpl_StudyHandle*)theLabel.Root().FindAttribute(SALOMEDSImpl_StudyHandle::GetID()))) {
    return Att->Get();
  }
  return NULL;
}

SALOMEDSImpl_SObject SALOMEDSImpl_Study::SObject(const DF_Label& theLabel)
{
  return GetStudyImpl(theLabel)->GetSObject(theLabel);
}

SALOMEDSImpl_SComponent SALOMEDSImpl_Study::SComponent(const DF_Label& theLabel)
{
  return GetStudyImpl(theLabel)->GetSComponent(theLabel);
}


void SALOMEDSImpl_Study::IORUpdated(const SALOMEDSImpl_AttributeIOR* theAttribute)
{
  std::string aString = theAttribute->Label().Entry();
  GetStudyImpl(theAttribute->Label())->UpdateIORLabelMap(theAttribute->Value(), aString);
}

std::vector<SALOMEDSImpl_SObject> SALOMEDSImpl_Study::FindDependances(const SALOMEDSImpl_SObject& anObject)
{
  _errorCode = "";
  std::vector<SALOMEDSImpl_SObject> aSeq;

  SALOMEDSImpl_AttributeTarget* aTarget;
  if ((aTarget=(SALOMEDSImpl_AttributeTarget*)anObject.GetLabel().FindAttribute(SALOMEDSImpl_AttributeTarget::GetID()))) {
    return aTarget->Get();
  }

  return aSeq;
}


SALOMEDSImpl_AttributeStudyProperties* SALOMEDSImpl_Study::GetProperties()
{
  _errorCode = "";
  return SALOMEDSImpl_AttributeStudyProperties::Set(_doc->Main());
}

std::string SALOMEDSImpl_Study::GetLastModificationDate()
{
  _errorCode = "";
  SALOMEDSImpl_AttributeStudyProperties* aProp = GetProperties();

  std::vector<std::string> aNames;
  std::vector<int> aMinutes, aHours, aDays, aMonths, aYears;
  aProp->GetModifications(aNames, aMinutes, aHours, aDays, aMonths, aYears);

  int aLastIndex = aNames.size()-1;
  char aResult[20];
  sprintf(aResult, "%2.2d/%2.2d/%4.4d %2.2d:%2.2d",
          (int)(aDays[aLastIndex]),(int)(aMonths[aLastIndex]), (int)(aYears[aLastIndex]),
          (int)(aHours[aLastIndex]), (int)(aMinutes[aLastIndex]));
  std::string aResStr (aResult);
  return aResStr;
}

std::vector<std::string> SALOMEDSImpl_Study::GetModificationsDate()
{
  _errorCode = "";
  SALOMEDSImpl_AttributeStudyProperties* aProp = GetProperties();

  std::vector<std::string> aNames;
  std::vector<int> aMinutes, aHours, aDays, aMonths, aYears;
  aProp->GetModifications(aNames, aMinutes, aHours, aDays, aMonths, aYears);

  int anIndex, aLength = aNames.size();
  std::vector<std::string> aDates;

  for (anIndex = 1; anIndex < aLength; anIndex++) {
    char aDate[20];
    sprintf(aDate, "%2.2d/%2.2d/%4.4d %2.2d:%2.2d",
            (int)(aDays[anIndex]), (int)(aMonths[anIndex]), (int)(aYears[anIndex]),
            (int)(aHours[anIndex]), (int)(aMinutes[anIndex]));
    aDates.push_back(aDate);
  }
  return aDates;
}



//============================================================================
/*! Function : GetUseCaseBuilder
 *  Purpose  : Returns a UseCase builder
 */
//============================================================================
SALOMEDSImpl_UseCaseBuilder* SALOMEDSImpl_Study::GetUseCaseBuilder()
{
  _errorCode = "";
  return _useCaseBuilder;
}

//============================================================================
/*! Function : GetSComponent
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_SComponent SALOMEDSImpl_Study::GetSComponent(const std::string& theEntry)
{
  SALOMEDSImpl_SComponent aSCO;
  if(_mapOfSCO.find(theEntry) != _mapOfSCO.end())
    aSCO = _mapOfSCO[theEntry];
  else {
    DF_Label aLabel = DF_Label::Label(_doc->Main(), theEntry);
    aSCO = SALOMEDSImpl_SComponent(aLabel);
    _mapOfSCO[theEntry] = aSCO;
  }

  return aSCO;
}

//============================================================================
/*! Function : GetSComponent
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_SComponent SALOMEDSImpl_Study::GetSComponent(const DF_Label& theLabel)
{
  return SALOMEDSImpl_SComponent(theLabel);
}

//============================================================================
/*! Function : GetSObject
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_SObject SALOMEDSImpl_Study::GetSObject(const std::string& theEntry)
{
  SALOMEDSImpl_SObject aSO;
  std::map<std::string, SALOMEDSImpl_SObject>::iterator it=_mapOfSO.find(theEntry);
  if(it != _mapOfSO.end())
    aSO = it->second;
  else {
    DF_Label aLabel = DF_Label::Label(_doc->Main(), theEntry);
    aSO = SALOMEDSImpl_SObject(aLabel);
    _mapOfSO[theEntry] = aSO;
  }

  return aSO;
}

//============================================================================
/*! Function : GetSObject
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_SObject SALOMEDSImpl_Study::GetSObject(const DF_Label& theLabel)
{
  return SALOMEDSImpl_SObject(theLabel);
}

//============================================================================
/*! Function : GetAttribute
 *  Purpose  :
 */
//============================================================================
DF_Attribute* SALOMEDSImpl_Study::GetAttribute(const std::string& theEntry,
                                               const std::string& theType)
{
  SALOMEDSImpl_SObject aSO = GetSObject(theEntry);
  DF_Attribute* anAttr;
  aSO.FindAttribute(anAttr, theType);
  return anAttr;
}

//! number of spaces for indentation in Python dump files (to replace \t symbols)
static const int indent_size = 2;

static std::string replace_tabs( const std::string& in )
{
  std::string out = in;
#ifdef WITHOUT_TABS
  size_t pos = out.find( '\t' );
  while ( pos != std::string::npos ) {
    out.replace( pos, 1, indent_size, ' ' );
    pos = out.find( '\t' );
  }
#endif
  return out;
}

static std::string GetComponentHeader(const char* theComponentName)
{
  std::stringstream txt;
  txt << "###" << std::endl;
  txt << "### " << theComponentName << " component" << std::endl;
  txt << "###" << std::endl;
  return txt.str();
}

//============================================================================
/*! Function : DumpStudy
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::DumpStudy(const std::string& thePath,
                                   const std::string& theBaseName,
                                   bool isPublished,
                                   bool isMultiFile,
                                   SALOMEDSImpl_DriverFactory* theFactory)
{
  _errorCode = "";

  if(theFactory == NULL) {
    _errorCode = "Null factory for creation of Engines";
    return false;
  }

  std::vector<std::string> aSeq;
  std::string aCompType, aFactoryType;

  //Build a list of all components in the Study
  SALOMEDSImpl_SComponentIterator itcomponent = NewComponentIterator();

  for (; itcomponent.More(); itcomponent.Next()) {
    SALOMEDSImpl_SComponent sco = itcomponent.Value();
    aCompType = sco.ComponentDataType();
   if (aCompType == "GEOM")
      aSeq.insert(aSeq.begin(), aCompType);
    else
      aSeq.push_back(aCompType);
  }
  // re-arrange modules in the sequence, if specific order is given via SALOME_MODULES_ORDER environment variable.
  if ( getenv("SALOME_MODULES_ORDER") != 0 ) {
    std::string order = getenv("SALOME_MODULES_ORDER");
    std::vector<std::string> mlist;
    while ( !order.empty() ) {
      size_t idx = order.find( ":" );
      std::string m = order.substr(0, idx);
      order = order.substr( ( idx == std::string::npos ) ? order.size() : idx+1 );
      if ( m.empty() || std::find( mlist.begin(), mlist.end(), m ) != mlist.end() ) continue;
      mlist.push_back( m );
    }

    for ( std::vector<std::string>::reverse_iterator mit = mlist.rbegin(); mit != mlist.rend(); ++mit ) {
      std::vector<std::string>::iterator it = std::find( aSeq.begin(), aSeq.end(), *mit );
      if ( it != aSeq.end() ) {
        aSeq.erase( it );
        aSeq.insert( aSeq.begin(), *mit );
      }
    }
  }

#ifdef WIN32
  std::string aFileName =
    thePath + std::string("\\") + theBaseName + std::string(".py");
#else
  std::string aFileName =
    thePath + std::string("/")  + theBaseName + std::string(".py");
#endif

  //Create a file that will contain a main Study script
  std::fstream fp;
#if defined(WIN32) && defined(UNICODE)
  std::wstring aConverterFN = Kernel_Utils::utf8_decode_s(aFileName);
  fp.open(aConverterFN.c_str(), std::ios::out);
#else
  fp.open(aFileName.c_str(), std::ios::out);
#endif

#ifdef WIN32
  bool isOpened = fp.is_open();
#else
  bool isOpened = fp.rdbuf()->is_open();
#endif

  if(!isOpened) {
    _errorCode = std::string("Can't create a file ")+aFileName;
    return false;
  }

  std::stringstream sfp;

  std::string aBatchModeScript = "salome";

  //Output to the main Study script required Python modules import,
  //set sys.path and add a creation of the study.

  // dump header
  sfp << GetDumpStudyComment() << std::endl;

  // global imports
  sfp << "import sys" << std::endl;
  sfp << "import " << aBatchModeScript << std::endl << std::endl;

  // initialization function
  sfp << aBatchModeScript << ".salome_init()" << std::endl;

  // notebook initialization
  sfp << _GetNoteBookAccess() << std::endl;

  // extend sys.path with the directory where the script is being dumped to
  sfp << "sys.path.insert(0, r\'" << thePath << "\')" << std::endl << std::endl;

  // dump NoteBook variables
  sfp << _GetStudyVariablesScript();

  // dump visual parameters if necessary
  bool isDumpVisuals = SALOMEDSImpl_IParameters::isDumpPython(this);
  int lastSavePoint = -1;
  if(isDumpVisuals) {
    lastSavePoint = SALOMEDSImpl_IParameters::getLastSavePoint(this);
    if(lastSavePoint > 0) {
      sfp << SALOMEDSImpl_IParameters::getStudyScript(this, lastSavePoint) << std::endl << std::endl;
    }
  }

  std::vector<std::string> aSeqOfFileNames;

  // dump all components and create the components specific scripts
  bool isOk = true;
  int aLength = aSeq.size();
  for(int i = 1; i <= aLength; i++) {

    aCompType = aSeq[i-1];
    SALOMEDSImpl_SComponent sco = FindComponent(aCompType);
    SALOMEDSImpl_Driver* aDriver = NULL;
    // if there is an associated Engine call its method for saving
    std::string IOREngine;
    try {
      if (!sco.ComponentIOR(IOREngine)) {
        if (!aCompType.empty()) {

          aDriver = theFactory->GetDriverByType(aCompType);

          if (aDriver != NULL) {
            SALOMEDSImpl_StudyBuilder* SB = NewBuilder();
            if(!SB->LoadWith(sco, aDriver)) {
              _errorCode = SB->GetErrorCode();
              return false;
            }
          }
          else continue;
        }
      }
      else {
        aDriver = theFactory->GetDriverByIOR(IOREngine);
      }
    } catch(...) {
      _errorCode = "Can not restore information to dump it";
      return false;
    }

    if(aDriver == NULL) continue;

    bool isValidScript;
    long aStreamLength  = 0;
    SALOMEDSImpl_TMPFile* aStream = aDriver->DumpPython(isPublished, isMultiFile, isValidScript, aStreamLength);
    if ( !isValidScript )
      isOk = false;

    std::stringstream sfp2;

    //Output the Python script generated by the component in the newly created file.
    if ( isMultiFile )
      sfp2 << GetDumpStudyComment( aCompType.c_str() ) << std::endl;
    else
      sfp2 << GetComponentHeader( aCompType.c_str() ) << std::endl;
    sfp2 << aStream->Data();

    if ( isMultiFile ) {
      //Create a file that will contain the component specific script
      std::fstream fp2;
#ifdef WIN32
      aFileName=thePath+std::string("\\");
#else
      aFileName=thePath+std::string("/");
#endif
      std::string aScriptName;
      aScriptName += theBaseName;
      aScriptName += "_";
      aScriptName += aCompType;

      aFileName += aScriptName+ std::string(".py");
      aSeqOfFileNames.push_back(aFileName);

#if defined(WIN32) && defined(UNICODE)
      std::wstring aConverterFN2 = Kernel_Utils::utf8_decode_s(aFileName);
      fp2.open(aConverterFN2.c_str(), std::ios::out);
#else
      fp2.open(aFileName.c_str(), std::ios::out);
#endif

#ifdef WIN32
      isOpened = fp2.is_open();
#else
      isOpened = fp2.rdbuf()->is_open();
#endif

      if(!isOpened) {
        _errorCode = std::string("Can't create a file ")+aFileName;
        SALOMEDSImpl_Tool::RemoveTemporaryFiles(thePath, aSeqOfFileNames, false);
        return false;
      }

      // replace '\t' symbols
      fp2 << replace_tabs( sfp2.str() );

      fp2.close();

      //Add to the main script a call to RebuildData of the generated by the component the Python script
      sfp << "import " << aScriptName << std::endl;
      sfp << aScriptName << ".RebuildData()" << std::endl;
    }
    else
      sfp << sfp2.str();

    if(aStream) delete aStream;
  }

  sfp << std::endl;
  sfp << "if salome.sg.hasDesktop():" << std::endl;
  sfp << "\tsalome.sg.updateObjBrowser()" << std::endl;

  if(isDumpVisuals) { //Output the call to Session's method restoreVisualState
    sfp << "\tiparameters.getSession().restoreVisualState(1)" << std::endl;
  }

  // replace '\t' symbols
  fp << replace_tabs( sfp.str() );

  fp.close();

  return isOk;
}

//=======================================================================
//function : GetDumpStudyComment
//purpose  : return a header comment for a DumpStudy script
//=======================================================================

std::string SALOMEDSImpl_Study::GetDumpStudyComment(const char* theComponentName)
{
  std::stringstream txt;
  txt << "#!/usr/bin/env python" << std::endl << std::endl;
  txt << "###" << std::endl;
  txt << "### This file is generated automatically by SALOME v"
      << KERNEL_VERSION_STR
      << " with dump python functionality";
  if ( theComponentName )
    txt << " (" << theComponentName << " component)";
  txt << std::endl;
  txt << "###" << std::endl;
  return txt.str();
}

void dumpSO(const SALOMEDSImpl_SObject& theSO,
            std::fstream& fp,
            const std::string& Tab,
            SALOMEDSImpl_Study* theStudy);

//============================================================================
/*! Function : dump
 *  Purpose  :
 */
//============================================================================
void SALOMEDSImpl_Study::dump(const std::string& theFileName)
{
  //Create a file that will contain a main Study script
  std::fstream fp;
  fp.open(theFileName.c_str(), std::ios::out);

#ifdef WIN32
  bool isOpened = fp.is_open();
#else
  bool isOpened = fp.rdbuf()->is_open();
#endif

  if(!isOpened) {
    _errorCode = std::string("Can't create a file ")+theFileName;
    std::cout << "### SALOMEDSImpl_Study::dump Error: " << _errorCode << std::endl;
    return;
  }

  SALOMEDSImpl_SObject aSO = FindObjectID("0:1");
  fp << "0:1" << std::endl;
  SALOMEDSImpl_ChildIterator Itr = NewChildIterator(aSO);
  std::string aTab("   ");
  for(; Itr.More(); Itr.Next()) {
    dumpSO(Itr.Value(), fp, aTab, this);
  }

  fp.close();
}


void dumpSO(const SALOMEDSImpl_SObject& theSO,
            std::fstream& fp,
            const std::string& Tab,
            SALOMEDSImpl_Study* theStudy)
{
  std::string aTab(Tab), anID(theSO.GetID());
  fp << aTab << anID << std::endl;
  std::vector<DF_Attribute*> attribs = theSO.GetLabel().GetAttributes();
  for(int i = 0; i<attribs.size(); i++) {
    SALOMEDSImpl_GenericAttribute* anAttr = dynamic_cast<SALOMEDSImpl_GenericAttribute*>(attribs[i]);

    if(!anAttr) {
      continue;
    }

    std::string aType = anAttr->GetClassType();
    fp << Tab << "  -- " << aType;

    if(aType == std::string("AttributeReal")) {
      fp << " : " << dynamic_cast<SALOMEDSImpl_AttributeReal*>(anAttr)->Value();
    }
    else if(aType == std::string("AttributeInteger")) {
      fp << " : " << dynamic_cast<SALOMEDSImpl_AttributeInteger*>(anAttr)->Value();
    }
    else if(aType ==  std::string("AttributeName")) {
      fp << " : " << dynamic_cast<SALOMEDSImpl_AttributeName*>(anAttr)->Value();
    }
    else if(aType == std::string("AttributeComment")) {
      fp << " : " << dynamic_cast<SALOMEDSImpl_AttributeComment*>(anAttr)->Value();
    }
    else if(aType == std::string("AttributeReference")) {
      fp << " : " << dynamic_cast<SALOMEDSImpl_AttributeReference*>(anAttr)->Save();
    }
    fp << std::endl;
  }

  SALOMEDSImpl_ChildIterator Itr = theStudy->NewChildIterator(theSO);
  std::string aNewTab("   ");
  aNewTab+=aTab;
  for(; Itr.More(); Itr.Next()) {
    dumpSO(Itr.Value(), fp, aNewTab, theStudy);
  }

  return;
}

void SALOMEDSImpl_Study::Modify()
{
  _errorCode = "";
  _doc->SetModified(true);
}

//============================================================================
/*! Function :
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_AttributeParameter* SALOMEDSImpl_Study::GetCommonParameters(const char* theID, int theSavePoint)
{
  if (theSavePoint < -1) return NULL;
  SALOMEDSImpl_StudyBuilder* builder = NewBuilder();
  SALOMEDSImpl_SObject so = FindComponent((char*)theID);
  if (!so) so = builder->NewComponent((char*)theID);
  SALOMEDSImpl_AttributeParameter* attParam = NULL;

  if (theSavePoint == -1) {
    int ctag = 1;
    DF_Label savePointLabel = so.GetLabel().FindChild( ctag, /*create=*/0 );
    DF_Label prevPointLabel;
    while ( !savePointLabel.IsNull() ) {
      ctag++;
      prevPointLabel = savePointLabel;
      savePointLabel = so.GetLabel().FindChild( ctag, /*create=*/0 );
    }
    if ( !prevPointLabel.IsNull() )
      so = GetSObject( prevPointLabel );
  }
  if (theSavePoint > 0) { // Try to find SObject that contains attribute parameter ...
    DF_Label savePointLabel = so.GetLabel().FindChild( theSavePoint, /*create=*/0 );
    if ( !savePointLabel.IsNull() )
      so = GetSObject( savePointLabel );
    else // ... if it does not exist - create a new one
      so = builder->NewObjectToTag( so, theSavePoint );
  }

  DF_Attribute* A;
  if (so) {
    builder->FindAttribute(so, A, "AttributeParameter");
    if ( !A ) { // first call of GetCommonParameters on "Interface Applicative" component
      A = builder->FindOrCreateAttribute(so, "AttributeParameter");
    }
    attParam = dynamic_cast<SALOMEDSImpl_AttributeParameter*>( A );
  }
  return attParam;
}

//============================================================================
/*! Function :
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_AttributeParameter* SALOMEDSImpl_Study::GetModuleParameters(const char* theID,
                                                                         const char* theModuleName,
                                                                         int theSavePoint)
{
  if(theSavePoint < -1) return NULL;
  SALOMEDSImpl_AttributeParameter* main_ap = GetCommonParameters(theID, theSavePoint);
  SALOMEDSImpl_SObject main_so = main_ap->GetSObject();
  SALOMEDSImpl_AttributeParameter* par = NULL;

  SALOMEDSImpl_ChildIterator it = NewChildIterator(main_so);
  std::string moduleName(theModuleName);
  for(; it.More(); it.Next()) {
    SALOMEDSImpl_SObject so(it.Value());
    if((par=(SALOMEDSImpl_AttributeParameter*)so.GetLabel().FindAttribute(SALOMEDSImpl_AttributeParameter::GetID()))) {
      if(!par->IsSet("AP_MODULE_NAME", (Parameter_Types)3)) continue; //3 -> PT_STRING
      if(par->GetString("AP_MODULE_NAME") == moduleName) return par;
    }
  }

  SALOMEDSImpl_StudyBuilder* builder = NewBuilder();
  SALOMEDSImpl_SObject so = builder->NewObject(main_so);
  par  = dynamic_cast<SALOMEDSImpl_AttributeParameter*>(builder->FindOrCreateAttribute(so, "AttributeParameter"));
  par->SetString("AP_MODULE_NAME", moduleName);
  return par;
}

//============================================================================
/*! Function : SetStudyLock
 *  Purpose  :
 */
//============================================================================
void SALOMEDSImpl_Study::SetStudyLock(const char* theLockerID)
{
  _lockers.push_back(theLockerID);
}

//============================================================================
/*! Function : IsStudyLocked
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::IsStudyLocked()
{
  return (_lockers.size() > 0);
}

//============================================================================
/*! Function : UnLockStudy
 *  Purpose  :
 */
//============================================================================
void SALOMEDSImpl_Study::UnLockStudy(const char* theLockerID)
{
  std::vector<std::string>::iterator vsI = _lockers.begin();
  int length = _lockers.size();
  bool isFound = false;
  std::string id(theLockerID);
  for(int i = 0; i<length; i++, vsI++) {
    if(id == _lockers[i]) {
      isFound = true;;
      break;
    }
  }
  if(isFound) _lockers.erase(vsI);
}

//============================================================================
/*! Function : GetLockerID
 *  Purpose  :
 */
//============================================================================
std::vector<std::string> SALOMEDSImpl_Study::GetLockerID()
{
  return _lockers;
}

//============================================================================
/*! Function : SetVariable
 *  Purpose  :
 */
//============================================================================
void SALOMEDSImpl_Study::SetVariable(const std::string& theVarName,
                                     const double theValue,
                                     const SALOMEDSImpl_GenericVariable::VariableTypes theType)
{
  bool modified = false;
  SALOMEDSImpl_GenericVariable* aGVar = GetVariable(theVarName);

  if( aGVar == NULL ) {

    SALOMEDSImpl_ScalarVariable* aSVar = new SALOMEDSImpl_ScalarVariable(theType, theVarName);

    aSVar->setValue(theValue);
    myNoteBookVars.push_back(aSVar);
    modified = true;
  }
  else {
    if(SALOMEDSImpl_ScalarVariable* aSVar = dynamic_cast<SALOMEDSImpl_ScalarVariable*>(aGVar)) {
      modified = aSVar->setValue(theValue) || modified;
      modified = aSVar->setType(theType) || modified;
    }
  }
  if(modified)
    Modify();
}

//============================================================================
/*! Function : SetStringVariable
 *  Purpose  :
 */
//============================================================================
void SALOMEDSImpl_Study::SetStringVariable(const std::string& theVarName,
                                           const std::string& theValue,
                                           const SALOMEDSImpl_GenericVariable::VariableTypes theType)
{
  bool modified = false;
  SALOMEDSImpl_GenericVariable* aGVar = GetVariable(theVarName);

  if( aGVar == NULL ) {

    SALOMEDSImpl_ScalarVariable* aSVar = new SALOMEDSImpl_ScalarVariable(theType, theVarName);

    aSVar->setStringValue(theValue);
    myNoteBookVars.push_back(aSVar);
    modified = true;
  }
  else {
    if(SALOMEDSImpl_ScalarVariable* aSVar = dynamic_cast<SALOMEDSImpl_ScalarVariable*>(aGVar)) {
      modified = aSVar->setStringValue(theValue) || modified;
      modified = aSVar->setType(theType) || modified;
    }
  }
  if(modified)
    Modify();
}

//============================================================================
/*! Function : SetStringVariableAsDouble
 *  Purpose  :
 */
//============================================================================
void SALOMEDSImpl_Study::SetStringVariableAsDouble(const std::string& theVarName,
                                                   const double theValue,
                                                   const SALOMEDSImpl_GenericVariable::VariableTypes theType)
{
  SALOMEDSImpl_GenericVariable* aGVar = GetVariable(theVarName);
  if(SALOMEDSImpl_ScalarVariable* aSVar = dynamic_cast<SALOMEDSImpl_ScalarVariable*>(aGVar))
    aSVar->setValue(theValue);
}

//============================================================================
/*! Function : GetReal
 *  Purpose  :
 */
//============================================================================
double SALOMEDSImpl_Study::GetVariableValue(const std::string& theVarName)
{
  SALOMEDSImpl_GenericVariable* aGVar = GetVariable(theVarName);

  if(aGVar != NULL )
    if(SALOMEDSImpl_ScalarVariable* aSVar = dynamic_cast<SALOMEDSImpl_ScalarVariable*>(aGVar))
      return aSVar->getValue();

  return 0;
}

//============================================================================
/*! Function : GetString
 *  Purpose  :
 */
//============================================================================
std::string SALOMEDSImpl_Study::GetStringVariableValue(const std::string& theVarName)
{
  SALOMEDSImpl_GenericVariable* aGVar = GetVariable(theVarName);

  if(aGVar != NULL )
    if(SALOMEDSImpl_ScalarVariable* aSVar = dynamic_cast<SALOMEDSImpl_ScalarVariable*>(aGVar))
      return aSVar->getStringValue();

  return 0;
}

//============================================================================
/*! Function : IsTypeOf
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::IsTypeOf(const std::string& theVarName,
                                  SALOMEDSImpl_GenericVariable::
                                  VariableTypes theType) const
{
  SALOMEDSImpl_GenericVariable* aGVar = GetVariable(theVarName);

  if(aGVar != NULL )
    return aGVar->Type() == theType;

  return false;
}

//============================================================================
/*! Function : IsVariable
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::IsVariable(const std::string& theVarName) const
{
  SALOMEDSImpl_GenericVariable* aGVar = GetVariable(theVarName);
  return (aGVar != NULL);
}

//============================================================================
/*! Function : GetVariableNames
 *  Purpose  :
 */
//============================================================================
std::vector<std::string> SALOMEDSImpl_Study::GetVariableNames() const
{
  std::vector<std::string> aResult;

  for(int i = 0; i < myNoteBookVars.size(); i++)
    aResult.push_back(myNoteBookVars[i]->Name());

  return aResult;
}

//============================================================================
/*! Function : AddVariable
 *  Purpose  :
 */
//============================================================================
void SALOMEDSImpl_Study::AddVariable(SALOMEDSImpl_GenericVariable* theVariable)
{
  myNoteBookVars.push_back(theVariable);
}

//============================================================================
/*! Function : AddVariable
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_GenericVariable* SALOMEDSImpl_Study::GetVariable(const std::string& theName) const
{
  SALOMEDSImpl_GenericVariable* aResult = NULL;
  for(int i = 0; i < myNoteBookVars.size();i++) {
    if(theName.compare(myNoteBookVars[i]->Name()) == 0) {
      aResult = myNoteBookVars[i];
      break;
    }
  }
  return aResult;
}

//============================================================================
/*! Function : RemoveVariable
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::RemoveVariable(const std::string& theVarName)
{
  SALOMEDSImpl_GenericVariable* aVariable = GetVariable( theVarName );
  if( !aVariable )
    return false;

  std::string aValue = aVariable->SaveToScript();
  ReplaceVariableAttribute( theVarName, aValue );

  std::vector<SALOMEDSImpl_GenericVariable*>::iterator it = myNoteBookVars.begin(), itEnd = myNoteBookVars.end();
  for( ; it != itEnd; it++ )
  {
    SALOMEDSImpl_GenericVariable* aVariableRef = *it;
    if( aVariableRef && theVarName.compare( aVariableRef->Name() ) == 0 )
    {
      myNoteBookVars.erase( it );
      Modify();
      break;
    }
  }

  return true;
}

//============================================================================
/*! Function : RenameVariable
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::RenameVariable(const std::string& theVarName, const std::string& theNewVarName)
{
  SALOMEDSImpl_GenericVariable* aVariable = GetVariable( theVarName );
  if( !aVariable )
    return false;

  ReplaceVariableAttribute( theVarName, theNewVarName );

  std::vector<SALOMEDSImpl_GenericVariable*>::iterator it = myNoteBookVars.begin(), itEnd = myNoteBookVars.end();
  for( ; it != itEnd; it++ )
  {
    SALOMEDSImpl_GenericVariable* aVariableRef = *it;
    if( aVariableRef && theVarName.compare( aVariableRef->Name() ) == 0 )
    {
      aVariableRef->setName( theNewVarName );
      Modify();
      break;
    }
  }

  return true;
}

//============================================================================
/*! Function : IsVariableUsed
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::IsVariableUsed(const std::string& theVarName)
{
  return FindVariableAttribute( theVarName );
}

//============================================================================
/*! Function : FindVariableAttribute
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::FindVariableAttribute(SALOMEDSImpl_StudyBuilder* theStudyBuilder,
                                               SALOMEDSImpl_SObject theSObject,
                                               const std::string& theName)
{
  SALOMEDSImpl_ChildIterator anIter = NewChildIterator( theSObject );
  for( ; anIter.More(); anIter.Next() )
    if( FindVariableAttribute( theStudyBuilder, anIter.Value(), theName ) )
      return true;

  DF_Attribute* anAttr;
  if( theStudyBuilder->FindAttribute( theSObject, anAttr, "AttributeString" ) )
  {
    if( SALOMEDSImpl_AttributeString* aStringAttr = ( SALOMEDSImpl_AttributeString* )anAttr )
    {
      std::string aString = aStringAttr->Value();

      std::vector< std::vector<std::string> > aSections = ParseVariables( aString );
      for( int i = 0, n = aSections.size(); i < n; i++ )
      {
        std::vector<std::string> aVector = aSections[i];
        for( int j = 0, m = aVector.size(); j < m; j++ )
        {
          std::string aStr = aVector[j];
          if( aStr.compare( theName ) == 0 )
            return true;
        }
      }
    }
  }
  return false;
}

//============================================================================
/*! Function : FindVariableAttribute
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::FindVariableAttribute(const std::string& theName)
{
  SALOMEDSImpl_StudyBuilder* aStudyBuilder = NewBuilder();
  SALOMEDSImpl_SComponentIterator aCompIter = NewComponentIterator();
  for( ; aCompIter.More(); aCompIter.Next() )
  {
    SALOMEDSImpl_SObject aComp = aCompIter.Value();
    if( FindVariableAttribute( aStudyBuilder, aComp, theName ) )
      return true;
  }
  return false;
}

//============================================================================
/*! Function : ReplaceVariableAttribute
 *  Purpose  :
 */
//============================================================================
void SALOMEDSImpl_Study::ReplaceVariableAttribute(SALOMEDSImpl_StudyBuilder* theStudyBuilder,
                                                  SALOMEDSImpl_SObject theSObject,
                                                  const std::string& theSource,
                                                  const std::string& theDest)
{
  SALOMEDSImpl_ChildIterator anIter = NewChildIterator( theSObject );
  for( ; anIter.More(); anIter.Next() )
    ReplaceVariableAttribute( theStudyBuilder, anIter.Value(), theSource, theDest );

  DF_Attribute* anAttr;
  if( theStudyBuilder->FindAttribute( theSObject, anAttr, "AttributeString" ) )
  {
    if( SALOMEDSImpl_AttributeString* aStringAttr = ( SALOMEDSImpl_AttributeString* )anAttr )
    {
      bool isChanged = false;
      std::string aNewString, aCurrentString = aStringAttr->Value();

      std::vector< std::vector<std::string> > aSections = ParseVariables( aCurrentString );
      for( int i = 0, n = aSections.size(); i < n; i++ )
      {
        std::vector<std::string> aVector = aSections[i];
        for( int j = 0, m = aVector.size(); j < m; j++ )
        {
          std::string aStr = aVector[j];
          if( aStr.compare( theSource ) == 0 )
          {
            isChanged = true;
            aStr = theDest;
          }

          aNewString.append( aStr );
          if( j != m - 1 )
            aNewString.append( ":" );
        }
        if( i != n - 1 )
          aNewString.append( "|" );
      }

      if( isChanged )
        aStringAttr->SetValue( aNewString );
    }
  }
}

//============================================================================
/*! Function : ReplaceVariableAttribute
 *  Purpose  :
 */
//============================================================================
void SALOMEDSImpl_Study::ReplaceVariableAttribute(const std::string& theSource, const std::string& theDest)
{
  SALOMEDSImpl_StudyBuilder* aStudyBuilder = NewBuilder();
  SALOMEDSImpl_SComponentIterator aCompIter = NewComponentIterator();
  for( ; aCompIter.More(); aCompIter.Next() )
  {
    SALOMEDSImpl_SObject aComp = aCompIter.Value();
    ReplaceVariableAttribute( aStudyBuilder, aComp, theSource, theDest );
  }
}

//============================================================================
/*! Function : ParseVariables
 *  Purpose  :
 */
//============================================================================
std::vector< std::vector< std::string > > SALOMEDSImpl_Study::ParseVariables(const std::string& theVariables) const
{
  return SALOMEDSImpl_Tool::splitStringWithEmpty( theVariables, OPERATION_SEPARATOR, VARIABLE_SEPARATOR );
}

//============================================================================
/*! Function : EnableUseCaseAutoFilling
 *  Purpose  :
 */
//============================================================================
void SALOMEDSImpl_Study::EnableUseCaseAutoFilling(bool isEnabled)
{
  _errorCode = ""; _autoFill = isEnabled;
  if(isEnabled) {
    _builder->SetOnAddSObject(_cb);
    _builder->SetOnRemoveSObject(_cb);
  }
  else {
    _builder->SetOnAddSObject(NULL);
    _builder->SetOnRemoveSObject(NULL);
  }
}

//============================================================================
/*! Function : GetIORs
 *  Purpose  :
 */
//============================================================================
std::vector<std::string> SALOMEDSImpl_Study::GetIORs()
{
  std::vector<std::string> anIORs;
  std::map<std::string, DF_Label>::const_iterator MI;
  for(MI = myIORLabels.begin(); MI!=myIORLabels.end(); MI++)
    anIORs.push_back(MI->first);

  return anIORs;
}

//============================================================================
/*! Function : addSO_Notification
 *  Purpose  : This function tells all the observers that a SO has been added
 */
//============================================================================
bool SALOMEDSImpl_Study::addSO_Notification (const SALOMEDSImpl_SObject& theSObject)
{
  if(_notifier)
    return _notifier->addSO_Notification(theSObject);
  else
    return false;
}

//============================================================================
/*! Function : removeSO_Notification
 *  Purpose  : This function tells all the observers that a SO has been removed
 */
//============================================================================
bool SALOMEDSImpl_Study::removeSO_Notification (const SALOMEDSImpl_SObject& theSObject)
{
  if(_notifier)
    return _notifier->removeSO_Notification(theSObject);
  else
    return false;
}

//============================================================================
/*! Function : modifySO_Notification
 *  Purpose  : This function tells all the observers that a SO has been modified and
               pass the mofification reason
 */
//============================================================================
bool SALOMEDSImpl_Study::modifySO_Notification (const SALOMEDSImpl_SObject& theSObject, int reason)
{
  if(_notifier)
    return _notifier->modifySO_Notification(theSObject, reason);
  else
    return false;
}

//============================================================================
/*! Function : setNotifier
 *  Purpose  : register a notifier
 */
//============================================================================
void SALOMEDSImpl_Study::setNotifier(SALOMEDSImpl_AbstractCallback* notifier)
{
  _notifier=notifier;
}

static SALOMEDSImpl_AbstractCallback* & getGenObjRegister( DF_Document* doc )
{
  static std::vector< SALOMEDSImpl_AbstractCallback* > _genObjRegVec;
  if ( doc->GetDocumentID() >= (int)_genObjRegVec.size() )
    _genObjRegVec.resize( doc->GetDocumentID() + 1, 0 );
  return _genObjRegVec[ doc->GetDocumentID() ];
}

//================================================================================
/*!
 * \brief Stores theRegister
 */
//================================================================================

void SALOMEDSImpl_Study::setGenObjRegister(SALOMEDSImpl_AbstractCallback* theRegister)
{
  getGenObjRegister( _doc ) = theRegister;
}

//================================================================================
/*!
 * \brief Indirectly invokes GenericObj_i::Register()
 */
//================================================================================

void SALOMEDSImpl_Study::RegisterGenObj  (const std::string& theIOR, DF_Label label)
{
  if ( SALOMEDSImpl_AbstractCallback* goRegister = getGenObjRegister( label.GetDocument() ))
    goRegister->RegisterGenObj( theIOR );
}

//================================================================================
/*!
 * \brief Indirectly invokes GenericObj_i::UnRegister()
 */
//================================================================================

void SALOMEDSImpl_Study::UnRegisterGenObj(const std::string& theIOR, DF_Label label)
{
  if ( SALOMEDSImpl_AbstractCallback* goRegister = getGenObjRegister( label.GetDocument() ))
    goRegister->UnRegisterGenObj( theIOR );
}

//#######################################################################################################
//#                                     STATIC PRIVATE FUNCTIONS
//#######################################################################################################

//============================================================================
/*! Function : SaveAttributes
 *  Purpose  : Save attributes for object
 */
//============================================================================
static void SaveAttributes(const SALOMEDSImpl_SObject& aSO, HDFgroup *hdf_group_sobject)
{
  hdf_size size[1];
  std::vector<DF_Attribute*> attrList = aSO.GetLabel().GetAttributes();
  DF_Attribute* anAttr = NULL;
  for(int i = 0, len = attrList.size(); i<len; i++) {
    anAttr = attrList[i];
    //The following attributes are not supposed to be written to the file
    std::string type = SALOMEDSImpl_GenericAttribute::Impl_GetType(anAttr);
    if(type == std::string("AttributeIOR")) continue; //IOR attribute is not saved
    std::string aSaveStr =anAttr->Save();
    //cout << "Saving: " << aSO.GetID() << " type: "<< type<<"|" << endl;
    size[0] = (hdf_int32) strlen(aSaveStr.c_str()) + 1;
    HDFdataset *hdf_dataset = new HDFdataset((char*)type.c_str(), hdf_group_sobject, HDF_STRING,size, 1);
    hdf_dataset->CreateOnDisk();
    hdf_dataset->WriteOnDisk((char*)aSaveStr.c_str());
    hdf_dataset->CloseOnDisk();
    hdf_dataset=0; //will be deleted by hdf_sco_group destructor
  }
}

//===========================================================================
//Function : ReadAttributes
//===========================================================================
static void ReadAttributes(SALOMEDSImpl_Study* theStudy,
                           const SALOMEDSImpl_SObject& aSO,
                           HDFdataset* hdf_dataset)
{
  hdf_dataset->OpenOnDisk();

  DF_Attribute* anAttr = NULL;
  char* current_string = new char[hdf_dataset->GetSize()+1];
  hdf_dataset->ReadFromDisk(current_string);
  //cout << "Reading attr type = " << hdf_dataset->GetName() << "  SO = " << aSO.GetID() << endl;
  if (!strcmp(hdf_dataset->GetName(),"COMPONENTDATATYPE")) {
    anAttr = theStudy->NewBuilder()->FindOrCreateAttribute(aSO, "AttributeComment");
  }
  else if (!strcmp(hdf_dataset->GetName(),"AttributeReference") ||
           !strcmp(hdf_dataset->GetName(),"Reference")) { // Old format maintenance
    theStudy->NewBuilder()->Addreference(aSO, theStudy->CreateObjectID(current_string));
    delete [] (current_string);
    hdf_dataset->CloseOnDisk();
    return;
  }
  else {
    anAttr = theStudy->NewBuilder()->FindOrCreateAttribute(aSO, hdf_dataset->GetName());
  }

  if (anAttr) {
    anAttr->Load(current_string);
  }

  delete [] (current_string);
  hdf_dataset->CloseOnDisk();
}

//============================================================================
//Function : BuildlTree
//============================================================================
static void BuildTree (SALOMEDSImpl_Study* theStudy, HDFgroup* hdf_current_group)
{
  hdf_current_group->OpenOnDisk();
  SALOMEDSImpl_SObject aSO;
  char* Entry = hdf_current_group->GetName();
  if (strcmp(Entry,"STUDY_STRUCTURE") == 0) {
    aSO = theStudy->CreateObjectID("0:1");
  }
  else {
    aSO = theStudy->CreateObjectID(Entry);
  }

  char name[HDF_NAME_MAX_LEN+1];
  int nbsons = hdf_current_group->nInternalObjects();
  for (int i=0; i<nbsons; i++) {
    hdf_current_group->InternalObjectIndentify(i,name);
    if (strncmp(name, "INTERNAL_COMPLEX",16) == 0) continue;
    hdf_object_type type = hdf_current_group->InternalObjectType(name);

    if (type == HDF_DATASET) {
      HDFdataset* new_dataset = new HDFdataset(name,hdf_current_group);
      ReadAttributes(theStudy,aSO,new_dataset);
      new_dataset = 0; // will be deleted by father destructor
    }
    else if (type == HDF_GROUP)   {
      HDFgroup* new_group = new HDFgroup(name,hdf_current_group);
      BuildTree (theStudy, new_group);
      new_group = 0; // will be deleted by father destructor
    }
  }
  hdf_current_group->CloseOnDisk();
}


//============================================================================
//Function : Translate_IOR_to_persistentID
//============================================================================
static void Translate_IOR_to_persistentID (const SALOMEDSImpl_SObject& so,
                                           SALOMEDSImpl_Driver*        engine,
                                           bool                        isMultiFile,
                                           bool                        isASCII)
{
  DF_ChildIterator itchild(so.GetLabel());
  std::string ior_string,  persistent_string, curid;

  for (; itchild.More(); itchild.Next()) {
    SALOMEDSImpl_SObject current = SALOMEDSImpl_Study::SObject(itchild.Value());
    SALOMEDSImpl_AttributeIOR* IOR = NULL;
    if ((IOR=(SALOMEDSImpl_AttributeIOR*)current.GetLabel().FindAttribute(SALOMEDSImpl_AttributeIOR::GetID()))) {
      ior_string = IOR->Value();

      persistent_string = engine->IORToLocalPersistentID (current, ior_string, isMultiFile, isASCII);
      SALOMEDSImpl_AttributePersistentRef::Set(current.GetLabel(), persistent_string);
    }
    Translate_IOR_to_persistentID (current, engine, isMultiFile, isASCII);
  }
}

void ReadNoteBookVariables(SALOMEDSImpl_Study* theStudy, HDFgroup* theGroup)
{
  if(!theGroup)
    return;

  HDFgroup* new_group =0;
  HDFdataset* new_dataset =0;

  char aVarName[HDF_NAME_MAX_LEN+1];
  char *currentVarType = 0;
  char *currentVarValue = 0;
  char *currentVarIndex = 0;
  int order = 0;
  //Open HDF group with notebook variables
  theGroup->OpenOnDisk();

  //Get Nb of variables
  int aNbVars = theGroup->nInternalObjects();

  std::map<int,SALOMEDSImpl_GenericVariable*> aVarsMap;

  for( int iVar=0;iVar < aNbVars;iVar++ ) {
    theGroup->InternalObjectIndentify(iVar,aVarName);
    hdf_object_type type = theGroup->InternalObjectType(aVarName);
    if(type == HDF_GROUP) {

      //Read Variable
      new_group = new HDFgroup(aVarName,theGroup);
      new_group->OpenOnDisk();

      //Read Type
      new_dataset = new HDFdataset("VARIABLE_TYPE",new_group);
      new_dataset->OpenOnDisk();
      currentVarType = new char[new_dataset->GetSize()+1];
      new_dataset->ReadFromDisk(currentVarType);
      new_dataset->CloseOnDisk();
      new_dataset = 0; //will be deleted by hdf_sco_group destructor

      //Read Order
      if(new_group->ExistInternalObject("VARIABLE_INDEX")) {
        new_dataset = new HDFdataset("VARIABLE_INDEX",new_group);
        new_dataset->OpenOnDisk();
        currentVarIndex = new char[new_dataset->GetSize()+1];
        new_dataset->ReadFromDisk(currentVarIndex);
        new_dataset->CloseOnDisk();
        new_dataset = 0; //will be deleted by hdf_sco_group destructor
        order = atoi(currentVarIndex);
        delete [] currentVarIndex;
      }
      else
        order = iVar;

      //Read Value
      new_dataset = new HDFdataset("VARIABLE_VALUE",new_group);
      new_dataset->OpenOnDisk();
      currentVarValue = new char[new_dataset->GetSize()+1];
      new_dataset->ReadFromDisk(currentVarValue);
      new_dataset->CloseOnDisk();
      new_dataset = 0; //will be deleted by hdf_sco_group destructor

      new_group->CloseOnDisk();
      new_group = 0;  //will be deleted by hdf_sco_group destructor

      SALOMEDSImpl_GenericVariable::VariableTypes aVarType =
        SALOMEDSImpl_GenericVariable::String2VariableType(std::string(currentVarType));
      delete [] currentVarType;

      //Create variable and add it in the study
      SALOMEDSImpl_GenericVariable* aVariable =
        new SALOMEDSImpl_ScalarVariable(aVarType,std::string(aVarName));
      aVariable->Load(std::string(currentVarValue));
      aVarsMap.insert(std::make_pair(order,aVariable));
      delete [] currentVarValue;
    }
  }

  std::map<int,SALOMEDSImpl_GenericVariable*>::const_iterator it= aVarsMap.begin();
  for(;it!=aVarsMap.end();it++)
    theStudy->AddVariable((*it).second);

  theGroup->CloseOnDisk();
}
