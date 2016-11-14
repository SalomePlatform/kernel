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

//  File   : SALOMEDSImpl_StudyBuilder.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_Attributes.hxx"
#include "SALOMEDSImpl_Study.hxx"
#include "SALOMEDSImpl_StudyBuilder.hxx"
#include "SALOMEDSImpl_SObject.hxx"
#include "SALOMEDSImpl_SComponent.hxx"
#include "SALOMEDSImpl_Tool.hxx"
#include "SALOMEDSImpl_ChildNodeIterator.hxx"

#include "DF_ChildIterator.hxx"
#include "DF_Label.hxx"

#include <HDFOI.hxx>
#include <stdlib.h> 
#include <string.h> 

#define USE_CASE_LABEL_TAG            2
#define DIRECTORYID                   16661
#define FILELOCALID                   26662 

static void Translate_persistentID_to_IOR(DF_Label& Lab, SALOMEDSImpl_Driver* driver, bool isMultiFile, bool isASCII);

//============================================================================
/*! Function : constructor
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_StudyBuilder::SALOMEDSImpl_StudyBuilder(const SALOMEDSImpl_Study* theOwner)
{
   _errorCode = "";
  _callbackOnAdd=NULL;
  _callbackOnRemove = NULL;
   _study = (SALOMEDSImpl_Study*)theOwner;
   _doc = _study->GetDocument();
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
SALOMEDSImpl_SComponent SALOMEDSImpl_StudyBuilder::NewComponent(const std::string& DataType)
{
  _errorCode = "";
  CheckLocked();

  SALOMEDSImpl_SComponent sco;

  if(DataType.size() == 0) return sco;

  //Always create component under main label.
  DF_Label L  = _doc->Main();

  DF_Label NL = L.NewChild();

  SALOMEDSImpl_AttributeComment::Set(NL, DataType);

  SALOMEDSImpl_SComponent so =  _study->GetSComponent (NL);

  if(_callbackOnAdd) _callbackOnAdd->OnAddSObject(so);
  _study->addSO_Notification(so);

  _doc->SetModified(true);

  return so;
}

//============================================================================
/*! Function : DefineComponentInstance
 *  Purpose  : Add IOR attribute of a Scomponent
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::DefineComponentInstance(const SALOMEDSImpl_SComponent& aComponent,
                                                        const std::string& IOR)
{
   _errorCode = "";

  CheckLocked();
  if(!aComponent || IOR.empty()) {
    _errorCode = "Invalid arguments";
    return false;
  }
  //add IOR definition 
  SALOMEDSImpl_AttributeIOR::Set(aComponent.GetLabel(), IOR);  

  return true;
}

//============================================================================
/*! Function : RemoveComponent
 *  Purpose  : Delete a Scomponent
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::RemoveComponent(const SALOMEDSImpl_SComponent& aComponent)
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
SALOMEDSImpl_SObject SALOMEDSImpl_StudyBuilder::NewObject(const SALOMEDSImpl_SObject& theFatherObject)
{
  _errorCode = "";
  CheckLocked();

  //Find label of father
  DF_Label Lab = theFatherObject.GetLabel();
  
  //Create a new label
  DF_Label NewLab = Lab.NewChild();
  
  SALOMEDSImpl_SObject so = _study->GetSObject(NewLab);
  if(_callbackOnAdd) _callbackOnAdd->OnAddSObject(so);
  _study->addSO_Notification(so);

  _doc->SetModified(true);  
  return so;
}

//============================================================================
/*! Function : NewObjectToTag
 *  Purpose  :
 */
//============================================================================
SALOMEDSImpl_SObject SALOMEDSImpl_StudyBuilder::NewObjectToTag(const SALOMEDSImpl_SObject& theFatherObject,
                                                        const int theTag)
{
  _errorCode = "";
  CheckLocked();
  //Find label of father
  DF_Label Lab = theFatherObject.GetLabel();

  //Create or find label
  DF_Label NewLab = Lab.FindChild(theTag, 1);

  SALOMEDSImpl_SObject so = _study->GetSObject(NewLab);

  if(_callbackOnAdd) _callbackOnAdd->OnAddSObject(so);
  _study->addSO_Notification(so);

  _doc->SetModified(true);  
  return so;
}

