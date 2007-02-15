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
//  File   : SALOMEDSImpl_Study.cxx
//  Author : Sergey RUIN
//  Module : SALOME


#include "SALOMEDSImpl_Study.hxx"

using namespace std;

#include <TColStd_SequenceOfExtendedString.hxx>
#include <TCollection_ExtendedString.hxx>

#include <TColStd_HSequenceOfAsciiString.hxx>
#include <TDocStd_Application.hxx>
#include <TDocStd_Owner.hxx>
#include <TDF_LabelList.hxx>
#include <TDF_ListIteratorOfLabelList.hxx>
#include <CDM_Document.hxx>
#include <CDM_Application.hxx>
#include <TDF_ChildIDIterator.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_AttributeIterator.hxx>

#include "SALOMEDSImpl_ChildNodeIterator.hxx"
#include "SALOMEDSImpl_Attributes.hxx"
#include "SALOMEDSImpl_UseCaseIterator.hxx"
#include "SALOMEDSImpl_AttributeReference.hxx"
#include "SALOMEDSImpl_StudyHandle.hxx"
#include "SALOMEDSImpl_Tool.hxx"
#include "SALOMEDSImpl_IParameters.hxx"

IMPLEMENT_STANDARD_HANDLE( SALOMEDSImpl_Study, MMgt_TShared )
IMPLEMENT_STANDARD_RTTIEXT( SALOMEDSImpl_Study, MMgt_TShared )

#define DIRECTORYID       16661
#define FILELOCALID       26662
#define FILEID            "FILE: "

//============================================================================
/*! Function : SALOMEDSImpl_Study
 *  Purpose  : SALOMEDSImpl_Study constructor
 */
//============================================================================
SALOMEDSImpl_Study::SALOMEDSImpl_Study(const Handle(TDocStd_Document)& doc,
				       const TCollection_AsciiString& study_name)
{
  doc->SetUndoLimit(1); // mpv (IPAL9237): if there is no undo limit, operations mechanism couldn't work
  _name = study_name;
  _doc = doc;
  _Saved = false ;
  _URL = "";
  _StudyId = -1;
  _autoFill = true;
  myNbPostponed.Append(0);
  myNbUndos = 0;
  _errorCode = "";
  _useCaseBuilder = new SALOMEDSImpl_UseCaseBuilder(_doc);
  _builder = new SALOMEDSImpl_StudyBuilder(this);
  _cb = new SALOMEDSImpl_Callback(_useCaseBuilder);
  //Put on the root label a StudyHandle attribute to store the address of this object
  //It will be used to retrieve the study object by TDF_Label that belongs to the study
  SALOMEDSImpl_StudyHandle::Set(_doc->Main().Root(), this);
  _lockers = new TColStd_HSequenceOfAsciiString();
}


//============================================================================
/*! Function : ~SALOMEDSImpl_Study
 *  Purpose  : SALOMEDSImpl_Study destructor
 */
//============================================================================
SALOMEDSImpl_Study::~SALOMEDSImpl_Study()
{}

//============================================================================
/*! Function : GetPersistentReference
 *  Purpose  : Get persistent reference of study (idem URL())
 */
//============================================================================
TCollection_AsciiString SALOMEDSImpl_Study::GetPersistentReference()
{
  _errorCode = "";
  return URL();
}
//============================================================================
/*! Function : GetTransientReference
 *  Purpose  : Get IOR of the Study (registred in OCAF document in doc->Root)
 */
//============================================================================
TCollection_AsciiString SALOMEDSImpl_Study::GetTransientReference()
{
  _errorCode = "";
  TCollection_AsciiString IOR = "";

  Handle(SALOMEDSImpl_AttributeIOR) Att;
  TDF_Label _lab = _doc->GetData()->Root();
  if (_lab.FindAttribute(SALOMEDSImpl_AttributeIOR::GetID(),Att)) {
    IOR = Att->Value();
  }
  else {
    _errorCode = "IOR is empty";
  }

  return IOR;
}

void SALOMEDSImpl_Study::SetTransientReference(const TCollection_AsciiString& theIOR)
{
  _errorCode = "";

  Handle(SALOMEDSImpl_AttributeStudyProperties) aProp = GetProperties();
  int aLocked = aProp->IsLocked();
  if (aLocked) aProp->SetLocked(Standard_False);

  // Assign the value of the IOR in the study->root
  SALOMEDSImpl_AttributeIOR::Set(_doc->Main().Root(), theIOR);

  if (aLocked) aProp->SetLocked(Standard_True);
}

//============================================================================
/*! Function : IsEmpty
 *  Purpose  : Detect if study is empty
 */
//============================================================================
bool SALOMEDSImpl_Study::IsEmpty()
{
  _errorCode = "";
  if (_doc.IsNull()) return true;
  return _doc->IsEmpty();
}

//============================================================================
/*! Function : FindComponent
 *  Purpose  : Find a Component with ComponentDataType = aComponentName
 */
//============================================================================
Handle(SALOMEDSImpl_SComponent) SALOMEDSImpl_Study::FindComponent (const TCollection_AsciiString& aComponentName)
{
  _errorCode = "";
  bool _find = false;
  TCollection_AsciiString name;
  SALOMEDSImpl_SComponentIterator itcomp = NewComponentIterator();
  Handle(SALOMEDSImpl_SComponent) compo;

  for (; itcomp.More(); itcomp.Next()) {
    Handle(SALOMEDSImpl_SComponent) SC = itcomp.Value();
    name = SC->ComponentDataType();
    if(aComponentName == name) {
      _find = true;
      return SC;
    }
  }

  if(!_find)
    {
      _errorCode = "No component was found";
      return NULL;
    }
  return compo;
}

//============================================================================
/*! Function : FindComponentID
 *  Purpose  : Find a Component from it's ID
 */
//============================================================================
Handle(SALOMEDSImpl_SComponent) SALOMEDSImpl_Study::FindComponentID(const TCollection_AsciiString& aComponentID)
{
  _errorCode = "";

  // Iterate on each components defined in the study
  // Get the component ID and compare with aComponentID
  bool _find = false;
  TCollection_AsciiString ID;
  Handle(SALOMEDSImpl_SComponent) compo;

  SALOMEDSImpl_SComponentIterator itcomp = NewComponentIterator();
  for (; itcomp.More(); itcomp.Next()) {
    Handle(SALOMEDSImpl_SComponent) SC = itcomp.Value();
    ID = SC->GetID();
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
      compo = NULL;
    }

  return compo;
}

