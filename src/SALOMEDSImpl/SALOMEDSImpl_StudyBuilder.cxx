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
// See http://www.salome-platform.org/ or email : webmaster.salome@opencascade.com
//
//  File   : SALOMEDSImpl_StudyBuilder.cxx
//  Author : Sergey RUIN
//  Module : SALOME



#include "SALOMEDSImpl_Attributes.hxx"

using namespace std;

#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_Tool.hxx"

#include <SALOMEDSImpl_ChildNodeIterator.hxx>

#include <TDF_ChildIterator.hxx>
#include <TDF_Label.hxx>
#include <TDF_Tool.hxx>
#include <TDF_Data.hxx>
#include <TDF_ListIteratorOfAttributeList.hxx>

#include <OSD_Path.hxx>

#include <HDFOI.hxx>
#include <stdlib.h> 

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_StudyBuilder, MMgt_TShared )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_StudyBuilder, MMgt_TShared )

#define USE_CASE_LABEL_TAG            2
#define DIRECTORYID                   16661
#define FILELOCALID                   26662 

static void Translate_persistentID_to_IOR(TDF_Label& Lab, SALOMEDSImpl_Driver* driver, bool isMultiFile, bool isASCII);

//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_StudyBuilder::SALOMEDSImpl_StudyBuilder(const Handle(Standard_Transient)& theOwner)
{
   _errorCode = "";
   _study = theOwner;
   _doc = Handle(SALOMEDSImpl_Study)::DownCast(theOwner)->GetDocument();
}

//============================================================================
/*! Function : destructor
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_StudyBuilder::~SALOMEDSImpl_StudyBuilder()
{}

//============================================================================
/*! Function : NewComponent
 *  Purpose  : Create a new component (Scomponent)
 */
//============================================================================
Handle(SALOMEDSImpl_SComponent) SALOMEDSImpl_StudyBuilder::NewComponent(const TCollection_AsciiString& DataType)
{
  _errorCode = "";
  CheckLocked();

  if(DataType.Length() == 0) return 0;

  //Always create component under main label.
  TDF_Label L  = _doc->Main();

  int imax = 0;
  for (TDF_ChildIterator it(L); it.More(); it.Next()) {
    if (it.Value().Tag() > imax)
      imax = it.Value().Tag();
  }
  imax++;
  TDF_Label NL = L.FindChild(imax);

  SALOMEDSImpl_AttributeComment::Set(NL, DataType);

  Handle(SALOMEDSImpl_SComponent) so =  Handle(SALOMEDSImpl_Study)::DownCast(_study)->GetSComponent (NL);

  if(!_callbackOnAdd.IsNull()) _callbackOnAdd->OnAddSObject(so);

  _doc->Modify();

  return so;
}

//============================================================================
/*! Function : DefineComponentInstance
 *  Purpose  : Add IOR attribute of a Scomponent
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::DefineComponentInstance(const Handle(SALOMEDSImpl_SComponent)& aComponent,
							const TCollection_AsciiString& IOR)
{
   _errorCode = "";

  CheckLocked();
  if(aComponent.IsNull() || IOR.IsEmpty()) {
    _errorCode = "Invalid arguments";
    return false;
  }
  //add IOR definition 
  SALOMEDSImpl_AttributeIOR::Set(aComponent->GetLabel(), IOR);  

  //_doc->Modify();

  return true;
}

//============================================================================
/*! Function : RemoveComponent
 *  Purpose  : Delete a Scomponent
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::RemoveComponent(const Handle(SALOMEDSImpl_SComponent)& aComponent)
{
   _errorCode = "";
  CheckLocked();
  return RemoveObject(aComponent);
}

//============================================================================
/*! Function : NewObject
 *  Purpose  : Create a new SObject
 */
//============================================================================
Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_StudyBuilder::NewObject(const Handle(SALOMEDSImpl_SObject)& theFatherObject)
{
   _errorCode = "";
  CheckLocked();

  //Find label of father
  TDF_Label Lab = theFatherObject->GetLabel();
  
  //Create a new label
  int imax = 0;
  for (TDF_ChildIterator it(Lab); it.More(); it.Next()) {
    if (it.Value().Tag() > imax)
      imax = it.Value().Tag();
  }
  imax++;
  TDF_Label NewLab = Lab.FindChild(imax);
  
  Handle(SALOMEDSImpl_SObject) so = Handle(SALOMEDSImpl_Study)::DownCast(_study)->GetSObject(NewLab);
  if(!_callbackOnAdd.IsNull()) _callbackOnAdd->OnAddSObject(so);

  _doc->Modify();  
  return so;
}

