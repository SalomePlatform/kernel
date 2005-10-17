//  File   : SALOMEDSImpl_StudyManager.cxx
//  Author : Sergey RUIN
//  Module : SALOME

#include "SALOMEDSImpl_StudyManager.hxx"

using namespace std;

#include <CDF_Session.hxx>
#include <CDF_DirectoryIterator.hxx>
#include <TDF_Label.hxx>
#include <TDF_Tool.hxx>
#include <TDF_Data.hxx>
#include <TDF_RelocationTable.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_AttributeIterator.hxx>
#include <TColStd_HSequenceOfReal.hxx>
#include <TColStd_HSequenceOfInteger.hxx>
#include <TColStd_HArray1OfCharacter.hxx>
#include <TColStd_HSequenceOfAsciiString.hxx>
#include <TColStd_HSequenceOfExtendedString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <OSD_Process.hxx>
#include <Quantity_Date.hxx>
#include "HDFexplorer.hxx"

#include "SALOMEDSImpl_Attributes.hxx"
#include "SALOMEDSImpl_Tool.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include <map>

#include "HDFOI.hxx"
#include <iostream>
#include <stdlib.h>

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_StudyManager, MMgt_TShared )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_StudyManager, MMgt_TShared )

#define USE_CASE_LABEL_ID                       "0:2"
#define AUTO_SAVE_GUID                          "128268A3-71C9-4036-89B1-F81BD6A4FCF2"
#define AUTO_SAVE_TAG                           "0:8"
#define AUTO_SAVE_TIME_OUT_IN_SECONDS           1200

static void SaveAttributes(Handle(SALOMEDSImpl_SObject) SO, HDFgroup *hdf_group_sobject);
static void ReadAttributes(const Handle(SALOMEDSImpl_Study)&, const Handle(SALOMEDSImpl_SObject)&, HDFdataset* );
static void BuildTree (const Handle(SALOMEDSImpl_Study)&, HDFgroup*);
static void Translate_IOR_to_persistentID (const Handle(SALOMEDSImpl_SObject)&,
					   SALOMEDSImpl_Driver*, bool isMultiFile, bool isASCII);

//============================================================================
/*! Function : SALOMEDSImpl_StudyManager
 *  Purpose  : SALOMEDSImpl_StudyManager constructor
 */
//============================================================================
SALOMEDSImpl_StudyManager::SALOMEDSImpl_StudyManager()
{
  _errorCode = "";
  _OCAFApp = new SALOMEDSImpl_OCAFApplication();
  _IDcounter = 0;
  _OCAFApp->NewDocument("SALOME_STUDY", _clipboard);
}

//============================================================================
/*! Function : ~SALOMEDSImpl_StudyManager
 *  Purpose  : SALOMEDSImpl_StudyManager destructor
 */
//============================================================================
SALOMEDSImpl_StudyManager::~SALOMEDSImpl_StudyManager()
{
  // Destroy OCAF application
  _OCAFApp.Nullify();
}


//============================================================================
/*! Function : NewStudy
 *  Purpose  : Create a New Study of name study_name
 */
//==================================================T==========================
Handle(SALOMEDSImpl_Study) SALOMEDSImpl_StudyManager::NewStudy(const TCollection_AsciiString& study_name)
{
  _errorCode = "";

  Handle(TDocStd_Document) Doc;
  _OCAFApp->NewDocument("SALOME_STUDY",Doc);

  Handle(SALOMEDSImpl_Study) Study = new SALOMEDSImpl_Study(Doc, study_name);

  _IDcounter++;
  Study->StudyId( _IDcounter );

  // set Study properties
  Handle(SALOMEDSImpl_AttributeStudyProperties) aProp = Study->GetProperties();
  OSD_Process aProcess;
  Quantity_Date aDate = aProcess.SystemDate();
  aProp->SetModification(aProcess.UserName().ToCString(),
                         aDate.Minute(), aDate.Hour(), aDate.Day(), aDate.Month(), aDate.Year());
  aProp->SetCreationMode(1);  //"from scratch"

  return Study;
}

//============================================================================
/*! Function : Open
 *  Purpose  : Open a Study from it's persistent reference
 */
//============================================================================
Handle(SALOMEDSImpl_Study) SALOMEDSImpl_StudyManager::Open(const TCollection_AsciiString& aUrl)
{
  _errorCode = "";

  // open the HDFFile
  HDFfile *hdf_file =0;
  HDFgroup *hdf_group_study_structure =0;

  char* aC_HDFUrl;
  TCollection_AsciiString aHDFUrl;
  bool isASCII = false;
  if (HDFascii::isASCII(aUrl.ToCString())) {
    isASCII = true;
    char* aResultPath = HDFascii::ConvertFromASCIIToHDF(aUrl.ToCString());
    aC_HDFUrl = new char[strlen(aResultPath) + 19];
    sprintf(aC_HDFUrl, "%shdf_from_ascii.hdf", aResultPath);
    delete(aResultPath);
    aHDFUrl = aC_HDFUrl;
    delete aC_HDFUrl;
  } else {
    aHDFUrl = aUrl;
  }

  hdf_file = new HDFfile((char*)aHDFUrl.ToCString());
  try {
    hdf_file->OpenOnDisk(HDF_RDONLY);// mpv: was RDWR, but opened file can be write-protected too
  }
  catch (HDFexception)
    {
#ifndef WNT
      char eStr[strlen(aUrl.ToCString())+17];
#else
	  char *eStr;
	  eStr = new char[strlen(aUrl.ToCString())+17];
#endif
      sprintf(eStr,"Can't open file %s",aUrl.ToCString());
#ifdef WNT
	  delete [] eStr;
#endif
      _errorCode = TCollection_AsciiString(eStr);
      return NULL;
    }

  // Temporary aStudyUrl in place of study name
  Handle(TDocStd_Document) Doc;
  _OCAFApp->NewDocument("SALOME_STUDY",Doc);

  Handle(SALOMEDSImpl_Study) Study = new SALOMEDSImpl_Study(Doc, aUrl);

  _IDcounter++;
  Study->StudyId( _IDcounter );

  // Assign the value of the URL in the study object
  Study->URL (aUrl);

  SALOMEDSImpl_AttributePersistentRef::Set(Doc->Main(), aUrl);

  if (!hdf_file->ExistInternalObject("STUDY_STRUCTURE")) {
     _errorCode = "Study is empty";
    return Study;
  }

  //Create  the Structure of the OCAF Document
  hdf_group_study_structure = new HDFgroup("STUDY_STRUCTURE",hdf_file);

  Handle(TDF_Data) DF = Doc->GetData();
  try {
    BuildTree (Study, hdf_group_study_structure);
  }
  catch (HDFexception)
    {
#ifndef WNT
      char eStr[strlen(aUrl.ToCString())+17];
#else
	  char *eStr = new char [strlen(aUrl.ToCString())+17];
#endif
      sprintf(eStr,"Can't open file %s", aUrl.ToCString());
      _errorCode = TCollection_AsciiString(eStr);
      return NULL;
    }

  hdf_file->CloseOnDisk();

  if (isASCII) {
    Handle(TColStd_HSequenceOfAsciiString) aFilesToRemove = new TColStd_HSequenceOfAsciiString;
    aFilesToRemove->Append(aHDFUrl);
    SALOMEDSImpl_Tool::RemoveTemporaryFiles(SALOMEDSImpl_Tool::GetDirFromPath(aHDFUrl), aFilesToRemove, true);
  }

  delete hdf_file; // all related hdf objects will be deleted

  return Study;
}



