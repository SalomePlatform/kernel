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
//  File   : SALOMEDS_StudyBuilder_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include "SALOMEDS_StudyBuilder_i.hxx"
#include "SALOMEDS_StudyManager_i.hxx"
#include "SALOMEDS_Study_i.hxx"

#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_SComponent_i.hxx"
#include "SALOMEDS_ChildIterator_i.hxx"

#include "SALOMEDS_TargetAttribute.hxx"
#include "SALOMEDS_IORAttribute.hxx"
#include "SALOMEDS_PersRefAttribute.hxx"
#include "SALOMEDS_LocalIDAttribute.hxx"
#include "SALOMEDS_StudyPropertiesAttribute.hxx"

#include "SALOMEDS_Tool.hxx"
#include "SALOMEDS.hxx"

#include "Utils_CorbaException.hxx"
#include "Utils_ExceptHandlers.hxx"

#include <TDF_ChildIterator.hxx>
#include <TDF_Label.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_Comment.hxx>
#include <TDataStd_UAttribute.hxx>
#include <TDataStd_Real.hxx>
#include <TDF_Tool.hxx>
#include <TDF_Reference.hxx>
#include <TDF_Data.hxx>
#include <TDataStd_ChildNodeIterator.hxx>
#include <TDF_ListIteratorOfAttributeList.hxx>

#include <HDFOI.hxx>
#include <stdlib.h> 

#define USE_CASE_LABEL_TAG            2
#define DIRECTORYID 16661
#define FILELOCALID 26662 

#include "utilities.h"

using namespace std;

UNEXPECT_CATCH(SBSalomeException, SALOME::SALOME_Exception);
UNEXPECT_CATCH(SBLockProtection, SALOMEDS::StudyBuilder::LockProtection);

//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_StudyBuilder_i::SALOMEDS_StudyBuilder_i(SALOMEDS_Study_i* theStudy,
						 const Handle(TDocStd_Document)& theDocument):
  _study(theStudy),
  _doc(theDocument)
{
}

//============================================================================
/*! Function : destructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_StudyBuilder_i::~SALOMEDS_StudyBuilder_i()
{
}


//============================================================================
CORBA::ORB_var SALOMEDS_StudyBuilder_i::GetORB() const
{
  return _study->GetORB();
}


//============================================================================
PortableServer::POA_var SALOMEDS_StudyBuilder_i::GetPOA() const
{
  return _study->GetPOA();
}


//============================================================================
/*! Function : NewComponent
 *  Purpose  : Create a new component (Scomponent)
 */
//============================================================================
SALOMEDS::SComponent_ptr 
SALOMEDS_StudyBuilder_i::NewComponent(const char* DataType)
{
  SALOMEDS::Locker lock;

  CheckLocked();
  //Always create component under main label.
  TDF_Label L  = _doc->Main();

  // YFR DEBUG : 13/02/2002 TDF_Label NL = L.NewChild();
  
  Standard_Integer imax = 0;
  for (TDF_ChildIterator it(L); it.More(); it.Next()) {
    if (it.Value().Tag() > imax)
      imax = it.Value().Tag();
  }
  imax++;
  TDF_Label NL = L.FindChild(imax);

   TDataStd_Comment::Set(NL,Standard_CString(DataType));
   //  TDataStd_Comment::Set(NL,Standard_CString(CORBA::string_dup(DataType)));

  SALOMEDS::SComponent_var aSComponent = SALOMEDS_SComponent_i::NewRef(_study,NL); 

  OnAddSObject(aSComponent);

  return aSComponent._retn();
}

//============================================================================
/*! Function : DefineComponentInstance
 *  Purpose  : Add IOR attribute of a Scomponent
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::DefineComponentInstance(SALOMEDS::SComponent_ptr theComponent,
						      CORBA::Object_ptr theObject)
{
  SALOMEDS::Locker lock;

  CheckLocked();

  if(CORBA::is_nil(theComponent) || CORBA::is_nil(theObject))
    return;

  //Find label
  TDF_Label Lab;
  CORBA::String_var aString = theComponent->GetID();
  TDF_Tool::Label(_doc->GetData(),const_cast<char*>(aString.in()),Lab);

  //add theObject definition 
  aString = GetORB()->object_to_string(theObject);
  SALOMEDS_IORAttribute::Set(Lab,const_cast<char*>(aString.in()),_study);
}

//============================================================================
/*! Function : RemoveComponent
 *  Purpose  : Delete a Scomponent
 */