//============================================================================
/*! Function : NewObjectToTag
 *  Purpose  :
 */
//============================================================================
Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_StudyBuilder::NewObjectToTag(const Handle(SALOMEDSImpl_SObject)& theFatherObject,
								       const int theTag)
{
  _errorCode = "";
  CheckLocked();
  //Find label of father
  TDF_Label Lab = theFatherObject->GetLabel();

  //Create or find label
  TDF_Label NewLab = Lab.FindChild(theTag, 1);

  Handle(SALOMEDSImpl_SObject) so = Handle(SALOMEDSImpl_Study)::DownCast(_study)->GetSObject(NewLab);

  if(!_callbackOnAdd.IsNull()) _callbackOnAdd->OnAddSObject(so);

  _doc->Modify();  
  return so;
}

//============================================================================
/*! Function : RemoveObject
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::RemoveObject(const Handle(SALOMEDSImpl_SObject)& anObject)
{
   _errorCode = "";
  CheckLocked();
  if(anObject.IsNull()) {
    _errorCode = "Null object";
    return false;
  }

  if(!_callbackOnRemove.IsNull()) _callbackOnRemove->OnRemoveSObject(anObject);

  TDF_Label Lab = anObject->GetLabel();

  Handle(SALOMEDSImpl_AttributeReference) aReference;
  if (Lab.FindAttribute(SALOMEDSImpl_AttributeReference::GetID(), aReference)) {
    Handle(SALOMEDSImpl_AttributeTarget) aTarget;
    if (aReference->Get().FindAttribute(SALOMEDSImpl_AttributeTarget::GetID(),aTarget))
      aTarget->Remove(SALOMEDSImpl_Study::SObject(Lab));
  }

  Handle(SALOMEDSImpl_AttributeIOR) anAttr; // postponed removing of CORBA objects
  if (Lab.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID(), anAttr))
    SALOMEDSImpl_Study::GetStudy(_doc->Main())->AddPostponed(TCollection_AsciiString(anAttr->Value()).ToCString());

  Lab.ForgetAllAttributes();
 
  _doc->Modify();  
    
  return true;
}

//============================================================================
/*! Function : RemoveObjectWithChildren
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::RemoveObjectWithChildren(const Handle(SALOMEDSImpl_SObject)& anObject)
{
   _errorCode = "";
  CheckLocked();
  if(anObject.IsNull()) {
    _errorCode = "Null object";
    return false;
  }

  if(!_callbackOnRemove.IsNull()) _callbackOnRemove->OnRemoveSObject(anObject);

  TDF_Label Lab = anObject->GetLabel();

  Handle(SALOMEDSImpl_AttributeReference) aReference;
  if (Lab.FindAttribute(SALOMEDSImpl_AttributeReference::GetID(), aReference)) {
    Handle(SALOMEDSImpl_AttributeTarget) aTarget;
    if (aReference->Get().FindAttribute(SALOMEDSImpl_AttributeTarget::GetID(),aTarget))
      aTarget->Remove(SALOMEDSImpl_Study::SObject(Lab));
  }
  Handle(SALOMEDSImpl_AttributeIOR) anAttr; // postponed removing of CORBA objects
  if (Lab.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID(), anAttr))
    SALOMEDSImpl_Study::GetStudy(_doc->Main())->AddPostponed(TCollection_AsciiString(anAttr->Value()).ToCString());

  TDF_ChildIterator it(Lab, Standard_True);
  for(;it.More();it.Next()) {
    TDF_Label aLabel = it.Value();
    if (aLabel.FindAttribute(SALOMEDSImpl_AttributeReference::GetID(), aReference)) {
      Handle(SALOMEDSImpl_AttributeTarget) aTarget;
      if (aReference->Get().FindAttribute(SALOMEDSImpl_AttributeTarget::GetID(),aTarget))
	aTarget->Remove(SALOMEDSImpl_Study::SObject(aLabel));
    }
    Handle(SALOMEDSImpl_AttributeIOR) anAttr; // postponed removing of CORBA objects
    if (aLabel.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID(), anAttr))
      SALOMEDSImpl_Study::GetStudy(_doc->Main())->AddPostponed(TCollection_AsciiString(anAttr->Value()).ToCString());
  }

  Lab.ForgetAllAttributes(Standard_True);

  _doc->Modify();  
  
  return true;
}

//============================================================================
/*! Function : LoadWith
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::LoadWith(const Handle(SALOMEDSImpl_SComponent)& anSCO,
                                         SALOMEDSImpl_Driver* aDriver) 
{
  _errorCode = "";

  TDF_Label Lab = anSCO->GetLabel();
  Handle(SALOMEDSImpl_AttributePersistentRef) Att;

  //Find the current Url of the study  
  if (_doc->Main().FindAttribute(SALOMEDSImpl_AttributePersistentRef::GetID(),Att)) {
    int aLocked = anSCO->GetStudy()->GetProperties()->IsLocked();
    if (aLocked) anSCO->GetStudy()->GetProperties()->SetLocked(false);

    TCollection_ExtendedString Res(Att->Value());
    TCollection_AsciiString aHDFPath(Res);

    Handle(SALOMEDSImpl_AttributeComment) type;
    TCollection_ExtendedString DataType;
    if (Lab.FindAttribute(SALOMEDSImpl_AttributeComment::GetID(),type))
      DataType = type->Value();

    // associate the driver to the SComponent
    if(aDriver == NULL) {
      _errorCode = "Driver is null";
      return false;
    }

    // mpv 06.03.2003: SAL1927 - if component data if already loaded, it is not necessary to do it again
    Handle(SALOMEDSImpl_AttributeIOR) attrIOR;
    if (Lab.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID(), attrIOR)) {
      if (aLocked) anSCO->GetStudy()->GetProperties()->SetLocked(true);
      return true;
    }

    DefineComponentInstance (anSCO, aDriver->GetIOR());

    TCollection_AsciiString aHDFUrl;
    bool isASCII = false;
    if (HDFascii::isASCII(aHDFPath.ToCString())) {
      isASCII = true;
      aHDFUrl = HDFascii::ConvertFromASCIIToHDF(aHDFPath.ToCString());
      aHDFUrl += "hdf_from_ascii.hdf";
    } else {
      aHDFUrl = aHDFPath;
    }

    //Open the Study HDF file 
    HDFfile *hdf_file = new HDFfile(aHDFUrl.ToCString()); 

    char aMultifileState[2] = "S"; // default: single
    char ASCIIfileState[2] = "B"; // default: binary
    try {
      TCollection_AsciiString scoid = anSCO->GetID();
      hdf_file->OpenOnDisk(HDF_RDONLY);
      HDFgroup *hdf_group = new HDFgroup("DATACOMPONENT",hdf_file);
      hdf_group->OpenOnDisk();
      HDFgroup *hdf_sco_group = new HDFgroup(scoid.ToCString(), hdf_group);
      hdf_sco_group->OpenOnDisk();

      unsigned char* aStreamFile = NULL;
      int aStreamSize = 0;

      if (hdf_sco_group->ExistInternalObject("FILE_STREAM")) {
	HDFdataset *hdf_dataset = new HDFdataset("FILE_STREAM", hdf_sco_group);
	hdf_dataset->OpenOnDisk();
	aStreamSize = hdf_dataset->GetSize();
	aStreamFile  = new unsigned char[aStreamSize];
	if(aStreamFile == NULL) throw HDFexception("Unable to open dataset FILE_STREAM");
	hdf_dataset->ReadFromDisk(aStreamFile);
	hdf_dataset->CloseOnDisk();
	hdf_dataset = 0;
      }
      else
        aStreamFile = NULL;

      if (hdf_sco_group->ExistInternalObject("MULTIFILE_STATE")) {
        HDFdataset *multifile_hdf_dataset = new HDFdataset("MULTIFILE_STATE", hdf_sco_group);
        multifile_hdf_dataset->OpenOnDisk();
        multifile_hdf_dataset->ReadFromDisk(aMultifileState);

        multifile_hdf_dataset->CloseOnDisk();
        multifile_hdf_dataset = 0;
      }

      if (hdf_sco_group->ExistInternalObject("ASCII_STATE")) {
        HDFdataset *ascii_hdf_dataset = new HDFdataset("ASCII_STATE", hdf_sco_group);
        ascii_hdf_dataset->OpenOnDisk();
        ascii_hdf_dataset->ReadFromDisk(ASCIIfileState);

        ascii_hdf_dataset->CloseOnDisk();
        ascii_hdf_dataset = 0;
      }

      // set path without file name from URL 
      //int aFileNameSize = Res.Length();
      //char* aDir = new char[aFileNameSize];
      //memcpy(aDir, TCollection_AsciiString(Res).ToCString(), aFileNameSize);
      //for(int aCounter = aFileNameSize-1; aCounter>=0; aCounter--)
      //if (aDir[aCounter] == '/') {
      //  aDir[aCounter+1] = 0;
      //  break;
      //}
      // Above code was working wrong for paths without '/' inside.
      TCollection_AsciiString aDir = SALOMEDSImpl_Tool::GetDirFromPath(Res);

      bool aResult = (ASCIIfileState[0]=='A')?
	aDriver->LoadASCII(anSCO, aStreamFile, aStreamSize, aDir.ToCString(), aMultifileState[0]=='M'):
	aDriver->Load(anSCO, aStreamFile, aStreamSize, aDir.ToCString(), aMultifileState[0]=='M');

      if(aStreamFile != NULL) delete []aStreamFile; 

      if(!aResult) {
	RemoveAttribute( anSCO, "AttributeIOR" );

	_errorCode = "Can't load component";
	throw HDFexception("Unable to load component");
      }

      //if(aDir != NULL) delete []aDir;

      hdf_sco_group->CloseOnDisk();
      hdf_sco_group = 0;
      hdf_group->CloseOnDisk();
      hdf_group = 0;
      hdf_file->CloseOnDisk();
      delete hdf_file;

      if (isASCII) {
	Handle(TColStd_HSequenceOfAsciiString) aFilesToRemove = new TColStd_HSequenceOfAsciiString;
	aFilesToRemove->Append("hdf_from_ascii.hdf");
	SALOMEDSImpl_Tool::RemoveTemporaryFiles(SALOMEDSImpl_Tool::GetDirFromPath(aHDFUrl),
                                                aFilesToRemove, true);
      }      
    }
    catch (HDFexception) {
      delete hdf_file;

      if (isASCII) {
	Handle(TColStd_HSequenceOfAsciiString) aFilesToRemove = new TColStd_HSequenceOfAsciiString;
	aFilesToRemove->Append(aHDFUrl);
        SALOMEDSImpl_Tool::RemoveTemporaryFiles(SALOMEDSImpl_Tool::GetDirFromPath(aHDFUrl),
                                                aFilesToRemove, true);
      }

      if (aLocked) anSCO->GetStudy()->GetProperties()->SetLocked(true);
      _errorCode = "No persistent file";   
      return false;
    }

    try {
      Translate_persistentID_to_IOR (Lab, aDriver, aMultifileState[0]=='M', ASCIIfileState[0] == 'A');
    } catch(...) {
      _errorCode = "Can not convert persistent IDs to IORs";
      return false;
    }

    if (aLocked) anSCO->GetStudy()->GetProperties()->SetLocked(true);
  } else {
    _errorCode = "No persistent file";   
  }

  return true;
}


//============================================================================
/*! Function : Load
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::Load(const Handle(SALOMEDSImpl_SObject)& sco)
{
  _errorCode = "Not implemented";
  return false;
}

//============================================================================
/*! Function : FindOrCreateAttribute
 *  Purpose  : Add attribute of given type to SObject, if there is attribute of such type, returns
 *  existing one
 */