//============================================================================
/*! Function : Close
 *  Purpose  : Close a study.
 *             If the study hasn't been saved, ask the user to confirm the
 *             close action without saving
 */

//============================================================================
void  SALOMEDSImpl_StudyManager::Close(const Handle(SALOMEDSImpl_Study)& aStudy)
{
  _errorCode = "";

  if(aStudy.IsNull()) {
    _errorCode = "Study is null";
    return;
  }

  aStudy->Close();
}

//============================================================================
/*! Function : Save
 *  Purpose  : Save a Study to it's persistent reference
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::Save(const Handle(SALOMEDSImpl_Study)& aStudy,
				     SALOMEDSImpl_DriverFactory* aFactory,
				     bool theMultiFile)
{
  _errorCode = "";

  TCollection_AsciiString url = aStudy->URL();
  if (url.IsEmpty()) {
    _errorCode = "No path specified to save the study. Nothing done";
    return false;
  }
  else {
    return Impl_SaveAs(url,aStudy, aFactory, theMultiFile, false);
  }

  return false;
}

bool SALOMEDSImpl_StudyManager::SaveASCII(const Handle(SALOMEDSImpl_Study)& aStudy,
					  SALOMEDSImpl_DriverFactory* aFactory,
					  bool theMultiFile)
{
  _errorCode = "";

  TCollection_AsciiString url = aStudy->URL();
  if (url.IsEmpty()) {
    _errorCode = "No path specified to save the study. Nothing done";
    return false;
  }
  else {
    return Impl_SaveAs(url,aStudy, aFactory, theMultiFile, true);
  }

  return false;
}

//=============================================================================
/*! Function : SaveAs
 *  Purpose  : Save a study to the persistent reference aUrl
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::SaveAs(const TCollection_AsciiString& aUrl,
				       const Handle(SALOMEDSImpl_Study)& aStudy,
				       SALOMEDSImpl_DriverFactory* aFactory,
				       bool theMultiFile)
{
  _errorCode = "";
  return Impl_SaveAs(aUrl,aStudy, aFactory, theMultiFile, false);
}

bool SALOMEDSImpl_StudyManager::SaveAsASCII(const TCollection_AsciiString& aUrl,
					    const Handle(SALOMEDSImpl_Study)& aStudy,
					    SALOMEDSImpl_DriverFactory* aFactory,
					    bool theMultiFile)
{
  _errorCode = "";
  return Impl_SaveAs(aUrl,aStudy, aFactory, theMultiFile, true);
}

//============================================================================
/*! Function : GetOpenStudies
 *  Purpose  : Get name list of open studies in the session
 */
//============================================================================
Handle(TColStd_HSequenceOfTransient) SALOMEDSImpl_StudyManager::GetOpenStudies()
{
  _errorCode = "";
  Handle(TColStd_HSequenceOfTransient) aList = new TColStd_HSequenceOfTransient;

  int nbDocs = _OCAFApp->NbDocuments();

  if(nbDocs == 0) {
    _errorCode = "No active study in this session";
    return aList;
  }
  else {
    Handle(SALOMEDSImpl_Study) aStudy;
    Handle(CDF_Session) S = CDF_Session::CurrentSession();
    CDF_DirectoryIterator it (S->Directory());
    for (;it.MoreDocument();it.NextDocument()) {
      Handle(TDocStd_Document) D = Handle(TDocStd_Document)::DownCast(it.Document());
      if(D == _clipboard) continue;
      aStudy = SALOMEDSImpl_Study::GetStudy(D->Main());
      if(aStudy.IsNull()) continue;
      aList->Append(aStudy);
    }
  }

  return aList;
}

//============================================================================
/*! Function : GetStudyByName
 *  Purpose  : Get a study from its name
 */
//============================================================================
Handle(SALOMEDSImpl_Study) SALOMEDSImpl_StudyManager::GetStudyByName
                                   (const TCollection_AsciiString& aStudyName)
{
  _errorCode = "";
  int nbDocs = _OCAFApp->NbDocuments();

  if (nbDocs == 0) {
    _errorCode = "No active study in this session";
    return NULL;
  }
  else {
    Handle(SALOMEDSImpl_Study) aStudy;
    Handle(CDF_Session) S = CDF_Session::CurrentSession();
    CDF_DirectoryIterator it (S->Directory());
    for (; it.MoreDocument(); it.NextDocument()) {
      Handle(TDocStd_Document) D = Handle(TDocStd_Document)::DownCast(it.Document());
      if (D == _clipboard) continue;
      aStudy = SALOMEDSImpl_Study::GetStudy(D->Main());
      if (aStudy.IsNull()) continue;
      if (aStudy->Name() == aStudyName) return aStudy;
    }
  }

  _errorCode = TCollection_AsciiString("Found no study with the name ") + aStudyName;
  return NULL;
}

//============================================================================
/*! Function : GetStudyByID
 *  Purpose  : Get a study from its ID
 */