//============================================================================
/*! Function : RemoveObject
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::RemoveObject(const SALOMEDSImpl_SObject& anObject)
{
   _errorCode = "";
  CheckLocked();
  if(!anObject) {
    _errorCode = "Null object";
    return false;
  }

  if(_callbackOnRemove) _callbackOnRemove->OnRemoveSObject(anObject);

  DF_Label Lab = anObject.GetLabel();

  SALOMEDSImpl_AttributeReference* aReference = NULL;
  if ((aReference=(SALOMEDSImpl_AttributeReference*)Lab.FindAttribute(SALOMEDSImpl_AttributeReference::GetID()))) {
    SALOMEDSImpl_AttributeTarget* aTarget = NULL;
    if ((aTarget=(SALOMEDSImpl_AttributeTarget*)aReference->Get().FindAttribute(SALOMEDSImpl_AttributeTarget::GetID())))
      aTarget->Remove(SALOMEDSImpl_Study::SObject(Lab));
  }

  SALOMEDSImpl_AttributeIOR* anAttr = NULL; //Remove from IORLabel map
  if ((anAttr=(SALOMEDSImpl_AttributeIOR*)Lab.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID()))) {
    _study->DeleteIORLabelMapItem(anAttr->Value());
  }

  Lab.ForgetAllAttributes();
 
  _doc->SetModified(true);  
  _study->removeSO_Notification(anObject);
    
  return true;
}

//============================================================================
/*! Function : RemoveObjectWithChildren
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::RemoveObjectWithChildren(const SALOMEDSImpl_SObject& anObject)
{
   _errorCode = "";
  CheckLocked();
  if(!anObject) {
    _errorCode = "Null object";
    return false;
  }

  if(_callbackOnRemove) _callbackOnRemove->OnRemoveSObject(anObject);

  DF_Label Lab = anObject.GetLabel();

  SALOMEDSImpl_AttributeReference* aReference = NULL;
  if ((aReference=(SALOMEDSImpl_AttributeReference*)Lab.FindAttribute(SALOMEDSImpl_AttributeReference::GetID()))) {
    SALOMEDSImpl_AttributeTarget* aTarget = NULL;
    if ((aTarget=(SALOMEDSImpl_AttributeTarget*)aReference->Get().FindAttribute(SALOMEDSImpl_AttributeTarget::GetID())))
      aTarget->Remove(SALOMEDSImpl_Study::SObject(Lab));
  }
  SALOMEDSImpl_AttributeIOR* anAttr = NULL; //Remove from IORLabel map
  if ((anAttr=(SALOMEDSImpl_AttributeIOR*)Lab.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID()))) {
    _study->DeleteIORLabelMapItem(anAttr->Value());
  }

  DF_ChildIterator it(Lab, true);
  for(;it.More();it.Next()) {
    DF_Label aLabel = it.Value();
    if ((aReference=(SALOMEDSImpl_AttributeReference*)aLabel.FindAttribute(SALOMEDSImpl_AttributeReference::GetID()))) {
      SALOMEDSImpl_AttributeTarget* aTarget = NULL;
      if ((aTarget=(SALOMEDSImpl_AttributeTarget*)aReference->Get().FindAttribute(SALOMEDSImpl_AttributeTarget::GetID())))
        aTarget->Remove(SALOMEDSImpl_Study::SObject(aLabel));
    }
    SALOMEDSImpl_AttributeIOR* anAttr = NULL; //Remove from IORLabel map
    if ((anAttr=(SALOMEDSImpl_AttributeIOR*)aLabel.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID()))) {
      _study->DeleteIORLabelMapItem(anAttr->Value());
    }
  }

  Lab.ForgetAllAttributes(true);

  _doc->SetModified(true);  
  _study->removeSO_Notification(anObject);
  
  return true;
}

//============================================================================
/*! Function : LoadWith
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::LoadWith(const SALOMEDSImpl_SComponent& anSCO,
                                         SALOMEDSImpl_Driver* aDriver) 
{
  _errorCode = "";

  DF_Label Lab = anSCO.GetLabel();
  SALOMEDSImpl_AttributePersistentRef* Att = NULL;

  //Find the current Url of the study  
  if ((Att=(SALOMEDSImpl_AttributePersistentRef*)_doc->Main().FindAttribute(SALOMEDSImpl_AttributePersistentRef::GetID()))) {
    int aLocked = _study->GetProperties()->IsLocked();
    if (aLocked) _study->GetProperties()->SetLocked(false);

    std::string Res(Att->Value());
    std::string aHDFPath(Res);

    SALOMEDSImpl_AttributeComment* type = NULL;
    std::string DataType;
    if ((type=(SALOMEDSImpl_AttributeComment*)Lab.FindAttribute(SALOMEDSImpl_AttributeComment::GetID())))
      DataType = type->Value();

    // associate the driver to the SComponent
    if(aDriver == NULL) {
      _errorCode = "Driver is null";
      return false;
    }

    // mpv 06.03.2003: SAL1927 - if component data if already loaded, it is not necessary to do it again
    SALOMEDSImpl_AttributeIOR* attrIOR = NULL;
    if ((attrIOR=(SALOMEDSImpl_AttributeIOR*)Lab.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID()))) {
      if (aLocked) _study->GetProperties()->SetLocked(true);
      return true;
    }

    DefineComponentInstance (anSCO, aDriver->GetIOR());

    std::string aHDFUrl;
    bool isASCII = false;
    if (HDFascii::isASCII(aHDFPath.c_str())) {
      isASCII = true;
      aHDFUrl = HDFascii::ConvertFromASCIIToHDF(aHDFPath.c_str());
      aHDFUrl += "hdf_from_ascii.hdf";
    } else {
      aHDFUrl = aHDFPath;
    }

    //Open the Study HDF file 
    HDFfile *hdf_file = new HDFfile((char*)aHDFUrl.c_str()); 

    char aMultifileState[2] = { '0','0' };
    char ASCIIfileState[2] = { '0','0' };
    bool hasModuleData = false;
    try {
      std::string scoid = anSCO.GetID();
      hdf_file->OpenOnDisk(HDF_RDONLY);
      HDFgroup *hdf_group = new HDFgroup("DATACOMPONENT",hdf_file);
      hdf_group->OpenOnDisk();
      HDFgroup *hdf_sco_group = new HDFgroup((char*)scoid.c_str(), hdf_group);
      hdf_sco_group->OpenOnDisk();
      hasModuleData = true;

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
      } else
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

      std::string aDir = SALOMEDSImpl_Tool::GetDirFromPath(Res);

      bool aResult = true;
      if(aStreamFile && aStreamSize > 0 ) {
        aResult = (ASCIIfileState[0]=='A')?
        aDriver->LoadASCII(anSCO, aStreamFile, aStreamSize, aDir.c_str(), aMultifileState[0]=='M'):
        aDriver->Load(anSCO, aStreamFile, aStreamSize, aDir.c_str(), aMultifileState[0]=='M');
      }

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
        std::vector<std::string> aFilesToRemove;
        aFilesToRemove.push_back("hdf_from_ascii.hdf");
        SALOMEDSImpl_Tool::RemoveTemporaryFiles(SALOMEDSImpl_Tool::GetDirFromPath(aHDFUrl),
                                                aFilesToRemove, true);
      }      
    }
    catch (HDFexception) {
      delete hdf_file;

      if (isASCII) {
        std::vector<std::string> aFilesToRemove;
        aFilesToRemove.push_back(aHDFUrl);
        SALOMEDSImpl_Tool::RemoveTemporaryFiles(SALOMEDSImpl_Tool::GetDirFromPath(aHDFUrl), aFilesToRemove, true);
      }

      if (aLocked) _study->GetProperties()->SetLocked(true);

      if (!hasModuleData)
        return true;

      _errorCode = "No persistent file";   
      return false;
    }

    try {
      Translate_persistentID_to_IOR (Lab, aDriver, aMultifileState[0]=='M', ASCIIfileState[0] == 'A');
    } catch(...) {
      _errorCode = "Can not convert persistent IDs to IORs";
      return false;
    }

    if (aLocked) _study->GetProperties()->SetLocked(true);
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
bool SALOMEDSImpl_StudyBuilder::Load(const SALOMEDSImpl_SObject& sco)
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
DF_Attribute* SALOMEDSImpl_StudyBuilder::FindOrCreateAttribute(const SALOMEDSImpl_SObject& anObject, 
                                                               const std::string& aTypeOfAttribute)
{
  _errorCode = "";
  if(!anObject) {
    _errorCode = "Invalid arguments";
    return NULL;
  }

  DF_Label Lab = anObject.GetLabel();
  if(Lab.IsNull()) {
    _errorCode = "Null label";
    return NULL;
  }

  _doc->SetModified(true);  

  //The macro adds all necessary checks for standardly behaiving attributes
  __FindOrCreateAttributeForBuilder

 
  //Add checks for TreeNode and UserID attributes  
  if (strncmp(aTypeOfAttribute.c_str(), "AttributeTreeNode",17) == 0 ) {
    
    std::string aTreeNodeGUID;
    if (strcmp(aTypeOfAttribute.c_str(), "AttributeTreeNode") == 0) {
      aTreeNodeGUID = SALOMEDSImpl_AttributeTreeNode::GetDefaultTreeID();
    } else {
      aTreeNodeGUID = aTypeOfAttribute.substr(21, aTypeOfAttribute.size()); // create tree node GUID by name
    }
    SALOMEDSImpl_AttributeTreeNode* anAttr = NULL;
    if (!(anAttr=(SALOMEDSImpl_AttributeTreeNode*)Lab.FindAttribute(aTreeNodeGUID))) {
      CheckLocked();
      anAttr = SALOMEDSImpl_AttributeTreeNode::Set(Lab, aTreeNodeGUID);
    }
    return anAttr;
  }

  if (strncmp(aTypeOfAttribute.c_str(), "AttributeUserID",15) == 0 ) {
    std::string aUserGUID;
    if (strcmp(aTypeOfAttribute.c_str(), "AttributeUserID") == 0) {
      aUserGUID = SALOMEDSImpl_AttributeUserID::DefaultID();
    } else {
      aUserGUID = aTypeOfAttribute.substr(15, aTypeOfAttribute.size()); // create tree node GUID by name
    }
    SALOMEDSImpl_AttributeUserID* anAttr = NULL;
    if (!(anAttr=(SALOMEDSImpl_AttributeUserID*)Lab.FindAttribute(aUserGUID))) {
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

bool SALOMEDSImpl_StudyBuilder::FindAttribute(const SALOMEDSImpl_SObject& anObject, 
                                              DF_Attribute*& anAttribute, 
                                              const std::string& aTypeOfAttribute)
{
  _errorCode = "";
  if(!anObject) {
    _errorCode = "Invalid arguments";
    return false;
  }
  DF_Label Lab = anObject.GetLabel();
  if ((anAttribute=Lab.FindAttribute(SALOMEDSImpl_SObject::GetGUID(aTypeOfAttribute)))) {
    // commented out because NO MODIFICATION is done to attributes when calling FindAttribute()
    // _doc->Modify();  
    return true;
  }
  return false;
}

//============================================================================
/*! Function : RemoveAttribute
 *  Purpose  : Remove attribute of given type assigned SObject
 */
