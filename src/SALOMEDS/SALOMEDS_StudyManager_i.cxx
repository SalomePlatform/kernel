//  SALOME SALOMEDS : data structure of SALOME and sources of Salome data server 
//
//  Copyright (C) 2003  OPEN CASCADE, EADS/CCR, LIP6, CEA/DEN,
//  CEDRAT, EDF R&D, LEG, PRINCIPIA R&D, BUREAU VERITAS 
// 
//  This library is free software; you can redistribute it and/or 
//  modify it under the terms of the GNU Lesser General Public 
//  License as published by the Free Software Foundation; either 
//  version 2.1 of the License. 
// 
//  This library is distributed in the hope that it will be useful, 
//  but WITHOUT ANY WARRANTY; without even the implied warranty of 
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
//  Lesser General Public License for more details. 
// 
//  You should have received a copy of the GNU Lesser General Public 
//  License along with this library; if not, write to the Free Software 
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA 
// 
//  See http://www.opencascade.org/SALOME/ or email : webmaster.salome@opencascade.org 
//
//
//
//  File   : SALOMEDS_StudyManager_i.cxx
//  Author : Yves FRICAUD 
//  Module : SALOME
//  $Header$

#include <memory>
#include <sstream>

#include <OSD_Process.hxx>
#include <Quantity_Date.hxx>

#include <TDF_Label.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_Comment.hxx>
#include <TDataStd_Integer.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TDataStd_UAttribute.hxx> 
#include <TDF_ChildIterator.hxx>
#include <TDF_Tool.hxx>
#include <TDF_Reference.hxx>
#include <TDF_Data.hxx>
#include <TDF_RelocationTable.hxx>
#include <TDF_AttributeIterator.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TCollection_AsciiString.hxx>

#include "SALOMEDS.hxx"
#include "SALOMEDS_StudyManager_i.hxx"
#include "SALOME_LifeCycleCORBA.hxx"

#include "SALOMEDS_Study_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_StudyBuilder_i.hxx"

#include "SALOMEDS_IORAttribute.hxx"
#include "SALOMEDS_PersRefAttribute.hxx"
#include "SALOMEDS_TargetAttribute.hxx"

#include "SALOMEDS_Tool.hxx"
#include "HDFexplorer.hxx"

// IDL headers
#include <SALOMEconfig.h>
#include CORBA_SERVER_HEADER(SALOMEDS_Attributes)

#include "SALOME_GenericObj_i.hh"
#include "Utils_CorbaException.hxx"
#include "Utils_ExceptHandlers.hxx"

UNEXPECT_CATCH(SalomeException,SALOME::SALOME_Exception);
UNEXPECT_CATCH(LockProtection, SALOMEDS::StudyBuilder::LockProtection);

#define USE_CASE_LABEL_ID             "0:2"
#define AUTO_SAVE_GUID                "128268A3-71C9-4036-89B1-F81BD6A4FCF2"
#define AUTO_SAVE_TAG                 "0:8"
#define AUTO_SAVE_TIME_OUT_IN_SECONDS 1200

#include "utilities.h"

using namespace std;

//===========================================================================
namespace SALOMEDS{

  CORBA::Object_var 
  GetObject(const TDF_Label& theLabel, CORBA::ORB_ptr theORB)
  {
    try {
      Handle(SALOMEDS_IORAttribute) anAttr;
      if(theLabel.FindAttribute(SALOMEDS_IORAttribute::GetID(),anAttr))
	return theORB->string_to_object(TCollection_AsciiString(anAttr->Get()).ToCString());
    }catch(...){
    }
    return CORBA::Object::_nil();
  }


  PortableServer::ServantBase_var 
  GetServant(CORBA::Object_ptr theObject, PortableServer::POA_ptr thePOA)
  {
    if(CORBA::is_nil(theObject))
      return NULL;
    try{
      return thePOA->reference_to_servant(theObject);
    }catch(...){
      return NULL;
    }
  }

}

//===========================================================================
//Function : LoadAttributes
//===========================================================================
static 
void 
ReadAttributes(SALOMEDS_Study_i* theStudy,
	       SALOMEDS::SObject_ptr aSO,
	       HDFdataset* hdf_dataset)
{
  hdf_dataset->OpenOnDisk();

  SALOMEDS::GenericAttribute_var anAttr;

  char* current_string = new char[hdf_dataset->GetSize()];
  hdf_dataset->ReadFromDisk(current_string);

  if (!strcmp(hdf_dataset->GetName(),"COMPONENTDATATYPE")) {
    anAttr = theStudy->GetBuilder()->FindOrCreateAttribute(aSO, "AttributeComment");
  } else if (!strcmp(hdf_dataset->GetName(),"Reference")) {
    theStudy->GetBuilder()->Addreference(aSO, theStudy->CreateObjectID(current_string));
    delete(current_string);
    hdf_dataset->CloseOnDisk();
    return;
  } else {
    MESSAGE("Read attribute "<<hdf_dataset->GetName())
    anAttr = theStudy->GetBuilder()->FindOrCreateAttribute(aSO, hdf_dataset->GetName());
  }

  if (!CORBA::is_nil(anAttr)) {
    anAttr->Restore(current_string);
    MESSAGE("Restoring attribute "<<hdf_dataset->GetName()<<" by string '"<<current_string<<"' done")
  } else {
    MESSAGE(hdf_dataset->GetName());
    MESSAGE("LoadAttributes: unknown types");
  }
  delete(current_string);
  hdf_dataset->CloseOnDisk();
}

//============================================================================
//Function : Translate_IOR_to_persistentID
//============================================================================
static void Translate_IOR_to_persistentID(SALOMEDS_Study_i* theStudy,
					  SALOMEDS_StudyBuilder_i* theBuilder,
					  SALOMEDS::SObject_ptr theSObject,
					  SALOMEDS::Driver_ptr theEngine,
					  CORBA::Boolean theIsMultiFile,
					  CORBA::Boolean theIsASCII)
{
  MESSAGE("In Translate_IOR_to_persistentID");
  SALOMEDS_ChildIterator_i anIter = theStudy->GetChildIterator(theSObject);
  for(; anIter.More(); anIter.Next()){
    SALOMEDS::GenericAttribute_var anAttr;
    SALOMEDS::SObject_var aSObject = anIter.Value();
    if(aSObject->FindAttribute(anAttr,"AttributeIOR")){
      SALOMEDS::AttributeIOR_var anIOR = SALOMEDS::AttributeIOR::_narrow(anAttr);
      CORBA::String_var aString = anIOR->Value();
      CORBA::String_var aPersistentID = 
	theEngine->IORToLocalPersistentID(aSObject,aString,theIsMultiFile,theIsASCII);
      anAttr = theBuilder->FindOrCreateAttribute(aSObject,"AttributePersistentRef");
      SALOMEDS::AttributePersistentRef_var aPersistentRef = SALOMEDS::AttributePersistentRef::_narrow(anAttr);
      aPersistentRef->SetValue(aPersistentID);
      aString = aSObject->GetID();
    }
    Translate_IOR_to_persistentID(theStudy,theBuilder,aSObject,theEngine,theIsMultiFile,theIsASCII);
  }
}