//============================================================================
Handle(SALOMEDSImpl_Study) SALOMEDSImpl_StudyManager::GetStudyByID(int aStudyID)
{
  _errorCode = "";
  int nbDocs = _OCAFApp->NbDocuments();

  if (nbDocs == 0) {
    _errorCode = "No active study in this session";
    return NULL;
  }
  else {
    Handle(SALOMEDSImpl_Study) aStudy;
    Handle(CDF_Session) S = CDF_Session::CurrentSession();
    CDF_DirectoryIterator it (S->Directory());
    for (; it.MoreDocument(); it.NextDocument()) {
      Handle(TDocStd_Document) D = Handle(TDocStd_Document)::DownCast(it.Document());
      if (D == _clipboard) continue;
      aStudy = SALOMEDSImpl_Study::GetStudy(D->Main());
      if (aStudy.IsNull()) continue;
      if (aStudy->StudyId() == aStudyID) return aStudy;
    }
  }

  _errorCode = "Found no study with the given ID";
  return NULL;
}

//=============================================================================
/*! Function : _SaveProperties
 *  Purpose  : save the study properties in HDF file
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::Impl_SaveProperties(const Handle(SALOMEDSImpl_Study)& aStudy,
                                                    HDFgroup *hdf_group)
{
  _errorCode = "";

  HDFdataset *hdf_dataset = 0;
  hdf_size size[1];
  hdf_int32 name_len;

  // add modifications list (user and date of save)
  Handle(SALOMEDSImpl_AttributeStudyProperties) aProp = aStudy->GetProperties();
  Handle(SALOMEDSImpl_StudyBuilder) SB= aStudy->NewBuilder();
  int aLocked = aProp->IsLocked();
  if (aLocked) aProp->SetLocked(Standard_False);

  OSD_Process aProcess;
  Quantity_Date aDate = aProcess.SystemDate();
  aProp->SetModification(aProcess.UserName().ToCString(),
                         aDate.Minute(), aDate.Hour(), aDate.Day(), aDate.Month(), aDate.Year());

  if (aLocked) aProp->SetLocked(Standard_True);

  Handle(TColStd_HSequenceOfExtendedString) aNames;
  Handle(TColStd_HSequenceOfInteger) aMinutes, aHours, aDays, aMonths, aYears;

  aProp->GetModifications(aNames, aMinutes, aHours, aDays, aMonths, aYears);

  int aLength = 0, anIndex, i;
  for(i=1; i<=aNames->Length(); i++)
    aLength += aNames->Value(i).Length() + 1;

  //string length: 1 byte = locked flag, 1 byte = modified flag, (12 + name length + 1) for each name and date, "zero" byte
  char* aProperty = new char[3 + aLength + 12 * aNames->Length()];


  sprintf(aProperty,"%c%c", (char)aProp->GetCreationMode(),  (aProp->IsLocked())?'l':'u');

  aLength = aNames->Length();
  int a = 2;
  for(anIndex = 1; anIndex  <= aLength; anIndex++) {
    sprintf(&(aProperty[a]),"%2d%2d%2d%2d%4d%s",
	    (int)(aMinutes->Value(anIndex)),
	    (int)(aHours->Value(anIndex)),
	    (int)(aDays->Value(anIndex)),
	    (int)(aMonths->Value(anIndex)),
	    (int)(aYears->Value(anIndex)),
	    TCollection_AsciiString(aNames->Value(anIndex)).ToCString());
    a = strlen(aProperty);
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
bool SALOMEDSImpl_StudyManager::Impl_SaveAs(const TCollection_AsciiString& aUrl,
					    const Handle(SALOMEDSImpl_Study)& aStudy,
					    SALOMEDSImpl_DriverFactory* aFactory,
					    bool theMultiFile,
					    bool theASCII)
{
  // HDF File will be composed of differents part :
  // * For each ComponentDataType, all data created by the component
  //   Informations in data group hdf_group_datacomponent
  // * Study Structure -> Exactly what is contained in OCAF document
  //   Informations in data group hdf_group_study_structure

  _errorCode = "";

  HDFfile *hdf_file=0;
  HDFgroup *hdf_group_study_structure =0;
  HDFgroup *hdf_sco_group =0;
  HDFgroup *hdf_sco_group2 =0;

  HDFgroup *hdf_group_datacomponent =0;
  HDFdataset *hdf_dataset =0;
  hdf_size size[1];
  hdf_int32 name_len = 0;
  char *component_name = 0;

  int aLocked = aStudy->GetProperties()->IsLocked();
  if (aLocked) aStudy->GetProperties()->SetLocked(false);

  Handle(SALOMEDSImpl_StudyBuilder) SB= aStudy->NewBuilder();
  map<char*, SALOMEDSImpl_Driver*> aMapTypeDriver;

  if(aStudy.IsNull()) {
    _errorCode = "Study is null";
    return false;
  }

  try
    {
      // mpv 15.12.2003: for saving components we have to load all data from all modules
      SALOMEDSImpl_SComponentIterator itcomponent1 = aStudy->NewComponentIterator();
      for (; itcomponent1.More(); itcomponent1.Next())
	{
	  Handle(SALOMEDSImpl_SComponent) sco = itcomponent1.Value();

	  // if there is an associated Engine call its method for saving
	  TCollection_AsciiString IOREngine;
	  try {
	    if (!sco->ComponentIOR(IOREngine)) {
	      TCollection_AsciiString aCompType = sco->GetComment();
	      if (!aCompType.IsEmpty()) {

		SALOMEDSImpl_Driver* aDriver = aFactory->GetDriverByType(aCompType);
		aMapTypeDriver[aCompType.ToCString()] = aDriver;

		if (aDriver != NULL) {
		  if(!SB->LoadWith(sco, aDriver)) {
		    _errorCode = SB->GetErrorCode();
		    return false;
		  }
		}
	      }
	    }
	  } catch(...) {
	    _errorCode = "Can not restore information to resave it";
	    return false;
	  }
	}

      TCollection_AsciiString anOldName = aStudy->Name();
      aStudy->URL(aUrl);

      // To change for Save
      // Do not have to do a new file but just a Open??? Rewrite all informations after erasing evrything??
      hdf_file = new HDFfile(aUrl.ToCString());
      hdf_file->CreateOnDisk();

      //-----------------------------------------------------------------------
      // 1 - Create a groupe for each SComponent and Update the PersistanceRef
      //-----------------------------------------------------------------------
      hdf_group_datacomponent = new HDFgroup("DATACOMPONENT",hdf_file);
      hdf_group_datacomponent->CreateOnDisk();

      SALOMEDSImpl_SComponentIterator itcomponent = aStudy->NewComponentIterator();

      //SRN: Added 17 Nov, 2003
      Handle(SALOMEDSImpl_SObject) anAutoSaveSO = aStudy->FindObjectID(AUTO_SAVE_TAG);
      //SRN: End
      for (; itcomponent.More(); itcomponent.Next())
	{
	  Handle(SALOMEDSImpl_SComponent) sco = itcomponent.Value();

	  TCollection_AsciiString scoid = sco->GetID();
	  hdf_sco_group = new HDFgroup(scoid.ToCString(), hdf_group_datacomponent);
	  hdf_sco_group->CreateOnDisk();

	  TCollection_AsciiString componentDataType = sco->ComponentDataType();

	  //SRN: Added 17 Nov 2003: If there is a specified attribute, the component peforms a special save
	  if(!anAutoSaveSO.IsNull() && SB->IsGUID(sco, AUTO_SAVE_GUID)) {

	    Handle(SALOMEDSImpl_AttributeTableOfString) aTable;
	    if(anAutoSaveSO->GetLabel().FindAttribute(SALOMEDSImpl_AttributeTableOfString::GetID(), aTable)) {
	      Standard_Integer nbRows = aTable->GetNbRows(), k, aTimeOut = 0;
              if(nbRows > 0 && aTable->GetNbColumns() > 1) {

		Handle(TColStd_HSequenceOfExtendedString) aRow;
		for(k=1; k<=nbRows; k++) {
		  aRow = aTable->GetRowData(k);
		  if (aRow->Value(1) ==  componentDataType) {
		    TCollection_AsciiString anEntry = TCollection_AsciiString(aRow->Value(2));
		    Handle(SALOMEDSImpl_SObject) aCompSpecificSO = aStudy->FindObjectID(anEntry);
		    if(!aCompSpecificSO.IsNull()) {
		      Handle(SALOMEDSImpl_AttributeInteger) anInteger;
		      if(aCompSpecificSO->GetLabel().FindAttribute(SALOMEDSImpl_AttributeInteger::GetID(), anInteger)) {
			anInteger->SetValue(-1);
			while(anInteger->Value() < 0) {
#ifndef WNT
				sleep(2);
#else
				Sleep(2);
#endif
				if(++aTimeOut > AUTO_SAVE_TIME_OUT_IN_SECONDS)
					break;
			}
		      }  // if(aCompSpecificSO->FindAttribute(anInteger, "AttributeInteger"))
		    }  // if(!CORBA::is_nil(aCompSpecificSO))
		  }  // if (strcmp(aRow[0], componentDataType) == 0)
		}  // for

	      }  // if(nbRows > 0 && aTable->GetNbColumns() > 1)

	    }  // if(anAutoSaveSO->FindAttribute(aTable, "AttributeTableOfString")

	  }  // if(SB->IsGUID(AUTO_SAVE_GUID)

	  //SRN: End
	  TCollection_AsciiString IOREngine;
	  if (sco->ComponentIOR(IOREngine))
	    {
	      SALOMEDSImpl_Driver* Engine = NULL;
	      if(aMapTypeDriver.find(componentDataType.ToCString()) != aMapTypeDriver.end()) {
		// we have found the associated engine to write the data
		Engine = aMapTypeDriver[componentDataType.ToCString()];
	      }
	      else {
		Engine = aFactory->GetDriverByIOR(IOREngine);
	      }

	      if (Engine != NULL)
		{
		  unsigned char* aStream;
		  long length;

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
		  if(length > 0) {  //The component saved some auxiliary files, then put them into HDF file

		    aHDFSize[0] = length;

		    HDFdataset *hdf_dataset = new HDFdataset("FILE_STREAM", hdf_sco_group, HDF_STRING, aHDFSize, 1);
		    hdf_dataset->CreateOnDisk();
		    hdf_dataset->WriteOnDisk(aStream);  //Save the stream in the HDF file
		    hdf_dataset->CloseOnDisk();
		  }

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
		  // Creation of the persistance reference  attribute
		  Translate_IOR_to_persistentID (sco, Engine, theMultiFile, theASCII);

		  if(aStream != NULL) delete [] aStream;
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
      SALOMEDSImpl_SComponentIterator itcomp = aStudy->NewComponentIterator();
      for (; itcomp.More(); itcomp.Next())
	{
	  Handle(SALOMEDSImpl_SComponent) SC = itcomp.Value();
	  TCollection_AsciiString scid = SC->GetID();
	  hdf_sco_group2 = new HDFgroup(scid.ToCString(), hdf_group_study_structure);
	  hdf_sco_group2->CreateOnDisk();
          SaveAttributes(SC, hdf_sco_group2);
	  // ComponentDataType treatment
	  component_name = SC->ComponentDataType().ToCString();
	  name_len = (hdf_int32)strlen(component_name);
	  size[0] = name_len +1 ;
	  hdf_dataset = new HDFdataset("COMPONENTDATATYPE",hdf_sco_group2,HDF_STRING,size,1);
	  hdf_dataset->CreateOnDisk();
	  hdf_dataset->WriteOnDisk(component_name);
	  hdf_dataset->CloseOnDisk();
	  hdf_dataset=0; //will be deleted by hdf_sco_group destructor
	  Impl_SaveObject(SC, hdf_sco_group2);
	  hdf_sco_group2->CloseOnDisk();
 	  hdf_sco_group2=0; // will be deleted by hdf_group_study_structure destructor
	}

      //-----------------------------------------------------------------------
      //4 - Write the Study UseCases Structure
      //-----------------------------------------------------------------------
      Handle(SALOMEDSImpl_SObject) aSO = aStudy->FindObjectID(USE_CASE_LABEL_ID);
      if (!aSO.IsNull()) {
	HDFgroup *hdf_soo_group = new HDFgroup(USE_CASE_LABEL_ID,hdf_group_study_structure);
	hdf_soo_group->CreateOnDisk();
	SaveAttributes(aSO, hdf_soo_group);
	Impl_SaveObject(aSO, hdf_soo_group);
	hdf_soo_group->CloseOnDisk();
	hdf_soo_group=0; // will be deleted by hdf_group_study_structure destructor
      }

      if (aLocked) aStudy->GetProperties()->SetLocked(true);
      //-----------------------------------------------------------------------
      //5 - Write the Study Properties
      //-----------------------------------------------------------------------
      name_len = (hdf_int32) aStudy->Name().Length();
      size[0] = name_len +1 ;
      hdf_dataset = new HDFdataset("STUDY_NAME",hdf_group_study_structure,HDF_STRING,size,1);
      hdf_dataset->CreateOnDisk();
      char* studid = aStudy->Name().ToCString();
      hdf_dataset->WriteOnDisk(studid);
      hdf_dataset->CloseOnDisk();
      hdf_dataset=0; // will be deleted by hdf_group_study_structure destructor

      Impl_SaveProperties(aStudy, hdf_group_study_structure);

      hdf_group_study_structure->CloseOnDisk();
      hdf_file->CloseOnDisk();

      aStudy->IsSaved(true);
      hdf_group_study_structure =0; // will be deleted by hdf_file destructor
      delete hdf_file; // recursively deletes all hdf objects...
    }
  catch (HDFexception)
    {
      _errorCode = "HDFexception ! ";
      return false;
    }
  if (theASCII) { // save file in ASCII format
    HDFascii::ConvertFromHDFToASCII(aUrl.ToCString(), true);
  }

  return true;
}

//============================================================================
/*! Function : Impl_SaveObject
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::Impl_SaveObject(const Handle(SALOMEDSImpl_SObject)& SC,
						HDFgroup *hdf_group_datatype)
{
  _errorCode = "";

  // Write in group hdf_group_datatype all informations of SObject SC
  // Iterative function to parse all SObjects under a SComponent

  HDFgroup *hdf_group_sobject = 0;

  TDF_ChildIterator itchild(SC->GetLabel());
  for (; itchild.More(); itchild.Next())
    {

      // mpv: don't save empty labels
      TDF_AttributeIterator AI1(itchild.Value());
      if (!AI1.More()) {  //No attributes on the label
	TDF_ChildIterator subchild(SC->GetLabel());
	if (!subchild.More()) {
	  continue;
	}
	subchild.Initialize(SC->GetLabel(), true);
	bool anEmpty = true;
	for (; subchild.More() && anEmpty; subchild.Next()) {
	  TDF_AttributeIterator AI2(subchild.Value());
	  if (AI2.More()) anEmpty = false;  //There are attributes on the child label
	}
	if (anEmpty) continue;
      }

      Handle(SALOMEDSImpl_SObject) SO = SALOMEDSImpl_Study::SObject(itchild.Value());

      char* scoid = (char*) SO->GetID().ToCString();
      hdf_group_sobject = new HDFgroup(scoid, hdf_group_datatype);
      hdf_group_sobject->CreateOnDisk();
      SaveAttributes(SO, hdf_group_sobject);
      Impl_SaveObject(SO, hdf_group_sobject);
      hdf_group_sobject->CloseOnDisk();
      hdf_group_sobject =0; // will be deleted by father hdf object destructor
    }

  return true;
}

//============================================================================
/*! Function : Impl_SubstituteSlash
 *  Purpose  :
 */