//============================================================================
Handle(TDF_Attribute) SALOMEDSImpl_StudyBuilder::FindOrCreateAttribute(const Handle(SALOMEDSImpl_SObject)& anObject, 
								       const TCollection_AsciiString& aTypeOfAttribute)
{
  _errorCode = "";
  if(anObject.IsNull()) {
    _errorCode = "Invalid arguments";
    return NULL;
  }

  TDF_Label Lab = anObject->GetLabel();
  if(Lab.IsNull()) {
    _errorCode = "Null label";
    return NULL;
  }

  _doc->Modify();  

  //The macro adds all necessary checks for standardly behaiving attributes
  __FindOrCreateAttributeForBuilder

 
  //Add checks for TreeNode and UserID attributes  
  if (strncmp(aTypeOfAttribute.ToCString(), "AttributeTreeNode",17) == 0 ) {
    
    Standard_GUID aTreeNodeGUID;
    if (strcmp(aTypeOfAttribute.ToCString(), "AttributeTreeNode") == 0) {
      aTreeNodeGUID = SALOMEDSImpl_AttributeTreeNode::GetDefaultTreeID();
    } else {
      char* aGUIDString = new char[41];
      char* aType = (char*)aTypeOfAttribute.ToCString();
      sprintf(aGUIDString, &(aType[17]));
      if(!Standard_GUID::CheckGUIDFormat(aGUIDString)) {
        delete(aGUIDString);
        return NULL;      
      }
      aTreeNodeGUID = Standard_GUID(aGUIDString); // create tree node GUID by name
      delete(aGUIDString);
    }
    Handle(SALOMEDSImpl_AttributeTreeNode) anAttr;
    if (!Lab.FindAttribute(aTreeNodeGUID, anAttr)) {
      CheckLocked();
      anAttr = SALOMEDSImpl_AttributeTreeNode::Set(Lab, aTreeNodeGUID);
    }
    return anAttr;
  }

  if (strncmp(aTypeOfAttribute.ToCString(), "AttributeUserID",15) == 0 ) {
    Standard_GUID aUserGUID;
    if (strcmp(aTypeOfAttribute.ToCString(), "AttributeUserID") == 0) {
      aUserGUID = SALOMEDSImpl_AttributeUserID::DefaultID();
    } else {
      char* aGUIDString = new char[41];
      char* aType = (char*)aTypeOfAttribute.ToCString();
      sprintf(aGUIDString, &(aType[15]));
      if(!Standard_GUID::CheckGUIDFormat(aGUIDString)) {
        delete(aGUIDString);
        return NULL;      
      }
      aUserGUID = Standard_GUID(aGUIDString); // create tree node GUID by name
      delete(aGUIDString);
    }
    Handle(SALOMEDSImpl_AttributeUserID) anAttr;
    if (!Lab.FindAttribute(SALOMEDSImpl_AttributeUserID::DefaultID(), anAttr)) {
      CheckLocked();
      anAttr = SALOMEDSImpl_AttributeUserID::Set(Lab, aUserGUID);
    }
    return anAttr;
  }
  _errorCode = "Can not create an attribute";

  return NULL;
}