//============================================================================
//Function : BuildlTree
//============================================================================
static 
void 
BuildTree(SALOMEDS_Study_i* theStudy, HDFgroup* hdf_current_group)
{
  hdf_current_group->OpenOnDisk();
  
  SALOMEDS::SObject_var aSO;
  char* Entry = hdf_current_group->GetName();
  if (strcmp(Entry,"STUDY_STRUCTURE") == 0) {
    MESSAGE("find the root of the document");
    aSO = theStudy->CreateObjectID("0:1");
  }
  else {
    aSO = theStudy->CreateObjectID(Entry);
    MESSAGE("BuildTree : Create a new label"<<Entry);
  }
  char name[HDF_NAME_MAX_LEN+1];
  Standard_Integer nbsons = hdf_current_group->nInternalObjects(); 
  
  for (Standard_Integer i=0; i<nbsons; i++) {
    hdf_current_group->InternalObjectIndentify(i,name);
    if (strncmp(name, "INTERNAL_COMPLEX",16) == 0) continue;
    hdf_object_type type = hdf_current_group->InternalObjectType(name);

    if  (type == HDF_DATASET) {
      MESSAGE("--> Dataset: Internal Object Name : " << name);
      HDFdataset* new_dataset = new HDFdataset(name,hdf_current_group);
      ReadAttributes(theStudy,aSO,new_dataset);      
      new_dataset = 0; // will be deleted by father destructor

    }
    else if (type == HDF_GROUP)   {
      MESSAGE( "--> Group: Internal Object Name : " << name);
      HDFgroup* new_group = new HDFgroup(name,hdf_current_group);
      BuildTree(theStudy, new_group);
      new_group = 0; // will be deleted by father destructor
    }
  }
  hdf_current_group->CloseOnDisk();
}


//============================================================================
/*! Function : SALOMEDS_StudyManager_i
 *  Purpose  : SALOMEDS_StudyManager_i constructor 
 */
//============================================================================
SALOMEDS_StudyManager_i::SALOMEDS_StudyManager_i(CORBA::ORB_ptr theORB, 
						 PortableServer::POA_ptr thePOA):
  _orb(CORBA::ORB::_duplicate(theORB)),
  _poa(PortableServer::POA::_duplicate(thePOA)),
  _OCAFApp(new SALOMEDS_OCAFApplication()),
  _name_service(theORB)
{ 
  // Study directory creation in the naming service : to register all
  // open studies in the session
  _name_service.Create_Directory("/Study");
  _IDcounter = 0;
}

//============================================================================
/*! Function : ~SALOMEDS_StudyManager_i
 *  Purpose  : SALOMEDS_StudyManager_i destructor
 */
//============================================================================
SALOMEDS_StudyManager_i::~SALOMEDS_StudyManager_i()
{
  // Destroy directory to register open studies
  _name_service.Destroy_Directory("/Study");
}

SALOMEDS_Study_i* 
SALOMEDS_StudyManager_i::DownCast(SALOMEDS::Study_ptr theStudy) const
{
  if(!CORBA::is_nil(theStudy)){
    PortableServer::POA_var aPOA = GetPOA();
    PortableServer::ServantBase_var aServant = SALOMEDS::GetServant(theStudy,aPOA);
    if(aServant.in())
      return dynamic_cast<SALOMEDS_Study_i*>(aServant.in());
  }
  return NULL;
}

//============================================================================
/*! Function : register_name
 *  Purpose  : Register the study Manager in the naming service under the  
 *             context name
 */
//============================================================================
void SALOMEDS_StudyManager_i::register_name(char * theName) {
  SALOMEDS::StudyManager_var aManager(_this());
  _name_service.Register(aManager.in(),theName);
}


//============================================================================
/*! Function : NewStudy
 *  Purpose  : Create a New Study of name study_name
 */
//============================================================================
SALOMEDS::Study_ptr SALOMEDS_StudyManager_i::NewStudy(const char* theStudyName) 
{
  SALOMEDS::Locker lock;
   
  Handle(TDocStd_Document) aDocument;
  _OCAFApp->NewDocument("SALOME_STUDY",aDocument); 

  MESSAGE("NewStudy : Creating the CORBA servant holding it... ");
  SALOMEDS_Study_i* aStudyServant = new SALOMEDS_Study_i(this,aDocument,theStudyName); 
  SALOMEDS::Study_var aStudy = aStudyServant->_this();

  //Study->StudyId( _OCAFApp->NbDocuments() ); 
  _IDcounter++;
  aStudyServant->StudyId( _IDcounter );

  // Register study in the naming service
  // Path to acces the study
  if(!_name_service.Change_Directory("/Study")){
    MESSAGE( "Unable to access the study directory" );
  }else
    _name_service.Register(aStudy, theStudyName);
						   
  // Assign the value of the IOR in the study->root
  CORBA::String_var anIOR = _orb->object_to_string(aStudy);
  SALOMEDS_IORAttribute::Set(aDocument->Main().Root(),
			     const_cast<char*>(anIOR.in()),
			     aStudyServant);

  // set Study properties
  SALOMEDS::AttributeStudyProperties_var aProp = aStudyServant->GetProperties();
  OSD_Process aProcess;
  Quantity_Date aDate = aProcess.SystemDate();
  aProp->SetCreationDate(CORBA::Long(aDate.Minute()), 
			 CORBA::Long(aDate.Hour()), 
			 CORBA::Long(aDate.Day()),
			 CORBA::Long(aDate.Month()), 
			 CORBA::Long(aDate.Year()));
  aProp->SetCreationMode("from scratch");
  aProp->SetUserName(aProcess.UserName().ToCString());

  return aStudy._retn();
}

//============================================================================
/*! Function : Open
 *  Purpose  : Open a Study from it's persistent reference
 */