//============================================================================
TCollection_AsciiString SALOMEDSImpl_StudyManager::Impl_SubstituteSlash(const TCollection_AsciiString& aUrl)
{
  _errorCode = "";

  TCollection_ExtendedString theUrl(aUrl);
  Standard_ExtCharacter val1 = ToExtCharacter('/');
  Standard_ExtCharacter val2 = ToExtCharacter(':');
  theUrl.ChangeAll(val1,val2);
  return theUrl;
}

//============================================================================
/*! Function : GetDocumentOfStudy
 *  Purpose  :
 */
//============================================================================
Handle(TDocStd_Document) SALOMEDSImpl_StudyManager::GetDocumentOfStudy(const Handle(SALOMEDSImpl_Study)& theStudy)
{
  _errorCode = "";
  return theStudy->_doc;
}

//============================================================================
/*! Function : CanCopy
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::CanCopy(const Handle(SALOMEDSImpl_SObject)& theObject,
					SALOMEDSImpl_Driver* theEngine)
{
  _errorCode = "";
  Handle(SALOMEDSImpl_SComponent) aComponent = theObject->GetFatherComponent();
  if (aComponent.IsNull()) return false;
  if (aComponent->GetLabel() == theObject->GetLabel()) return false;
  TCollection_AsciiString IOREngine;
  if (!aComponent->ComponentIOR(IOREngine)) return false;
  if (theEngine == NULL) return false;
  return theEngine->CanCopy(theObject);
}

//============================================================================
/*! Function : CopyLabel
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::CopyLabel(const Handle(SALOMEDSImpl_Study)& theSourceStudy,
					  SALOMEDSImpl_Driver* theEngine,
					  const Standard_Integer theSourceStartDepth,
					  const TDF_Label& theSource,
					  const TDF_Label& theDestinationMain)
{
  _errorCode = "";

  int a;
  TDF_Label aTargetLabel = theDestinationMain;
  TDF_Label aAuxTargetLabel = theDestinationMain.Father().FindChild(2);
  for(a = theSource.Depth() - theSourceStartDepth; a > 0 ; a--) {
    TDF_Label aSourceLabel = theSource;
    for(int aNbFather = 1; aNbFather < a; aNbFather++) aSourceLabel = aSourceLabel.Father();
    aTargetLabel = aTargetLabel.FindChild(aSourceLabel.Tag());
    aAuxTargetLabel = aAuxTargetLabel.FindChild(aSourceLabel.Tag());
  }
  // iterate attributes
  TDF_AttributeIterator anAttrIterator(theSource);
  Handle(TDF_RelocationTable) aRT = new TDF_RelocationTable();
  for(; anAttrIterator.More(); anAttrIterator.Next()) {
    Handle(TDF_Attribute) anAttr = anAttrIterator.Value();
    if (!Handle(SALOMEDSImpl_AttributeTreeNode)::DownCast(anAttr).IsNull()) continue; // never copy tree node attribute
    if (!Handle(SALOMEDSImpl_AttributeTarget)::DownCast(anAttr).IsNull()) continue; // and target attribute

    if (!Handle(SALOMEDSImpl_AttributeReference)::DownCast(anAttr).IsNull()) { // reference copied as Comment in aux tree
      TDF_Label aReferenced = Handle(SALOMEDSImpl_AttributeReference)::DownCast(anAttr)->Get();
      TCollection_AsciiString anEntry;
      TDF_Tool::Entry(aReferenced, anEntry);
      // store the value of name attribute of referenced label
      Handle(SALOMEDSImpl_AttributeName) aNameAttribute;
      if (aReferenced.FindAttribute(SALOMEDSImpl_AttributeName::GetID(), aNameAttribute)) {
	anEntry += " ";
	anEntry += aNameAttribute->Value();
      }
      SALOMEDSImpl_AttributeComment::Set(aAuxTargetLabel, TCollection_ExtendedString(anEntry));
      continue;
    }

    if (!Handle(SALOMEDSImpl_AttributeIOR)::DownCast(anAttr).IsNull()) { // IOR => ID and TMPFile of Engine
      TCollection_AsciiString anEntry;
      TDF_Tool::Entry(theSource, anEntry);
      Handle(SALOMEDSImpl_SObject) aSO = theSourceStudy->FindObjectID(anEntry.ToCString());
      int anObjID;
      long aLen;
      unsigned char* aStream = theEngine->CopyFrom(aSO, anObjID, aLen);
      TCollection_ExtendedString aResStr("");
      for(a = 0; a < aLen; a++) {
	aResStr += TCollection_ExtendedString(ToExtCharacter((Standard_Character)aStream[a]));
      }
      if(aStream != NULL) delete [] aStream;
      SALOMEDSImpl_AttributeInteger::Set(aAuxTargetLabel, anObjID);
      SALOMEDSImpl_AttributeName::Set(aAuxTargetLabel, aResStr);
      continue;
    }
    Handle(TDF_Attribute) aNewAttribute = anAttr->NewEmpty();
    aTargetLabel.AddAttribute(aNewAttribute);
    anAttr->Paste(aNewAttribute, aRT);
  }

  return true;
}

//============================================================================
/*! Function : Copy
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::Copy(const Handle(SALOMEDSImpl_SObject)& theObject,
				     SALOMEDSImpl_Driver* theEngine)
{
  _errorCode = "";

  // adoptation for alliances datamodel copy: without IOR attributes !!!
  bool aStructureOnly; // copy only SObjects and attributes without component help
  aStructureOnly = !theObject->GetLabel().IsAttribute(SALOMEDSImpl_AttributeIOR::GetID());

  // get component-engine
  Handle(SALOMEDSImpl_Study) aStudy = theObject->GetStudy();

  // CAF document of current study usage
  Handle(TDocStd_Document) aDocument = GetDocumentOfStudy(aStudy);
  if (aDocument.IsNull()) {
    _errorCode = "OCAF document is null";
    return false;
  }

  //Clear the clipboard
  _clipboard->Main().Root().ForgetAllAttributes(Standard_True);
  _OCAFApp->Close(_clipboard);
  Handle(TDocStd_Document) aDoc;
  _OCAFApp->NewDocument("SALOME_STUDY", aDoc);
  _clipboard = aDoc;

  // set component data type to the name attribute of root label
  if (!aStructureOnly) {
    SALOMEDSImpl_AttributeComment::Set(_clipboard->Main().Root(),
				       TCollection_ExtendedString(theEngine->ComponentDataType()));
  }
  // set to the Root label integer attribute: study id
  SALOMEDSImpl_AttributeInteger::Set(_clipboard->Main().Root(), aStudy->StudyId());
  // iterate all theObject's label children
  TDF_Label aStartLabel = theObject->GetLabel();
  Standard_Integer aSourceStartDepth = aStartLabel.Depth();

  // copy main source label
  CopyLabel(aStudy, theEngine, aSourceStartDepth, aStartLabel, _clipboard->Main());

  // copy all subchildren of the main source label (all levels)
  TDF_ChildIterator anIterator(aStartLabel, Standard_True);
  for(; anIterator.More(); anIterator.Next()) {
    CopyLabel(aStudy, theEngine, aSourceStartDepth, anIterator.Value(), _clipboard->Main());
  }

  return true;
}
//============================================================================
/*! Function : CanPaste
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_StudyManager::CanPaste(const Handle(SALOMEDSImpl_SObject)& theObject,
					 SALOMEDSImpl_Driver* theEngine)
{
  _errorCode = "";

  if (_clipboard.IsNull()) {
    _errorCode = "Clipboard is null";
    return false;
  }

  Handle(SALOMEDSImpl_AttributeComment) aCompName;
  if (!_clipboard->Main().Root().FindAttribute(SALOMEDSImpl_AttributeComment::GetID(), aCompName)) {
    _errorCode = "Clipboard has no component type";
    return false;
  }
  Handle(SALOMEDSImpl_AttributeInteger) anObjID;
  if (!_clipboard->Main().Father().FindChild(2).FindAttribute(SALOMEDSImpl_AttributeInteger::GetID(), anObjID)) {
    _errorCode = "Clipboard has no object id";
    return false;
  }
  Handle(SALOMEDSImpl_SComponent) aComponent = theObject->GetFatherComponent();
  if (aComponent.IsNull()) {
    _errorCode = "Object doesn't belong to component";
    return false;
  }

  TCollection_AsciiString IOREngine;
  if (!aComponent->ComponentIOR(IOREngine)) {
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
TDF_Label SALOMEDSImpl_StudyManager::PasteLabel(const Handle(SALOMEDSImpl_Study)& theDestinationStudy,
						SALOMEDSImpl_Driver* theEngine,
						const TDF_Label& theSource,
						const TDF_Label& theDestinationStart,
						const int theCopiedStudyID,
						const bool isFirstElement)
{
  _errorCode = "";

  // get corresponding source, target and auxiliary labels
  TDF_Label aTargetLabel = theDestinationStart;

  TDF_Label aAuxSourceLabel = theSource.Root().FindChild(2);
  int a;
  if (!isFirstElement) {
    for(a = theSource.Depth() - 1; a > 0 ; a--) {
      TDF_Label aSourceLabel = theSource;
      for(int aNbFather = 1; aNbFather < a; aNbFather++) aSourceLabel = aSourceLabel.Father();
      aTargetLabel = aTargetLabel.FindChild(aSourceLabel.Tag());
      aAuxSourceLabel = aAuxSourceLabel.FindChild(aSourceLabel.Tag());
    }
  }

  // check auxiliary label for TMPFile => IOR
  Handle(SALOMEDSImpl_AttributeName) aNameAttribute;
  if (aAuxSourceLabel.FindAttribute(SALOMEDSImpl_AttributeName::GetID(), aNameAttribute)) {
    Handle(SALOMEDSImpl_AttributeInteger) anObjID;

    aAuxSourceLabel.FindAttribute(SALOMEDSImpl_AttributeInteger::GetID(), anObjID);
    Handle(SALOMEDSImpl_AttributeComment) aComponentName;
    theSource.Root().FindAttribute(SALOMEDSImpl_AttributeComment::GetID(), aComponentName);
    TCollection_AsciiString aCompName = aComponentName->Value();

    if (theEngine->CanPaste(aCompName, anObjID->Value())) {
      TCollection_ExtendedString aTMPStr = aNameAttribute->Value();
      int aLen = aTMPStr.Length();
      unsigned char* aStream = NULL;
      if(aLen > 0) {
	aStream = new unsigned char[aLen+10];
	for(a = 0; a < aLen; a++) {
	  aStream[a] = ToCharacter(aTMPStr.Value(a+1));
	}
      }

      TCollection_AsciiString anEntry;
      TDF_Tool::Entry(aTargetLabel, anEntry);
      Handle(SALOMEDSImpl_SObject) aPastedSO = theDestinationStudy->FindObjectID(anEntry);

      if (isFirstElement) {
	TCollection_AsciiString aDestEntry = theEngine->PasteInto(aStream,
								  aLen,
								  anObjID->Value(),
								  aPastedSO->GetFatherComponent());
	TDF_Tool::Label(theDestinationStart.Data(), aDestEntry, aTargetLabel);
      } else
	theEngine->PasteInto(aStream, aLen, anObjID->Value(), aPastedSO);

      if(aStream != NULL) delete []aStream;
    }
  }

  // iterate attributes
  TDF_AttributeIterator anAttrIterator(theSource);
  Handle(TDF_RelocationTable) aRT = new TDF_RelocationTable();
  for(; anAttrIterator.More(); anAttrIterator.Next()) {
    Handle(TDF_Attribute) anAttr = anAttrIterator.Value();
    if (aTargetLabel.FindAttribute(anAttr->ID(), anAttr)) {
      aTargetLabel.ForgetAttribute(anAttr->ID());
      anAttr = anAttrIterator.Value();
    }
    Handle(TDF_Attribute) aNewAttribute = anAttr->NewEmpty();
    aTargetLabel.AddAttribute(aNewAttribute);
    anAttr->Paste(aNewAttribute, aRT);
  }

  // check auxiliary label for Comment => reference or name attribute of the referenced object
  Handle(SALOMEDSImpl_AttributeComment) aCommentAttribute;
  if (aAuxSourceLabel.FindAttribute(SALOMEDSImpl_AttributeComment::GetID(), aCommentAttribute)) {
    char * anEntry = new char[aCommentAttribute->Value().Length() + 1];
    strcpy(anEntry, TCollection_AsciiString(aCommentAttribute->Value()).ToCString());
    char* aNameStart = strchr(anEntry, ' ');
    if (aNameStart) {
      *aNameStart = '\0';
      aNameStart++;
    }
    if (theCopiedStudyID == theDestinationStudy->StudyId()) { // if copy to the same study, reanimate reference
      TDF_Label aRefLabel;
      TDF_Tool::Label(aTargetLabel.Data(), anEntry, aRefLabel);
      SALOMEDSImpl_AttributeReference::Set(aTargetLabel, aRefLabel);
      // target attributes structure support
      SALOMEDSImpl_AttributeTarget::Set(aRefLabel)->Add(SALOMEDSImpl_Study::SObject(aTargetLabel));
    } else {
      if (aNameStart) SALOMEDSImpl_AttributeName::Set(aTargetLabel, aNameStart);
      else SALOMEDSImpl_AttributeName::Set(aTargetLabel, TCollection_ExtendedString("Reference to:")+anEntry);
    }
    delete [] anEntry;
  }

  return aTargetLabel;
}

//============================================================================
/*! Function : Paste
 *  Purpose  :
 */