//============================================================================
/*! Function : FindObject
 *  Purpose  : Find an Object with SALOMEDSImpl_Name = anObjectName
 */
//============================================================================
Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_Study::FindObject(const TCollection_AsciiString& anObjectName)
{
  _errorCode = "";

  // Iterate to all components defined in the study
  // After testing the component name, iterate in all objects defined under
  // components (function _FindObject)
  bool _find = false;
  Handle(SALOMEDSImpl_SObject) RefSO = NULL;

  SALOMEDSImpl_SComponentIterator it = NewComponentIterator();
  for (; it.More();it.Next()){
    if(!_find)
      {
	Handle(SALOMEDSImpl_SComponent) SC = it.Value();
	if (SC->GetName() == anObjectName)
	{
	    _find = true;
	    RefSO = SC;

	}
	if (!_find) RefSO =  _FindObject(SC, anObjectName, _find);
      }
  }
  if(RefSO.IsNull()) _errorCode = "No object was found";
  return RefSO;
}

//============================================================================
/*! Function : FindObjectID
 *  Purpose  : Find an Object with ID = anObjectID
 */
//============================================================================
Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_Study::FindObjectID(const TCollection_AsciiString& anObjectID)
{
  _errorCode = "";

  // Convert aSO->GetID in TDF_Label.
  TDF_Label Lab;
  TDF_Tool::Label(_doc->Main().Data(), anObjectID, Lab);

  if (Lab.IsNull()) {
    _errorCode = "No label was found by ID";
    return NULL;
  }
  return GetSObject(Lab);

}

//============================================================================
/*! Function : CreateObjectID
 *  Purpose  : Creates an Object with ID = anObjectID
 */
//============================================================================
Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_Study::CreateObjectID(const TCollection_AsciiString& anObjectID)
{
  _errorCode = "";

  // Convert aSO->GetID in TDF_Label.
  TDF_Label Lab;
  TDF_Tool::Label(_doc->Main().Data(), anObjectID, Lab, Standard_True);

  if (Lab.IsNull()) {
    _errorCode = "Can not create a label";
    return NULL;
  }
  return GetSObject(Lab);

}

//============================================================================
/*! Function : FindObjectByName
 *  Purpose  : Find Objects with SALOMEDSImpl_Name = anObjectName in a Component
 *           : with ComponentDataType = aComponentName
 */
//============================================================================
Handle(TColStd_HSequenceOfTransient) SALOMEDSImpl_Study::FindObjectByName(const TCollection_AsciiString& anObjectName,
								          const TCollection_AsciiString& aComponentName)
{
  _errorCode = "";

  Handle(TColStd_HSequenceOfTransient) listSO = new TColStd_HSequenceOfTransient();

  Handle(SALOMEDSImpl_SComponent) compo = FindComponent(aComponentName) ;
  if ( compo.IsNull() ) {
    _errorCode = "Can not find the component";
    return listSO;
  }

  // Iterate on each object and subobject of the component
  // If objectName is found add it to the list of SObjects
  TCollection_AsciiString childName ;

  TCollection_AsciiString compoId = compo->GetID();
  Handle(SALOMEDSImpl_ChildIterator) it = NewChildIterator(compo);
  for ( ; it->More(); it->Next() ) {

    Handle(SALOMEDSImpl_SObject) CSO = it->Value();
    if ( CSO->GetName() == anObjectName ) {
	/* add to list */
	listSO->Append(CSO) ;
    }

    /* looks also for eventual children */
    bool found = false ;
    CSO = _FindObject( CSO, anObjectName, found ) ;
    if( found) {
      listSO->Append(CSO) ;
    }
  }

  return listSO;
}



//============================================================================
/*! Function : FindObjectIOR
 *  Purpose  : Find an Object with IOR = anObjectIOR
 */
//============================================================================
Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_Study::FindObjectIOR(const TCollection_AsciiString& anObjectIOR)
{
  _errorCode = "";

  // firstly searching in the datamap for optimization
  if (myIORLabels.IsBound(anObjectIOR)) {
    Handle(SALOMEDSImpl_SObject) aResult = GetSObject(myIORLabels.Find(anObjectIOR));
    // 11 oct 2002: forbidden attributes must be checked here
    if (!aResult->GetLabel().IsAttribute(SALOMEDSImpl_AttributeIOR::GetID())) {
      myIORLabels.UnBind(anObjectIOR);
    } else
      return aResult;
  }
  // Iterate to all components defined in the study
  // After testing the component name, iterate in all objects defined under
  // components (function _FindObject)
  bool _find = false;
  Handle(SALOMEDSImpl_SObject) RefSO = NULL;

  SALOMEDSImpl_SComponentIterator it = NewComponentIterator();
  Handle(SALOMEDSImpl_SComponent) SC;
  for (; it.More();it.Next()){
    if(!_find)
      {
	SC = it.Value();
	TCollection_AsciiString ior = SC->GetIOR();
	if (ior != "")
	{
	  if (ior ==  anObjectIOR)
	    {
	      _find = true;
	      RefSO = SC;
	    }
	}
	if (!_find)
	  RefSO =  _FindObjectIOR(SC, anObjectIOR, _find);
      }
  }

  if(RefSO.IsNull()) _errorCode = "No object was found";
  return RefSO;
}

//============================================================================
/*! Function : FindObjectByPath
 *  Purpose  : Find an Object by its path = thePath
 */