//============================================================================
void 
SALOMEDS_StudyBuilder_i::RemoveComponent(SALOMEDS::SComponent_ptr theComponent)
{
  SALOMEDS::Locker lock;

  CheckLocked();
  RemoveObject(theComponent);
}

//============================================================================
/*! Function : NewObject
 *  Purpose  : Create a new SObject
 */
//============================================================================
SALOMEDS::SObject_ptr 
SALOMEDS_StudyBuilder_i::NewObject(SALOMEDS::SObject_ptr theFatherObject)
{
  SALOMEDS::Locker lock;

  CheckLocked();

  if(CORBA::is_nil(theFatherObject)) 
    return SALOMEDS::SObject::_nil();

  //Find label of father
  TDF_Label aLabel;
  TCollection_AsciiString anEntry;
  
  CORBA::String_var aFatherID = theFatherObject->GetID();
  TDF_Tool::Label(_doc->GetData(),aFatherID,aLabel);

  //Create a new label
  //YFR DEBUG : 13/02/2002  TDF_Label NewLab = Lab.NewChild();
  Standard_Integer imax = 0;
  for (TDF_ChildIterator it(aLabel); it.More(); it.Next()) {
    if (it.Value().Tag() > imax)
      imax = it.Value().Tag();
  }
  imax++;
  TDF_Label aNewLabel = aLabel.FindChild(imax);
  
  SALOMEDS::SObject_var aSObject = SALOMEDS_SObject_i::NewRef(_study,aNewLabel); 

  OnAddSObject(aSObject);

  return aSObject._retn();
}

//============================================================================
/*! Function : NewObjectToTag
 *  Purpose  :
 */
//============================================================================
SALOMEDS::SObject_ptr 
SALOMEDS_StudyBuilder_i::NewObjectToTag(SALOMEDS::SObject_ptr theFatherObject,
					CORBA::Long theTag)
{
  SALOMEDS::Locker lock;

  CheckLocked();

  if(CORBA::is_nil(theFatherObject)) 
    return SALOMEDS::SObject::_nil();

  //Find label of father
  TDF_Label Lab;
  CORBA::String_var aFatherID = theFatherObject->GetID();
  TDF_Tool::Label(_doc->GetData(),aFatherID,Lab);

  //Create or find label
  TDF_Label aNewLab = Lab.FindChild(theTag,1);

  SALOMEDS::SObject_var aSObject = SALOMEDS_SObject_i::NewRef(_study,aNewLab); 

  OnAddSObject(aSObject);

  return aSObject._retn();
}

//============================================================================
/*! Function : RemoveObject
 *  Purpose  :
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::RemoveObject(SALOMEDS::SObject_ptr theSObject)
{
  SALOMEDS::Locker lock;

  RemoveSObject(theSObject);
}

SALOMEDS_SObject_i*
SALOMEDS_StudyBuilder_i::RemoveSObject(SALOMEDS::SObject_ptr theSObject,
				       bool theIsForgetAllAttributes)
{
  CheckLocked();

  if(SALOMEDS_SObject_i* aSObject = _study->DownCast(theSObject)){
    OnRemoveSObject(theSObject);
    aSObject->OnRemove();
    if(theIsForgetAllAttributes){
      TDF_Label aLabel = aSObject->GetLabel();
      aLabel.ForgetAllAttributes();
    }
    return aSObject;
  }

  return NULL;
}

//============================================================================
/*! Function : RemoveObjectWithChildren
 *  Purpose  :
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::RemoveObjectWithChildren(SALOMEDS::SObject_ptr theSObject)
{
  SALOMEDS::Locker lock;

  if(SALOMEDS_SObject_i* aSObject = RemoveSObject(theSObject,false)){
    SALOMEDS_ChildIterator_i aChildIter(_study,aSObject->GetLabel(),true);
    for(; aChildIter.More(); aChildIter.Next()){
      if(SALOMEDS_SObject_i* aSObj = aChildIter.GetValue())
	aSObj->OnRemove();
    }
    TDF_Label aLabel = aSObject->GetLabel();
    aLabel.ForgetAllAttributes(Standard_True);
  }
}

//============================================================================
/*! Function : Translate_persistentID_to_IOR
 *  Purpose  :
 */