//============================================================================
Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_StudyManager::Paste(const Handle(SALOMEDSImpl_SObject)& theObject,
							      SALOMEDSImpl_Driver* theEngine)
{
  _errorCode = "";

  Handle(SALOMEDSImpl_Study) aStudy = theObject->GetStudy();

  // if study is locked, then paste can't be done
  if (aStudy->GetProperties()->IsLocked()) {
    _errorCode = "LockProtection";
    throw LockProtection("LockProtection");
  }

  // if there is no component name, then paste only SObjects and attributes: without component help
  Handle(SALOMEDSImpl_AttributeComment) aComponentName;
  bool aStructureOnly = !_clipboard->Main().Root().FindAttribute(SALOMEDSImpl_AttributeComment::GetID(), aComponentName);

  // get copied study ID
  Handle(SALOMEDSImpl_AttributeInteger) aStudyIDAttribute;
  if (!_clipboard->Main().Root().FindAttribute(SALOMEDSImpl_AttributeInteger::GetID(), aStudyIDAttribute)) {
    _errorCode = "No study ID was found";
    return NULL;
  }
  int aCStudyID = aStudyIDAttribute->Value();

  // CAF document of current study usage
  Handle(TDocStd_Document) aDocument = GetDocumentOfStudy(aStudy);
  if (aDocument.IsNull()) {
    _errorCode = "OCAF document is null";
    return NULL;
  }

  Handle(SALOMEDSImpl_SComponent) aComponent = theObject->GetFatherComponent();

  // fill root inserted SObject
  TDF_Label aStartLabel;
  if (aStructureOnly) {
    TDF_Label anObjectLabel;
    TDF_Tool::Label(aDocument->GetData(), theObject->GetID(), anObjectLabel);
    aStartLabel = PasteLabel(aStudy, theEngine, _clipboard->Main(), anObjectLabel, aCStudyID, false);
  } else {
    TDF_Label aComponentLabel;
    TDF_Tool::Label(aDocument->GetData(), aComponent->GetID(), aComponentLabel);
    aStartLabel = PasteLabel(aStudy, theEngine, _clipboard->Main(), aComponentLabel, aCStudyID, true);
  }

  // paste all sublebels
  TDF_ChildIterator anIterator(_clipboard->Main(), Standard_True);
  for(; anIterator.More(); anIterator.Next()) {
    PasteLabel(aStudy, theEngine, anIterator.Value(), aStartLabel, aCStudyID, false);
  }

  return SALOMEDSImpl_Study::SObject(aStartLabel);
}