//============================================================================
Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_Study::FindObjectByPath(const TCollection_AsciiString& thePath)
{
  _errorCode = "";

  TCollection_AsciiString aPath(thePath), aToken;
  Handle(SALOMEDSImpl_SObject) aSO = NULL;
  int i = 1, aLength = aPath.Length();
  bool isRelative = false;

  if(aLength == 0) {  //Empty path - return the current context
    return GetSObject(_current);
  }

  if(aPath.Value(1) != '/')  //Relative path
    isRelative = true;

  TDF_ChildIterator anIterator;
  TDF_Label aLabel;
  Handle(SALOMEDSImpl_AttributeName) anAttr;

  if(isRelative) {
    if(_current.IsNull()) return NULL;
    anIterator.Initialize(_current, Standard_False);
  }
  else {
    if(aPath.Length() == 1 && aPath.Value(1) == '/') {    //Root
      return GetSObject(_doc->Main());
    }
    anIterator.Initialize(_doc->Main(), Standard_False);
  }

  while(i <= aLength) {

    aToken = aPath.Token("/", i);
    if(aToken.Length() == 0) break;

    for ( ; anIterator.More(); anIterator.Next() ) {
      aLabel = anIterator.Value();
      if(aLabel.FindAttribute(SALOMEDSImpl_AttributeName::GetID(), anAttr)) {
	if(anAttr->Value() == aToken) {
	  aToken = aPath.Token("/", i+1); //Check if it was the last part of the path
	  if(aToken.Length() == 0) {  //The searched label is found (no part of the path is left)
	      return GetSObject(aLabel);
	  }

	  anIterator.Initialize(aLabel, Standard_False);
	  break;
	}
      }
    }

    i++;
  }

  if(aSO.IsNull()) _errorCode = "No object was found";
  return aSO;
}

//============================================================================
/*! Function : GetObjectPath
 *  Purpose  :
 */