//============================================================================
SALOMEDS::Study_ptr  SALOMEDS_StudyManager_i::Open(const char* theURL)
     throw(SALOME::SALOME_Exception)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(SalomeException);
  MESSAGE("Begin of SALOMEDS_StudyManager_i::Open");

  bool isASCII = false;
  std::ostringstream anURLStream;
  if (HDFascii::isASCII(theURL)) {
    isASCII = true;
    auto_ptr<char> aResultPath(HDFascii::ConvertFromASCIIToHDF(theURL));
    anURLStream<<aResultPath.get()<<"hdf_from_ascii.hdf";
  } else {
    anURLStream<<theURL;
  }
  std::string aHDFUrl = anURLStream.str();

  // open the HDFFile (all related hdf objects will be deleted     )
  auto_ptr<HDFfile> hdf_file(new HDFfile(const_cast<char*>(aHDFUrl.c_str())));

  try {
    hdf_file->OpenOnDisk(HDF_RDONLY);// mpv: was RDWR, but opened file can be write-protected too
  }catch(HDFexception){
    std::ostringstream aStream;
    aStream<<"Can't open file "<<theURL;
    std::string eStr = aStream.str();
    THROW_SALOME_CORBA_EXCEPTION(eStr.c_str(),SALOME::BAD_PARAM);
  } 
  MESSAGE("Open : Creating the CORBA servant holding it... ");

  // Temporary aStudyUrl in place of study name
  Handle(TDocStd_Document) Doc;
  _OCAFApp->NewDocument("SALOME_STUDY",Doc); 

  SALOMEDS_Study_i* aStudyServant = new SALOMEDS_Study_i(this,Doc,theURL);  
  SALOMEDS::Study_var aStudy = aStudyServant->_this(); 

  //  aStudy->StudyId( _OCAFApp->NbDocuments() ); 
  _IDcounter++;
  aStudy->StudyId( _IDcounter );

  // Assign the value of the URL in the study object
  aStudyServant->URL(theURL);
  SCRUTE(theURL);

  // Assign the value of the IOR in the study->root
  CORBA::String_var anIOR = _orb->object_to_string(aStudy);
  SALOMEDS_IORAttribute::Set(Doc->Main().Root(),
			     const_cast<char*>(anIOR.in()),
			     aStudyServant);

  SALOMEDS_PersRefAttribute::Set(Doc->Main(),const_cast<char*>(theURL)); 

  if (!hdf_file->ExistInternalObject("STUDY_STRUCTURE")) {
    MESSAGE("SALOMEDS_StudyManager::Open : the study is empty");
    return aStudy._retn();
  }

  //Create  the Structure of the OCAF Document
  HDFgroup *hdf_group_study_structure = new HDFgroup("STUDY_STRUCTURE",hdf_file.get());

  Handle(TDF_Data) DF = Doc->GetData();

  try{
    BuildTree(aStudyServant,hdf_group_study_structure);
  }catch(HDFexception){
    std::ostringstream aStream;
    aStream<<"Can't open file "<<theURL;
    std::string eStr = aStream.str();
    THROW_SALOME_CORBA_EXCEPTION(eStr.c_str(),SALOME::BAD_PARAM);
  } 
  
  hdf_file->CloseOnDisk();

  // Register study in the naming service
  // Path to acces the study
  if(!_name_service.Change_Directory("/Study")){
    MESSAGE( "Unable to access the study directory" );
  }else{
    CORBA::String_var aString(aStudyServant->Name());
    _name_service.Register(aStudy,aString.in());
  }

  if (isASCII) {
    SALOMEDS::ListOfFileNames_var aFilesToRemove = new SALOMEDS::ListOfFileNames;
    aFilesToRemove->length(1);
    std::string aDir = SALOMEDS_Tool::GetDirFromPath(aHDFUrl);
    aFilesToRemove[0] = CORBA::string_dup(&aHDFUrl[strlen(aDir.c_str())]);
    SALOMEDS_Tool::RemoveTemporaryFiles(aDir,aFilesToRemove,true);
  }

  return aStudy._retn();
}



//============================================================================
/*! Function : Close
 *  Purpose  : Close a study.
 *             If the study hasn't been saved, ask the user to confirm the
 *             close action without saving 
 */
//============================================================================
void  SALOMEDS_StudyManager_i::Close(SALOMEDS::Study_ptr aStudy)
{
  SALOMEDS::Locker lock;

  if(aStudy->_is_nil()) return;
  
  aStudy->RemovePostponed(-1);
  
  // Destroy study name in the naming service
  if(_name_service.Change_Directory("/Study")){
    CORBA::String_var aString(aStudy->Name());
    _name_service.Destroy_Name(aString.in());
  }

  aStudy->Close();
}

//============================================================================
/*! Function : Save
 *  Purpose  : Save a Study to it's persistent reference
 */
//============================================================================
void SALOMEDS_StudyManager_i::Save(SALOMEDS::Study_ptr theStudy, CORBA::Boolean theMultiFile)
{
  SALOMEDS::Locker lock;

  CORBA::String_var anURL = theStudy->URL();
  if(strcmp(anURL.in(),"") == 0){
    MESSAGE( "No path specified to save the study. Nothing done");
  }else{
    _SaveAs(anURL,theStudy,theMultiFile,false);
  }
}

void SALOMEDS_StudyManager_i::SaveASCII(SALOMEDS::Study_ptr theStudy, CORBA::Boolean theMultiFile)
{
  SALOMEDS::Locker lock;

  CORBA::String_var anURL = theStudy->URL();
  if(strcmp(anURL.in(),"") == 0){
    MESSAGE( "No path specified to save the study. Nothing done");
  }else{
    _SaveAs(anURL,theStudy,theMultiFile,true);
  }
}

//=============================================================================
/*! Function : SaveAs
 *  Purpose  : Save a study to the persistent reference aUrl
 */
//============================================================================
void SALOMEDS_StudyManager_i::SaveAs(const char* aUrl, SALOMEDS::Study_ptr theStudy, CORBA::Boolean theMultiFile)
{
  SALOMEDS::Locker lock;

  _SaveAs(aUrl,theStudy,theMultiFile, false);

}

void SALOMEDS_StudyManager_i::SaveAsASCII(const char* aUrl, SALOMEDS::Study_ptr theStudy, CORBA::Boolean theMultiFile)
{
  SALOMEDS::Locker lock;

  _SaveAs(aUrl,theStudy,theMultiFile, true);
}

//============================================================================
/*! Function : GetOpenStudies
 *  Purpose  : Get name list of open studies in the session
 */
//============================================================================
SALOMEDS::ListOfOpenStudies*  SALOMEDS_StudyManager_i::GetOpenStudies()
{
  // MESSAGE("Begin of GetOpenStudies");
  SALOMEDS::ListOfOpenStudies_var aStudyList = new SALOMEDS::ListOfOpenStudies;

  if(!_name_service.Change_Directory("/Study")){
    MESSAGE("No active study in this session");
  }else{
    vector<string> aList = _name_service.list_directory();
    aStudyList->length(aList.size());
    for(unsigned int ind = 0; ind < aList.size(); ind++){
      aStudyList[ind] = CORBA::string_dup(aList[ind].c_str());
      SCRUTE(aStudyList[ind]) ;
    }
  }

  return aStudyList._retn();
}

//============================================================================
/*! Function : GetStudyByName
 *  Purpose  : Get a study from its name
 */
//============================================================================
SALOMEDS::Study_ptr  
SALOMEDS_StudyManager_i::GetStudyByName(const char* theStudyName) 
{
  SALOMEDS::Study_var aStudy;

  // Go to study directory and look for aStudyName
  if(!_name_service.Change_Directory("/Study")){
    MESSAGE("No active study in this session");
    ASSERT(false); // Stop here...
  }
  
  if(_name_service.Find(theStudyName) > 0){
    // Study found
    CORBA::Object_ptr anObj = _name_service.Resolve(theStudyName) ;
    aStudy = SALOMEDS::Study::_narrow(anObj);
    MESSAGE("Study " << theStudyName << " found in the naming service");
  }else{
    MESSAGE("Study " << theStudyName << " not found in the naming service");
  }
  return aStudy._retn();
}

//============================================================================
/*! Function : GetStudyByID
 *  Purpose  : Get a study from its ID
 */
