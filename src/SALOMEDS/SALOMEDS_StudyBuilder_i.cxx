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
#include <TDF_ChildIterator.hxx>
#include <TDF_Label.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_Comment.hxx>
#include <TDataStd_UAttribute.hxx>
#include <TDF_Tool.hxx>
#include <TDF_Reference.hxx>
#include <TDF_Data.hxx>
#include <TDataStd_ChildNodeIterator.hxx>
#include <TDF_ListIteratorOfAttributeList.hxx>
#include "SALOMEDS_AttributePersistentRef_i.hxx"
#include "SALOMEDS_AttributeIOR_i.hxx"
#include "SALOMEDS_AttributeComment_i.hxx"
#include "SALOMEDS_AttributeExternalFileDef_i.hxx"
#include "SALOMEDS_AttributeFileType_i.hxx"
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

#include <HDFOI.hxx>
#include <stdlib.h> 
using namespace std;

#define USE_CASE_LABEL_TAG            2
#define DIRECTORYID 16661
#define FILELOCALID 26662 

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

//   TDataStd_Comment::Set(NL,Standard_CString(DataType));
  TDataStd_Comment::Set(NL,Standard_CString(strdup(DataType)));

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
  TDF_Tool::Label(_doc->GetData(),strdup(compid),Lab);

  //add IOR definition 
  ASSERT(!CORBA::is_nil(IOR));
  CORBA::String_var iorstr = _orb->object_to_string(IOR);
  SALOMEDS_IORAttribute::Set(Lab,strdup(iorstr),_orb);
  
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
  TDF_Tool::Label(_doc->GetData(),strdup(fatherid),Lab);

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
  TDF_Tool::Label(_doc->GetData(),strdup(fatherid),Lab);
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
    SALOMEDS_Study_i::GetStudy(_doc->Main(), _orb)->AddPostponed(strdup(TCollection_AsciiString(anAttr->Get()).ToCString()));

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
    SALOMEDS_Study_i::GetStudy(_doc->Main(), _orb)->AddPostponed(strdup(TCollection_AsciiString(anAttr->Get()).ToCString()));

  TDF_ChildIterator it(Lab);
  for(;it.More();it.Next()) {
    TDF_Label aLabel = it.Value();
    if (aLabel.FindAttribute(TDF_Reference::GetID(), aReference)) {
      Handle(SALOMEDS_TargetAttribute) aTarget;
      if (aReference->Get().FindAttribute(SALOMEDS_TargetAttribute::GetID(),aTarget))
	aTarget->Remove(aLabel);
    }
    Handle(SALOMEDS_IORAttribute) anAttr; // postponed removing of CORBA objects
    if (aLabel.FindAttribute(SALOMEDS_IORAttribute::GetID(), anAttr))
      SALOMEDS_Study_i::GetStudy(_doc->Main(), _orb)->AddPostponed(strdup(TCollection_AsciiString(anAttr->Get()).ToCString()));
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

      TCollection_ExtendedString value(strdup(ior_string )); 
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
  TDF_Label Lab;
  ASSERT(!CORBA::is_nil(anSCO));
  CORBA::String_var scoid = anSCO->GetID();
  TDF_Tool::Label(_doc->GetData(),strdup(scoid),Lab);
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
      aHDFUrl = strdup(aHDFPath.ToCString());
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
	if (isASCII) {
	  SALOMEDS::ListOfFileNames_var aFilesToRemove = new SALOMEDS::ListOfFileNames;
	  aFilesToRemove->length(1);
	  aFilesToRemove[0] = strdup(&(aHDFUrl[strlen(SALOMEDS_Tool::GetDirFromPath(aHDFUrl))]));
	  SALOMEDS_Tool::RemoveTemporaryFiles(SALOMEDS_Tool::GetDirFromPath(aHDFUrl), aFilesToRemove, true);
	}
	delete aHDFUrl;
	MESSAGE("Can't load component");
	THROW_SALOME_CORBA_EXCEPTION("Unable to load component data",SALOME::BAD_PARAM);
//  	throw HDFexception("Unable to load component");
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
	aFilesToRemove[0] = strdup(&(aHDFUrl[strlen(SALOMEDS_Tool::GetDirFromPath(aHDFUrl))]));
	SALOMEDS_Tool::RemoveTemporaryFiles(SALOMEDS_Tool::GetDirFromPath(aHDFUrl), aFilesToRemove, true);
      }
      delete aHDFUrl;
    }
    catch (HDFexception) {
      MESSAGE("No persistent file Name");
      delete hdf_file;
      if (isASCII) {
	SALOMEDS::ListOfFileNames_var aFilesToRemove = new SALOMEDS::ListOfFileNames;
	aFilesToRemove->length(1);
	aFilesToRemove[0] = strdup(&(aHDFUrl[strlen(SALOMEDS_Tool::GetDirFromPath(aHDFUrl))]));
	SALOMEDS_Tool::RemoveTemporaryFiles(SALOMEDS_Tool::GetDirFromPath(aHDFUrl), aFilesToRemove, true);
      }
      delete aHDFUrl;
      if (aLocked) anSCO->GetStudy()->GetProperties()->SetLocked(true);
      return;
    }
    
    try {
      Translate_persistentID_to_IOR (Lab,aDriver,_orb, aMultifileState[0]=='M', ASCIIfileState[0] == 'A');
    } catch (SALOME::SALOME_Exception) {
      MESSAGE("Can't translate PersRef to IOR");
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
/*! Function : AddAttribute
 *  Purpose  : 
 */
//============================================================================
/*
void SALOMEDS_StudyBuilder_i::AddAttribute(SALOMEDS::SObject_ptr anObject, 
					   SALOMEDS::AttributeType aType, 
					   const char * AttributeValue )
{
  TDF_Label Lab;
  ASSERT(!CORBA::is_nil(anObject));
  CORBA::String_var anobid = anObject->GetID();
  TDF_Tool::Label(_doc->GetData(),strdup(anobid),Lab);
  
  TCollection_ExtendedString Value(strdup(AttributeValue)); 

  Standard_GUID ID = SALOMEDS_SObject_i::TypeToAttributeID(aType);

  if      (ID == TDataStd_Name::GetID())             TDataStd_Name::Set            (Lab,Value); 
  else if (ID == TDataStd_Comment::GetID())          TDataStd_Comment::Set         (Lab,Value); 
  else if (ID == SALOMEDS_IORAttribute::GetID())     SALOMEDS_IORAttribute::Set    (Lab,Value); 
  else if (ID == SALOMEDS_PersRefAttribute::GetID()) SALOMEDS_PersRefAttribute::Set(Lab,Value); 
}
*/
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
  TDF_Tool::Label(_doc->GetData(),strdup(anobid),Lab);

  if (strcmp(aTypeOfAttribute, "AttributeReal") == 0 ) {
    Handle(TDataStd_Real) anAttr;
    if (!Lab.FindAttribute(TDataStd_Real::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new TDataStd_Real;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeReal_i* aRealAttr = new SALOMEDS_AttributeReal_i(anAttr, _orb);
    SALOMEDS::AttributeReal_var aRA = aRealAttr->AttributeReal::_this();
    return  aRA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeInteger") == 0 ) {
    Handle(TDataStd_Integer) anAttr;
    if (!Lab.FindAttribute(TDataStd_Integer::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new TDataStd_Integer;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeInteger_i* aIntAttr = new SALOMEDS_AttributeInteger_i(anAttr, _orb);
    SALOMEDS::AttributeInteger_var aIA = aIntAttr->AttributeInteger::_this();
    return  aIA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeSequenceOfReal") == 0 ) {
    Handle(SALOMEDS_SequenceOfRealAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_SequenceOfRealAttribute::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new SALOMEDS_SequenceOfRealAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeSequenceOfReal_i* aSeqRealAttr = new SALOMEDS_AttributeSequenceOfReal_i(anAttr, _orb);
    SALOMEDS::AttributeSequenceOfReal_var aSRA = aSeqRealAttr->AttributeSequenceOfReal::_this();
    return  aSRA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeSequenceOfInteger") == 0 ) {
    Handle(SALOMEDS_SequenceOfIntegerAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_SequenceOfIntegerAttribute::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new SALOMEDS_SequenceOfIntegerAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeSequenceOfInteger_i* aSeqIntegerAttr = new SALOMEDS_AttributeSequenceOfInteger_i(anAttr, _orb);
    SALOMEDS::AttributeSequenceOfInteger_var aSIA = aSeqIntegerAttr->AttributeSequenceOfInteger::_this();
    return  aSIA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeName") == 0 ) {
    Handle(TDataStd_Name) anAttr;
    if (!Lab.FindAttribute(TDataStd_Name::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new TDataStd_Name;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeName_i* aNameAttr = new SALOMEDS_AttributeName_i(anAttr, _orb);
    SALOMEDS::AttributeName_var aSNA = aNameAttr->AttributeName::_this();
    return  aSNA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeComment") == 0 ) {
    Handle(TDataStd_Comment) anAttr;
    if (!Lab.FindAttribute(TDataStd_Comment::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new TDataStd_Comment;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeComment_i* aCommentAttr = new SALOMEDS_AttributeComment_i(anAttr, _orb);
    SALOMEDS::AttributeComment_var aCA = aCommentAttr->AttributeComment::_this();
    return  aCA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeExternalFileDef") == 0 ) {
    Handle(SALOMEDS_ExternalFileDef) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_ExternalFileDef::GetID(), anAttr)) {
      anAttr = new SALOMEDS_ExternalFileDef;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeExternalFileDef_i* anExternalFileDefAttr = new SALOMEDS_AttributeExternalFileDef_i(anAttr, _orb);
    SALOMEDS::AttributeExternalFileDef_var aCA = anExternalFileDefAttr->AttributeExternalFileDef::_this();
    return  aCA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeFileType") == 0 ) {
    Handle(SALOMEDS_FileType) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_FileType::GetID(), anAttr)) {
      anAttr = new SALOMEDS_FileType;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeFileType_i* anFileTypeAttr = new SALOMEDS_AttributeFileType_i(anAttr, _orb);
    SALOMEDS::AttributeFileType_var aCA = anFileTypeAttr->AttributeFileType::_this();
    return  aCA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeIOR") == 0 ) {
    Handle(SALOMEDS_IORAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_IORAttribute::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new SALOMEDS_IORAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeIOR_i* aIORAttr = new SALOMEDS_AttributeIOR_i(anAttr, _orb);
    SALOMEDS::AttributeIOR_var aIA = aIORAttr->AttributeIOR::_this();
    return  aIA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributePersistentRef") == 0 ) {
    Handle(SALOMEDS_PersRefAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_PersRefAttribute::GetID(), anAttr)) {
      anAttr = new SALOMEDS_PersRefAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributePersistentRef_i* aPerRefAttr = new SALOMEDS_AttributePersistentRef_i(anAttr, _orb);
    SALOMEDS::AttributePersistentRef_var aPRA = aPerRefAttr->AttributePersistentRef::_this();
    return  aPRA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeDrawable") == 0 ) {
    Handle(SALOMEDS_DrawableAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_DrawableAttribute::GetID(), anAttr)) {
      anAttr = new SALOMEDS_DrawableAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeDrawable_i* aDrawableAttr = new SALOMEDS_AttributeDrawable_i(anAttr, _orb);
    SALOMEDS::AttributeDrawable_var aDrawA = aDrawableAttr->AttributeDrawable::_this();
    return  aDrawA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeSelectable") == 0 ) {
    Handle(SALOMEDS_SelectableAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_SelectableAttribute::GetID(), anAttr)) {
      anAttr = new SALOMEDS_SelectableAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeSelectable_i* aSelectableAttr = new SALOMEDS_AttributeSelectable_i(anAttr, _orb);
    SALOMEDS::AttributeSelectable_var aSelA = aSelectableAttr->AttributeSelectable::_this();
    return  aSelA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeExpandable") == 0 ) {
    Handle(SALOMEDS_ExpandableAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_ExpandableAttribute::GetID(), anAttr)) {
      anAttr = new SALOMEDS_ExpandableAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeExpandable_i* aExpandableAttr = new SALOMEDS_AttributeExpandable_i(anAttr, _orb);
    SALOMEDS::AttributeExpandable_var aExpA = aExpandableAttr->AttributeExpandable::_this();
    return  aExpA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeOpened") == 0 ) {
    Handle(SALOMEDS_OpenedAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_OpenedAttribute::GetID(), anAttr)) {
      anAttr = new SALOMEDS_OpenedAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeOpened_i* aOpenedAttr = new SALOMEDS_AttributeOpened_i(anAttr, _orb);
    SALOMEDS::AttributeOpened_var aOpenA = aOpenedAttr->AttributeOpened::_this();
    return  aOpenA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeTextColor") == 0 ) {
    Handle(SALOMEDS_TextColorAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_TextColorAttribute::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new SALOMEDS_TextColorAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeTextColor_i* aTextColorAttr = new SALOMEDS_AttributeTextColor_i(anAttr, _orb);
    SALOMEDS::AttributeTextColor_var aTCA = aTextColorAttr->AttributeTextColor::_this();
    return  aTCA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeTextHighlightColor") == 0 ) {
    Handle(SALOMEDS_TextHighlightColorAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_TextHighlightColorAttribute::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new SALOMEDS_TextHighlightColorAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeTextHighlightColor_i* aTextHighlightColorAttr = new SALOMEDS_AttributeTextHighlightColor_i(anAttr, _orb);
    SALOMEDS::AttributeTextHighlightColor_var aTHCA = aTextHighlightColorAttr->AttributeTextHighlightColor::_this();
    return  aTHCA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributePixMap") == 0 ) {
    Handle(SALOMEDS_PixMapAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_PixMapAttribute::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new SALOMEDS_PixMapAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributePixMap_i* aPixMapAttr = new SALOMEDS_AttributePixMap_i(anAttr, _orb);
    SALOMEDS::AttributePixMap_var aPMA = aPixMapAttr->AttributePixMap::_this();
    return  aPMA._retn();
  }
  else if (strncmp(aTypeOfAttribute, "AttributeTreeNode",17) == 0 ) {
    Standard_GUID aTreeNodeGUID;
    if (strcmp(aTypeOfAttribute, "AttributeTreeNode") == 0) 
      aTreeNodeGUID = TDataStd_TreeNode::GetDefaultTreeID();
    else {
      char* aGUIDString = new char[40];
      sprintf(aGUIDString, &(aTypeOfAttribute[21]));
      Standard_GUID aGUID = Standard_GUID(aGUIDString); // create tree node GUID by name
      delete(aGUIDString);
    }
    Handle(TDataStd_TreeNode) anAttr;
    if (!Lab.FindAttribute(aTreeNodeGUID, anAttr)) {
      CheckLocked();
      anAttr = TDataStd_TreeNode::Set(Lab, aTreeNodeGUID);
    }
    SALOMEDS_AttributeTreeNode_i* aTreeNodeAttr = new SALOMEDS_AttributeTreeNode_i(anAttr, _orb);
    SALOMEDS::AttributeTreeNode_var aTNA = aTreeNodeAttr->AttributeTreeNode::_this();
    return  aTNA._retn();
  }
  else if (strncmp(aTypeOfAttribute, "AttributeUserID",15) == 0 ) {
    Handle(TDataStd_UAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_AttributeUserID_i::DefaultID(), anAttr)) {
      CheckLocked();
      anAttr = TDataStd_UAttribute::Set(Lab, SALOMEDS_AttributeUserID_i::DefaultID());
    }
    SALOMEDS_AttributeUserID_i* aUAttr = new SALOMEDS_AttributeUserID_i(anAttr, _orb);
    SALOMEDS::AttributeUserID_var aUA = aUAttr->AttributeUserID::_this();
    return  aUA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeLocalID") == 0 ) {
    Handle(SALOMEDS_LocalIDAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_LocalIDAttribute::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new SALOMEDS_LocalIDAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeLocalID_i* aLIDAttr = new SALOMEDS_AttributeLocalID_i(anAttr, _orb);
    SALOMEDS::AttributeLocalID_var aLIDA = aLIDAttr->AttributeLocalID::_this();
    return  aLIDA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeTarget") == 0 ) {
    Handle(SALOMEDS_TargetAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_TargetAttribute::GetID(), anAttr)) {
      anAttr = new SALOMEDS_TargetAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeTarget_i* aLIDAttr = new SALOMEDS_AttributeTarget_i(anAttr, _orb);
    SALOMEDS::AttributeTarget_var aLIDA = aLIDAttr->AttributeTarget::_this();
    return  aLIDA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeTableOfInteger") == 0 ) {
    Handle(SALOMEDS_TableOfIntegerAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_TableOfIntegerAttribute::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new SALOMEDS_TableOfIntegerAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeTableOfInteger_i* aTabIntegerAttr = new SALOMEDS_AttributeTableOfInteger_i(anAttr, _orb);
    SALOMEDS::AttributeTableOfInteger_var aTIA = aTabIntegerAttr->AttributeTableOfInteger::_this();
    return  aTIA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeTableOfReal") == 0 ) {
    Handle(SALOMEDS_TableOfRealAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_TableOfRealAttribute::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new SALOMEDS_TableOfRealAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeTableOfReal_i* aTabRealAttr = new SALOMEDS_AttributeTableOfReal_i(anAttr, _orb);
    SALOMEDS::AttributeTableOfReal_var aTRA = aTabRealAttr->AttributeTableOfReal::_this();
    return  aTRA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeTableOfString") == 0 ) {
    Handle(SALOMEDS_TableOfStringAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_TableOfStringAttribute::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new SALOMEDS_TableOfStringAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributeTableOfString_i* aTabStringAttr = new SALOMEDS_AttributeTableOfString_i(anAttr, _orb);
    SALOMEDS::AttributeTableOfString_var aTRA = aTabStringAttr->AttributeTableOfString::_this();
    return  aTRA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributeStudyProperties") == 0 ) {
    Handle(SALOMEDS_StudyPropertiesAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_StudyPropertiesAttribute::GetID(), anAttr)) {
      CheckLocked();
      if (!Lab.FindAttribute(SALOMEDS_StudyPropertiesAttribute::GetID(), anAttr)) {
	anAttr = new SALOMEDS_StudyPropertiesAttribute;
	Lab.AddAttribute(anAttr); 
	anAttr->SetModified(0);
      }
    }
    SALOMEDS_AttributeStudyProperties_i* aStPropAttr = new SALOMEDS_AttributeStudyProperties_i(anAttr, _orb);
    SALOMEDS::AttributeStudyProperties_var aSPA = aStPropAttr->AttributeStudyProperties::_this();
    return  aSPA._retn();
  }
  else if (strcmp(aTypeOfAttribute, "AttributePythonObject") == 0 ) {
    Handle(SALOMEDS_PythonObjectAttribute) anAttr;
    if (!Lab.FindAttribute(SALOMEDS_PythonObjectAttribute::GetID(), anAttr)) {
      CheckLocked();
      anAttr = new SALOMEDS_PythonObjectAttribute;
      Lab.AddAttribute(anAttr); 
    }
    SALOMEDS_AttributePythonObject_i* aPObjAttr = new SALOMEDS_AttributePythonObject_i(anAttr, _orb);
    SALOMEDS::AttributePythonObject_var aPOA = aPObjAttr->AttributePythonObject::_this();
    return  aPOA._retn();
  }
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
  TDF_Tool::Label(_doc->GetData(),strdup(anobid),Lab);
  Handle(TDF_Attribute) anAttr;
  Standard_Boolean found = Lab.FindAttribute(SALOMEDS_SObject_i::ReturnGUIDForAttribute(aTypeOfAttribute), anAttr);
  if (found) {
    if (strcmp(aTypeOfAttribute, "AttributeReal") == 0 )  {
      SALOMEDS_AttributeReal_i* Attr= new SALOMEDS_AttributeReal_i(Handle(TDataStd_Real)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeReal::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeInteger") == 0 )  {
      SALOMEDS_AttributeInteger_i* Attr= new SALOMEDS_AttributeInteger_i(Handle(TDataStd_Integer)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeInteger::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeSequenceOfReal") == 0 )  {
      SALOMEDS_AttributeSequenceOfReal_i* Attr= new SALOMEDS_AttributeSequenceOfReal_i(Handle(SALOMEDS_SequenceOfRealAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeSequenceOfReal::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeSequenceOfInteger") == 0 )  {
      SALOMEDS_AttributeSequenceOfInteger_i* Attr= new SALOMEDS_AttributeSequenceOfInteger_i(Handle(SALOMEDS_SequenceOfIntegerAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeSequenceOfInteger::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeName") == 0 ) {
      SALOMEDS_AttributeName_i* Attr= new SALOMEDS_AttributeName_i(Handle(TDataStd_Name)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeName::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeComment") == 0 ) {
      SALOMEDS_AttributeComment_i* Attr= new SALOMEDS_AttributeComment_i(Handle(TDataStd_Comment)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeComment::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeExternalFileDef") == 0 ) {
      SALOMEDS_AttributeExternalFileDef_i* Attr= new SALOMEDS_AttributeExternalFileDef_i(Handle(SALOMEDS_ExternalFileDef)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeExternalFileDef::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeFileType") == 0 ) {
      SALOMEDS_AttributeFileType_i* Attr= new SALOMEDS_AttributeFileType_i(Handle(SALOMEDS_FileType)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeFileType::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeIOR") == 0 ) {
      SALOMEDS_AttributeIOR_i* Attr= new SALOMEDS_AttributeIOR_i(Handle(SALOMEDS_IORAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeIOR::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributePersistentRef") == 0 )  {
      SALOMEDS_AttributePersistentRef_i* Attr= new SALOMEDS_AttributePersistentRef_i(Handle(SALOMEDS_PersRefAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributePersistentRef::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeDrawable") == 0 )  {
      SALOMEDS_AttributeDrawable_i* Attr= new SALOMEDS_AttributeDrawable_i(Handle(SALOMEDS_DrawableAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeDrawable::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeSelectable") == 0 )  {
      SALOMEDS_AttributeSelectable_i* Attr= new SALOMEDS_AttributeSelectable_i(Handle(SALOMEDS_SelectableAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeSelectable::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeExpandable") == 0 )  {
      SALOMEDS_AttributeExpandable_i* Attr= new SALOMEDS_AttributeExpandable_i(Handle(SALOMEDS_ExpandableAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeExpandable::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeOpened") == 0 )  {
      SALOMEDS_AttributeOpened_i* Attr= new SALOMEDS_AttributeOpened_i(Handle(SALOMEDS_OpenedAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeOpened::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeTextColor") == 0 )  {
      SALOMEDS_AttributeTextColor_i* Attr= new SALOMEDS_AttributeTextColor_i(Handle(SALOMEDS_TextColorAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeTextColor::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeTextHighlightColor") == 0 )  {
      SALOMEDS_AttributeTextHighlightColor_i* Attr= new SALOMEDS_AttributeTextHighlightColor_i(Handle(SALOMEDS_TextHighlightColorAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeTextHighlightColor::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributePixMap") == 0 )  {
      SALOMEDS_AttributePixMap_i* Attr= new SALOMEDS_AttributePixMap_i(Handle(SALOMEDS_PixMapAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributePixMap::_this();
      return Standard_True;
    }
    else if (strncmp(aTypeOfAttribute, "AttributeTreeNode",17) == 0 )  {
      SALOMEDS_AttributeTreeNode_i* Attr= new SALOMEDS_AttributeTreeNode_i(Handle(TDataStd_TreeNode)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeTreeNode::_this();
      return Standard_True;
    }
    else if (strncmp(aTypeOfAttribute, "AttributeUserID",15) == 0 )  {
      SALOMEDS_AttributeUserID_i* Attr= new SALOMEDS_AttributeUserID_i(Handle(TDataStd_UAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeUserID::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeLocalID") == 0 )  {
      SALOMEDS_AttributeLocalID_i* Attr= new SALOMEDS_AttributeLocalID_i(Handle(SALOMEDS_LocalIDAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeLocalID::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeTarget") == 0 )  {
      SALOMEDS_AttributeTarget_i* Attr= new SALOMEDS_AttributeTarget_i(Handle(SALOMEDS_TargetAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeTarget::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeTableOfInteger") == 0 )  {
      SALOMEDS_AttributeTableOfInteger_i* Attr= new SALOMEDS_AttributeTableOfInteger_i(Handle(SALOMEDS_TableOfIntegerAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeTableOfInteger::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeTableOfReal") == 0 )  {
      SALOMEDS_AttributeTableOfReal_i* Attr= new SALOMEDS_AttributeTableOfReal_i(Handle(SALOMEDS_TableOfRealAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeTableOfReal::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeTableOfString") == 0 )  {
      SALOMEDS_AttributeTableOfString_i* Attr= new SALOMEDS_AttributeTableOfString_i(Handle(SALOMEDS_TableOfStringAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeTableOfString::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributeStudyProperties") == 0 )  {
      SALOMEDS_AttributeStudyProperties_i* Attr= new SALOMEDS_AttributeStudyProperties_i(Handle(SALOMEDS_StudyPropertiesAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributeStudyProperties::_this();
      return Standard_True;
    }
    else if (strcmp(aTypeOfAttribute, "AttributePythonObject") == 0 )  {
      SALOMEDS_AttributePythonObject_i* Attr= new SALOMEDS_AttributePythonObject_i(Handle(SALOMEDS_PythonObjectAttribute)::DownCast(anAttr), _orb);
      anAttribute = Attr->AttributePythonObject::_this();
      return Standard_True;
    }
  } else return Standard_False;
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
  TDF_Tool::Label(_doc->GetData(),strdup(anobid),Lab);
  
  if (strcmp(aTypeOfAttribute, "AttributeIOR") == 0) { // postponed removing of CORBA objects
    Handle(SALOMEDS_IORAttribute) anAttr;
    if (Lab.FindAttribute(SALOMEDS_IORAttribute::GetID(), anAttr))
      SALOMEDS_Study_i::GetStudy(_doc->Main(), _orb)->AddPostponed(strdup(TCollection_AsciiString(anAttr->Get()).ToCString()));
    else return;
  }

  Lab.ForgetAttribute (SALOMEDS_SObject_i::ReturnGUIDForAttribute(aTypeOfAttribute));
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
  TDF_Tool::Label(_doc->GetData(),strdup(meid),Lab);  
  TDF_Label RefLab;
  ASSERT(!CORBA::is_nil(theReferencedObject));
  CORBA::String_var roid = theReferencedObject->GetID();
  TDF_Tool::Label(_doc->GetData(),strdup(roid),RefLab);
  TDF_Reference::Set(Lab,RefLab);

  SALOMEDS_TargetAttribute::Set(RefLab)->Append(Lab);

  if(!CORBA::is_nil(_callbackOnRemove)) _callbackOnRemove->OnRemoveSObject(me);
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
  TDF_Tool::Label(_doc->GetData(), strdup(anEntry), aLabel);
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
  TDF_Tool::Label(_doc->GetData(), strdup(anEntry), aLabel);
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
  Handle(SALOMEDS_StudyPropertiesAttribute) anAttr;
  if (!_doc->Main().FindAttribute(SALOMEDS_StudyPropertiesAttribute::GetID(), anAttr)) {
    anAttr = new SALOMEDS_StudyPropertiesAttribute;
    _doc->Main().AddAttribute(anAttr);
  }
  if (anAttr->IsLocked() && !anAttr->IsLockChanged(true)) {
    INFOS("Locked document modification !!!");
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
  _doc->HasOpenCommand();
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
  Handle(SALOMEDS_StudyPropertiesAttribute) anAttr;
  if (!_doc->Main().FindAttribute(SALOMEDS_StudyPropertiesAttribute::GetID(), anAttr)) {
    anAttr = new SALOMEDS_StudyPropertiesAttribute;
    _doc->Main().AddAttribute(anAttr);
    }
  if (anAttr->IsLocked()) {
    INFOS("Locked document modification !!!");
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
   Handle(SALOMEDS_StudyPropertiesAttribute) anAttr;
    if (!_doc->Main().FindAttribute(SALOMEDS_StudyPropertiesAttribute::GetID(), anAttr)) {
      anAttr = new SALOMEDS_StudyPropertiesAttribute;
      _doc->Main().AddAttribute(anAttr);
    }

  if (anAttr->IsLocked()) {
    INFOS("Locked document modification !!!");
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

void SALOMEDS_StudyBuilder_i::CheckLocked() throw (SALOMEDS::StudyBuilder::LockProtection) {
  if (_doc->HasOpenCommand()) return;
  Handle(SALOMEDS_StudyPropertiesAttribute) anAttr;
  if (!_doc->Main().FindAttribute(SALOMEDS_StudyPropertiesAttribute::GetID(), anAttr)) {
    anAttr = new SALOMEDS_StudyPropertiesAttribute;
    _doc->Main().AddAttribute(anAttr);
  }
  if (anAttr->IsLocked()) throw SALOMEDS::StudyBuilder::LockProtection();
}