//============================================================================
TCollection_AsciiString SALOMEDSImpl_Study::GetObjectPath(const Handle(SALOMEDSImpl_SObject)& theObject)
{
  _errorCode = "";

  TCollection_AsciiString aPath("");
  if(theObject.IsNull()) {
    _errorCode = "Null object";
    return aPath.ToCString();
  }

  TCollection_AsciiString aName = theObject->GetName();
  if(!aName.IsEmpty() && aName != "" ) {
    TCollection_AsciiString aValue((char*)aName.ToCString());
    aValue.Prepend("/");
    aValue += aPath;
    aPath = aValue;
    Handle(SALOMEDSImpl_SObject) aFather = theObject->GetFather();
    if(!aFather.IsNull()) {
       aName = aFather->GetName();
       if(!aName.IsEmpty() && aName != "") {
 	  aValue = (char*)GetObjectPath(aFather).ToCString();
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
TCollection_AsciiString SALOMEDSImpl_Study::GetObjectPathByIOR(const TCollection_AsciiString& theIOR)
{
  _errorCode = "";

  TCollection_AsciiString aPath;
  Handle(SALOMEDSImpl_SObject) so = FindObjectIOR(theIOR);
  if(so.IsNull()) {
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
bool SALOMEDSImpl_Study::SetContext(const TCollection_AsciiString& thePath)
{
  _errorCode = "";
  if(thePath.IsEmpty()) {
    _errorCode = "InvalidPath";
    return false;
  }

  TCollection_AsciiString aPath(thePath), aContext("");
  bool isInvalid = false;
  Handle(SALOMEDSImpl_SObject) aSO;

  if(aPath.Value(1) != '/') { //Relative path
    aContext = GetContext();
    aContext += '/';
    aContext += aPath;
  }
  else
    aContext = aPath;

  try {
    aSO = FindObjectByPath(aContext.ToCString());
  }
  catch( ... ) {
    isInvalid = true;
  }

  if(isInvalid || aSO.IsNull()) {
    _errorCode = "InvalidContext";
    return false;
  }

  TDF_Label aLabel = aSO->GetLabel();
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
TCollection_AsciiString SALOMEDSImpl_Study::GetContext()
{
  _errorCode = "";

  if(_current.IsNull()) {
    _errorCode = "InvaidContext";
    return "";
  }
  Handle(SALOMEDSImpl_SObject) so = GetSObject(_current);
  return GetObjectPath(so);
}

//============================================================================
/*! Function : GetObjectNames
 *  Purpose  : method to get all object names in the given context (or in the current context, if 'theContext' is empty)
 */
//============================================================================
Handle(TColStd_HSequenceOfAsciiString) SALOMEDSImpl_Study::GetObjectNames(const TCollection_AsciiString& theContext)
{
  _errorCode = "";

  Handle(TColStd_HSequenceOfAsciiString) aResultSeq = new TColStd_HSequenceOfAsciiString;
  TDF_Label aLabel;
  if (theContext.IsEmpty()) {
    aLabel = _current;
  } else {
    TDF_Label aTmp = _current;
    SetContext(theContext);
    aLabel = _current;
    _current = aTmp;
  }
  if (aLabel.IsNull()) {
    _errorCode = "InvalidContext";
    return aResultSeq;
  }

  TDF_ChildIterator anIter (aLabel, Standard_True); // iterate all subchildren at all sublevels
  for (; anIter.More(); anIter.Next()) {
    TDF_Label aLabel = anIter.Value();
    Handle(SALOMEDSImpl_AttributeName) aName;
    if (aLabel.FindAttribute(SALOMEDSImpl_AttributeName::GetID(), aName)) aResultSeq->Append(aName->Value());
  }

  return aResultSeq;
}

//============================================================================
/*! Function : GetDirectoryNames
 *  Purpose  : method to get all directory names in the given context (or in the current context, if 'theContext' is empty)
 */
//============================================================================
Handle(TColStd_HSequenceOfAsciiString) SALOMEDSImpl_Study::GetDirectoryNames(const TCollection_AsciiString& theContext)
{
  _errorCode = "";

  Handle(TColStd_HSequenceOfAsciiString) aResultSeq = new TColStd_HSequenceOfAsciiString;
  TDF_Label aLabel;
  if (theContext.IsEmpty()) {
    aLabel = _current;
  } else {
    TDF_Label aTmp = _current;
    SetContext(theContext);
    aLabel = _current;
    _current = aTmp;
  }
  if (aLabel.IsNull()) {
    _errorCode = "InvalidContext";
    return aResultSeq;
  }

  TDF_ChildIterator anIter (aLabel, Standard_True); // iterate first-level children at all sublevels
  for (; anIter.More(); anIter.Next()) {
    TDF_Label aLabel = anIter.Value();
    Handle(SALOMEDSImpl_AttributeLocalID) anID;
    if (aLabel.FindAttribute(SALOMEDSImpl_AttributeLocalID::GetID(), anID)) {
      if (anID->Value() == DIRECTORYID) {
	Handle(SALOMEDSImpl_AttributeName) aName;
	if (aLabel.FindAttribute(SALOMEDSImpl_AttributeName::GetID(), aName)) {
	  aResultSeq->Append(aName->Value());
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
Handle(TColStd_HSequenceOfAsciiString) SALOMEDSImpl_Study::GetFileNames(const TCollection_AsciiString& theContext)
{
  _errorCode = "";

  Handle(TColStd_HSequenceOfAsciiString) aResultSeq = new TColStd_HSequenceOfAsciiString;
  TDF_Label aLabel;
  if (theContext.IsEmpty()) {
    aLabel = _current;
  } else {
    TDF_Label aTmp = _current;
    SetContext(theContext);
    aLabel = _current;
    _current = aTmp;
  }
  if (aLabel.IsNull()) {
    _errorCode = "InvalidContext";
    return aResultSeq;
  }

  TDF_ChildIterator anIter (aLabel, Standard_True); // iterate all subchildren at all sublevels
  for (; anIter.More(); anIter.Next()) {
    TDF_Label aLabel = anIter.Value();
    Handle(SALOMEDSImpl_AttributeLocalID) anID;
    if (aLabel.FindAttribute(SALOMEDSImpl_AttributeLocalID::GetID(), anID)) {
      if (anID->Value() == FILELOCALID) {
	Handle(SALOMEDSImpl_AttributePersistentRef) aName;
	if (aLabel.FindAttribute(SALOMEDSImpl_AttributePersistentRef::GetID(), aName)) {
	  TCollection_ExtendedString aFileName = aName->Value();
	  if (aFileName.Length() > 0)
	    aResultSeq->Append(aFileName.Split(strlen(FILEID)));
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
Handle(TColStd_HSequenceOfAsciiString) SALOMEDSImpl_Study::GetComponentNames(const TCollection_AsciiString& theContext)
{
  _errorCode = "";

  Handle(TColStd_HSequenceOfAsciiString) aResultSeq = new TColStd_HSequenceOfAsciiString;
  TDF_ChildIterator anIter(_doc->Main(), Standard_False); // iterate all subchildren at first level
  for(; anIter.More(); anIter.Next()) {
    TDF_Label aLabel = anIter.Value();
    Handle(SALOMEDSImpl_AttributeName) aName;
    if (aLabel.FindAttribute(SALOMEDSImpl_AttributeName::GetID(), aName)) aResultSeq->Append(aName->Value());
  }

  return aResultSeq;
}

//============================================================================
/*! Function : NewChildIterator
 *  Purpose  : Create a ChildIterator from an SObject
 */
//============================================================================
Handle(SALOMEDSImpl_ChildIterator) SALOMEDSImpl_Study::NewChildIterator(const Handle(SALOMEDSImpl_SObject)& aSO)
{
  _errorCode = "";
  return new SALOMEDSImpl_ChildIterator(aSO);
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
Handle(SALOMEDSImpl_StudyBuilder) SALOMEDSImpl_Study::NewBuilder()
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
TCollection_AsciiString SALOMEDSImpl_Study::Name()
{
  _errorCode = "";
  return _name;
}

//============================================================================
/*! Function : Name
 *  Purpose  : set study name
 */
//============================================================================
void SALOMEDSImpl_Study::Name(const TCollection_AsciiString& name)
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
  if(save) _doc->UnModify();
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
  if (_doc->IsModified()) return true;

  return false;
}

//============================================================================
/*! Function : URL
 *  Purpose  : get URL of the study (persistent reference of the study)
 */
//============================================================================
TCollection_AsciiString SALOMEDSImpl_Study::URL()
{
  _errorCode = "";
  return _URL;
}

//============================================================================
/*! Function : URL
 *  Purpose  : set URL of the study (persistent reference of the study)
 */
//============================================================================
void SALOMEDSImpl_Study::URL(const TCollection_AsciiString& url)
{
  _errorCode = "";
  _URL = url;

  /*jfa: Now name of SALOMEDS study will correspond to name of SalomeApp study
  TCollection_AsciiString tmp(_URL);

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
Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_Study::_FindObject(const Handle(SALOMEDSImpl_SObject)& SO,
						             const TCollection_AsciiString& theObjectName,
						             bool& _find)
{
  if(SO.IsNull()) return NULL;

  // Iterate on each objects and subobjects of the component
  // If objectName find, stop the loop and get the object reference
  Handle(SALOMEDSImpl_SObject) RefSO;
  Handle(SALOMEDSImpl_AttributeName) anAttr;

  TCollection_AsciiString soid = SO->GetID();
  TDF_ChildIterator it(SO->GetLabel());
  for (; it.More(); it.Next()){
    if(!_find)
      {
	if (it.Value().FindAttribute(SALOMEDSImpl_AttributeName::GetID(), anAttr))
	{
          TCollection_AsciiString Val(anAttr->Value());
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
Handle(SALOMEDSImpl_SObject)
SALOMEDSImpl_Study::_FindObjectIOR(const Handle(SALOMEDSImpl_SObject)& SO,
				   const TCollection_AsciiString& theObjectIOR,
				   bool& _find)
{
  if(SO.IsNull()) return NULL;

  // Iterate on each objects and subobjects of the component
  // If objectName find, stop the loop and get the object reference
  Handle(SALOMEDSImpl_SObject) RefSO, aSO;
  Handle(SALOMEDSImpl_AttributeIOR) anAttr;

  TDF_ChildIterator it(SO->GetLabel());
  for (; it.More();it.Next()){
    if(!_find)
      {
	if (it.Value().FindAttribute(SALOMEDSImpl_AttributeIOR::GetID(), anAttr))
	{
          TCollection_AsciiString Val(anAttr->Value());
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

void SALOMEDSImpl_Study::UpdateIORLabelMap(const TCollection_AsciiString& anIOR,const TCollection_AsciiString& anEntry)
{
  _errorCode = "";
  TDF_Label aLabel;
  char* anEn = (char*)anEntry.ToCString();
  char* IOR = (char*)anIOR.ToCString();
  TDF_Tool::Label(_doc->GetData(),anEn, aLabel, Standard_True);
  if (myIORLabels.IsBound(TCollection_ExtendedString(IOR))) myIORLabels.UnBind(TCollection_ExtendedString(IOR));
  myIORLabels.Bind(TCollection_ExtendedString(IOR), aLabel);
}

Handle(SALOMEDSImpl_Study) SALOMEDSImpl_Study::GetStudy(const TDF_Label& theLabel)
{
  Handle(SALOMEDSImpl_StudyHandle) Att;
  if (theLabel.Root().FindAttribute(SALOMEDSImpl_StudyHandle::GetID(),Att)) {
    return Att->GetHandle();
  }
  return NULL;
}

Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_Study::SObject(const TDF_Label& theLabel)
{
  return GetStudy(theLabel)->GetSObject(theLabel);
}

Handle(SALOMEDSImpl_SComponent) SALOMEDSImpl_Study::SComponent(const TDF_Label& theLabel)
{
  return GetStudy(theLabel)->GetSComponent(theLabel);
}


void SALOMEDSImpl_Study::IORUpdated(const Handle(SALOMEDSImpl_AttributeIOR)& theAttribute)
{
  TCollection_AsciiString aString;
  TDF_Tool::Entry(theAttribute->Label(), aString);
  GetStudy(theAttribute->Label())->UpdateIORLabelMap(theAttribute->Value(), aString);
}

Handle(TColStd_HSequenceOfTransient) SALOMEDSImpl_Study::FindDependances(const Handle(SALOMEDSImpl_SObject)& anObject)
{
  _errorCode = "";
  Handle(TColStd_HSequenceOfTransient) aSeq;

  Handle(SALOMEDSImpl_AttributeTarget) aTarget;
  if (anObject->GetLabel().FindAttribute(SALOMEDSImpl_AttributeTarget::GetID(), aTarget)) {
    return aTarget->Get();
  }

  return aSeq;
}


Handle(SALOMEDSImpl_AttributeStudyProperties) SALOMEDSImpl_Study::GetProperties()
{
  _errorCode = "";
  return SALOMEDSImpl_AttributeStudyProperties::Set(_doc->Main());
}

TCollection_AsciiString SALOMEDSImpl_Study::GetLastModificationDate()
{
  _errorCode = "";
  Handle(SALOMEDSImpl_AttributeStudyProperties) aProp = GetProperties();

  Handle(TColStd_HSequenceOfExtendedString) aNames;
  Handle(TColStd_HSequenceOfInteger) aMinutes, aHours, aDays, aMonths, aYears;
  aProp->GetModifications(aNames, aMinutes, aHours, aDays, aMonths, aYears);

  int aLastIndex = aNames->Length();
  char aResult[20];
  sprintf(aResult, "%2.2d/%2.2d/%4.4d %2.2d:%2.2d",
          (int)(aDays->Value(aLastIndex)),(int)(aMonths->Value(aLastIndex)), (int)(aYears->Value(aLastIndex)),
          (int)(aHours->Value(aLastIndex)), (int)(aMinutes->Value(aLastIndex)));
  TCollection_AsciiString aResStr (aResult);
  return aResStr;
}

Handle(TColStd_HSequenceOfAsciiString) SALOMEDSImpl_Study::GetModificationsDate()
{
  _errorCode = "";
  Handle(SALOMEDSImpl_AttributeStudyProperties) aProp = GetProperties();

  Handle(TColStd_HSequenceOfExtendedString) aNames;
  Handle(TColStd_HSequenceOfInteger) aMinutes, aHours, aDays, aMonths, aYears;
  aProp->GetModifications(aNames, aMinutes, aHours, aDays, aMonths, aYears);

  int anIndex, aLength = aNames->Length();
  Handle(TColStd_HSequenceOfAsciiString) aDates = new TColStd_HSequenceOfAsciiString;

  for (anIndex = 2; anIndex <= aLength; anIndex++) {
    char aDate[20];
    sprintf(aDate, "%2.2d/%2.2d/%4.4d %2.2d:%2.2d",
            (int)(aDays->Value(anIndex)), (int)(aMonths->Value(anIndex)), (int)(aYears->Value(anIndex)),
	    (int)(aHours->Value(anIndex)), (int)(aMinutes->Value(anIndex)));
    aDates->Append(aDate);
  }
  return aDates;
}



//============================================================================
/*! Function : GetUseCaseBuilder
 *  Purpose  : Returns a UseCase builder
 */
//============================================================================
Handle(SALOMEDSImpl_UseCaseBuilder) SALOMEDSImpl_Study::GetUseCaseBuilder()
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
  Handle(TDocStd_Application) anApp = Handle(TDocStd_Application)::DownCast(_doc->Application());
  if(!anApp.IsNull()) anApp->Close(_doc);
  _doc.Nullify();
  _mapOfSO.Clear();
  _mapOfSCO.Clear();
}

//============================================================================
/*! Function : AddPostponed
 *  Purpose  :
 */
 //============================================================================
void SALOMEDSImpl_Study::AddPostponed(const TCollection_AsciiString& theIOR)
{
  _errorCode = "";
  if (!NewBuilder()->HasOpenCommand()) return;
  TCollection_AsciiString anIOR(theIOR);
  anIOR.Prepend("d");
  myPostponedIORs.Append(anIOR); // add prefix: deleted
  myNbPostponed.SetValue(myNbPostponed.Length(), myNbPostponed.Last() + 1);
}

//============================================================================
/*! Function : AddCreatedPostponed
 *  Purpose  :
 */
 //============================================================================
void SALOMEDSImpl_Study::AddCreatedPostponed(const TCollection_AsciiString& theIOR)
{
  _errorCode = "";
  if (!NewBuilder()->HasOpenCommand()) return;
  TCollection_AsciiString anIOR(theIOR);
  anIOR.Prepend("c");
  myPostponedIORs.Append(anIOR); // add prefix: created
  myNbPostponed.SetValue(myNbPostponed.Length(), myNbPostponed.Last() + 1);
}

//============================================================================
/*! Function : RemovePostponed
 *  Purpose  :
 */
//============================================================================
Handle(TColStd_HSequenceOfAsciiString) SALOMEDSImpl_Study::RemovePostponed(const int theUndoLimit)
{
  _errorCode = "";

  int anIndex;
  int anOld;

  int aUndoLimit = theUndoLimit;
  if (theUndoLimit < 0) aUndoLimit = 0;

  Handle(TColStd_HSequenceOfAsciiString) aSeq = new TColStd_HSequenceOfAsciiString;

  if (myNbUndos > 0) { // remove undone
    anOld = 0;
    for(anIndex = 1; anIndex < myNbPostponed.Length() - myNbUndos; anIndex++)
      anOld += myNbPostponed(anIndex);
    int aNew = myPostponedIORs.Length() - myNbPostponed.Last();

    for(anIndex = anOld + 1; anIndex <= aNew; anIndex++) {
      TCollection_AsciiString anIOR = myPostponedIORs(anIndex);
      if (anIOR.Value(1) == 'c') {
	aSeq->Append(anIOR.Split(1).ToCString());
      }
    }
    if (anOld < aNew) myPostponedIORs.Remove(anOld + 1, aNew);
    if (myNbPostponed.Length() > 0) myNbPostponed.Remove(myNbPostponed.Length() - myNbUndos, myNbPostponed.Length() - 1);

    myNbUndos = 0;
  }

  if (myNbPostponed.Length() > aUndoLimit) { // remove objects, that can not be undone
    anOld = 0;
    for(anIndex = myNbPostponed.Length() - aUndoLimit; anIndex >= 1; anIndex--)
      anOld += myNbPostponed(anIndex);
    for(anIndex = 1; anIndex <= anOld; anIndex++) {
      TCollection_AsciiString anIOR = myPostponedIORs(anIndex);
      if (anIOR.Value(1) == 'd') {
	aSeq->Append(anIOR.Split(1).ToCString());
      }
    }
    if (anOld > 0) myPostponedIORs.Remove(1, anOld);
    myNbPostponed.Remove(1, myNbPostponed.Length() - aUndoLimit);
  }

  if (theUndoLimit == -1) { // remove all IORs from the study on the study close
    TDF_ChildIDIterator anIter(_doc->GetData()->Root(), SALOMEDSImpl_AttributeIOR::GetID(), Standard_True);
    for(; anIter.More(); anIter.Next()) {
      Handle(SALOMEDSImpl_AttributeIOR) anAttr = Handle(SALOMEDSImpl_AttributeIOR)::DownCast(anIter.Value());
      aSeq->Append(anAttr->Value());
    }
  } else myNbPostponed.Append(0);

  return aSeq;
}

//============================================================================
/*! Function : UndoPostponed
 *  Purpose  :
 */
//============================================================================
void SALOMEDSImpl_Study::UndoPostponed(const int theWay)
{
  _errorCode = "";

  myNbUndos += theWay;
  // remove current postponed
  if (myNbPostponed.Last() > 0)
    myPostponedIORs.Remove(myPostponedIORs.Length() - myNbPostponed.Last() + 1, myPostponedIORs.Length());
  myNbPostponed(myNbPostponed.Length()) = 0;
}


//============================================================================
/*! Function : GetSComponent
 *  Purpose  :
 */
//============================================================================
Handle(SALOMEDSImpl_SComponent) SALOMEDSImpl_Study::GetSComponent(const TCollection_AsciiString& theEntry)
{
  Handle(SALOMEDSImpl_SComponent) aSCO;
  if(_mapOfSCO.IsBound(theEntry))
    aSCO = Handle(SALOMEDSImpl_SComponent)::DownCast(_mapOfSCO.Find(theEntry));
  else {
    TDF_Label aLabel;
    TDF_Tool::Label(_doc->GetData(), theEntry, aLabel);
    aSCO = new SALOMEDSImpl_SComponent(aLabel);
    _mapOfSCO.Bind(theEntry, aSCO);
  }

  return aSCO;
}

//============================================================================
/*! Function : GetSComponent
 *  Purpose  :
 */
//============================================================================
Handle(SALOMEDSImpl_SComponent) SALOMEDSImpl_Study::GetSComponent(const TDF_Label& theLabel)
{
  TCollection_AsciiString anEntry;
  TDF_Tool::Entry(theLabel, anEntry);
  return GetSComponent(anEntry);
}

//============================================================================
/*! Function : GetSObject
 *  Purpose  :
 */
//============================================================================
Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_Study::GetSObject(const TCollection_AsciiString& theEntry)
{
  Handle(SALOMEDSImpl_SObject) aSO;
  if(_mapOfSO.IsBound(theEntry))
    aSO = Handle(SALOMEDSImpl_SObject)::DownCast(_mapOfSO.Find(theEntry));
  else {
    TDF_Label aLabel;
    TDF_Tool::Label(_doc->GetData(), theEntry, aLabel);
    aSO = new SALOMEDSImpl_SObject(aLabel);
    _mapOfSO.Bind(theEntry, aSO);
  }

  return aSO;
}

//============================================================================
/*! Function : GetSObject
 *  Purpose  :
 */
//============================================================================
Handle(SALOMEDSImpl_SObject) SALOMEDSImpl_Study::GetSObject(const TDF_Label& theLabel)
{
  TCollection_AsciiString anEntry;
  TDF_Tool::Entry(theLabel, anEntry);
  return GetSObject(anEntry);
}

//============================================================================
/*! Function : GetAttribute
 *  Purpose  :
 */
//============================================================================
Handle(TDF_Attribute) SALOMEDSImpl_Study::GetAttribute(const TCollection_AsciiString& theEntry,
						       const TCollection_AsciiString& theType)
{
  Handle(SALOMEDSImpl_SObject) aSO = GetSObject(theEntry);
  Handle(TDF_Attribute) anAttr;
  aSO->FindAttribute(anAttr, theType);
  return anAttr;
}

//============================================================================
/*! Function : DumpStudy
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::DumpStudy(const TCollection_AsciiString& thePath,
				   const TCollection_AsciiString& theBaseName,
				   bool isPublished,
				   SALOMEDSImpl_DriverFactory* theFactory)
{
  _errorCode = "";

  if(theFactory == NULL) {
    _errorCode = "Null factory for creation of Engines";
    return false;
  }

  TColStd_SequenceOfExtendedString aSeq;
  TCollection_AsciiString aCompType, aFactoryType;

  //Build a list of all components in the Study
  SALOMEDSImpl_SComponentIterator itcomponent = NewComponentIterator();

  for (; itcomponent.More(); itcomponent.Next()) {
    Handle(SALOMEDSImpl_SComponent) sco = itcomponent.Value();
    aCompType = sco->ComponentDataType();
    //GEOM and MED are independent components
    if (aCompType == "GEOM" || aCompType == "MED")
      aSeq.Prepend(TCollection_ExtendedString(aCompType));
    else
      aSeq.Append(TCollection_ExtendedString(aCompType));
  }

#ifdef WIN32
  TCollection_AsciiString aFileName =
    thePath + TCollection_AsciiString("\\") + theBaseName + TCollection_AsciiString(".py");
#else
  TCollection_AsciiString aFileName =
    thePath + TCollection_AsciiString("/")  + theBaseName + TCollection_AsciiString(".py");
#endif

  //Create a file that will contain a main Study script
  fstream fp;
  fp.open(aFileName.ToCString(), ios::out);

#ifdef WIN32
  bool isOpened = fp.is_open();
#else
  bool isOpened = fp.rdbuf()->is_open();
#endif

  if(!isOpened) {
    _errorCode = TCollection_AsciiString("Can't create a file ")+aFileName;
    return false;
  }

  TCollection_AsciiString aBatchModeScript = "salome";

  //Output to the main Study script required Python modules import,
  //set sys.path and add a creation of the study.
  fp << GetDumpStudyComment().ToCString() << endl << endl;
  fp << "import sys" << endl;
  fp << "import " << aBatchModeScript << endl << endl;

  fp << aBatchModeScript << ".salome_init()" << endl << endl;

  fp << "sys.path.insert( 0, \'" << thePath << "\')" << endl << endl;


  //Check if it's necessary to dump visual parameters
  bool isDumpVisuals = SALOMEDSImpl_IParameters::isDumpPython(this);
  int lastSavePoint = -1;
  if(isDumpVisuals) {
    lastSavePoint = SALOMEDSImpl_IParameters::getLastSavePoint(this);
    if(lastSavePoint > 0) {
      fp << SALOMEDSImpl_IParameters::getStudyScript(this, lastSavePoint) << endl << endl;
    }
  }
  

  Handle(TColStd_HSequenceOfAsciiString) aSeqOfFileNames = new TColStd_HSequenceOfAsciiString;

  //Iterate all components and create the componponents specific scripts.
  bool isOk = true;
  int aLength = aSeq.Length();
  for(int i = 1; i <= aLength; i++) {

    aCompType = aSeq.Value(i);
    Handle(SALOMEDSImpl_SComponent) sco = FindComponent(aCompType);
    SALOMEDSImpl_Driver* aDriver = NULL;
    // if there is an associated Engine call its method for saving
    TCollection_AsciiString IOREngine;
    try {
      if (!sco->ComponentIOR(IOREngine)) {
	if (!aCompType.IsEmpty()) {

	  aDriver = theFactory->GetDriverByType(aCompType);

	  if (aDriver != NULL) {
	    Handle(SALOMEDSImpl_StudyBuilder) SB = NewBuilder();
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
    Handle(SALOMEDSImpl_TMPFile) aStream = aDriver->DumpPython(this, isPublished, isValidScript, aStreamLength);
    if ( !isValidScript )
      isOk = false;

    //Create a file that will contain the component specific script
    fstream fp2;
#ifdef WIN32
    aFileName=thePath+TCollection_AsciiString("\\");
#else
    aFileName=thePath+TCollection_AsciiString("/");
#endif
    TCollection_AsciiString aScriptName;
    aScriptName += theBaseName;
    aScriptName += "_";
    aScriptName += aCompType;

    aFileName += aScriptName+ TCollection_AsciiString(".py");
    aSeqOfFileNames->Append(aFileName);

    fp2.open(aFileName.ToCString(), ios::out);

#ifdef WIN32
    isOpened = fp2.is_open();
#else
    isOpened = fp2.rdbuf()->is_open();
#endif

    if(!isOpened) {
      _errorCode = TCollection_AsciiString("Can't create a file ")+aFileName;
      SALOMEDSImpl_Tool::RemoveTemporaryFiles(thePath, aSeqOfFileNames, false);
      return false;
    }

    //Output the Python script generated by the component in the newly created file.
    fp2 << aStream->Data();
    fp2.close();

    //Add to the main script a call to RebuildData of the generated by the component the Python script
    fp << "import " << aScriptName << endl;
    fp << aScriptName << ".RebuildData(" << aBatchModeScript << ".myStudy)" << endl;
  }

  fp << endl;
  fp << "if salome.sg.hasDesktop():" << endl;
  fp << "\tsalome.sg.updateObjBrowser(1)" << endl;

  if(isDumpVisuals) { //Output the call to Session's method restoreVisualState
    fp << "\tiparameters.getSession().restoreVisualState(1)" << endl;
  }


  fp.close();
  return isOk;
}

//=======================================================================
//function : GetDumpStudyComment
//purpose  : return a header comment for a DumpStudy script
//=======================================================================

TCollection_AsciiString SALOMEDSImpl_Study::GetDumpStudyComment(const char* theComponentName)
{
  TCollection_AsciiString txt
    ("### This file is generated by SALOME automatically by dump python functionality");
  if ( theComponentName )
    txt += TCollection_AsciiString(" of ") + (char*) theComponentName + " component";
  return txt;
}

void dumpSO(const Handle(SALOMEDSImpl_SObject)& theSO,
	    fstream& fp,
	    const TCollection_AsciiString& Tab,
	    const Handle(SALOMEDSImpl_Study) theStudy);
//============================================================================
/*! Function : dump
 *  Purpose  :
 */
//============================================================================
void SALOMEDSImpl_Study::dump(const TCollection_AsciiString& theFileName)
{
  //Create a file that will contain a main Study script
  fstream fp;
  fp.open(theFileName.ToCString(), ios::out);

#ifdef WIN32
  bool isOpened = fp.is_open();
#else
  bool isOpened = fp.rdbuf()->is_open();
#endif

  if(!isOpened) {
    _errorCode = TCollection_AsciiString("Can't create a file ")+theFileName;
    cout << "### SALOMEDSImpl_Study::dump Error: " << _errorCode << endl;
    return;
  }

  Handle(SALOMEDSImpl_SObject) aSO = FindObjectID("0:1");
  fp << "0:1" << endl;
  Handle(SALOMEDSImpl_ChildIterator) Itr = NewChildIterator(aSO);
  TCollection_AsciiString aTab("   ");
  for(; Itr->More(); Itr->Next()) {
    dumpSO(Itr->Value(), fp, aTab, this);
  }

  fp.close();
}


void dumpSO(const Handle(SALOMEDSImpl_SObject)& theSO,
	    fstream& fp,
	    const TCollection_AsciiString& Tab,
	    const Handle(SALOMEDSImpl_Study) theStudy)
{
  TCollection_AsciiString aTab(Tab), anID(theSO->GetID());
  fp << aTab << anID << endl;
  TDF_AttributeIterator anItr(theSO->GetLabel());
  for(; anItr.More(); anItr.Next()) {
    Handle(SALOMEDSImpl_GenericAttribute) anAttr = Handle(SALOMEDSImpl_GenericAttribute)::DownCast(anItr.Value());

    if(anAttr.IsNull()) {
      fp << Tab << "  -- " << anItr.Value()->DynamicType();
      continue;
    }

    TCollection_AsciiString aType = anAttr->GetClassType();
    fp << Tab << "  -- " << aType;

    if(aType == "AttributeReal") {
      fp << " : " << Handle(SALOMEDSImpl_AttributeReal)::DownCast(anAttr)->Value();
    }
    else if(aType == "AttributeInteger") {
      fp << " : " << Handle(SALOMEDSImpl_AttributeInteger)::DownCast(anAttr)->Value();
    }
    else if(aType ==  "AttributeName") {
      fp << " : " << Handle(SALOMEDSImpl_AttributeName)::DownCast(anAttr)->Value();
    }
    else if(aType == "AttributeComment") {
      fp << " : " << Handle(SALOMEDSImpl_AttributeComment)::DownCast(anAttr)->Value();
    }
    else if(aType == "AttributeReference") {
      fp << " : " << Handle(SALOMEDSImpl_AttributeReference)::DownCast(anAttr)->Save();
    }
    fp << endl;
  }

  Handle(SALOMEDSImpl_ChildIterator) Itr = theStudy->NewChildIterator(theSO);
  TCollection_AsciiString aNewTab("   ");
  aNewTab+=aTab;
  for(; Itr->More(); Itr->Next()) {
    dumpSO(Itr->Value(), fp, aNewTab, theStudy);
  }

  return;
}

void SALOMEDSImpl_Study::Modify()
{
  _errorCode = "";
  _doc->Modify();
}

//============================================================================
/*! Function : 
 *  Purpose  :
 */
//============================================================================
Handle(SALOMEDSImpl_AttributeParameter) SALOMEDSImpl_Study::GetCommonParameters(const char* theID, int theSavePoint)
{
  if (theSavePoint < 0) return NULL;
  Handle(SALOMEDSImpl_StudyBuilder) builder = NewBuilder();
  Handle(SALOMEDSImpl_SObject) so = FindComponent((char*)theID);
  if (so.IsNull()) so = builder->NewComponent((char*)theID);
  Handle(SALOMEDSImpl_AttributeParameter) attParam;

  if (theSavePoint > 0) { // Try to find SObject that contains attribute parameter ...
    TDF_Label savePointLabel = so->GetLabel().FindChild( theSavePoint, /*create=*/0 );
    if ( !savePointLabel.IsNull() )
      so = GetSObject( savePointLabel );
    else // ... if it does not exist - create a new one
      so = builder->NewObjectToTag( so, theSavePoint );
  }

  if (!so.IsNull()) {
    builder->FindAttribute(so, attParam, "AttributeParameter");
    if ( attParam.IsNull() ) { // first call of GetCommonParameters on "Interface Applicative" component
      Handle(TDF_Attribute) att = builder->FindOrCreateAttribute(so, "AttributeParameter");
      attParam = Handle(SALOMEDSImpl_AttributeParameter)::DownCast( att );
    }
  }
  return attParam;
}

//============================================================================
/*! Function : 
 *  Purpose  :
 */
//============================================================================
Handle(SALOMEDSImpl_AttributeParameter) SALOMEDSImpl_Study::GetModuleParameters(const char* theID, 
										const char* theModuleName,
										int theSavePoint)
{
  if(theSavePoint <= 0) return NULL;
  Handle(SALOMEDSImpl_AttributeParameter) main_ap = GetCommonParameters(theID, theSavePoint);
  Handle(SALOMEDSImpl_SObject) main_so = main_ap->GetSObject();
  Handle(SALOMEDSImpl_AttributeParameter) par;

  Handle(SALOMEDSImpl_ChildIterator) it = NewChildIterator(main_so);
  string moduleName(theModuleName);
  for(; it->More(); it->Next()) {
    Handle(SALOMEDSImpl_SObject) so(it->Value());
    Handle(SALOMEDSImpl_GenericAttribute) ga;
    if(so->FindAttribute(ga, "AttributeParameter")) {
      par = Handle(SALOMEDSImpl_AttributeParameter)::DownCast(ga);
      if(!par->IsSet("AP_MODULE_NAME", (Parameter_Types)3)) continue; //3 -> PT_STRING
      if(par->GetString("AP_MODULE_NAME") == moduleName) return par;
    }
  }

  Handle(SALOMEDSImpl_StudyBuilder) builder = NewBuilder();
  Handle(SALOMEDSImpl_SObject) so = builder->NewObject(main_so);
  par  = Handle(SALOMEDSImpl_AttributeParameter)::DownCast(builder->FindOrCreateAttribute(so, "AttributeParameter"));
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
  _lockers->Append(TCollection_AsciiString((char*)theLockerID));
}

//============================================================================
/*! Function : IsStudyLocked
 *  Purpose  :
 */
//============================================================================
bool SALOMEDSImpl_Study::IsStudyLocked()
{
  return (_lockers->Length() > 0);
}

//============================================================================
/*! Function : UnLockStudy
 *  Purpose  :
 */
//============================================================================
void SALOMEDSImpl_Study::UnLockStudy(const char* theLockerID)
{
  int length = _lockers->Length(), pos = -1;
  TCollection_AsciiString id((char*)theLockerID);
  for(int i = 1; i<=length; i++) {
    if(id == _lockers->Value(i)) {
      pos = i;
      break;
    }
  }
  if(pos > 0) _lockers->Remove(pos);
}
  
//============================================================================
/*! Function : GetLockerID
 *  Purpose  :
 */
//============================================================================
Handle(TColStd_HSequenceOfAsciiString) SALOMEDSImpl_Study::GetLockerID()
{
  return _lockers;
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