//============================================================================

bool SALOMEDSImpl_StudyBuilder::RemoveAttribute(const SALOMEDSImpl_SObject& anObject, 
                                                const std::string& aTypeOfAttribute)
{
  _errorCode = "";
  CheckLocked();
  if(!anObject) {
    _errorCode = "Invalid arguments";
    return false;
  }
  DF_Label Lab = anObject.GetLabel();
  
  if (aTypeOfAttribute == std::string("AttributeIOR")) { // Remove from IORLabel map
    SALOMEDSImpl_AttributeIOR* anAttr = NULL;
    if ((anAttr=(SALOMEDSImpl_AttributeIOR*)Lab.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID()))) {
      _study->DeleteIORLabelMapItem(anAttr->Value());
    }
  }

  Lab.ForgetAttribute (SALOMEDSImpl_SObject::GetGUID(aTypeOfAttribute));
    
  _doc->SetModified(true);  
  _study->modifySO_Notification(anObject,0);
    
  return true;
}

//============================================================================
/*! Function : Addreference
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::Addreference(const SALOMEDSImpl_SObject& me, 
                                             const SALOMEDSImpl_SObject& theReferencedObject)
{
  _errorCode = "";
  if(!me || !theReferencedObject) {
   _errorCode = "Invalid arguments";
   return false;
  }
  CheckLocked();
  DF_Label Lab = me.GetLabel();
  DF_Label RefLab = theReferencedObject.GetLabel();
  SALOMEDSImpl_AttributeReference::Set(Lab,RefLab);

  SALOMEDSImpl_AttributeTarget::Set(RefLab)->Add(SALOMEDSImpl_Study::SObject(Lab));

  if(_callbackOnRemove && Lab.IsDescendant(_doc->Main())) _callbackOnRemove->OnRemoveSObject(me);
  
  return true;
}

//============================================================================
/*! Function : RemoveReference
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::RemoveReference(const SALOMEDSImpl_SObject& me)
{
  _errorCode = "";
  SALOMEDSImpl_SObject theReferencedObject;
  
  if(!me.ReferencedObject(theReferencedObject)) return false;  //No reference is found
  
  CheckLocked();
  DF_Label Lab = me.GetLabel();

  //SRN: 30 Aug, 2004 : fix from Ecole l'ete version 

  DF_Label RefLab = theReferencedObject.GetLabel();
       
  SALOMEDSImpl_AttributeTarget* aTarget = NULL;
  if((aTarget=(SALOMEDSImpl_AttributeTarget*)RefLab.FindAttribute(SALOMEDSImpl_AttributeTarget::GetID()))) {
    aTarget->Remove(SALOMEDSImpl_Study::SObject(Lab));
  }
  
  Lab.ForgetAttribute(SALOMEDSImpl_AttributeReference::GetID());  
  
  _doc->SetModified(true);  
  
  return true;
}



//============================================================================
/*! Function : AddDirectory
 *  Purpose  : adds a new directory with a path = thePath
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::AddDirectory(const std::string& thePath) 
{
  _errorCode = "";
  CheckLocked();
  if(thePath.empty()) {
    _errorCode = "Invalid path";
    return false;
  }

  std::string aPath(thePath), aContext(""), aFatherPath;
  DF_Label aLabel;
  SALOMEDSImpl_SObject anObject;

  try { 
    anObject = _study->FindObjectByPath(thePath); //Check if the directory already exists
  }
  catch(...) { }

  if(anObject) {
    _errorCode = "StudyNameAlreadyUsed";
    return false; 
  }

  if(aPath[0] != '/') { //Relative path 
    aPath.insert(aPath.begin(), '/');
    aPath = _study->GetContext() + aPath;
  }

  std::vector<std::string> vs = SALOMEDSImpl_Tool::splitString(aPath, '/');
  if(vs.size() == 1) 
    aFatherPath = "/";
  else {
    for(int i = 0, len = vs.size()-1; i<len; i++) { 
      aFatherPath += "/";
      aFatherPath += vs[i];
    }
  }

  try { 
    anObject = _study->FindObjectByPath(aFatherPath); //Check if the father directory exists
  }
  catch(...) { ; }
  if(!anObject) {
    _errorCode = "StudyInvalidDirectory";
    return false; 
  }

  SALOMEDSImpl_SObject aNewObject = NewObject(anObject);
  aLabel = aNewObject.GetLabel();
  if(aLabel.IsNull()) {
    _errorCode = "StudyInvalidComponent";
    return false;
  }

  SALOMEDSImpl_AttributeName::Set(aLabel, vs.back());

  //Set LocalID attribute to identify the directory object
  SALOMEDSImpl_AttributeLocalID::Set(aLabel, DIRECTORYID);
  
  _doc->SetModified(true); 
  
  return true;
}


//============================================================================
/*! Function : SetGUID
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::SetGUID(const SALOMEDSImpl_SObject& anObject, 
                                        const std::string& theGUID)
{
  _errorCode = "";
  CheckLocked();
  if(!anObject) {
    _errorCode = "Invalid arguments";
    return false;
  }

  DF_Label aLabel = anObject.GetLabel();
  SALOMEDSImpl_AttributeUserID::Set(aLabel, theGUID);

  _doc->SetModified(true);  

  return true;
}

//============================================================================
/*! Function : IsGUID
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::IsGUID(const SALOMEDSImpl_SObject& anObject, 
                                       const std::string& theGUID)
{
  _errorCode = "";
  if(!anObject) {
    _errorCode = "Invalid arguments";
    return false;
  }
  DF_Label aLabel = anObject.GetLabel();
  return aLabel.IsAttribute(theGUID);
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
  _study->GetProperties()->IsLockChanged(true);

  //Not implemented
}

//============================================================================
/*! Function : CommitCommand
 *  Purpose  : 
 */