//============================================================================
static void  Translate_persistentID_to_IOR(TDF_Label theLabel,
					   SALOMEDS::Driver_ptr theDriver,
					   SALOMEDS_Study_i* theStudy,
					   CORBA::Boolean theIsMultiFile,
					   CORBA::Boolean theIsASCII)
{
  if(CORBA::is_nil(theDriver))
    return;

  for (TDF_ChildIterator  aChildIter (theLabel); aChildIter.More(); aChildIter.Next()){
    TDF_Label aCurrentLabel = aChildIter.Value();
    Handle(TDF_Attribute) anAttr;
    if(aCurrentLabel.FindAttribute(SALOMEDS_PersRefAttribute::GetID(),anAttr)){
      Handle(SALOMEDS_LocalIDAttribute) anID;
      if (aCurrentLabel.FindAttribute(SALOMEDS_LocalIDAttribute::GetID(),anID)) 
	if (anID->Get() == FILELOCALID) continue;        //SRN: This attribute store a file name, skip it 

      TCollection_ExtendedString res = Handle(TDataStd_Comment)::DownCast(anAttr)->Get();
      TCollection_AsciiString ch(res);
      
      SALOMEDS::SObject_var aSObject = SALOMEDS_SObject_i::NewRef(theStudy,aCurrentLabel); 
      
      // PAL8065: san - Translate_persistentID_to_IOR() should always be called from some CORBA method 
      // protected with a lock
      SALOMEDS::unlock();
      CORBA::String_var anIOR = 
	theDriver->LocalPersistentIDToIOR(aSObject,ch.ToCString(),theIsMultiFile,theIsASCII);
      SALOMEDS::lock();

      SALOMEDS_IORAttribute::Set(aCurrentLabel,const_cast<char*>(anIOR.in()),theStudy); 
    }

    Translate_persistentID_to_IOR(aCurrentLabel,theDriver,theStudy,theIsMultiFile,theIsASCII);
  }
}

//============================================================================
/*! Function : LoadWith
 *  Purpose  : 
 */
//============================================================================