//============================================================================
/*! Function : FindAttribute
 *  Purpose  : Find attribute of given type assigned SObject, returns Standard_True if it is found
 */
//============================================================================

bool SALOMEDSImpl_StudyBuilder::FindAttribute(const Handle(SALOMEDSImpl_SObject)& anObject, 
					      Handle(TDF_Attribute)& anAttribute, 
					      const TCollection_AsciiString& aTypeOfAttribute)
{
  _errorCode = "";
  if(anObject.IsNull()) {
    _errorCode = "Invalid arguments";
    return false;
  }
  TDF_Label Lab = anObject->GetLabel();
  if (Lab.FindAttribute(SALOMEDSImpl_SObject::GetGUID(aTypeOfAttribute), anAttribute)) {
    // commented out because NO MODIFICATION is done to attributes when calling FindAttribute()
    // _doc->Modify();  
    return Standard_True;
  }
  return Standard_False;
}

//============================================================================
/*! Function : RemoveAttribute
 *  Purpose  : Remove attribute of given type assigned SObject
 */
//============================================================================

bool SALOMEDSImpl_StudyBuilder::RemoveAttribute(const Handle(SALOMEDSImpl_SObject)& anObject, 
						const TCollection_AsciiString& aTypeOfAttribute)
{
  _errorCode = "";
  CheckLocked();
  if(anObject.IsNull()) {
    _errorCode = "Invalid arguments";
    return false;
  }
  TDF_Label Lab = anObject->GetLabel();
  
  if (aTypeOfAttribute == "AttributeIOR") { // postponed removing of CORBA objects
    Handle(SALOMEDSImpl_AttributeIOR) anAttr;
    if (Lab.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID(), anAttr))
      SALOMEDSImpl_Study::GetStudy(_doc->Main())->AddPostponed(anAttr->Value());
  }

  Lab.ForgetAttribute (SALOMEDSImpl_SObject::GetGUID(aTypeOfAttribute));
    
  _doc->Modify();  
    
  return true;
}