//============================================================================
void SALOMEDSImpl_StudyBuilder::CommitCommand()
{
  _errorCode = "";
  SALOMEDSImpl_AttributeStudyProperties* anAttr = _study->GetProperties();
  if (anAttr->IsLocked() && !anAttr->IsLockChanged(true)) {
    _errorCode = "LockProtection";
    throw LockProtection("LockProtection");
  } else {
    int aModif = anAttr->GetModified();
    if (aModif < 0) aModif = 1000; // if user make undo and then - new transaction "modify" will never be zero
    anAttr->SetModified(aModif+1);
  }
  

  //Not implemented
  _doc->SetModified(true);  
}

//============================================================================
/*! Function : HasOpenCommand
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::HasOpenCommand()
{
  _errorCode = "";

  //Not implememnted
  return false;
}

//============================================================================
/*! Function : AbortCommand
 *  Purpose  : 
 */
//============================================================================
void SALOMEDSImpl_StudyBuilder::AbortCommand()
{
  _errorCode = "";
  //Not implemented    
}

//============================================================================
/*! Function : Undo
 *  Purpose  : 
 */
//============================================================================
void SALOMEDSImpl_StudyBuilder::Undo()
{
  //Not implemented
  _errorCode = "";
  SALOMEDSImpl_AttributeStudyProperties* anAttr = _study->GetProperties();
  if (anAttr->IsLocked()) {
    _errorCode = "LockProtection";
    throw LockProtection("LockProtection");
  } else {
    anAttr->SetModified(anAttr->GetModified()-1);
  }

  _doc->SetModified(true);  
}