//============================================================================
void SALOMEDS_StudyBuilder_i::LoadWith(SALOMEDS::SComponent_ptr theSComponent, 
				       SALOMEDS::Driver_ptr theDriver) 
  throw(SALOME::SALOME_Exception)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(SBSalomeException);

  if(CORBA::is_nil(theSComponent))
    return;

  TDF_Label Lab;
  CORBA::String_var aString = theSComponent->GetID();
  TDF_Tool::Label(_doc->GetData(),const_cast<char*>(aString.in()),Lab);
  
  //Find the current Url of the study  
  Handle(TDF_Attribute) Att;
  if (_doc->Main().FindAttribute(SALOMEDS_PersRefAttribute::GetID(),Att)) {
    if(CORBA::is_nil(theDriver))
      return;

    int aLocked = _study->GetProperties()->IsLocked();
    if(aLocked) 
      _study->GetProperties()->SetLocked(false);
    
    // mpv 06.03.2003: SAL1927 - if component data if already loaded, it is not necessary to do it again
    if (Lab.FindAttribute(SALOMEDS_IORAttribute::GetID(), Att)) {
      if(aLocked) 
	_study->GetProperties()->SetLocked(true);
      return;
    }
    DefineComponentInstance (theSComponent,theDriver);
    
    TCollection_AsciiString aHDFPath(Handle(TDataStd_Comment)::DownCast(Att)->Get());
    
    bool isASCII = false;
    std::ostringstream anURLStream;
    if(HDFascii::isASCII(aHDFPath.ToCString())){
      isASCII = true;
      auto_ptr<char> aResultPath(HDFascii::ConvertFromASCIIToHDF(aHDFPath.ToCString()));
      anURLStream<<aResultPath.get()<<"hdf_from_ascii.hdf";
    } else {
      anURLStream<<aHDFPath.ToCString();
    }
    std::string aHDFUrl = anURLStream.str();

    //Open the Study HDF file 
    auto_ptr<HDFfile> hdf_file(new HDFfile(const_cast<char*>(aHDFUrl.c_str())));
    
    char aMultifileState[2];
    char ASCIIfileState[2];
    try {
      CORBA::String_var scoid = theSComponent->GetID();
      hdf_file->OpenOnDisk(HDF_RDONLY);
      HDFgroup *hdf_group = new HDFgroup("DATACOMPONENT",hdf_file.get());
      hdf_group->OpenOnDisk();
      HDFgroup *hdf_sco_group = new HDFgroup(scoid, hdf_group);
      hdf_sco_group->OpenOnDisk();
	
      SALOMEDS::TMPFile_var aStreamFile;
      if (hdf_sco_group->ExistInternalObject("FILE_STREAM")) {
	HDFdataset *hdf_dataset = new HDFdataset("FILE_STREAM", hdf_sco_group);
	hdf_dataset->OpenOnDisk();
	int aStreamSize = hdf_dataset->GetSize();
	unsigned char* aBuffer = new unsigned char[aStreamSize];
	if(aBuffer == NULL) 
	  throw HDFexception("Unable to open dataset FILE_STREAM");
	hdf_dataset->ReadFromDisk(aBuffer);
	aStreamFile = new SALOMEDS::TMPFile(aStreamSize, aStreamSize, aBuffer, 1);
	hdf_dataset->CloseOnDisk();
	hdf_dataset = 0;
      } else 
	aStreamFile = new SALOMEDS::TMPFile(0);
      
      HDFdataset *multifile_hdf_dataset = new HDFdataset("MULTIFILE_STATE", hdf_sco_group);
      multifile_hdf_dataset->OpenOnDisk();
      multifile_hdf_dataset->ReadFromDisk(aMultifileState);
      
      HDFdataset *ascii_hdf_dataset = new HDFdataset("ASCII_STATE", hdf_sco_group);
      ascii_hdf_dataset->OpenOnDisk();
      ascii_hdf_dataset->ReadFromDisk(ASCIIfileState);
      
      // set path without file name from URL 
      std::string aDir(aHDFPath.ToCString());
      aDir = aDir.substr(0,aDir.rfind('/') + 1);
      
      SALOMEDS::unlock();
      CORBA::Boolean aResult = (ASCIIfileState[0]=='A')?
	theDriver->LoadASCII(theSComponent, aStreamFile.in(), aDir.c_str(), aMultifileState[0]=='M'):
	  theDriver->Load(theSComponent, aStreamFile.in(), aDir.c_str(), aMultifileState[0]=='M');
      SALOMEDS::lock();

      if(!aResult) {
	RemoveAttribute( theSComponent, "AttributeIOR" );
	if (isASCII) {
	  SALOMEDS::ListOfFileNames_var aFilesToRemove = new SALOMEDS::ListOfFileNames;
	  aFilesToRemove->length(1);
	  std::string aDir = SALOMEDS_Tool::GetDirFromPath(aHDFUrl);
	  aFilesToRemove[0] = CORBA::string_dup(&aHDFUrl[strlen(aDir.c_str())]);
	  SALOMEDS_Tool::RemoveTemporaryFiles(aDir,aFilesToRemove,true);
	}
	MESSAGE("Can't load component");
	THROW_SALOME_CORBA_EXCEPTION("Unable to load component data",SALOME::BAD_PARAM);
      }
      
      multifile_hdf_dataset->CloseOnDisk();
      multifile_hdf_dataset = 0;
      ascii_hdf_dataset->CloseOnDisk();
      ascii_hdf_dataset = 0;
      hdf_sco_group->CloseOnDisk();
      hdf_sco_group = 0;
      hdf_group->CloseOnDisk();
      hdf_group = 0;
      hdf_file->CloseOnDisk();
      
      if (isASCII) {
	SALOMEDS::ListOfFileNames_var aFilesToRemove = new SALOMEDS::ListOfFileNames;
	aFilesToRemove->length(1);
	std::string aDir = SALOMEDS_Tool::GetDirFromPath(aHDFUrl);
	aFilesToRemove[0] = CORBA::string_dup(&aHDFUrl[strlen(aDir.c_str())]);
	SALOMEDS_Tool::RemoveTemporaryFiles(aDir,aFilesToRemove,true);
      }
    }
    catch (HDFexception) {
      INFOS("No persistent file Name");
      if (isASCII) {
	SALOMEDS::ListOfFileNames_var aFilesToRemove = new SALOMEDS::ListOfFileNames;
	aFilesToRemove->length(1);
	std::string aDir = SALOMEDS_Tool::GetDirFromPath(aHDFUrl);
	aFilesToRemove[0] = CORBA::string_dup(&aHDFUrl[strlen(aDir.c_str())]);
	SALOMEDS_Tool::RemoveTemporaryFiles(aDir,aFilesToRemove,true);
      }
      if(aLocked)
	_study->GetProperties()->SetLocked(true);
      THROW_SALOME_CORBA_EXCEPTION("No persistent file Name found",SALOME::BAD_PARAM);
    }
    
    try {
      Translate_persistentID_to_IOR(Lab,theDriver,_study,aMultifileState[0]=='M',ASCIIfileState[0] == 'A');
    } catch (SALOME::SALOME_Exception) {
      INFOS("Can't translate PersRef to IOR");
      if (aLocked) 
	_study->GetProperties()->SetLocked(true);
      THROW_SALOME_CORBA_EXCEPTION("Unable to convert component persistent data to the transient",SALOME::BAD_PARAM);
      //        throw HDFexception("Unable to load component data");
    }
    if(aLocked)
      _study->GetProperties()->SetLocked(true);
  } else
    MESSAGE("No persistent file Name");
}