//#######################################################################################################
//#                                     STATIC PRIVATE FUNCTIONS
//#######################################################################################################

//============================================================================
/*! Function : SaveAttributes
 *  Purpose  : Save attributes for object
 */
//============================================================================
static void SaveAttributes(Handle(SALOMEDSImpl_SObject) aSO, HDFgroup *hdf_group_sobject)
{
  hdf_size size[1];
  TDF_AttributeIterator Itr(aSO->GetLabel());
  Handle(TDF_Attribute) anAttr;
  for(; Itr.More(); Itr.Next()) {
    anAttr = Itr.Value();
    //The following attributes are not supposed to be written to the file
    if(anAttr->DynamicType() == STANDARD_TYPE(SALOMEDSImpl_AttributeIOR)) continue; //IOR attribute is not saved
    Handle(SALOMEDSImpl_GenericAttribute) ga = Handle(SALOMEDSImpl_GenericAttribute)::DownCast(anAttr);
    TCollection_AsciiString aSaveStr = ga->Save();
    //cout << "Saving: " << aSO->GetID() << " "<< ga->Type() << " value: " << aSaveStr << endl;
    size[0] = (hdf_int32) strlen(aSaveStr.ToCString()) + 1;
    HDFdataset *hdf_dataset = new HDFdataset((char*)ga->Type().ToCString(), hdf_group_sobject,HDF_STRING,size,1);
    hdf_dataset->CreateOnDisk();
    hdf_dataset->WriteOnDisk((char*)aSaveStr.ToCString());
    hdf_dataset->CloseOnDisk();
    hdf_dataset=0; //will be deleted by hdf_sco_group destructor
  }
}