//============================================================================
/*! Function : Redo
 *  Purpose  : 
 */
//============================================================================
void SALOMEDSImpl_StudyBuilder::Redo() 
{
  _errorCode = "";
  SALOMEDSImpl_AttributeStudyProperties* anAttr = _study->GetProperties();
  if (anAttr->IsLocked()) {
    _errorCode = "LockProtection";
    throw LockProtection("LockProtection");
  } else {
    anAttr->SetModified(anAttr->GetModified()+1);
  }

  //Not implemented

  _doc->SetModified(true);  
}

//============================================================================
/*! Function : GetAvailableUndos
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::GetAvailableUndos()
{
  _errorCode = "";
  return false;
}

//============================================================================
/*! Function : GetAvailableRedos
 *  Purpose  : 
 */
//============================================================================
bool  SALOMEDSImpl_StudyBuilder::GetAvailableRedos()
{
  _errorCode = "";
  return false;
}

//============================================================================
/*! Function : UndoLimit
 *  Purpose  : 
 */
//============================================================================
int  SALOMEDSImpl_StudyBuilder::UndoLimit()
{
  _errorCode = "";
  return 1;
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
  //Not implemented
}

//============================================================================
/*! Function : SetOnAddSObject
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_Callback*
SALOMEDSImpl_StudyBuilder::SetOnAddSObject(const SALOMEDSImpl_Callback* theCallback)
{
  _errorCode = "";
  SALOMEDSImpl_Callback* aRet = _callbackOnAdd;
  _callbackOnAdd = (SALOMEDSImpl_Callback*)theCallback;
  return aRet;
}

//============================================================================
/*! Function : SetOnNewSObject
 *  Purpose  : 
 */