//============================================================================
/*! Function : Load
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::Load(SALOMEDS::SObject_ptr sco)
{
  MESSAGE ( "This function is not yet implemented");
}

//============================================================================
/*! Function : FindOrCreateAttribute
 *  Purpose  : Add attribute of given type to SObject, if there is attribute of such type, returns
 *  existing one
 */
//============================================================================
SALOMEDS::GenericAttribute_ptr 
SALOMEDS_StudyBuilder_i::FindOrCreateAttribute(SALOMEDS::SObject_ptr theObject, 
					       const char* theTypeOfAttribute)
{
  SALOMEDS::Locker lock;

  if(SALOMEDS_SObject_i* aSObject = _study->DownCast(theObject))
    return aSObject->FindOrCreateAttribute(theTypeOfAttribute);

  return SALOMEDS::GenericAttribute::_nil();
}

//============================================================================
/*! Function : FindAttribute
 *  Purpose  : Find attribute of given type assigned SObject, returns Standard_True if it is found
 */
//============================================================================

CORBA::Boolean SALOMEDS_StudyBuilder_i::FindAttribute(SALOMEDS::SObject_ptr theObject, 
						      SALOMEDS::GenericAttribute_out theAttr, 
						      const char* theTypeOfAttribute)
{
  SALOMEDS::Locker lock;

  if(SALOMEDS_SObject_i* aSObject = _study->DownCast(theObject))
    return aSObject->FindAttribute(theAttr,theTypeOfAttribute);

  return Standard_False;
}

//============================================================================
/*! Function : RemoveAttribute
 *  Purpose  : Remove attribute of given type assigned SObject
 */
//============================================================================

void SALOMEDS_StudyBuilder_i::RemoveAttribute(SALOMEDS::SObject_ptr theSObject, 
					      const char* aTypeOfAttribute)
{
  SALOMEDS::Locker lock;

  CheckLocked();

  if(CORBA::is_nil(theSObject))
    return;

  TDF_Label Lab;
  CORBA::String_var anobid = theSObject->GetID();
  TDF_Tool::Label(_doc->GetData(),anobid,Lab);
  
  if (strcmp(aTypeOfAttribute, "AttributeIOR") == 0) { // postponed removing of CORBA objects
    Handle(SALOMEDS_IORAttribute) anAttr;
    if (Lab.FindAttribute(SALOMEDS_IORAttribute::GetID(), anAttr))
      _study->AddPostponed(TCollection_AsciiString(anAttr->Get()).ToCString());
    else
      return;
  }

  Lab.ForgetAttribute(SALOMEDS::GetGUID(aTypeOfAttribute));
}

//============================================================================
/*! Function : Addreference
 *  Purpose  : 
 */