//===========================================================================
//Function : ReadAttributes
//===========================================================================
static void ReadAttributes(const Handle(SALOMEDSImpl_Study)& theStudy,
			   const Handle(SALOMEDSImpl_SObject)& aSO,
			   HDFdataset* hdf_dataset)
{
  hdf_dataset->OpenOnDisk();

  Handle(TDF_Attribute) anAttr;

  char* current_string = new char[hdf_dataset->GetSize()];
  hdf_dataset->ReadFromDisk(current_string);

  if (!strcmp(hdf_dataset->GetName(),"COMPONENTDATATYPE")) {
    anAttr = theStudy->NewBuilder()->FindOrCreateAttribute(aSO, "AttributeComment");
  } else if (!strcmp(hdf_dataset->GetName(),"AttributeReference")) {
    theStudy->NewBuilder()->Addreference(aSO, theStudy->CreateObjectID(current_string));
    delete(current_string);
    hdf_dataset->CloseOnDisk();
    return;
  } else {
    anAttr = theStudy->NewBuilder()->FindOrCreateAttribute(aSO, hdf_dataset->GetName());
  }

  if (!anAttr.IsNull()) {

    Handle(SALOMEDSImpl_GenericAttribute) ga = Handle(SALOMEDSImpl_GenericAttribute)::DownCast(anAttr);
    ga->Load(current_string);
    //cout << "Reading: " << aSO->GetID() << " "<< ga->Type() << " value: " << current_string << endl;
  }

  delete(current_string);
  hdf_dataset->CloseOnDisk();
}

