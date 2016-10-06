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

//  File   : SALOMEDSImpl_Study.cxx
//  Author : Sergey RUIN
//  Module : SALOME
//
#include "SALOMEDSImpl_Study.hxx"
#include <string.h>

#include <KERNEL_version.h>
#include <Basics_Utils.hxx>

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

//============================================================================
/*! Function : SALOMEDSImpl_Study
 *  Purpose  : SALOMEDSImpl_Study constructor
 */
//============================================================================
SALOMEDSImpl_Study::SALOMEDSImpl_Study(const DF_Document* doc,
                                       const std::string& study_name)
{
  _name = study_name;
  _doc = (DF_Document*)doc;
  _Saved = false ;
  _URL = "";
  _StudyId = -1;
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
}


//============================================================================
/*! Function : ~SALOMEDSImpl_Study
 *  Purpose  : SALOMEDSImpl_Study destructor
 */
//============================================================================
SALOMEDSImpl_Study::~SALOMEDSImpl_Study()
{
  delete _builder;
  delete _cb;
  delete _useCaseBuilder;
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
/*! Function : GetTransientReference
 *  Purpose  : Get IOR of the Study (registred in Document in doc->Root)
 */
//============================================================================
std::string SALOMEDSImpl_Study::GetTransientReference()
{
  _errorCode = "";
  std::string IOR = "";

  SALOMEDSImpl_AttributeIOR* Att;
  DF_Label _lab = _doc->Root();
  if ((Att=(SALOMEDSImpl_AttributeIOR*)_lab.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID()))) {
    IOR = Att->Value();
  }
  else {
    _errorCode = "IOR is empty";
  }

  return IOR;
}