//============================================================================
/*! Function : Addreference
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::Addreference(const Handle(SALOMEDSImpl_SObject)& me, 
					     const Handle(SALOMEDSImpl_SObject)& theReferencedObject)
{
  _errorCode = "";
  if(me.IsNull() || theReferencedObject.IsNull()) {
   _errorCode = "Invalid arguments";
   return false;
  }
  CheckLocked();
  TDF_Label Lab = me->GetLabel();
  TDF_Label RefLab = theReferencedObject->GetLabel();
  SALOMEDSImpl_AttributeReference::Set(Lab,RefLab);

  SALOMEDSImpl_AttributeTarget::Set(RefLab)->Add(SALOMEDSImpl_Study::SObject(Lab));

  if(!_callbackOnRemove.IsNull() && Lab.IsDescendant(_doc->Main())) _callbackOnRemove->OnRemoveSObject(me);
  
  return true;
}

//============================================================================
/*! Function : RemoveReference
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::RemoveReference(const Handle(SALOMEDSImpl_SObject)& me)
{
  _errorCode = "";
  Handle(SALOMEDSImpl_SObject) theReferencedObject;
  if(!me->ReferencedObject(theReferencedObject)) return false;  //No reference is found

  CheckLocked();
  TDF_Label Lab = me->GetLabel();

  Lab.ForgetAttribute(SALOMEDSImpl_AttributeReference::GetID());  

  //SRN: 30 Aug, 2004 : fix from Ecole l'ete version 

  TDF_Label RefLab = theReferencedObject->GetLabel();
       
  Handle(SALOMEDSImpl_AttributeTarget) aTarget;
  if(RefLab.FindAttribute(SALOMEDSImpl_AttributeTarget::GetID(), aTarget)) 
    aTarget->Remove(SALOMEDSImpl_Study::SObject(Lab));
  
  _doc->Modify();  
  
  return true;
}



//============================================================================
/*! Function : AddDirectory
 *  Purpose  : adds a new directory with a path = thePath
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::AddDirectory(const TCollection_AsciiString& thePath) 
{
  _errorCode = "";
  CheckLocked();
  if(thePath.IsEmpty() || thePath == "") {
    _errorCode = "Invalid path";
    return false;
  }

  TCollection_AsciiString aPath(thePath), aContext(""), aFatherPath;
  TDF_Label aLabel;
  Handle(SALOMEDSImpl_Study) aStudy = SALOMEDSImpl_Study::GetStudy(_doc->Main());
  Handle(SALOMEDSImpl_SObject) anObject;

  try { 
    anObject = aStudy->FindObjectByPath(thePath); //Check if the directory already exists
  }
  catch(...) { }

  if(!anObject.IsNull()) {
    _errorCode = "StudyNameAlreadyUsed";
    return false; 
  }

  if(aPath.Value(1) != '/') { //Relative path 
    aPath.Prepend('/');
    aPath = aStudy->GetContext() + aPath;
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

  anObject.Nullify();
  try { 
    anObject = aStudy->FindObjectByPath(aFatherPath); //Check if the father directory exists
  }
  catch(...) { ; }
  if(anObject.IsNull()) {
    _errorCode = "StudyInvalidDirectory";
    return false; 
  }

  Handle(SALOMEDSImpl_SObject) aNewObject = NewObject(anObject);
  aLabel = aNewObject->GetLabel();
  if(aLabel.IsNull()) {
    _errorCode = "StudyInvalidComponent";
    return false;
  }

  SALOMEDSImpl_AttributeName::Set(aLabel, aPath.Token("/", i-1));

  //Set LocalID attribute to identify the directory object
  Handle(SALOMEDSImpl_AttributeLocalID) aLocalID = SALOMEDSImpl_AttributeLocalID::Set(aLabel, DIRECTORYID);
  
  _doc->Modify(); 
  
  return true;
}


//============================================================================
/*! Function : SetGUID
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::SetGUID(const Handle(SALOMEDSImpl_SObject)& anObject, 
					const TCollection_AsciiString& theGUID)
{
  _errorCode = "";
  CheckLocked();
  if(anObject.IsNull()) {
    _errorCode = "Invalid arguments";
    return false;
  }

  TDF_Label aLabel = anObject->GetLabel();
  SALOMEDSImpl_AttributeUserID::Set(aLabel, theGUID.ToCString());

  _doc->Modify();  

  return true;
}

//============================================================================
/*! Function : IsGUID
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::IsGUID(const Handle(SALOMEDSImpl_SObject)& anObject, 
				       const TCollection_AsciiString& theGUID)
{
  _errorCode = "";
  if(anObject.IsNull()) {
    _errorCode = "Invalid arguments";
    return false;
  }
  TDF_Label aLabel = anObject->GetLabel();
  return aLabel.IsAttribute(theGUID.ToCString());
}


//============================================================================
/*! Function : NewCommand
 *  Purpose  : 
 */
