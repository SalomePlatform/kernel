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

#include "utilities.h"
#include "SALOMEDS_Study_i.hxx"
//#include "SALOMEDS_StudyBuilder_i.hxx"
#include "SALOMEDS_SObject_i.hxx"
#include "SALOMEDS_SComponent_i.hxx"

#include "SALOMEDS_IORAttribute.hxx"
#include "SALOMEDS_PersRefAttribute.hxx"
#include "SALOMEDS_TargetAttribute.hxx"
#include "SALOMEDS_StudyPropertiesAttribute.hxx"
#include "SALOMEDS_PythonObjectAttribute.hxx"
#include "SALOMEDS_ExternalFileDef.hxx"
#include "SALOMEDS_FileType.hxx"
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
#include "SALOMEDS_AttributePersistentRef_i.hxx"
#include "SALOMEDS_AttributeIOR_i.hxx"
#include "SALOMEDS_AttributeExternalFileDef_i.hxx"
#include "SALOMEDS_AttributeFileType_i.hxx"
#include "SALOMEDS_AttributeComment_i.hxx"
#include "SALOMEDS_AttributeName_i.hxx"
#include "SALOMEDS_AttributeSequenceOfInteger_i.hxx"
#include "SALOMEDS_AttributeSequenceOfReal_i.hxx"
#include "SALOMEDS_AttributeTableOfInteger_i.hxx"
#include "SALOMEDS_AttributeTableOfReal_i.hxx"
#include "SALOMEDS_AttributeTableOfString_i.hxx"
#include "SALOMEDS_AttributeInteger_i.hxx"
#include "SALOMEDS_AttributeReal_i.hxx"
#include "SALOMEDS_AttributeDrawable_i.hxx"
#include "SALOMEDS_AttributeSelectable_i.hxx"
#include "SALOMEDS_AttributeExpandable_i.hxx"
#include "SALOMEDS_AttributeOpened_i.hxx"
#include "SALOMEDS_AttributeTextColor_i.hxx"
#include "SALOMEDS_AttributeTextHighlightColor_i.hxx"
#include "SALOMEDS_AttributePixMap_i.hxx"
#include "SALOMEDS_AttributeTreeNode_i.hxx"
#include "SALOMEDS_AttributeTarget_i.hxx"
#include "SALOMEDS_AttributeLocalID_i.hxx"
#include "SALOMEDS_AttributeUserID_i.hxx"
#include "SALOMEDS_AttributeStudyProperties_i.hxx"
#include "SALOMEDS_AttributePythonObject_i.hxx"
#include "SALOMEDS_Tool.hxx"
#include "Utils_CorbaException.hxx"
#include "Utils_ExceptHandlers.hxx"

#include <HDFOI.hxx>
#include <stdlib.h> 
using namespace std;

#define USE_CASE_LABEL_TAG            2
#define DIRECTORYID 16661
#define FILELOCALID 26662 