//============================================================================
SALOMEDS::Study_ptr  
SALOMEDS_StudyManager_i::GetStudyByID(CORBA::Short aStudyID) 
{
  SALOMEDS::Study_var aStudy;

  if(!_name_service.Change_Directory("/Study")){
    MESSAGE("No active study in this session");
  }else{
    vector<string> aList = _name_service.list_directory();
    for(unsigned int ind = 0; ind < aList.size(); ind++){
      const char* aStudyName = aList[ind].c_str();
      MESSAGE( "GetStudyByID = " << aStudyName );
      if(_name_service.Find(aStudyName) > 0){
	CORBA::Object_ptr anObj = _name_service.Resolve(aStudyName) ;
	aStudy = SALOMEDS::Study::_narrow(anObj);
	MESSAGE( " aStudyID : " << aStudyID << "-" << aStudy->StudyId() );
	if(aStudyID == aStudy->StudyId()){
	  MESSAGE("Study with studyID = " << aStudyID << " found in the naming service");
	  return aStudy._retn();
	}
      }else{
	MESSAGE("Study " << aStudyName << " not found in the naming service");
      }
    }
  }
  
  return aStudy._retn();
}
//============================================================================
/*! Function : SaveAttributes
 *  Purpose  : Save attributes for object
 */
//============================================================================
static void SaveAttributes(SALOMEDS::SObject_ptr SO, HDFgroup *hdf_group_sobject) {
  int a;
  hdf_size size[1];
  SALOMEDS::ListOfAttributes_var anAttrList = SO->GetAllAttributes();
  for(a = anAttrList->length() - 1; a >= 0; a--) {
    if (strcmp(anAttrList[a]->Type(), "AttributeIOR") == 0) continue; // never write AttributeIOR to file
    if (strcmp(anAttrList[a]->Type(), "AttributeExternalFileDef") == 0) continue; // never write ExternalFileDef to file
    if (strcmp(anAttrList[a]->Type(), "AttributeFileType") == 0) continue; // never write FileType to file
    CORBA::String_var aSaveStr(anAttrList[a]->Store());
    size[0] = (hdf_int32) strlen(aSaveStr.in()) + 1;
    HDFdataset *hdf_dataset = new HDFdataset(anAttrList[a]->Type(),hdf_group_sobject,HDF_STRING,size,1);
    hdf_dataset->CreateOnDisk();
    hdf_dataset->WriteOnDisk(aSaveStr);
    hdf_dataset->CloseOnDisk();
    //cout<<"********** Write Attribute "<<anAttrList[a]->Type()<<" : "<<aSaveStr<<" done"<<endl;
    hdf_dataset = 0; //will be deleted by hdf_sco_group destructor
  }

  // Reference attribute has no CORBA attribute representation, so, GetAllAttributes can not return this attribute
  SALOMEDS::SObject_var RefSO;
  if(SO->ReferencedObject(RefSO)) {
    CORBA::String_var attribute_reference(RefSO->GetID());
    size[0] = strlen(attribute_reference) + 1 ; 
    HDFdataset *hdf_dataset = new HDFdataset("Reference",hdf_group_sobject,HDF_STRING,size,1);
    hdf_dataset->CreateOnDisk();
    hdf_dataset->WriteOnDisk(attribute_reference);
    hdf_dataset->CloseOnDisk();
    hdf_dataset = 0; // will be deleted by father hdf object destructor
  }
}

//=============================================================================
/*! Function : _SaveProperties
 *  Purpose  : save the study properties in HDF file
 */
//============================================================================
void SALOMEDS_StudyManager_i::_SaveProperties(SALOMEDS_Study_i* theStudy, HDFgroup *hdf_group) 
{
  // add modifications list (user and date of save)
  SALOMEDS::AttributeStudyProperties_ptr aProp = theStudy->GetProperties();
  int aLocked = aProp->IsLocked();
  if (aLocked) 
    aProp->SetLocked(Standard_False);
  OSD_Process aProcess;
  Quantity_Date aDate = aProcess.SystemDate();
  aProp->SetModification(aProcess.UserName().ToCString(),
			 CORBA::Long(aDate.Minute()), 
			 CORBA::Long(aDate.Hour()), 
			 CORBA::Long(aDate.Day()),
			 CORBA::Long(aDate.Month()), 
			 CORBA::Long(aDate.Year()));
  if(aLocked) 
    aProp->SetLocked(Standard_True);

  SALOMEDS::StringSeq_var aNames;
  SALOMEDS::LongSeq_var aMinutes, aHours, aDays, aMonths, aYears;
  aProp->GetModificationsList(aNames,aMinutes,aHours,aDays,aMonths,aYears,true);

  std::ostringstream aPropertyList;
  aPropertyList<<(strlen(aProp->GetCreationMode()) != 0? aProp->GetCreationMode()[0] : '0');
  aPropertyList<<(aProp->IsLocked()? 'l': 'u');

  int aLength = aNames->length();
  for(int anIndex = 0; anIndex  < aLength; anIndex++) {
    aPropertyList<<std::setw(2)<<aMinutes[anIndex];
    aPropertyList<<std::setw(2)<<aHours[anIndex];
    aPropertyList<<std::setw(2)<<aDays[anIndex];
    aPropertyList<<std::setw(2)<<aMonths[anIndex];
    aPropertyList<<std::setw(4)<<aYears[anIndex];
    aPropertyList<<aNames[anIndex];
    aPropertyList<<char(0x1);
  }
  std::string aProperty = aPropertyList.str();

  hdf_size size[] = {aProperty.size() + 1};
  HDFdataset *hdf_dataset = new HDFdataset("AttributeStudyProperties",hdf_group,HDF_STRING,size,1);
  hdf_dataset->CreateOnDisk();
  hdf_dataset->WriteOnDisk(const_cast<char*>(aProperty.c_str()));
  MESSAGE("attribute StudyProperties " <<  aProperty << " wrote on file");
  hdf_dataset->CloseOnDisk();
  hdf_dataset = 0; //will be deleted by hdf_sco_group destructor
  aProp->SetModified(0);
}

//=============================================================================
/*! Function : _SaveAs
 *  Purpose  : save the study in HDF file
 */