//============================================================================
//Function : BuildlTree
//============================================================================
static void BuildTree (const Handle(SALOMEDSImpl_Study)& theStudy, HDFgroup* hdf_current_group)
{
  hdf_current_group->OpenOnDisk();
  Handle(SALOMEDSImpl_SObject) aSO;
  char* Entry = hdf_current_group->GetName();
  if (strcmp(Entry,"STUDY_STRUCTURE") == 0) {
    aSO = theStudy->CreateObjectID("0:1");
  }
  else {
    aSO = theStudy->CreateObjectID(Entry);
  }

  char name[HDF_NAME_MAX_LEN+1];
  Standard_Integer nbsons = hdf_current_group->nInternalObjects();
  for (Standard_Integer i=0; i<nbsons; i++) {
    hdf_current_group->InternalObjectIndentify(i,name);
    if (strncmp(name, "INTERNAL_COMPLEX",16) == 0) continue;
    hdf_object_type type = hdf_current_group->InternalObjectType(name);

    if  (type == HDF_DATASET) {
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
static void Translate_IOR_to_persistentID (const Handle(SALOMEDSImpl_SObject)& so,
					   SALOMEDSImpl_Driver*                engine,
					   bool                                isMultiFile,
					   bool                                isASCII)
{
  TDF_ChildIterator itchild(so->GetLabel());
  TCollection_AsciiString ior_string,  persistent_string, curid;

  for (; itchild.More(); itchild.Next()) {
    Handle(SALOMEDSImpl_SObject) current = SALOMEDSImpl_Study::SObject(itchild.Value());
    Handle(SALOMEDSImpl_AttributeIOR) IOR;
    if (current->GetLabel().FindAttribute(SALOMEDSImpl_AttributeIOR::GetID(), IOR)) {
      ior_string = IOR->Value();

      persistent_string = engine->IORToLocalPersistentID (current, ior_string, isMultiFile, isASCII);
      SALOMEDSImpl_AttributePersistentRef::Set(current->GetLabel(), persistent_string);
    }
    Translate_IOR_to_persistentID (current, engine, isMultiFile, isASCII);
  }
}