void SALOMEDSImpl_Study::SetTransientReference(const std::string& theIOR)
{
  _errorCode = "";

  SALOMEDSImpl_AttributeStudyProperties* aProp = GetProperties();
  int aLocked = aProp->IsLocked();
  if (aLocked) aProp->SetLocked(false);

  // Assign the value of the IOR in the study->root
  SALOMEDSImpl_AttributeIOR::Set(_doc->Main().Root(), theIOR);

  if (aLocked) aProp->SetLocked(true);
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
    return GetSObject(_current);
  }

  if(aPath[0] != '/')  //Relative path
    isRelative = true;

  DF_ChildIterator anIterator;
  DF_Label aLabel;
  SALOMEDSImpl_AttributeName* anAttr;

  if(isRelative) {
    if(_current.IsNull()) return aSO;
    anIterator.Init(_current, false);
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
/*! Function : SetContext
 *  Purpose  : Sets the current context
 */
//============================================================================
bool SALOMEDSImpl_Study::SetContext(const std::string& thePath)
{
  _errorCode = "";
  if(thePath.empty()) {
    _errorCode = "InvalidPath";
    return false;
  }

  std::string aPath(thePath), aContext("");
  bool isInvalid = false;
  SALOMEDSImpl_SObject aSO;

  if(aPath[0] != '/') { //Relative path
    aContext = GetContext();
    aContext += '/';
    aContext += aPath;
  }
  else
    aContext = aPath;

  try {
    aSO = FindObjectByPath(aContext);
  }
  catch( ... ) {
    isInvalid = true;
  }

  if(isInvalid || !aSO) {
    _errorCode = "InvalidContext";
    return false;
  }

  DF_Label aLabel = aSO.GetLabel();
  if(aLabel.IsNull()) {
    _errorCode = "InvalidContext";
    return false;
  }
  else
    _current = aLabel;  //Set the current context

  return true;
}

//============================================================================
/*! Function : GetContext
 *  Purpose  : Gets the current context
 */
//============================================================================
std::string SALOMEDSImpl_Study::GetContext()
{
  _errorCode = "";

  if(_current.IsNull()) {
    _errorCode = "InvaidContext";
    return "";
  }
  SALOMEDSImpl_SObject so = GetSObject(_current);
  return GetObjectPath(so);
}

//============================================================================
/*! Function : GetObjectNames
 *  Purpose  : method to get all object names in the given context (or in the current context, if 'theContext' is empty)
 */
//============================================================================
std::vector<std::string> SALOMEDSImpl_Study::GetObjectNames(const std::string& theContext)
{
  _errorCode = "";

  std::vector<std::string> aResultSeq;
  DF_Label aLabel;
  if (theContext.empty()) {
    aLabel = _current;
  } else {
    DF_Label aTmp = _current;
    SetContext(theContext);
    aLabel = _current;
    _current = aTmp;
  }
  if (aLabel.IsNull()) {
    _errorCode = "InvalidContext";
    return aResultSeq;
  }

  DF_ChildIterator anIter (aLabel, true); // iterate all subchildren at all sublevels
  for (; anIter.More(); anIter.Next()) {
    DF_Label aLabel = anIter.Value();
    SALOMEDSImpl_AttributeName* aName;
    if ((aName=(SALOMEDSImpl_AttributeName*)aLabel.FindAttribute(SALOMEDSImpl_AttributeName::GetID())))
      aResultSeq.push_back(aName->Value());
  }

  return aResultSeq;
}

//============================================================================
/*! Function : GetDirectoryNames
 *  Purpose  : method to get all directory names in the given context (or in the current context, if 'theContext' is empty)
 */
//============================================================================
std::vector<std::string> SALOMEDSImpl_Study::GetDirectoryNames(const std::string& theContext)
{
  _errorCode = "";

  std::vector<std::string> aResultSeq;
  DF_Label aLabel;
  if (theContext.empty()) {
    aLabel = _current;
  } else {
    DF_Label aTmp = _current;
    SetContext(theContext);
    aLabel = _current;
    _current = aTmp;
  }
  if (aLabel.IsNull()) {
    _errorCode = "InvalidContext";
    return aResultSeq;
  }

  DF_ChildIterator anIter (aLabel, true); // iterate first-level children at all sublevels
  for (; anIter.More(); anIter.Next()) {
    DF_Label aLabel = anIter.Value();
    SALOMEDSImpl_AttributeLocalID* anID;
    if ((anID=(SALOMEDSImpl_AttributeLocalID*)aLabel.FindAttribute(SALOMEDSImpl_AttributeLocalID::GetID()))) {
      if (anID->Value() == DIRECTORYID) {
        SALOMEDSImpl_AttributeName* aName;
        if ((aName=(SALOMEDSImpl_AttributeName*)aLabel.FindAttribute(SALOMEDSImpl_AttributeName::GetID()))) {
          aResultSeq.push_back(aName->Value());
        }
      }
    }
  }

  return aResultSeq;
}

//============================================================================
/*! Function : GetFileNames
 *  Purpose  : method to get all file names in the given context (or in the current context, if 'theContext' is empty)
 */
//============================================================================
std::vector<std::string> SALOMEDSImpl_Study::GetFileNames(const std::string& theContext)
{
  _errorCode = "";

  std::vector<std::string> aResultSeq;
  DF_Label aLabel;
  if (theContext.empty()) {
    aLabel = _current;
  } else {
    DF_Label aTmp = _current;
    SetContext(theContext);
    aLabel = _current;
    _current = aTmp;
  }
  if (aLabel.IsNull()) {
    _errorCode = "InvalidContext";
    return aResultSeq;
  }

  DF_ChildIterator anIter (aLabel, true); // iterate all subchildren at all sublevels
  for (; anIter.More(); anIter.Next()) {
    DF_Label aLabel = anIter.Value();
    SALOMEDSImpl_AttributeLocalID* anID;
    if ((anID=(SALOMEDSImpl_AttributeLocalID*)aLabel.FindAttribute(SALOMEDSImpl_AttributeLocalID::GetID()))) {
      if (anID->Value() == FILELOCALID) {
        SALOMEDSImpl_AttributePersistentRef* aName;
        if ((aName=(SALOMEDSImpl_AttributePersistentRef*)aLabel.FindAttribute(SALOMEDSImpl_AttributePersistentRef::GetID()))) {
          std::string aFileName = aName->Value();
          if (aFileName.size() > 0)
            aResultSeq.push_back(aFileName.substr(strlen(FILEID), aFileName.size()));
        }
      }
    }
  }

  return aResultSeq;
}

//============================================================================
/*! Function : GetComponentNames
 *  Purpose  : method to get all components names
 */
//============================================================================
std::vector<std::string> SALOMEDSImpl_Study::GetComponentNames(const std::string& theContext)
{
  _errorCode = "";

  std::vector<std::string> aResultSeq;
  DF_ChildIterator anIter(_doc->Main(), false); // iterate all subchildren at first level
  for(; anIter.More(); anIter.Next()) {
    DF_Label aLabel = anIter.Value();
    SALOMEDSImpl_AttributeName* aName;
    if ((aName=(SALOMEDSImpl_AttributeName*)aLabel.FindAttribute(SALOMEDSImpl_AttributeName::GetID())))
      aResultSeq.push_back(aName->Value());
  }

  return aResultSeq;
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
  return _name;
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
  Name(url);
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
std::string SALOMEDSImpl_Study::_GetNoteBookAccess(const std::string& theStudyVar)
{
  std::string notebook = "import salome_notebook\n";
  notebook += _GetNoteBookAccessor() + " = salome_notebook.NoteBook(" + theStudyVar + ")" ;
  return notebook;
}

bool SALOMEDSImpl_Study::IsLocked()
{
  _errorCode = "";
  return GetProperties()->IsLocked();
}

int SALOMEDSImpl_Study::StudyId()
{
  _errorCode = "";
  return _StudyId;
}

void SALOMEDSImpl_Study::StudyId(int id)
{
  _errorCode = "";
  _StudyId = id;
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

SALOMEDSImpl_Study* SALOMEDSImpl_Study::GetStudy(const DF_Label& theLabel)
{
  SALOMEDSImpl_StudyHandle* Att;
  if ((Att=(SALOMEDSImpl_StudyHandle*)theLabel.Root().FindAttribute(SALOMEDSImpl_StudyHandle::GetID()))) {
    return Att->Get();
  }
  return NULL;
}

SALOMEDSImpl_SObject SALOMEDSImpl_Study::SObject(const DF_Label& theLabel)
{
  return GetStudy(theLabel)->GetSObject(theLabel);
}

SALOMEDSImpl_SComponent SALOMEDSImpl_Study::SComponent(const DF_Label& theLabel)
{
  return GetStudy(theLabel)->GetSComponent(theLabel);
}


void SALOMEDSImpl_Study::IORUpdated(const SALOMEDSImpl_AttributeIOR* theAttribute)
{
  std::string aString = theAttribute->Label().Entry();
  GetStudy(theAttribute->Label())->UpdateIORLabelMap(theAttribute->Value(), aString);
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
/*! Function : Close
 *  Purpose  :
 */
//============================================================================
void SALOMEDSImpl_Study::Close()
{
  _errorCode = "";
  _notifier = 0;
  _doc->GetApplication()->Close(_doc);
  _doc = NULL;
  _mapOfSO.clear();
  _mapOfSCO.clear();
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
      size_t idx = order.find( "," );
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
  fp.open(aFileName.c_str(), std::ios::out);

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

  std::string aStudyVar = "salome.myStudy";
  // initialization function
  sfp << aBatchModeScript << ".salome_init()" << std::endl;
  if ( !isMultiFile ) {
    sfp << "theStudy = " << aStudyVar << std::endl << std::endl;
    aStudyVar = "theStudy";
  }
  // notebook initialization
  sfp << _GetNoteBookAccess(aStudyVar) << std::endl;

  // extend sys.path with the directory where the script is being dumped to
  sfp << "sys.path.insert( 0, r\'" << thePath << "\')" << std::endl << std::endl;

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
    SALOMEDSImpl_TMPFile* aStream = aDriver->DumpPython(this, isPublished, isMultiFile, isValidScript, aStreamLength);
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
      
      fp2.open(aFileName.c_str(), std::ios::out);
      
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
      sfp << aScriptName << ".RebuildData(" << aBatchModeScript << ".myStudy)" << std::endl;
    }
    else
      sfp << sfp2.str();

    if(aStream) delete aStream;
  }

  sfp << std::endl;
  sfp << "if salome.sg.hasDesktop():" << std::endl;
  sfp << "\tsalome.sg.updateObjBrowser(True)" << std::endl;

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
  txt << "# -*- coding: utf-8 -*-" << std::endl << std::endl;
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