//============================================================================
void SALOMEDS_StudyManager_i::_SaveAs(const char* aUrl, 
				      SALOMEDS::Study_ptr theStudy,
				      CORBA::Boolean theMultiFile,
				      CORBA::Boolean theASCII)
{
  // HDF File will be composed of differents part :
  // * For each ComponentDataType, all data created by the component
  //   Informations in data group hdf_group_datacomponent
  // * Study Structure -> Exactly what is contained in OCAF document
  //   Informations in data group hdf_group_study_structure

  if(SALOMEDS_Study_i* aStudy = DownCast(theStudy)){
    HDFfile *hdf_file=0;         
    HDFgroup *hdf_group_study_structure =0;
    HDFgroup *hdf_sco_group =0;
    HDFgroup *hdf_sco_group2 =0;

    HDFgroup *hdf_group_datacomponent =0;
    HDFdataset *hdf_dataset =0;
    hdf_size size[1];
    hdf_int32 name_len = 0;
  
    int aLocked = aStudy->GetProperties()->IsLocked();
    if(aLocked) 
      aStudy->GetProperties()->SetLocked(false);

    SALOMEDS_StudyBuilder_i* SB= aStudy->GetBuilder();
    try{
      // mpv 15.12.2003: for saving components we have to load all data from all modules
      SALOMEDS_SComponentIterator_i aComponentIter = aStudy->GetComponentIterator();
      for(; aComponentIter.More(); aComponentIter.Next()){
	SALOMEDS::SComponent_var sco = aComponentIter.Value();
	// if there is an associated Engine call its method for saving
	CORBA::String_var IOREngine;
	try{
	  if(!sco->ComponentIOR(IOREngine)){
	    SALOMEDS::GenericAttribute_var aGeneric;
	    SALOMEDS::AttributeName_var aName;
	    if(sco->FindAttribute(aGeneric, "AttributeName"))
	      aName = SALOMEDS::AttributeName::_narrow(aGeneric);
	    
	    if(!aName->_is_nil()){
	      CORBA::String_var aCompType = aName->Value();

	      CORBA::String_var aFactoryType;
	      if(strcmp(aCompType, "SUPERV") == 0) 
		aFactoryType = "SuperVisionContainer";
	      else
		aFactoryType = "FactoryServer";
	      
	      Engines::Component_var aComp =
		SALOME_LifeCycleCORBA(&_name_service).FindOrLoad_Component(aFactoryType, aCompType);
		
	      if(aComp->_is_nil()){
		Engines::Component_var aComp =
		  SALOME_LifeCycleCORBA(&_name_service).FindOrLoad_Component("FactoryServerPy", aCompType);
	      }
		
	      if(!aComp->_is_nil()){
		SALOMEDS::Driver_var aDriver = SALOMEDS::Driver::_narrow(aComp);
		if (!CORBA::is_nil(aDriver)) {
		  // PAL8065: san - _SaveAs() should always be called from some CORBA method protected with a lock
		  SALOMEDS::unlock();
		  SB->LoadWith(sco, aDriver);
		  SALOMEDS::lock();
		}
	      }
	    }
	  }
	}catch(...){
	  MESSAGE("Can not restore information to resave it");
	  return;
	}
      }

      CORBA::String_var anOldName = aStudy->Name();
      aStudy->URL(aUrl);

      // To change for Save 
      // Do not have to do a new file but just a Open??? Rewrite all informations after erasing evrything??
      hdf_file = new HDFfile((char *)aUrl);
      hdf_file->CreateOnDisk();
      MESSAGE("File " << aUrl << " created");

      //-----------------------------------------------------------------------
      // 1 - Create a groupe for each SComponent and Update the PersistanceRef
      //-----------------------------------------------------------------------
      hdf_group_datacomponent = new HDFgroup("DATACOMPONENT",hdf_file);
      hdf_group_datacomponent->CreateOnDisk();

      //SRN: Added 17 Nov, 2003
      SALOMEDS::SObject_var anAutoSaveSO = aStudy->FindObjectID(AUTO_SAVE_TAG);
      //SRN: End

      aComponentIter.Init();
      for(; aComponentIter.More(); aComponentIter.Next()){
	SALOMEDS::SComponent_var sco = aComponentIter.Value();
	  
	CORBA::String_var scoid = sco->GetID();
	hdf_sco_group = new HDFgroup(scoid,hdf_group_datacomponent);
	hdf_sco_group->CreateOnDisk();
	
	CORBA::String_var componentDataType = sco->ComponentDataType();
	MESSAGE ( "Look for  an engine for data type :"<< componentDataType);
	
	//SRN: Added 17 Nov 2003: If there is a specified attribute, the component peforms a special save	  
	if(!CORBA::is_nil(anAutoSaveSO) && SB->IsGUID(sco, AUTO_SAVE_GUID)){	    
	  SALOMEDS::GenericAttribute_var aGeneric;
	  SALOMEDS::AttributeTableOfString_var aTable;
	  if(anAutoSaveSO->FindAttribute(aGeneric, "AttributeTableOfString")){
	    aTable = SALOMEDS::AttributeTableOfString::_narrow(aGeneric);
	    Standard_Integer nbRows = aTable->GetNbRows(), k, aTimeOut = 0;
	    if(nbRows > 0 && aTable->GetNbColumns() > 1) {	
	      SALOMEDS::StringSeq_var aRow;
	      for(k=1; k<=nbRows; k++){
		aRow = aTable->GetRow(k);
		if(strcmp(aRow[0], componentDataType) == 0){
		  CORBA::String_var anEntry = CORBA::string_dup(aRow[1]);
		  SALOMEDS::SObject_var aCompSpecificSO = aStudy->FindObjectID(anEntry);
		  if(!CORBA::is_nil(aCompSpecificSO)) {
		    SALOMEDS::AttributeInteger_var anInteger;
		    if(aCompSpecificSO->FindAttribute(aGeneric, "AttributeInteger")) {
		      anInteger = SALOMEDS::AttributeInteger::_narrow(aGeneric);
		      anInteger->SetValue(-1);
		      while(anInteger->Value() < 0) { sleep(2); if(++aTimeOut > AUTO_SAVE_TIME_OUT_IN_SECONDS) break; }
		    }  // if(aCompSpecificSO->FindAttribute(anInteger, "AttributeInteger"))
		  }  // if(!CORBA::is_nil(aCompSpecificSO)) 
		}  // if (strcmp(aRow[0], componentDataType) == 0)
	      }  // for
	      
	    }  // if(nbRows > 0 && aTable->GetNbColumns() > 1)
	    
	  }  // if(anAutoSaveSO->FindAttribute(aTable, "AttributeTableOfString")
	  
	}  // if(SB->IsGUID(AUTO_SAVE_GUID)
	
	//SRN: End
	
	CORBA::String_var IOREngine;
	if(sco->ComponentIOR(IOREngine)){
	  // we have found the associated engine to write the data 
	  MESSAGE ( "We have found an engine for data type :"<< componentDataType);
	  CORBA::Object_var obj = _orb->string_to_object(IOREngine);
	  SALOMEDS::Driver_var Engine = SALOMEDS::Driver::_narrow(obj) ;
	  
	  if(!CORBA::is_nil(Engine)){
	    MESSAGE ( "Save the data of type:"<< componentDataType);
	    MESSAGE("Engine :"<<Engine->ComponentDataType());
	    
	    SALOMEDS::TMPFile_var aStream;
	    
	    SALOMEDS::unlock();	// asv : fix for PAL8727 
	    if(theASCII) 
	      aStream = Engine->SaveASCII(sco,SALOMEDS_Tool::GetDirFromPath(aUrl).c_str(),theMultiFile);
	    else
	      aStream = Engine->Save(sco,SALOMEDS_Tool::GetDirFromPath(aUrl).c_str(),theMultiFile);
	    SALOMEDS::lock();  // asv : fix for PAL8727

	    HDFdataset *hdf_dataset;
	    hdf_size aHDFSize[1];
	    if(aStream->length() > 0){  //The component saved some auxiliary files, then put them into HDF file 
	      
	      aHDFSize[0] = aStream->length();
	      
	      HDFdataset *hdf_dataset = new HDFdataset("FILE_STREAM", hdf_sco_group, HDF_STRING, aHDFSize, 1);
	      hdf_dataset->CreateOnDisk();
	      hdf_dataset->WriteOnDisk((unsigned char*) &aStream[0]);  //Save the stream in the HDF file
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
	    
	    Translate_IOR_to_persistentID(aStudy,SB,sco,Engine,theMultiFile, theASCII);
	    MESSAGE("After Translate_IOR_to_persistentID");
		  
	    // Creation of the persistance reference  attribute
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
      aComponentIter.Init();
      for(; aComponentIter.More(); aComponentIter.Next()){
	SALOMEDS::SComponent_var SC = aComponentIter.Value();
	
	CORBA::String_var scid = SC->GetID();
	hdf_sco_group2 = new HDFgroup(scid,hdf_group_study_structure);
	hdf_sco_group2->CreateOnDisk();
	SaveAttributes(SC, hdf_sco_group2);
	// ComponentDataType treatment
	CORBA::String_var component_name = SC->ComponentDataType();
	MESSAGE("Component data type " << component_name << " treated");
	
	name_len = (hdf_int32) strlen(component_name.in());
	size[0] = name_len +1 ; 
	hdf_dataset = new HDFdataset("COMPONENTDATATYPE",hdf_sco_group2,HDF_STRING,size,1);
	hdf_dataset->CreateOnDisk();
	hdf_dataset->WriteOnDisk(const_cast<char*>(component_name.in()));
	MESSAGE("component name " <<  component_name << " wrote on file");
	hdf_dataset->CloseOnDisk();
	hdf_dataset=0; //will be deleted by hdf_sco_group destructor
	_SaveObject(aStudy, SC, hdf_sco_group2);
	hdf_sco_group2->CloseOnDisk();
	hdf_sco_group2=0; // will be deleted by hdf_group_study_structure destructor
      }
      //-----------------------------------------------------------------------
      //4 - Write the Study UseCases Structure
      //-----------------------------------------------------------------------
      SALOMEDS::SObject_var aSO = aStudy->FindObjectID(USE_CASE_LABEL_ID);
      if(!aSO->_is_nil()){
	HDFgroup *hdf_soo_group = new HDFgroup(USE_CASE_LABEL_ID,hdf_group_study_structure);
	hdf_soo_group->CreateOnDisk();
	SaveAttributes(aSO, hdf_soo_group);
	_SaveObject(aStudy, aSO, hdf_soo_group);
	MESSAGE("Use cases data structure writed");
	hdf_soo_group->CloseOnDisk();
	hdf_soo_group=0; // will be deleted by hdf_group_study_structure destructor
      }

      if (aLocked) 
	aStudy->GetProperties()->SetLocked(true);
      //-----------------------------------------------------------------------
      //5 - Write the Study Properties
      //-----------------------------------------------------------------------
      name_len = (hdf_int32) strlen(aStudy->Name());
      size[0] = name_len +1 ; 
      hdf_dataset = new HDFdataset("STUDY_NAME",hdf_group_study_structure,HDF_STRING,size,1);
      hdf_dataset->CreateOnDisk();
      CORBA::String_var studid = aStudy->Name();
      hdf_dataset->WriteOnDisk(studid);
      MESSAGE("study name " << studid << " wrote on file");
      hdf_dataset->CloseOnDisk();
      hdf_dataset=0; // will be deleted by hdf_group_study_structure destructor

      _SaveProperties(aStudy, hdf_group_study_structure);

      hdf_group_study_structure->CloseOnDisk();
      hdf_file->CloseOnDisk();

      _name_service.Change_Directory("/Study");
      _name_service.Destroy_Name(anOldName);
      _name_service.Register(theStudy, aStudy->Name());

      aStudy->IsSaved(true);
      hdf_group_study_structure =0; // will be deleted by hdf_file destructor
      delete hdf_file; // recursively deletes all hdf objects...
    }catch(HDFexception){
      MESSAGE( "HDFexception ! " );
    }
    if(theASCII){ // save file in ASCII format
      HDFascii::ConvertFromHDFToASCII(aUrl, true);
    }
  }
}

//============================================================================
/*! Function : _SaveObject
 *  Purpose  :
 */
//============================================================================
void SALOMEDS_StudyManager_i::_SaveObject(SALOMEDS_Study_i* theStudy, 
					  SALOMEDS::SObject_ptr theSObject, 
					  HDFgroup *hdf_group_datatype)
{
  // Write in group hdf_group_datatype all informations of SObject SC
  // Iterative function to parse all SObjects under a SComponent
  SALOMEDS::SObject_var RefSO;
  HDFgroup *hdf_group_sobject = 0;

  SALOMEDS_ChildIterator_i aChildIter = theStudy->GetChildIterator(theSObject);
  for(; aChildIter.More(); aChildIter.Next()){
    SALOMEDS::SObject_var aSObject = aChildIter.Value();
    SALOMEDS::ListOfAttributes_var anAllAttributes = aSObject->GetAllAttributes();
    
    // mpv: don't save empty labels
    if(anAllAttributes->length() == 0 && !aSObject->ReferencedObject(RefSO)){
      SALOMEDS_ChildIterator_i aSubChildIter = theStudy->GetChildIterator(theSObject);
      if(!aSubChildIter.More())
	continue;

      aSubChildIter.InitEx(true);
      bool anEmpty = true;
      for(; aSubChildIter.More() && anEmpty; aSubChildIter.Next()){
	SALOMEDS::SObject_var aSObj = aSubChildIter.Value();
	SALOMEDS::ListOfAttributes_var anAllAttr = aSObj->GetAllAttributes();
	if(anAllAttr->length() != 0 || aSObj->ReferencedObject(RefSO)) 
	  anEmpty = false;
      }
      if(anEmpty)
	continue;
    }

    CORBA::String_var scoid(aSObject->GetID());
    hdf_group_sobject = new HDFgroup(scoid,hdf_group_datatype);
    hdf_group_sobject->CreateOnDisk();
    SaveAttributes(aSObject, hdf_group_sobject);
    _SaveObject(theStudy,aSObject, hdf_group_sobject);
    hdf_group_sobject->CloseOnDisk();
    hdf_group_sobject =0; // will be deleted by father hdf object destructor
  }
}

//============================================================================
/*! Function : _SubstituteSlash
 *  Purpose  :
 */
//============================================================================

std::string SALOMEDS_StudyManager_i::_SubstituteSlash(const char *theUrl)
{
  ASSERT(1==0);
  TCollection_ExtendedString aUrl(const_cast<char*>(theUrl));
  aUrl.ChangeAll(ToExtCharacter('/'),ToExtCharacter(':'));
  TCollection_AsciiString ch(aUrl);
  return ch.ToCString();
}

//============================================================================
/*! Function : CanCopy
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_StudyManager_i::CanCopy(SALOMEDS::SObject_ptr theObject) {
  SALOMEDS::Driver_var Engine;
  { // Guarded block of code
    SALOMEDS::Locker lock;

    SALOMEDS::SComponent_var aComponent = theObject->GetFatherComponent();

    if(aComponent->_is_nil()) 
      return false;

    if(aComponent == theObject) 
      return false;

    CORBA::String_var IOREngine;
    if(!aComponent->ComponentIOR(IOREngine)) 
      return false;

    CORBA::Object_var obj = _orb->string_to_object(IOREngine);
    Engine = SALOMEDS::Driver::_narrow(obj);
    if (CORBA::is_nil(Engine)) 
      return false;
  } // End of guarded block of code
  return Engine->CanCopy(theObject);
}

//============================================================================
/*! Function : CopyLabel
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyManager_i::CopyLabel(SALOMEDS_Study_i* theSourceStudy,
					const SALOMEDS::Driver_ptr theEngine,
					const Standard_Integer theSourceStartDepth,
					const TDF_Label& theSource,
					const TDF_Label& theDestinationMain) 
{
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
    if (!Handle(TDataStd_TreeNode)::DownCast(anAttr).IsNull()) continue; // never copy tree node attribute
    if (!Handle(SALOMEDS_TargetAttribute)::DownCast(anAttr).IsNull()) continue; // and target attribute
    
    if (!Handle(TDF_Reference)::DownCast(anAttr).IsNull()) { // reference copied as Comment in auxiliary tree
      TDF_Label aReferenced = Handle(TDF_Reference)::DownCast(anAttr)->Get();
      TCollection_AsciiString anEntry;
      TDF_Tool::Entry(aReferenced, anEntry);
      // store the value of name attribute of referenced label
      Handle(TDataStd_Name) aNameAttribute;
      if (aReferenced.FindAttribute(TDataStd_Name::GetID(), aNameAttribute)) {
	anEntry += " ";
	anEntry += aNameAttribute->Get();
      }
      TDataStd_Comment::Set(aAuxTargetLabel, TCollection_ExtendedString(anEntry));
      continue;
    }
    
    if (Handle(SALOMEDS_IORAttribute)::DownCast(anAttr).IsNull()) { // IOR => ID and TMPFile of Engine
      Handle(TDF_Attribute) aNewAttribute = anAttr->NewEmpty();
      aTargetLabel.AddAttribute(aNewAttribute);
      anAttr->Paste(aNewAttribute, aRT);
      continue;
    }

    TCollection_AsciiString anEntry;
    TDF_Tool::Entry(theSource, anEntry);
    SALOMEDS::SObject_var aSO = theSourceStudy->FindObjectID(anEntry.ToCString());
    CORBA::Long anObjID;

    // PAL8065: san - CopyLabel() should always be called from some CORBA method protected with a lock
    SALOMEDS::unlock();
    SALOMEDS::TMPFile_var aStream = theEngine->CopyFrom(aSO, anObjID);
    SALOMEDS::lock();

    int aLen = aStream->length();
    TCollection_ExtendedString aResStr("");
    for(a = 0; a < aLen; a++) {
      aResStr += TCollection_ExtendedString(ToExtCharacter((Standard_Character)aStream[a]));
    }
    TDataStd_Integer::Set(aAuxTargetLabel, anObjID);
    TDataStd_Name::Set(aAuxTargetLabel, aResStr);

//      aRT->SetRelocation(anAttr, aNewAttribute);
  }
}

//============================================================================
/*! Function : Copy
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_StudyManager_i::Copy(SALOMEDS::SObject_ptr theObject) {
  SALOMEDS::Locker lock;
   
  // adoptation for alliances datamodel copy: without IOR attributes !!!
  // copy only SObjects and attributes without component help
  SALOMEDS::GenericAttribute_var anAttribute;
  bool aStructureOnly = !theObject->FindAttribute(anAttribute, "AttributeIOR");

  PortableServer::ServantBase_var aServant = GetServant(theObject,_poa);
  SALOMEDS_SObject_i* aSObject = dynamic_cast<SALOMEDS_SObject_i*>(aServant.in());
  if(aSObject == NULL) 
    return false;

  SALOMEDS_Study_i* aStudy = aSObject->GetStudyServant();
  SALOMEDS::Driver_var anEngine;
  CORBA::String_var aString;
  if (!aStructureOnly) {
    SALOMEDS::SComponent_var aComponent = theObject->GetFatherComponent();
    if(!aComponent->ComponentIOR(aString)) 
      return false;

    CORBA::Object_var anObj = _orb->string_to_object(aString);
    anEngine = SALOMEDS::Driver::_narrow(anObj) ;
  }

  // CAF document of current study usage
  Handle(TDocStd_Document) aDocument = aStudy->GetDocument();
  if(aDocument.IsNull()) 
    return false;

  // create new document for clipboard
  Handle(TDocStd_Document) aTargetDocument;
  _OCAFApp->NewDocument("SALOME_STUDY", aTargetDocument);
  // set component data type to the name attribute of root label
  if(!aStructureOnly){
    aString = anEngine->ComponentDataType();
    TDataStd_Comment::Set(aTargetDocument->Main().Root(),const_cast<char*>(aString.in()));
  }
  // set to the Root label integer attribute: study id
  TDataStd_Integer::Set(aTargetDocument->Main().Root(),aStudy->StudyId());

  // iterate all theObject's label children
  TDF_Label aStartLabel;
  aString = theObject->GetID();
  TDF_Tool::Label(aDocument->GetData(),const_cast<char*>(aString.in()),aStartLabel);
  Standard_Integer aSourceStartDepth = aStartLabel.Depth();
  
  // copy main source label
  CopyLabel(aStudy,anEngine,aSourceStartDepth,aStartLabel,aTargetDocument->Main());

  // copy all subchildren of the main source label (all levels)
  TDF_ChildIterator anIterator(aStartLabel,Standard_True);
  for(; anIterator.More(); anIterator.Next()){
    CopyLabel(aStudy,anEngine,aSourceStartDepth,anIterator.Value(),aTargetDocument->Main());
  }

  // done: free old clipboard document and 
  if (!_clipboard.IsNull()) {
//      Handle(TDocStd_Owner) anOwner;
//      if (_clipboard->Main().Root().FindAttribute(TDocStd_Owner::GetID(), anOwner)) {
//        Handle(TDocStd_Document) anEmptyDoc;
//        anOwner->SetDocument(anEmptyDoc);
//      }
    _OCAFApp->Close(_clipboard);
  }

  _clipboard = aTargetDocument;

  return true;
}
//============================================================================
/*! Function : CanPaste
 *  Purpose  :
 */
//============================================================================
CORBA::Boolean SALOMEDS_StudyManager_i::CanPaste(SALOMEDS::SObject_ptr theObject) {
  CORBA::String_var aName;
  Standard_Integer anID;
  SALOMEDS::Driver_var Engine;
  { // Guarded block of code
    SALOMEDS::Locker lock;

    if (_clipboard.IsNull()) return false;

    Handle(TDataStd_Comment) aCompName;
    if (!_clipboard->Main().Root().FindAttribute(TDataStd_Comment::GetID(), aCompName)) return false;
    Handle(TDataStd_Integer) anObjID;
    if (!_clipboard->Main().Father().FindChild(2).FindAttribute(TDataStd_Integer::GetID(), anObjID))
      return false;

    SALOMEDS::SComponent_var aComponent = theObject->GetFatherComponent();
    if(aComponent->_is_nil()) 
      return false;
  
    CORBA::String_var IOREngine;
    if(!aComponent->ComponentIOR(IOREngine)) 
      return false;
  
    CORBA::Object_var obj = _orb->string_to_object(IOREngine);
    Engine = SALOMEDS::Driver::_narrow(obj) ;
    if (CORBA::is_nil(Engine)) 
      return false;

    aName = CORBA::string_dup( TCollection_AsciiString(aCompName->Get()).ToCString() );
    anID   = anObjID->Get();
  } // End of guarded block of code
  return Engine->CanPaste(aName.in(),anID);
}
//============================================================================
/*! Function : PasteLabel
 *  Purpose  :
 */
//============================================================================
TDF_Label SALOMEDS_StudyManager_i::PasteLabel(SALOMEDS_Study_i* theDestinationStudy,
					      const SALOMEDS::Driver_ptr theEngine,
					      const TDF_Label& theSource,
					      const TDF_Label& theDestinationStart,
					      const int theCopiedStudyID,
					      const bool isFirstElement) 
{

  // get corresponding source, target and auxiliary labels
  TDF_Label aTargetLabel = theDestinationStart;
  TDF_Label aAuxSourceLabel = theSource.Root().FindChild(2);
  int a;
  if (!isFirstElement) {
    for(a = theSource.Depth() - 1; a > 0 ; a--) {
      TDF_Label aSourceLabel = theSource;
      for(int aNbFather = 1; aNbFather < a; aNbFather++)
	aSourceLabel = aSourceLabel.Father();
      aTargetLabel = aTargetLabel.FindChild(aSourceLabel.Tag());
      aAuxSourceLabel = aAuxSourceLabel.FindChild(aSourceLabel.Tag());
    }
  }

  // check auxiliary label for TMPFile => IOR
  Handle(TDataStd_Name) aNameAttribute;
  if (aAuxSourceLabel.FindAttribute(TDataStd_Name::GetID(), aNameAttribute)) {
    Handle(TDataStd_Integer) anObjID;

    aAuxSourceLabel.FindAttribute(TDataStd_Integer::GetID(), anObjID);
    Handle(TDataStd_Comment) aComponentName;
    theSource.Root().FindAttribute(TDataStd_Comment::GetID(), aComponentName);
    std::string aCompName = TCollection_AsciiString(aComponentName->Get()).ToCString();
    if (theEngine->CanPaste(aCompName.c_str(),anObjID->Get())) {
      SALOMEDS::TMPFile_var aTMPFil = new SALOMEDS::TMPFile();
      TCollection_ExtendedString aTMPStr = aNameAttribute->Get();
      int aLen = aTMPStr.Length();
      aTMPFil->length(aLen);
      for(a = 0; a < aLen; a++) {
	aTMPFil[a] = ToCharacter(aTMPStr.Value(a+1));
      }
      TCollection_AsciiString anEntry;
      TDF_Tool::Entry(aTargetLabel, anEntry);
      SALOMEDS::SObject_var aPastedSO = theDestinationStudy->FindObjectID(anEntry.ToCString());
      if(isFirstElement){
	// PAL8065: san - PasteLabel() should always be called from some CORBA method protected with a lock
	SALOMEDS::unlock();
	SALOMEDS::SObject_var aDestSO =
	  theEngine->PasteInto(aTMPFil.in(),
			       anObjID->Get(),
			       aPastedSO->GetFatherComponent());
	SALOMEDS::lock();
	TDF_Tool::Label(theDestinationStart.Data(), aDestSO->GetID(), aTargetLabel);
      }else 
	// PAL8065: san - PasteLabel() should always be called from some CORBA method protected with a lock
	SALOMEDS::unlock();
	theEngine->PasteInto(aTMPFil.in(),anObjID->Get(),aPastedSO);
	SALOMEDS::lock();
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
//      aRT->SetRelocation(anAttr, aNewAttribute);
  }
  // check auxiliary label for Comment => reference or name attribute of the referenced object
  Handle(TDataStd_Comment) aCommentAttribute;
  if (aAuxSourceLabel.FindAttribute(TDataStd_Comment::GetID(), aCommentAttribute)) {
    std::string anEntry(TCollection_AsciiString(aCommentAttribute->Get()).ToCString());
    std::size_t aNameStart = anEntry.find(' ');
    std::string aName;
    if(aNameStart != std::string::npos){
      aName = anEntry.substr(aNameStart+1);
      anEntry = anEntry.substr(0,aNameStart);
    }
    if (theCopiedStudyID == theDestinationStudy->StudyId()) { // if copy to the same study, reanimate reference
      TDF_Label aRefLabel;
      TDF_Tool::Label(aTargetLabel.Data(),&anEntry[0],aRefLabel);
      TDF_Reference::Set(aTargetLabel, aRefLabel);
      SALOMEDS_TargetAttribute::Set(aRefLabel)->Append(aTargetLabel); // target attributes structure support
    } else {
      if(aNameStart != std::string::npos)
	TDataStd_Name::Set(aTargetLabel, &aName[0]);
      else
	TDataStd_Name::Set(aTargetLabel, 
			   TCollection_ExtendedString("Reference to:") + &anEntry[0]);
    }
  }

  return aTargetLabel;
}
//============================================================================
/*! Function : Paste
 *  Purpose  :
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_StudyManager_i::Paste(SALOMEDS::SObject_ptr theObject)
     throw(SALOMEDS::StudyBuilder::LockProtection)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(LockProtection);

  PortableServer::ServantBase_var aServant = GetServant(theObject,_poa);
  SALOMEDS_SObject_i* aSObject = dynamic_cast<SALOMEDS_SObject_i*>(aServant.in());
  if(aSObject == NULL) 
    return false;

  SALOMEDS_Study_i* aStudy = aSObject->GetStudyServant();

  // if study is locked, then paste can't be done
  if (aStudy->GetProperties()->IsLocked())
    throw SALOMEDS::StudyBuilder::LockProtection();

  // if there is no component name, then paste only SObjects and attributes: without component help
  Handle(TDataStd_Comment) aComponentName;
  bool aStructureOnly = !_clipboard->Main().Root().FindAttribute(TDataStd_Comment::GetID(), aComponentName);

  // get copied study ID
  Handle(TDataStd_Integer) aStudyIDAttribute;
  if (!_clipboard->Main().Root().FindAttribute(TDataStd_Integer::GetID(), aStudyIDAttribute))
    return SALOMEDS::SObject::_nil();

  // get component-engine
  SALOMEDS::SComponent_var aComponent;
  SALOMEDS::Driver_var anEngine;
  CORBA::String_var aString;
  if (!aStructureOnly) {
    aComponent = theObject->GetFatherComponent();
    if(!aComponent->ComponentIOR(aString)) 
      return SALOMEDS::SObject::_nil();

    CORBA::Object_var anObj = _orb->string_to_object(aString);
    anEngine = SALOMEDS::Driver::_narrow(anObj) ;
  }

  // CAF document of current study usage
  Handle(TDocStd_Document) aDocument = aStudy->GetDocument();
  if (aDocument.IsNull()) 
    return SALOMEDS::SObject::_nil();

  // fill root inserted SObject
  int aCStudyID = aStudyIDAttribute->Get();
  TDF_Label aLabel = aStructureOnly? aSObject->GetLabel(): aSObject->GetFatherComponentLabel();
  TDF_Label aStartLabel = PasteLabel(aStudy,anEngine,_clipboard->Main(),aLabel,aCStudyID,true);

  // paste all sublebels
  TDF_ChildIterator anIterator(_clipboard->Main(),Standard_True);
  for(; anIterator.More(); anIterator.Next()) {
    PasteLabel(aStudy,anEngine,anIterator.Value(),aStartLabel,aCStudyID,false);
  }

  return SALOMEDS_SObject_i::NewRef(aStudy,aStartLabel)._retn();
}