//============================================================================
SALOMEDSImpl_Callback* 
SALOMEDSImpl_StudyBuilder::SetOnRemoveSObject(const SALOMEDSImpl_Callback* theCallback)
{
  _errorCode = "";
  SALOMEDSImpl_Callback* aRet = _callbackOnRemove;
  _callbackOnRemove = (SALOMEDSImpl_Callback*)theCallback;
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
  if (HasOpenCommand()) return;
  SALOMEDSImpl_AttributeStudyProperties* anAttr = _study->GetProperties();
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
bool SALOMEDSImpl_StudyBuilder::SetName(const SALOMEDSImpl_SObject& theSO, 
                                        const std::string& theValue)
{
  _errorCode = "";
  CheckLocked();
  if(!theSO) {
    _errorCode = "Invalid arguments";
    return false;
  }
  SALOMEDSImpl_AttributeName::Set(theSO.GetLabel(), theValue);

  _doc->SetModified(true);  

  return true;
}

//============================================================================
/*! Function : SetComment
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::SetComment(const SALOMEDSImpl_SObject& theSO, 
                                           const std::string& theValue)
{
  _errorCode = "";
  CheckLocked();
  if(!theSO) {
    _errorCode = "Invalid arguments";
    return false;
  }
  SALOMEDSImpl_AttributeComment::Set(theSO.GetLabel(), theValue);

  _doc->SetModified(true);  

  return true;
}

//============================================================================
/*! Function : SetIOR
 *  Purpose  : 
 */
//============================================================================
bool SALOMEDSImpl_StudyBuilder::SetIOR(const SALOMEDSImpl_SObject& theSO, 
                                       const std::string& theValue)
{
  _errorCode = "";
  CheckLocked();
  if(!theSO) {
    _errorCode = "Invalid arguments";
    return false;
  }
  SALOMEDSImpl_AttributeIOR::Set(theSO.GetLabel(), theValue);

  _doc->SetModified(true);  

  return true;
}


//============================================================================
/*! Function : Translate_persistentID_to_IOR
 *  Purpose  :
 */
//============================================================================
static void Translate_persistentID_to_IOR(DF_Label& Lab, SALOMEDSImpl_Driver* driver, bool isMultiFile, bool isASCII)
{
  if(driver == NULL) return;
  DF_ChildIterator  itchild (Lab);
  
  for (; itchild.More(); itchild.Next()) {
    DF_Label current = itchild.Value();
    SALOMEDSImpl_AttributePersistentRef* Att = NULL;
    if ((Att=(SALOMEDSImpl_AttributePersistentRef*)current.FindAttribute(SALOMEDSImpl_AttributePersistentRef::GetID()))) {  

      SALOMEDSImpl_AttributeLocalID* anID = NULL;
      if ((anID=(SALOMEDSImpl_AttributeLocalID*)current.FindAttribute(SALOMEDSImpl_AttributeLocalID::GetID()))) 
        if (anID->Value() == FILELOCALID) continue;   //SRN: This attribute store a file name, skip it 

      std::string persist_ref = Att->Value();
      SALOMEDSImpl_SObject so = SALOMEDSImpl_Study::SObject(current);
      std::string ior_string = driver->LocalPersistentIDToIOR(so, 
                                                              persist_ref, 
                                                              isMultiFile, 
                                                              isASCII);
      SALOMEDSImpl_AttributeIOR* iorAttr = SALOMEDSImpl_AttributeIOR::Set (current, ior_string);

      // make myRefCounter of a loaded GenericObj == 1
      SALOMEDSImpl_Study::UnRegisterGenObj( ior_string, iorAttr->Label());
    }
    Translate_persistentID_to_IOR (current, driver, isMultiFile, isASCII);
  }
}