UNEXPECT_CATCH(SBSalomeException, SALOME::SALOME_Exception);
UNEXPECT_CATCH(SBLockProtection, SALOMEDS::StudyBuilder::LockProtection);
//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDS_StudyBuilder_i::SALOMEDS_StudyBuilder_i(const Handle(TDocStd_Document) doc, 
						 CORBA::ORB_ptr orb) : _doc(doc)
{
  _orb = CORBA::ORB::_duplicate(orb);
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
/*! Function : NewComponent
 *  Purpose  : Create a new component (Scomponent)
 */
//============================================================================
SALOMEDS::SComponent_ptr 
SALOMEDS_StudyBuilder_i::NewComponent(const char* DataType)
{
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

  SALOMEDS_SComponent_i *  so_servant = new SALOMEDS_SComponent_i (NL,_orb);
  SALOMEDS::SComponent_var so;
  so= SALOMEDS::SComponent::_narrow(so_servant->SComponent::_this()); 

  if(!CORBA::is_nil(_callbackOnAdd)) _callbackOnAdd->OnAddSObject(so);

  return so;
}

//============================================================================
/*! Function : DefineComponentInstance
 *  Purpose  : Add IOR attribute of a Scomponent
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::DefineComponentInstance(SALOMEDS::SComponent_ptr aComponent,
						 CORBA::Object_ptr IOR)
{
  CheckLocked();
  //Find label
  TDF_Label Lab;
  ASSERT(!CORBA::is_nil(aComponent));
  CORBA::String_var compid = aComponent->GetID();
  TDF_Tool::Label(_doc->GetData(),compid,Lab);

  //add IOR definition 
  ASSERT(!CORBA::is_nil(IOR));
  CORBA::String_var iorstr = _orb->object_to_string(IOR);
  SALOMEDS_IORAttribute::Set(Lab,(char*)iorstr,_orb);
  
}

//============================================================================
/*! Function : RemoveComponent
 *  Purpose  : Delete a Scomponent
 */
//============================================================================
void 
SALOMEDS_StudyBuilder_i::RemoveComponent(SALOMEDS::SComponent_ptr aComponent)
{
  CheckLocked();
  ASSERT(!CORBA::is_nil(aComponent));
  RemoveObject(aComponent);
}

//============================================================================
/*! Function : NewObject
 *  Purpose  : Create a new SObject
 */
//============================================================================
SALOMEDS::SObject_ptr 
SALOMEDS_StudyBuilder_i::NewObject(SALOMEDS::SObject_ptr theFatherObject)
{
  CheckLocked();
  TCollection_AsciiString anEntry;
 
  //Find label of father
  TDF_Label Lab;
  
  ASSERT(!CORBA::is_nil(theFatherObject));
  CORBA::String_var fatherid = theFatherObject->GetID();
  TDF_Tool::Label(_doc->GetData(),fatherid,Lab);

  //Create a new label
  //YFR DEBUG : 13/02/2002  TDF_Label NewLab = Lab.NewChild();
  Standard_Integer imax = 0;
  for (TDF_ChildIterator it(Lab); it.More(); it.Next()) {
    if (it.Value().Tag() > imax)
      imax = it.Value().Tag();
  }
  imax++;
  TDF_Label NewLab = Lab.FindChild(imax);
  
  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (NewLab,_orb);
  SALOMEDS::SObject_var so = SALOMEDS::SObject::_narrow(so_servant->_this()); 

  if(!CORBA::is_nil(_callbackOnAdd)) _callbackOnAdd->OnAddSObject(so);

  return so;
}

//============================================================================
/*! Function : NewObjectToTag
 *  Purpose  :
 */
//============================================================================
SALOMEDS::SObject_ptr 
SALOMEDS_StudyBuilder_i::NewObjectToTag(SALOMEDS::SObject_ptr theFatherObject,
					CORBA::Long atag)
{
  CheckLocked();
  //Find label of father
  TDF_Label Lab;

  ASSERT(!CORBA::is_nil(theFatherObject));
  CORBA::String_var fatherid = theFatherObject->GetID();
  TDF_Tool::Label(_doc->GetData(),fatherid,Lab);
  //Create or find label
  TDF_Label NewLab = Lab.FindChild(atag,1);

  SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (NewLab,_orb);
  SALOMEDS::SObject_var so = SALOMEDS::SObject::_narrow(so_servant->_this()); 

  if(!CORBA::is_nil(_callbackOnAdd)) _callbackOnAdd->OnAddSObject(so);

  return so;
}

//============================================================================
/*! Function : RemoveObject
 *  Purpose  :
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::RemoveObject(SALOMEDS::SObject_ptr anObject)
{
  CheckLocked();
  if(!CORBA::is_nil(_callbackOnRemove)) _callbackOnRemove->OnRemoveSObject(anObject);

  TDF_Label Lab;
  ASSERT(!CORBA::is_nil(anObject));
  TDF_Tool::Label(_doc->GetData(),anObject->GetID(),Lab);

  Handle(TDF_Reference) aReference;
  if (Lab.FindAttribute(TDF_Reference::GetID(), aReference)) {
    Handle(SALOMEDS_TargetAttribute) aTarget;
    if (aReference->Get().FindAttribute(SALOMEDS_TargetAttribute::GetID(),aTarget))
      aTarget->Remove(Lab);
  }

  Handle(SALOMEDS_IORAttribute) anAttr; // postponed removing of CORBA objects
  if (Lab.FindAttribute(SALOMEDS_IORAttribute::GetID(), anAttr))
    SALOMEDS_Study_i::GetStudy(_doc->Main(), _orb)->AddPostponed(TCollection_AsciiString(anAttr->Get()).ToCString());

  Lab.ForgetAllAttributes();
}

//============================================================================
/*! Function : RemoveObjectWithChildren
 *  Purpose  :
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::RemoveObjectWithChildren(SALOMEDS::SObject_ptr anObject)
{
  CheckLocked();
  if(!CORBA::is_nil(_callbackOnRemove)) _callbackOnRemove->OnRemoveSObject(anObject);

  TDF_Label Lab;
  ASSERT(!CORBA::is_nil(anObject));
  TDF_Tool::Label(_doc->GetData(),anObject->GetID(),Lab);

  Handle(TDF_Reference) aReference;
  if (Lab.FindAttribute(TDF_Reference::GetID(), aReference)) {
    Handle(SALOMEDS_TargetAttribute) aTarget;
    if (aReference->Get().FindAttribute(SALOMEDS_TargetAttribute::GetID(),aTarget))
      aTarget->Remove(Lab);
  }
  Handle(SALOMEDS_IORAttribute) anAttr; // postponed removing of CORBA objects
  if (Lab.FindAttribute(SALOMEDS_IORAttribute::GetID(), anAttr))
    SALOMEDS_Study_i::GetStudy(_doc->Main(), _orb)->AddPostponed(TCollection_AsciiString(anAttr->Get()).ToCString());

  TDF_ChildIterator it(Lab, Standard_True);
  for(;it.More();it.Next()) {
    TDF_Label aLabel = it.Value();
    if (aLabel.FindAttribute(TDF_Reference::GetID(), aReference)) {
      Handle(SALOMEDS_TargetAttribute) aTarget;
      if (aReference->Get().FindAttribute(SALOMEDS_TargetAttribute::GetID(),aTarget))
	aTarget->Remove(aLabel);
    }
    Handle(SALOMEDS_IORAttribute) anAttr; // postponed removing of CORBA objects
    if (aLabel.FindAttribute(SALOMEDS_IORAttribute::GetID(), anAttr))
      SALOMEDS_Study_i::GetStudy(_doc->Main(), _orb)->AddPostponed(TCollection_AsciiString(anAttr->Get()).ToCString());
  }

  Lab.ForgetAllAttributes(Standard_True);
}

//============================================================================
/*! Function : Translate_persistentID_to_IOR
 *  Purpose  :
 */
//============================================================================
static void  Translate_persistentID_to_IOR(TDF_Label                  Lab,
					   SALOMEDS::Driver_ptr       driver,
					   CORBA::ORB_ptr             orb,
					   CORBA::Boolean             isMultiFile,
					   CORBA::Boolean             isASCII)
{
  TDF_ChildIterator  itchild (Lab);
  
  for (; itchild.More(); itchild.Next()) {
    TDF_Label current = itchild.Value();
    Handle(TDF_Attribute) Att;
    if (current.FindAttribute(SALOMEDS_PersRefAttribute::GetID(),Att)) {  

      Handle(SALOMEDS_LocalIDAttribute) anID;
      if (current.FindAttribute(SALOMEDS_LocalIDAttribute::GetID(), anID)) 
	if (anID->Get() == FILELOCALID) continue;        //SRN: This attribute store a file name, skip it 

      TCollection_ExtendedString res;   
      res = Handle(TDataStd_Comment)::DownCast(Att)->Get();
      TCollection_AsciiString ch(res);
      
      CORBA::String_var persistent_string = CORBA::string_dup(ch.ToCString());
      ASSERT(! CORBA::is_nil(driver));
      SALOMEDS_SObject_i *  so_servant = new SALOMEDS_SObject_i (current,orb);
      SALOMEDS::SObject_var so = SALOMEDS::SObject::_narrow(so_servant->_this()); 

      CORBA::String_var ior_string = driver->LocalPersistentIDToIOR(so, persistent_string, isMultiFile, isASCII);

      TCollection_ExtendedString value(ior_string); 
      SALOMEDS_IORAttribute::Set (current,value,orb); 
      
      //TCollection_AsciiString anEntry;TDF_Tool::Entry (current,anEntry); //SRN: No use here
      //delete persistent_string;
      //delete ior_string;
    }
    Translate_persistentID_to_IOR (current,driver,orb,isMultiFile, isASCII);
  }
}

//============================================================================
/*! Function : LoadWith
 *  Purpose  : 
 */
//============================================================================

//============================================================================
void SALOMEDS_StudyBuilder_i::LoadWith(SALOMEDS::SComponent_ptr anSCO, 
				       SALOMEDS::Driver_ptr aDriver) throw(SALOME::SALOME_Exception)
{
  Unexpect aCatch(SBSalomeException);
  TDF_Label Lab;
  ASSERT(!CORBA::is_nil(anSCO));
  CORBA::String_var scoid = anSCO->GetID();
  TDF_Tool::Label(_doc->GetData(),scoid,Lab);
  Handle(TDF_Attribute) Att;
  
  //Find the current Url of the study  
  if (_doc->Main().FindAttribute(SALOMEDS_PersRefAttribute::GetID(),Att)) {
    int aLocked = anSCO->GetStudy()->GetProperties()->IsLocked();
    if (aLocked) anSCO->GetStudy()->GetProperties()->SetLocked(false);
    
    TCollection_ExtendedString Res = Handle(TDataStd_Comment)::DownCast(Att)->Get();
    
    Handle(TDataStd_Comment) type;
    TCollection_ExtendedString DataType;
    if (Lab.FindAttribute(TDataStd_Comment::GetID(),type))
      DataType = type->Get();
    else 
      MESSAGE("No Data Type");
    
    // associate the driver to the SComponent
    ASSERT(!CORBA::is_nil(aDriver));
    // mpv 06.03.2003: SAL1927 - if component data if already loaded, it is not necessary to do it again
    if (Lab.FindAttribute(SALOMEDS_IORAttribute::GetID(), Att)) {
      if (aLocked) anSCO->GetStudy()->GetProperties()->SetLocked(true);
      return;
    }
    DefineComponentInstance (anSCO, aDriver);
    
    TCollection_AsciiString aHDFPath(Res);
    
    char* aHDFUrl;
    bool isASCII = false;
    if (HDFascii::isASCII(aHDFPath.ToCString())) {
      isASCII = true;
      char* aResultPath = HDFascii::ConvertFromASCIIToHDF(aHDFPath.ToCString());
      aHDFUrl = new char[strlen(aResultPath) + 19];
      sprintf(aHDFUrl, "%shdf_from_ascii.hdf", aResultPath);
      delete(aResultPath);
    } else {
      aHDFUrl = CORBA::string_dup(aHDFPath.ToCString());
    }

    //Open the Study HDF file 
    HDFfile *hdf_file = new HDFfile(aHDFUrl); 
    
    char aMultifileState[2];
    char ASCIIfileState[2];
    try {
      CORBA::String_var scoid = anSCO->GetID();
      hdf_file->OpenOnDisk(HDF_RDONLY);
      HDFgroup *hdf_group = new HDFgroup("DATACOMPONENT",hdf_file);
      hdf_group->OpenOnDisk();
      HDFgroup *hdf_sco_group = new HDFgroup(scoid, hdf_group);
      hdf_sco_group->OpenOnDisk();
	
      SALOMEDS::TMPFile_var aStreamFile;
      if (hdf_sco_group->ExistInternalObject("FILE_STREAM")) {
	HDFdataset *hdf_dataset = new HDFdataset("FILE_STREAM", hdf_sco_group);
	hdf_dataset->OpenOnDisk();
	int aStreamSize = hdf_dataset->GetSize();
	unsigned char* aBuffer = new unsigned char[aStreamSize];
	if(aBuffer == NULL) throw HDFexception("Unable to open dataset FILE_STREAM");
	hdf_dataset->ReadFromDisk(aBuffer);
	aStreamFile = new SALOMEDS::TMPFile(aStreamSize, aStreamSize, aBuffer, 1);
	hdf_dataset->CloseOnDisk();
	hdf_dataset = 0;
      } else aStreamFile = new SALOMEDS::TMPFile(0);
      
      HDFdataset *multifile_hdf_dataset = new HDFdataset("MULTIFILE_STATE", hdf_sco_group);
      multifile_hdf_dataset->OpenOnDisk();
      multifile_hdf_dataset->ReadFromDisk(aMultifileState);
      
      HDFdataset *ascii_hdf_dataset = new HDFdataset("ASCII_STATE", hdf_sco_group);
      ascii_hdf_dataset->OpenOnDisk();
      ascii_hdf_dataset->ReadFromDisk(ASCIIfileState);
      
      // set path without file name from URL 
      int aFileNameSize = Res.Length();
      char* aDir = new char[aFileNameSize];
      memcpy(aDir, TCollection_AsciiString(Res).ToCString(), aFileNameSize);
      for(int aCounter = aFileNameSize-1; aCounter>=0; aCounter--)
	if (aDir[aCounter] == '/') {
	  aDir[aCounter+1] = 0;
	  break;
	}
      
      CORBA::Boolean aResult = (ASCIIfileState[0]=='A')?
	aDriver->LoadASCII(anSCO, aStreamFile.in(), aDir, aMultifileState[0]=='M'):
	  aDriver->Load(anSCO, aStreamFile.in(), aDir, aMultifileState[0]=='M');
      if(!aResult) {
	RemoveAttribute( anSCO, "AttributeIOR" );

	MESSAGE("Can't load component");
	//THROW_SALOME_CORBA_EXCEPTION("Unable to load component data",SALOME::BAD_PARAM);
	throw HDFexception("Unable to load component");
      }
      
      delete(aDir);

      multifile_hdf_dataset->CloseOnDisk();
      multifile_hdf_dataset = 0;
      ascii_hdf_dataset->CloseOnDisk();
      ascii_hdf_dataset = 0;
      hdf_sco_group->CloseOnDisk();
      hdf_sco_group = 0;
      hdf_group->CloseOnDisk();
      hdf_group = 0;
      hdf_file->CloseOnDisk();
      delete hdf_file;
      
      if (isASCII) {
	SALOMEDS::ListOfFileNames_var aFilesToRemove = new SALOMEDS::ListOfFileNames;
	aFilesToRemove->length(1);
	aFilesToRemove[0] = CORBA::string_dup(&(aHDFUrl[strlen(SALOMEDS_Tool::GetDirFromPath(aHDFUrl).c_str())]));
	SALOMEDS_Tool::RemoveTemporaryFiles(SALOMEDS_Tool::GetDirFromPath(aHDFUrl).c_str(), aFilesToRemove, true);
      }
      delete aHDFUrl;
    }
    catch (HDFexception) {
      INFOS("No persistent file Name");
      delete hdf_file;
      if (isASCII) {
	SALOMEDS::ListOfFileNames_var aFilesToRemove = new SALOMEDS::ListOfFileNames;
	aFilesToRemove->length(1);
	aFilesToRemove[0] = CORBA::string_dup(&(aHDFUrl[strlen(SALOMEDS_Tool::GetDirFromPath(aHDFUrl).c_str())]));
	SALOMEDS_Tool::RemoveTemporaryFiles(SALOMEDS_Tool::GetDirFromPath(aHDFUrl).c_str(), aFilesToRemove, true);
      }
      delete aHDFUrl;
      if (aLocked) anSCO->GetStudy()->GetProperties()->SetLocked(true);
      THROW_SALOME_CORBA_EXCEPTION("No persistent file Name found",SALOME::BAD_PARAM);   
    }
    
    try {
      Translate_persistentID_to_IOR (Lab,aDriver,_orb, aMultifileState[0]=='M', ASCIIfileState[0] == 'A');
    } catch (SALOME::SALOME_Exception) {
      INFOS("Can't translate PersRef to IOR");
      if (aLocked) anSCO->GetStudy()->GetProperties()->SetLocked(true);
      THROW_SALOME_CORBA_EXCEPTION("Unable to convert component persistent data to the transient",SALOME::BAD_PARAM);
      //        throw HDFexception("Unable to load component data");
    }
    if (aLocked) anSCO->GetStudy()->GetProperties()->SetLocked(true);
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
SALOMEDS::GenericAttribute_ptr SALOMEDS_StudyBuilder_i::FindOrCreateAttribute(SALOMEDS::SObject_ptr anObject, 
									      const char* aTypeOfAttribute)
{
  TDF_Label Lab;
  ASSERT(!CORBA::is_nil(anObject));
  CORBA::String_var anobid = anObject->GetID();
  TDF_Tool::Label(_doc->GetData(),anobid,Lab);

  __FindOrCreateAttributeLocked(TDataStd_Real, AttributeReal)
  __FindOrCreateAttributeLocked(TDataStd_Integer, AttributeInteger)
  __FindOrCreateAttributeLocked(SALOMEDS_SequenceOfRealAttribute, AttributeSequenceOfReal)
  __FindOrCreateAttributeLocked(SALOMEDS_SequenceOfIntegerAttribute, AttributeSequenceOfInteger)
  __FindOrCreateAttributeLocked(TDataStd_Name, AttributeName)
  __FindOrCreateAttributeLocked(TDataStd_Comment, AttributeComment)
  __FindOrCreateAttributeLocked(SALOMEDS_IORAttribute, AttributeIOR)
  __FindOrCreateAttributeLocked(SALOMEDS_PixMapAttribute, AttributePixMap)
  __FindOrCreateAttributeLocked(SALOMEDS_LocalIDAttribute, AttributeLocalID)
  __FindOrCreateAttributeLocked(SALOMEDS_TableOfIntegerAttribute, AttributeTableOfInteger)
  __FindOrCreateAttributeLocked(SALOMEDS_TableOfRealAttribute, AttributeTableOfReal)
  __FindOrCreateAttributeLocked(SALOMEDS_TableOfStringAttribute, AttributeTableOfString)
  __FindOrCreateAttributeLocked(SALOMEDS_PythonObjectAttribute, AttributePythonObject)

  __FindOrCreateAttribute(SALOMEDS_PersRefAttribute, AttributePersistentRef)
  __FindOrCreateAttribute(SALOMEDS_DrawableAttribute, AttributeDrawable)
  __FindOrCreateAttribute(SALOMEDS_SelectableAttribute, AttributeSelectable)
  __FindOrCreateAttribute(SALOMEDS_ExpandableAttribute, AttributeExpandable)
  __FindOrCreateAttribute(SALOMEDS_OpenedAttribute, AttributeOpened)
  __FindOrCreateAttribute(SALOMEDS_TextColorAttribute, AttributeTextColor)
  __FindOrCreateAttribute(SALOMEDS_TextHighlightColorAttribute, AttributeTextHighlightColor)
  __FindOrCreateAttribute(SALOMEDS_TargetAttribute, AttributeTarget)
  __FindOrCreateAttribute(SALOMEDS_StudyPropertiesAttribute, AttributeStudyProperties)
  __FindOrCreateAttribute(SALOMEDS_ExternalFileDef, AttributeExternalFileDef)
  __FindOrCreateAttribute(SALOMEDS_FileType, AttributeFileType)

  if (strncmp(aTypeOfAttribute, "AttributeTreeNode",17) == 0 ) {
    Standard_GUID aTreeNodeGUID;
    if (strcmp(aTypeOfAttribute, "AttributeTreeNode") == 0) {
      aTreeNodeGUID = TDataStd_TreeNode::GetDefaultTreeID();
    } else {
      char* aGUIDString = new char[40];
      sprintf(aGUIDString, &(aTypeOfAttribute[21]));
      aTreeNodeGUID = Standard_GUID(aGUIDString); // create tree node GUID by name
      delete(aGUIDString);
    }
    Handle(TDataStd_TreeNode) anAttr;
    if (!Lab.FindAttribute(aTreeNodeGUID, anAttr)) {
      CheckLocked();
      anAttr = TDataStd_TreeNode::Set(Lab, aTreeNodeGUID);
    }
    SALOMEDS_AttributeTreeNode_i* aTreeNodeAttr = new SALOMEDS_AttributeTreeNode_i(anAttr, _orb);
    return aTreeNodeAttr->AttributeTreeNode::_this();
  }

  if (strncmp(aTypeOfAttribute, "AttributeUserID",15) == 0 ) {
    Handle(TDataStd_UAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_AttributeUserID_i::DefaultID(), anAttr)) {
      CheckLocked();
      anAttr = TDataStd_UAttribute::Set(Lab, SALOMEDS_AttributeUserID_i::DefaultID());
    }
    SALOMEDS_AttributeUserID_i* aUAttr = new SALOMEDS_AttributeUserID_i(anAttr, _orb);
    return aUAttr->AttributeUserID::_this();
  }
  return SALOMEDS::GenericAttribute::_nil();
}

//============================================================================
/*! Function : FindAttribute
 *  Purpose  : Find attribute of given type assigned SObject, returns Standard_True if it is found
 */
//============================================================================

CORBA::Boolean SALOMEDS_StudyBuilder_i::FindAttribute(SALOMEDS::SObject_ptr anObject, 
                                                             SALOMEDS::GenericAttribute_out anAttribute, 
                                                             const char* aTypeOfAttribute)
{
  TDF_Label Lab;
  ASSERT(!CORBA::is_nil(anObject));
  CORBA::String_var anobid = anObject->GetID();
  TDF_Tool::Label(_doc->GetData(),anobid,Lab);
  Handle(TDF_Attribute) anAttr;
  if (Lab.FindAttribute(SALOMEDS_GenericAttribute_i::GetGUID(aTypeOfAttribute), anAttr)) {
    anAttribute = SALOMEDS::GenericAttribute::_duplicate(SALOMEDS_GenericAttribute_i::CreateAttribute(_orb, anAttr));
    return Standard_True;
  }
  return Standard_False;
}

//============================================================================
/*! Function : RemoveAttribute
 *  Purpose  : Remove attribute of given type assigned SObject
 */
//============================================================================

void SALOMEDS_StudyBuilder_i::RemoveAttribute(SALOMEDS::SObject_ptr anObject, 
					      const char* aTypeOfAttribute)
{
  CheckLocked();
  TDF_Label Lab;
  ASSERT(!CORBA::is_nil(anObject));
  CORBA::String_var anobid = anObject->GetID();
  TDF_Tool::Label(_doc->GetData(),anobid,Lab);
  
  if (strcmp(aTypeOfAttribute, "AttributeIOR") == 0) { // postponed removing of CORBA objects
    Handle(SALOMEDS_IORAttribute) anAttr;
    if (Lab.FindAttribute(SALOMEDS_IORAttribute::GetID(), anAttr))
      SALOMEDS_Study_i::GetStudy(_doc->Main(), _orb)->AddPostponed(TCollection_AsciiString(anAttr->Get()).ToCString());
    else return;
  }

  Lab.ForgetAttribute (SALOMEDS_GenericAttribute_i::GetGUID(aTypeOfAttribute));
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
  CheckLocked();
  TDF_Label Lab;
  ASSERT(!CORBA::is_nil(me));
  CORBA::String_var meid = me->GetID();
  TDF_Tool::Label(_doc->GetData(),meid,Lab);  
  TDF_Label RefLab;
  ASSERT(!CORBA::is_nil(theReferencedObject));
  CORBA::String_var roid = theReferencedObject->GetID();
  TDF_Tool::Label(_doc->GetData(),roid,RefLab);
  TDF_Reference::Set(Lab,RefLab);

  SALOMEDS_TargetAttribute::Set(RefLab)->Append(Lab);

  if(!CORBA::is_nil(_callbackOnRemove) && Lab.IsDescendant(_doc->Main())) _callbackOnRemove->OnRemoveSObject(me);
}

//============================================================================
/*! Function : RemoveReference
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::RemoveReference(SALOMEDS::SObject_ptr me)
{
  SALOMEDS::SObject_var theReferencedObject;
  if(!me->ReferencedObject(theReferencedObject)) return;  //No reference is found

  CheckLocked();
  TDF_Label Lab;
  ASSERT(!CORBA::is_nil(me));
  CORBA::String_var meid = me->GetID();
  TDF_Tool::Label(_doc->GetData(),meid,Lab);  

  Lab.ForgetAttribute(TDF_Reference::GetID());  

  //SRN: 30 Aug, 2004 : fix from Ecole l'ete version 

  TDF_Label RefLab;
  ASSERT(!CORBA::is_nil(theReferencedObject));
  CORBA::String_var roid = theReferencedObject->GetID();
  TDF_Tool::Label(_doc->GetData(),roid,RefLab);
       
  Handle(SALOMEDS_TargetAttribute) aTarget;
  if(RefLab.FindAttribute(SALOMEDS_TargetAttribute::GetID(), aTarget)) aTarget->Remove(Lab);
}



//============================================================================
/*! Function : AddDirectory
 *  Purpose  : adds a new directory with a path = thePath
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::AddDirectory(const char* thePath) 
{
  CheckLocked();
  if(thePath == NULL || strlen(thePath) == 0) throw SALOMEDS::Study::StudyInvalidDirectory();

  TCollection_AsciiString aPath(CORBA::string_dup(thePath)), aContext(""), aFatherPath;
  TDF_ChildIterator anIterator(_doc->Main());
  Handle(TDataStd_Name) aName;
  TDF_Label aLabel;
  SALOMEDS_SObject_i* so_servant = new SALOMEDS_SObject_i (_doc->Main(), _orb);
  SALOMEDS::SObject_var anObject = SALOMEDS::SObject::_narrow(so_servant->_this()); 
  SALOMEDS::Study_var aStudy = anObject->GetStudy();

  try { 
    anObject = aStudy->FindObjectByPath(thePath); //Check if the directory already exists
  }
  catch(...) { }

  if(!anObject->_is_nil()) throw SALOMEDS::Study::StudyNameAlreadyUsed(); 

  if(aPath.Value(1) != '/') { //Relative path 
    aPath.Prepend('/');
    aPath = TCollection_AsciiString(aStudy->GetContext()) + aPath;
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
    anObject = aStudy->FindObjectByPath(aFatherPath.ToCString()); //Check if the father directory exists
  }
  catch(...) { ; }
  if(anObject->_is_nil()) throw SALOMEDS::Study::StudyInvalidDirectory(); 

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
  if(aPersRef->_is_nil()) throw SALOMEDS::Study::StudyInvalidDirectory();

  aPersRef->SetValue(DIRECTORYID);
}


//============================================================================
/*! Function : SetGUID
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::SetGUID(SALOMEDS::SObject_ptr anObject, const char* theGUID)
{
  CheckLocked();
  TDF_Label aLabel;
  ASSERT(!CORBA::is_nil(anObject));
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
  TDF_Label aLabel;
  ASSERT(!CORBA::is_nil(anObject));
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
    SALOMEDS_Study_i::GetStudy(_doc->Main(), _orb)->RemovePostponed(_doc->GetUndoLimit());

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
  return _doc->HasOpenCommand();
}

//============================================================================
/*! Function : AbortCommand
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::AbortCommand()
{
  SALOMEDS_Study_i::GetStudy(_doc->Main(), _orb)->UndoPostponed(0);
  
  _doc->AbortCommand();
}

//============================================================================
/*! Function : Undo
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::Undo() throw (SALOMEDS::StudyBuilder::LockProtection)
{
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
    SALOMEDS_Study_i::GetStudy(_doc->Main(), _orb)->UndoPostponed(1);
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
    SALOMEDS_Study_i::GetStudy(_doc->Main(), _orb)->UndoPostponed(-1);
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
  return _doc->GetAvailableUndos();
}

//============================================================================
/*! Function : GetAvailableRedos
 *  Purpose  : 
 */
//============================================================================
CORBA::Boolean  SALOMEDS_StudyBuilder_i::GetAvailableRedos()
{
  return _doc->GetAvailableRedos();
}

//============================================================================
/*! Function : UndoLimit
 *  Purpose  : 
 */
//============================================================================
CORBA::Long  SALOMEDS_StudyBuilder_i::UndoLimit()
{
  return _doc->GetUndoLimit();
}

//============================================================================
/*! Function : UndoLimit
 *  Purpose  : 
 */
//============================================================================
void  SALOMEDS_StudyBuilder_i::UndoLimit(CORBA::Long n)
{
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
  SALOMEDS::Callback_ptr aRet = (CORBA::is_nil(_callbackOnAdd))?NULL:_callbackOnAdd._retn();
  _callbackOnAdd = SALOMEDS::Callback::_duplicate(theCallback);
  return aRet;
}

//============================================================================
/*! Function : SetOnNewSObject
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::Callback_ptr SALOMEDS_StudyBuilder_i::SetOnRemoveSObject(SALOMEDS::Callback_ptr theCallback)
{
  SALOMEDS::Callback_ptr aRet = (CORBA::is_nil(_callbackOnRemove))?NULL:_callbackOnRemove._retn();
  _callbackOnRemove = SALOMEDS::Callback::_duplicate(theCallback);
  return aRet;
}

//============================================================================
/*! Function : CheckLocked
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::CheckLocked() throw (SALOMEDS::StudyBuilder::LockProtection) {
  Unexpect aCatch(SBLockProtection);
  if (_doc->HasOpenCommand()) return;
  Handle(SALOMEDS_StudyPropertiesAttribute) anAttr;
  if (!_doc->Main().FindAttribute(SALOMEDS_StudyPropertiesAttribute::GetID(), anAttr)) {
    anAttr = new SALOMEDS_StudyPropertiesAttribute;
    _doc->Main().AddAttribute(anAttr);
    }
  if (anAttr->IsLocked()) throw SALOMEDS::StudyBuilder::LockProtection();
}

//============================================================================
/*! Function : SetName
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_StudyBuilder_i::SetName(SALOMEDS::SObject_ptr theSO, const char* theValue)
     throw(SALOMEDS::StudyBuilder::LockProtection)
{
  Unexpect aCatch(SBLockProtection);
  CheckLocked();
  //Find label
  TDF_Label aLabel;
  ASSERT(!CORBA::is_nil(theSO));
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
  Unexpect aCatch(SBLockProtection);
  CheckLocked();
   //Find label
  TDF_Label aLabel;
  ASSERT(!CORBA::is_nil(theSO));
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
  Unexpect aCatch(SBLockProtection);
  CheckLocked();
  //Find label
  TDF_Label aLabel;
  ASSERT(!CORBA::is_nil(theSO));
  CORBA::String_var aSOID = theSO->GetID();
  TDF_Tool::Label(_doc->GetData(), aSOID, aLabel);
  SALOMEDS_IORAttribute::Set(aLabel, TCollection_ExtendedString((char*)theValue), _orb);
}