//============================================================================
void 
SALOMEDS_StudyBuilder_i::Addreference(SALOMEDS::SObject_ptr me, 
				      SALOMEDS::SObject_ptr theReferencedObject)
{
  SALOMEDS::Locker lock;

  CheckLocked();
  if(CORBA::is_nil(me) || CORBA::is_nil(theReferencedObject))
    return;

  TDF_Label Lab;
  CORBA::String_var meid = me->GetID();
  TDF_Tool::Label(_doc->GetData(),meid,Lab);  

  TDF_Label RefLab;
  CORBA::String_var roid = theReferencedObject->GetID();
  TDF_Tool::Label(_doc->GetData(),roid,RefLab);

  TDF_Reference::Set(Lab,RefLab);
  SALOMEDS_TargetAttribute::Set(RefLab)->Append(Lab);

  if(Lab.IsDescendant(_doc->Main())) 
    OnRemoveSObject(me);
}

//============================================================================
/*! Function : RemoveReference
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::RemoveReference(SALOMEDS::SObject_ptr me)
{
  SALOMEDS::Locker lock;

  SALOMEDS::SObject_var theReferencedObject;
  if(!me->ReferencedObject(theReferencedObject)) return;  //No reference is found

  if(CORBA::is_nil(me) || CORBA::is_nil(theReferencedObject))
    return;

  CheckLocked();
  TDF_Label Lab;
  CORBA::String_var meid = me->GetID();
  TDF_Tool::Label(_doc->GetData(),meid,Lab);  

  Lab.ForgetAttribute(TDF_Reference::GetID());  

  TDF_Label RefLab;  
  CORBA::String_var roid = theReferencedObject->GetID();
  TDF_Tool::Label(_doc->GetData(),roid,RefLab);

  RemoveAttribute(theReferencedObject, "AttributeTarget");
}



//============================================================================
/*! Function : AddDirectory
 *  Purpose  : adds a new directory with a path = thePath
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::AddDirectory(const char* thePath) 
{
  SALOMEDS::Locker lock;

  CheckLocked();
  if(thePath == NULL || strlen(thePath) == 0) throw SALOMEDS::Study::StudyInvalidDirectory();

  TCollection_AsciiString aPath(CORBA::string_dup(thePath)), aContext(""), aFatherPath;
  Handle(TDataStd_Name) aName;
  TDF_Label aLabel;
  SALOMEDS::SObject_var anObject = SALOMEDS_SObject_i::NewRef(_study,_doc->Main()); 

  try { 
    anObject = _study->FindObjectByPath(thePath); //Check if the directory already exists
  }
  catch(...) { }

  if(!anObject->_is_nil()) throw SALOMEDS::Study::StudyNameAlreadyUsed(); 

  if(aPath.Value(1) != '/') { //Relative path 
    aPath.Prepend('/');
    aPath = TCollection_AsciiString(_study->GetContext()) + aPath;
  }

  TCollection_AsciiString aToken = aPath.Token("/", 1);
  if(aToken.Length() == 0) aFatherPath = "/";

  int i = 1;  
  while(aToken.Length() != 0) {
    if(aPath.Token("/", i+1).Length() > 0) {
      aFatherPath += "/";
      aFatherPath += aToken;
    }
    aToken = aPath.Token("/", ++i);
  }

  anObject = SALOMEDS::SObject::_nil();
  try { 
    anObject = _study->FindObjectByPath(aFatherPath.ToCString()); //Check if the father directory exists
  }
  catch(...) { ; }
  if(anObject->_is_nil()) 
    throw SALOMEDS::Study::StudyInvalidDirectory(); 

  SALOMEDS::SObject_var aNewObject = NewObject(anObject);
  TDF_Tool::Label(_doc->GetData(), aNewObject->GetID(), aLabel);
  if(aLabel.IsNull()) {
    MESSAGE("### NULL label");
    throw SALOMEDS::Study::StudyInvalidComponent();      
  }

  TDataStd_Name::Set(aLabel, aPath.Token("/", i-1));

  //Set LocalID attribute to identify the directory object
  SALOMEDS::GenericAttribute_var anAttr = FindOrCreateAttribute(aNewObject, "AttributeLocalID");
  SALOMEDS::AttributeLocalID_var aPersRef = SALOMEDS::AttributeLocalID::_narrow(anAttr);
  if(aPersRef->_is_nil()) 
    throw SALOMEDS::Study::StudyInvalidDirectory();

  aPersRef->SetValue(DIRECTORYID);
}


//============================================================================
/*! Function : SetGUID
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::SetGUID(SALOMEDS::SObject_ptr anObject, const char* theGUID)
{
  SALOMEDS::Locker lock;

  CheckLocked();

  if(CORBA::is_nil(anObject))
    return;

  TDF_Label aLabel;
  CORBA::String_var anEntry = anObject->GetID();
  TDF_Tool::Label(_doc->GetData(), anEntry, aLabel);
  TDataStd_UAttribute::Set(aLabel, (char*)theGUID);
}

//============================================================================
/*! Function : IsGUID
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDS_StudyBuilder_i::IsGUID(SALOMEDS::SObject_ptr anObject, const char* theGUID)
{
  SALOMEDS::Locker lock;

  if(CORBA::is_nil(anObject))
    return false;

  TDF_Label aLabel;
  CORBA::String_var anEntry = anObject->GetID();
  TDF_Tool::Label(_doc->GetData(), anEntry, aLabel);
  return aLabel.IsAttribute((char*)theGUID);
}


//============================================================================
/*! Function : NewCommand
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::NewCommand()
{
  SALOMEDS::Locker lock;

  // mpv: for SAL2114 - unset "lock changed" flag at the operation start
  Handle(SALOMEDS_StudyPropertiesAttribute) anAttr;
  if (!_doc->Main().FindAttribute(SALOMEDS_StudyPropertiesAttribute::GetID(), anAttr)) {
    anAttr = new SALOMEDS_StudyPropertiesAttribute;
    _doc->Main().AddAttribute(anAttr);
  }
  anAttr->IsLockChanged(true);
  
  _doc->NewCommand();
}

//============================================================================
/*! Function : CommitCommand
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::CommitCommand() throw (SALOMEDS::StudyBuilder::LockProtection)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(SBLockProtection);
  Handle(SALOMEDS_StudyPropertiesAttribute) anAttr;
  if (!_doc->Main().FindAttribute(SALOMEDS_StudyPropertiesAttribute::GetID(), anAttr)) {
    anAttr = new SALOMEDS_StudyPropertiesAttribute;
    _doc->Main().AddAttribute(anAttr);
  }
  if (anAttr->IsLocked() && !anAttr->IsLockChanged(true)) {
    MESSAGE("Locked document modification !!!");
    AbortCommand();
    throw SALOMEDS::StudyBuilder::LockProtection();
  } else {
    _study->RemovePostponed(_doc->GetUndoLimit());

    int aModif = anAttr->GetModified();
    if (aModif < 0) aModif = 1000; // if user make undo and then - new transaction "modify" will never be zero
    anAttr->SetModified(aModif+1);
    _doc->CommitCommand();
  }
}

//============================================================================
/*! Function : HasOpenCommand
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_StudyBuilder_i::HasOpenCommand()
{
  SALOMEDS::Locker lock;

  return _doc->HasOpenCommand();
}

//============================================================================
/*! Function : AbortCommand
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::AbortCommand()
{
  SALOMEDS::Locker lock;

  _study->UndoPostponed(0);
  
  _doc->AbortCommand();
}

//============================================================================
/*! Function : Undo
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::Undo() throw (SALOMEDS::StudyBuilder::LockProtection)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(SBLockProtection);
  Handle(SALOMEDS_StudyPropertiesAttribute) anAttr;
  if (!_doc->Main().FindAttribute(SALOMEDS_StudyPropertiesAttribute::GetID(), anAttr)) {
    anAttr = new SALOMEDS_StudyPropertiesAttribute;
    _doc->Main().AddAttribute(anAttr);
    }
  if (anAttr->IsLocked()) {
    MESSAGE("Locked document modification !!!");
    throw SALOMEDS::StudyBuilder::LockProtection();
  } else {
    _study->UndoPostponed(1);
    _doc->Undo();
    anAttr->SetModified(anAttr->GetModified()-1);
  }
}

//============================================================================
/*! Function : Redo
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::Redo() throw (SALOMEDS::StudyBuilder::LockProtection)
{  
  SALOMEDS::Locker lock;

  Unexpect aCatch(SBLockProtection);
  Handle(SALOMEDS_StudyPropertiesAttribute) anAttr;
  if (!_doc->Main().FindAttribute(SALOMEDS_StudyPropertiesAttribute::GetID(), anAttr)) {
    anAttr = new SALOMEDS_StudyPropertiesAttribute;
    _doc->Main().AddAttribute(anAttr);
  }
  
  if (anAttr->IsLocked()) {
    MESSAGE("Locked document modification !!!");
    throw SALOMEDS::StudyBuilder::LockProtection();
  } else {
    _doc->Redo();
    _study->UndoPostponed(-1);
    anAttr->SetModified(anAttr->GetModified()+1);
  }
 }

//============================================================================
/*! Function : GetAvailableUndos
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean SALOMEDS_StudyBuilder_i::GetAvailableUndos()
{
  SALOMEDS::Locker lock;

  return _doc->GetAvailableUndos();
}

//============================================================================
/*! Function : GetAvailableRedos
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean  SALOMEDS_StudyBuilder_i::GetAvailableRedos()
{
  SALOMEDS::Locker lock;

  return _doc->GetAvailableRedos();
}

//============================================================================
/*! Function : UndoLimit
 *  Purpose  : 
 */