//============================================================================
void SALOMEDSImpl_StudyBuilder::NewCommand()
{
  _errorCode = "";
  // mpv: for SAL2114 - unset "lock changed" flag at the operation start
  Handle(SALOMEDSImpl_AttributeStudyProperties) anAttr;
  if (!_doc->Main().FindAttribute(SALOMEDSImpl_AttributeStudyProperties::GetID(), anAttr)) {
    anAttr = new SALOMEDSImpl_AttributeStudyProperties;
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
void SALOMEDSImpl_StudyBuilder::CommitCommand()
{
  _errorCode = "";
  Handle(SALOMEDSImpl_AttributeStudyProperties) anAttr;
  if (!_doc->Main().FindAttribute(SALOMEDSImpl_AttributeStudyProperties::GetID(), anAttr)) {
    anAttr = new SALOMEDSImpl_AttributeStudyProperties;
    _doc->Main().AddAttribute(anAttr);
  }
  if (anAttr->IsLocked() && !anAttr->IsLockChanged(true)) {
    AbortCommand();
    _errorCode = "LockProtection";
    throw LockProtection("LockProtection");
  } else {
    SALOMEDSImpl_Study::GetStudy(_doc->Main())->RemovePostponed(_doc->GetUndoLimit());

    int aModif = anAttr->GetModified();
    if (aModif < 0) aModif = 1000; // if user make undo and then - new transaction "modify" will never be zero
    anAttr->SetModified(aModif+1);
    _doc->CommitCommand();
  }
  
  _doc->Modify();  
}

//============================================================================
/*! Function : HasOpenCommand
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::HasOpenCommand()
{
  _errorCode = "";
  return _doc->HasOpenCommand();
}

//============================================================================
/*! Function : AbortCommand
 *  Purpose  : 
 */
//============================================================================
void SALOMEDSImpl_StudyBuilder::AbortCommand()
{
  _errorCode = "";
  SALOMEDSImpl_Study::GetStudy(_doc->Main())->UndoPostponed(0);
  
  _doc->AbortCommand();
}

//============================================================================
/*! Function : Undo
 *  Purpose  : 
 */
//============================================================================
void SALOMEDSImpl_StudyBuilder::Undo()
{
  _errorCode = "";
  Handle(SALOMEDSImpl_AttributeStudyProperties) anAttr;
  if (!_doc->Main().FindAttribute(SALOMEDSImpl_AttributeStudyProperties::GetID(), anAttr)) {
    anAttr = new SALOMEDSImpl_AttributeStudyProperties;
    _doc->Main().AddAttribute(anAttr);
    }
  if (anAttr->IsLocked()) {
    _errorCode = "LockProtection";
    throw LockProtection("LockProtection");
  } else {
    SALOMEDSImpl_Study::GetStudy(_doc->Main())->UndoPostponed(1);
    _doc->Undo();
    anAttr->SetModified(anAttr->GetModified()-1);
  }

  _doc->Modify();  
}

//============================================================================
/*! Function : Redo
 *  Purpose  : 
 */
//============================================================================
void SALOMEDSImpl_StudyBuilder::Redo() 
{
  _errorCode = "";
  Handle(SALOMEDSImpl_AttributeStudyProperties) anAttr;
  if (!_doc->Main().FindAttribute(SALOMEDSImpl_AttributeStudyProperties::GetID(), anAttr)) {
    anAttr = new SALOMEDSImpl_AttributeStudyProperties;
    _doc->Main().AddAttribute(anAttr);
  }
  
  if (anAttr->IsLocked()) {
    _errorCode = "LockProtection";
    throw LockProtection("LockProtection");
  } else {
    _doc->Redo();
    SALOMEDSImpl_Study::GetStudy(_doc->Main())->UndoPostponed(-1);
    anAttr->SetModified(anAttr->GetModified()+1);
  }
   
  _doc->Modify();  
}

//============================================================================
/*! Function : GetAvailableUndos
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::GetAvailableUndos()
{
  _errorCode = "";
  return _doc->GetAvailableUndos();
}

//============================================================================
/*! Function : GetAvailableRedos
 *  Purpose  : 
 */
//============================================================================
bool  SALOMEDSImpl_StudyBuilder::GetAvailableRedos()
{
  _errorCode = "";
  return _doc->GetAvailableRedos();
}

//============================================================================
/*! Function : UndoLimit
 *  Purpose  : 
 */
//============================================================================
int  SALOMEDSImpl_StudyBuilder::UndoLimit()
{
  _errorCode = "";
  return _doc->GetUndoLimit();
}

//============================================================================
/*! Function : UndoLimit
 *  Purpose  : 
 */
//============================================================================
void SALOMEDSImpl_StudyBuilder::UndoLimit(int n)
{
  _errorCode = "";
  CheckLocked();
  _doc->SetUndoLimit (n);
}

//============================================================================
/*! Function : SetOnAddSObject
 *  Purpose  : 
 */
//============================================================================
Handle(SALOMEDSImpl_Callback) 
SALOMEDSImpl_StudyBuilder::SetOnAddSObject(const Handle(SALOMEDSImpl_Callback)& theCallback)
{
  _errorCode = "";
  Handle(SALOMEDSImpl_Callback) aRet = _callbackOnAdd;
  _callbackOnAdd = theCallback;
  return aRet;
}

//============================================================================
/*! Function : SetOnNewSObject
 *  Purpose  : 
 */
//============================================================================
Handle(SALOMEDSImpl_Callback) 
SALOMEDSImpl_StudyBuilder::SetOnRemoveSObject(const Handle(SALOMEDSImpl_Callback)& theCallback)
{
  _errorCode = "";
  Handle(SALOMEDSImpl_Callback) aRet = _callbackOnRemove;
  _callbackOnRemove = theCallback;
  return aRet;
}

//============================================================================
/*! Function : CheckLocked
 *  Purpose  : 
 */
//============================================================================
void SALOMEDSImpl_StudyBuilder::CheckLocked()
{
  _errorCode = "";
  if (_doc->HasOpenCommand()) return;
  Handle(SALOMEDSImpl_AttributeStudyProperties) anAttr;
  if (!_doc->Main().FindAttribute(SALOMEDSImpl_AttributeStudyProperties::GetID(), anAttr)) {
    anAttr = new SALOMEDSImpl_AttributeStudyProperties;
    _doc->Main().AddAttribute(anAttr);
    }
  if (anAttr->IsLocked()) {
    _errorCode = "LockProtection";
    throw LockProtection("LockProtection");
  }
}

//============================================================================
/*! Function : SetName
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::SetName(const Handle(SALOMEDSImpl_SObject)& theSO, 
					const TCollection_AsciiString& theValue)
{
  _errorCode = "";
  CheckLocked();
  if(theSO.IsNull()) {
    _errorCode = "Invalid arguments";
    return false;
  }
  SALOMEDSImpl_AttributeName::Set(theSO->GetLabel(), theValue);

  _doc->Modify();  

  return true;
}

//============================================================================
/*! Function : SetComment
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::SetComment(const Handle(SALOMEDSImpl_SObject)& theSO, 
					   const TCollection_AsciiString& theValue)
{
  _errorCode = "";
  CheckLocked();
  if(theSO.IsNull()) {
    _errorCode = "Invalid arguments";
    return false;
  }
  SALOMEDSImpl_AttributeComment::Set(theSO->GetLabel(), theValue);

  _doc->Modify();  

  return true;
}

//============================================================================
/*! Function : SetIOR
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::SetIOR(const Handle(SALOMEDSImpl_SObject)& theSO, 
				       const TCollection_AsciiString& theValue)
{
  _errorCode = "";
  CheckLocked();
  if(theSO.IsNull()) {
    _errorCode = "Invalid arguments";
    return false;
  }
  SALOMEDSImpl_AttributeIOR::Set(theSO->GetLabel(), theValue);

  _doc->Modify();  

  return true;
}


//============================================================================
/*! Function : Translate_persistentID_to_IOR
 *  Purpose  :
 */
//============================================================================
static void Translate_persistentID_to_IOR(TDF_Label& Lab, SALOMEDSImpl_Driver* driver, bool isMultiFile, bool isASCII)
{
  if(driver == NULL) return;
  TDF_ChildIterator  itchild (Lab);
  
  for (; itchild.More(); itchild.Next()) {
    TDF_Label current = itchild.Value();
    Handle(SALOMEDSImpl_AttributePersistentRef) Att;
    if (current.FindAttribute(SALOMEDSImpl_AttributePersistentRef::GetID(),Att)) {  

      Handle(SALOMEDSImpl_AttributeLocalID) anID;
      if (current.FindAttribute(SALOMEDSImpl_AttributeLocalID::GetID(), anID)) 
	if (anID->Value() == FILELOCALID) continue;        //SRN: This attribute store a file name, skip it 

      TCollection_AsciiString persist_ref(Att->Value());
      Handle(SALOMEDSImpl_SObject) so = SALOMEDSImpl_Study::SObject(current);
      TCollection_AsciiString ior_string = driver->LocalPersistentIDToIOR(so, 
									  persist_ref, 
									  isMultiFile, 
									  isASCII);
      SALOMEDSImpl_AttributeIOR::Set (current, ior_string); 
     
    }
    Translate_persistentID_to_IOR (current, driver, isMultiFile, isASCII);
  }
}