//============================================================================
CORBA::Long  SALOMEDS_StudyBuilder_i::UndoLimit()
{
  SALOMEDS::Locker lock;

  return _doc->GetUndoLimit();
}

//============================================================================
/*! Function : UndoLimit
 *  Purpose  : 
 */
//============================================================================
void  SALOMEDS_StudyBuilder_i::UndoLimit(CORBA::Long n)
{
  SALOMEDS::Locker lock;

  CheckLocked();
  _doc->SetUndoLimit (n);
}

//============================================================================
/*! Function : SetOnAddSObject
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::Callback_ptr SALOMEDS_StudyBuilder_i::SetOnAddSObject(SALOMEDS::Callback_ptr theCallback)
{
  return _study->SetOnAddSObject(theCallback);
}

void SALOMEDS_StudyBuilder_i::OnAddSObject(SALOMEDS::SObject_ptr theObject)
{
  _study->OnAddSObject(theObject);
}

//============================================================================
/*! Function : SetOnNewSObject
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::Callback_ptr SALOMEDS_StudyBuilder_i::SetOnRemoveSObject(SALOMEDS::Callback_ptr theCallback)
{
  return _study->SetOnRemoveSObject(theCallback);
}

void SALOMEDS_StudyBuilder_i::OnRemoveSObject(SALOMEDS::SObject_ptr theObject)
{
  _study->OnRemoveSObject(theObject);
}

//============================================================================
/*! Function : CheckLocked
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::CheckLocked() throw (SALOMEDS::StudyBuilder::LockProtection) {
  _study->CheckLocked();
}

//============================================================================
/*! Function : SetName
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::SetName(SALOMEDS::SObject_ptr theSO, const char* theValue)
     throw(SALOMEDS::StudyBuilder::LockProtection)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(SBLockProtection);
  CheckLocked();

  if(CORBA::is_nil(theSO))
    return;

  //Find label
  TDF_Label aLabel;
  CORBA::String_var aSOID = theSO->GetID();
  TDF_Tool::Label(_doc->GetData(), aSOID, aLabel);
  TDataStd_Name::Set(aLabel, (char*)theValue);
}

//============================================================================
/*! Function : SetComment
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::SetComment(SALOMEDS::SObject_ptr theSO, const char* theValue)
 throw(SALOMEDS::StudyBuilder::LockProtection)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(SBLockProtection);
  CheckLocked();

  if(CORBA::is_nil(theSO))
    return;

   //Find label
  TDF_Label aLabel;
  CORBA::String_var aSOID = theSO->GetID();
  TDF_Tool::Label(_doc->GetData(), aSOID, aLabel);
  TDataStd_Comment::Set(aLabel, (char*)theValue);
}

//============================================================================
/*! Function : SetIOR
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::SetIOR(SALOMEDS::SObject_ptr theSO, const char* theValue)
 throw(SALOMEDS::StudyBuilder::LockProtection)
{
  SALOMEDS::Locker lock;

  Unexpect aCatch(SBLockProtection);
  CheckLocked();

  if(CORBA::is_nil(theSO))
    return;

  //Find label
  TDF_Label aLabel;
  CORBA::String_var aSOID = theSO->GetID();
  TDF_Tool::Label(_doc->GetData(), aSOID, aLabel);
  SALOMEDS_IORAttribute::Set(aLabel, TCollection_ExtendedString((char*)theValue),_study);
}
