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
//  File   : SALOMEDS_Study_i.cxx
//  Author : Yves FRICAUD
//  Module : SALOME
//  $Header$

#include <TDataStd_ChildNodeIterator.hxx>
#include <TDocStd_Application.hxx>
#include <TDocStd_Owner.hxx>
#include <CDM_Document.hxx>
#include <CDM_Application.hxx>
#include <TDF_ChildIDIterator.hxx>

#include <TColStd_SequenceOfExtendedString.hxx>
#include <TCollection_ExtendedString.hxx>
#include <TCollection_AsciiString.hxx>

#include <TColStd_ListIteratorOfListOfInteger.hxx>
#include <TColStd_ListOfInteger.hxx>

#include "SALOMEDS.hxx"
#include "SALOMEDS_Study_i.hxx"

#include "SALOMEDS_StudyManager_i.hxx"
#include "SALOMEDS_Callback_i.hxx"

#include "SALOMEDS_SComponent_i.hxx"
#include "SALOMEDS_SObject_i.hxx"

#include "SALOMEDS_StudyBuilder_i.hxx"
#include "SALOMEDS_ChildIterator_i.hxx"

#include "SALOMEDS_UseCaseBuilder_i.hxx"
#include "SALOMEDS_SComponentIterator_i.hxx"

#include "SALOME_GenericObj_i.hh"
#include "SALOMEDS_LocalIDAttribute.hxx"
#include "SALOMEDS_PersRefAttribute.hxx"

#include "SALOMEDS_StudyPropertiesAttribute.hxx"
#include "SALOMEDS_DataMapIteratorOfDataMapStringLabel.hxx"

#include "utilities.h"

#define DIRECTORYID 16661
#define FILEID "FILE: "
#define FILELOCALID 26662 

using namespace std;

bool operator<(const TDF_Label& theLeft, const TDF_Label& theRight)
{
  TColStd_ListOfInteger aTagLeftList;
  TDF_Tool::TagList(theLeft,aTagLeftList);
  TColStd_ListIteratorOfListOfInteger anLeftIter(aTagLeftList);
  
  TColStd_ListOfInteger aTagRightList;
  TDF_Tool::TagList(theRight,aTagRightList);
  TColStd_ListIteratorOfListOfInteger anRightIter(aTagRightList);
  
  for(;;){
    Standard_Boolean aLeftMore = anLeftIter.More();
    Standard_Boolean aRightMore = anRightIter.More();
    
    if(!aLeftMore && !aRightMore)
      return Standard_False;
    
    if(!aLeftMore)
      return Standard_True;
    
    if(!aRightMore)
      return Standard_False;
    
    Standard_Integer aLeftTag = anLeftIter.Value();
    anLeftIter.Next();
    
    Standard_Integer aRightTag = anRightIter.Value();
    anRightIter.Next();
    
    if(aLeftTag == aRightTag)
      continue;
    
    return aLeftTag < aRightTag;
  }
  
  return Standard_False;
}

  
//============================================================================
/*! Function : SALOMEDS_Study_i
 *  Purpose  : SALOMEDS_Study_i constructor
 */
//============================================================================
SALOMEDS_Study_i::SALOMEDS_Study_i(SALOMEDS_StudyManager_i* theStudyManager,
				   const Handle(TDocStd_Document)& theDoc,
				   const char* theStudyName):
  _StudyManager(theStudyManager),
  _doc(theDoc),
  _isSaved(false),
  _URL(NULL),
  _StudyId(-1),
  _autoFill(true),
  myNbUndos(0)
{
  _UseCaseBuilder = new SALOMEDS_UseCaseBuilder_i(this,_doc);
  SALOMEDS::UseCaseBuilder_var aUseCaseBuilder = _UseCaseBuilder->_this();

  _Builder = new SALOMEDS_StudyBuilder_i(this,_doc);
  SALOMEDS::StudyBuilder_var aStudyBuilder = _Builder->_this(); 

  SALOMEDS_Callback_i* aCallBackServant = new SALOMEDS_Callback_i(aUseCaseBuilder);
  _callbackOnAdd = aCallBackServant->_this();
  _callbackOnRemove = _callbackOnAdd;

  _name = new char[strlen(theStudyName) +1];
  strcpy(_name,theStudyName);
  myNbPostponed.Append(0);
}
  
//============================================================================
/*! Function : ~SALOMEDS_Study_i
 *  Purpose  : SALOMEDS_Study_i destructor
 */
//============================================================================
SALOMEDS_Study_i::~SALOMEDS_Study_i()
{
  delete [] _name ;
  delete [] _URL ;
}  

//============================================================================
CORBA::ORB_var SALOMEDS_Study_i::GetORB() const
{
  return _StudyManager->GetORB();
}

//============================================================================
PortableServer::POA_var SALOMEDS_Study_i::GetPOA() const
{
  return _StudyManager->GetPOA();
}


SALOMEDS_SObject_i* 
SALOMEDS_Study_i::DownCast(SALOMEDS::SObject_ptr theSObject) const
{
  if(!CORBA::is_nil(theSObject)){
    PortableServer::POA_var aPOA = GetPOA();
    PortableServer::ServantBase_var aServant = SALOMEDS::GetServant(theSObject,aPOA);
    if(aServant.in())
      return dynamic_cast<SALOMEDS_SObject_i*>(aServant.in());
  }
  return NULL;
}

//============================================================================
/*! Function : SetOnAddSObject
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::Callback_ptr SALOMEDS_Study_i::SetOnAddSObject(SALOMEDS::Callback_ptr theCallback)
{
  SALOMEDS::Callback_var aRet = _callbackOnAdd;
  _callbackOnAdd = SALOMEDS::Callback::_duplicate(theCallback);
  return aRet._retn();
}

//============================================================================
/*! Function : SetOnNewSObject
 *  Purpose  : 
 */
//============================================================================
SALOMEDS::Callback_ptr SALOMEDS_Study_i::SetOnRemoveSObject(SALOMEDS::Callback_ptr theCallback)
{
  SALOMEDS::Callback_var aRet = _callbackOnRemove;
  _callbackOnAdd = SALOMEDS::Callback::_duplicate(theCallback);
  return aRet._retn();
}

//============================================================================
void SALOMEDS_Study_i::OnAddSObject(SALOMEDS::SObject_ptr theObject)
{
  if(!CORBA::is_nil(_callbackOnAdd.in()))
    _callbackOnAdd->OnAddSObject(theObject);
}

//============================================================================
void SALOMEDS_Study_i::OnRemoveSObject(SALOMEDS::SObject_ptr theObject)
{
  if(!CORBA::is_nil(_callbackOnRemove.in()))
    _callbackOnRemove->OnRemoveSObject(theObject);
}

//============================================================================
void SALOMEDS_Study_i::CheckLocked()
{
  SALOMEDS::Locker lock;

  if(_doc->HasOpenCommand()) 
    return;

  Handle(SALOMEDS_StudyPropertiesAttribute) anAttr;
  if(_doc->Main().FindAttribute(SALOMEDS_StudyPropertiesAttribute::GetID(),anAttr))
    if(anAttr->IsLocked())
      throw SALOMEDS::StudyBuilder::LockProtection();
}


//============================================================================
char* SALOMEDS_Study_i::ConvertObjectToIOR(CORBA::Object_ptr theObject)
{
  return GetORB()->object_to_string(theObject); 
}


//============================================================================
CORBA::Object_ptr SALOMEDS_Study_i::ConvertIORToObject(const char* theIOR) 
{ 
  return GetORB()->string_to_object(theIOR); 
}


//============================================================================
/*! Function : GetPersistentReference
 *  Purpose  : Get persistent reference of study (idem URL())
 */
//============================================================================
char* SALOMEDS_Study_i::GetPersistentReference()
{
  SALOMEDS::Locker lock;

  return URL();
}
//============================================================================
/*! Function : GetTransientReference
 *  Purpose  : Get IOR of the Study (registred in OCAF document in doc->Root)
 */
//============================================================================
char* SALOMEDS_Study_i::GetTransientReference()
{
  SALOMEDS::Locker lock;

  CORBA::String_var IOR;

  Handle(SALOMEDS_IORAttribute) Att;
  TDF_Label _lab = _doc->GetData()->Root();
  if (!_lab.FindAttribute(SALOMEDS_IORAttribute::GetID(),Att)){

    TCollection_AsciiString ch(Att->Get());
    IOR = CORBA::string_dup(ch.ToCString());
  }
  else IOR = CORBA::string_dup(""); // NULL ?

  return CORBA::string_dup(IOR); 
}

//============================================================================
/*! Function : IsEmpty
 *  Purpose  : Detect if study is empty
 */
//============================================================================
CORBA::Boolean SALOMEDS_Study_i::IsEmpty()
{
  SALOMEDS::Locker lock;

  if (_doc.IsNull()) return true;
  return _doc->IsEmpty();
}

//============================================================================
/*! Function : FindComponent
 *  Purpose  : Find a Component with ComponentDataType = aComponentName
 */
//============================================================================
SALOMEDS::SComponent_ptr 
SALOMEDS_Study_i::FindComponent(const char* theComponentName)
{
  SALOMEDS::Locker lock;

  bool anIsFound = false;
  SALOMEDS::SComponent_var aSComponent;
  SALOMEDS_SComponentIterator_i aComponentIter(this,_doc);
  for(; aComponentIter.More() && !anIsFound; aComponentIter.Next()){
    SALOMEDS::SComponent_var aSComp = aComponentIter.Value();
    CORBA::String_var aName = aSComp->ComponentDataType();
    if(strcmp(theComponentName,aName.in()) == 0){
      aSComponent = aSComp;
      anIsFound = true;
    }
  }
  return aSComponent._retn();
}

//============================================================================
/*! Function : FindComponentID
 *  Purpose  : Find a Component from it's ID
 */
//============================================================================
SALOMEDS::SComponent_ptr SALOMEDS_Study_i::FindComponentID(const char* aComponentID)
{
  SALOMEDS::Locker lock;

  // Iterate on each components defined in the study
  // Get the component ID and compare with aComponentID 
  bool _find = false;
  char *ID;
  SALOMEDS::SComponent_ptr compo;

  SALOMEDS_SComponentIterator_i itcomp(this,_doc);
  for (; itcomp.More(); itcomp.Next()) {
    SALOMEDS::SComponent_var SC = itcomp.Value();
    ID = SC->GetID();
    if(strcmp(aComponentID,ID)==0)
      {
	// ComponentID found
	_find = true;
        compo = SALOMEDS::SComponent::_narrow(SC);
      }
  }
  if(!_find)
    {
      compo = SALOMEDS::SComponent::_nil();
    }
  return compo;
}

//============================================================================
/*! Function : FindObject
 *  Purpose  : Find an Object with SALOMEDS::Name = anObjectName
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::FindObject(const char* theObjectName)
{
  SALOMEDS::Locker lock;

  // Iterate to all components defined in the study
  // After testing the component name, iterate in all objects defined under
  // components (function _FindObject)
  bool aIsFound = false;
  SALOMEDS::SObject_var aRefSO;
  SALOMEDS_SComponentIterator_i aComponentIter(this,_doc);
  for(; aComponentIter.More() && !aIsFound; aComponentIter.Next()){
    TDF_Label aLab = aComponentIter.GetValue();
    Handle(TDataStd_Name) anAttr;
    if(aLab.FindAttribute(TDataStd_Name::GetID(),anAttr)){
      TCollection_AsciiString aString(anAttr->Get());
      if(strcmp(aString.ToCString(),theObjectName) == 0){
	aRefSO = SALOMEDS_SComponent_i::NewRef(this,aLab)._retn();
	aIsFound = true;
      }
    }
    if(!aIsFound) 
      aRefSO = _FindObject(aLab,theObjectName,aIsFound);
  }

  return aRefSO._retn();
}

//============================================================================
/*! Function : FindObjectID
 *  Purpose  : Find an Object with ID = anObjectID
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::FindObjectID(const char* anObjectID)
{
  SALOMEDS::Locker lock;

  // Convert aSO->GetID in TDF_Label.
  TDF_Label Lab;
  TDF_Tool::Label(_doc->GetData(), (char*)anObjectID, Lab);
  
  if (Lab.IsNull()) 
    return SALOMEDS::SObject::_nil();

  return SALOMEDS_SObject_i::NewRef(this,Lab)._retn();

}

//============================================================================
/*! Function : CreateObjectID
 *  Purpose  : Creates an Object with ID = anObjectID
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::CreateObjectID(const char* anObjectID)
{
  SALOMEDS::Locker lock;

  // Convert aSO->GetID in TDF_Label.
  TDF_Label Lab;
  TDF_Tool::Label(_doc->GetData(), (char*)anObjectID, Lab, Standard_True);
  
  if (Lab.IsNull()) 
    return SALOMEDS::SObject::_nil();

  return SALOMEDS_SObject_i::NewRef(this,Lab)._retn();
}

//============================================================================
/*! Function : FindObjectByName
 *  Purpose  : Find Objects with SALOMEDS::Name = anObjectName in a Component
 *           : with ComponentDataType = aComponentName
 */
//============================================================================
SALOMEDS::Study::ListOfSObject* 
SALOMEDS_Study_i::FindObjectByName(const char* theObjectName,
				   const char* theComponentName)
{
  SALOMEDS::Locker lock;

  SALOMEDS::Study::ListOfSObject_var aListOfSObj = new SALOMEDS::Study::ListOfSObject ;
  aListOfSObj->length(0);

  SALOMEDS::SComponent_ptr aSComponent = FindComponent(theComponentName) ;
  if(aSComponent->_is_nil()){
    MESSAGE ("In FindObjectByName() :  Component named " << theComponentName << " not found ");
    return aListOfSObj._retn();
  }

  // Iterate on each object and subobject of the component
  // If objectName is found add it to the list of SObjects 
  TDF_Label aLabel;
  CORBA::String_var anEntry = aSComponent->GetID();
  TDF_Tool::Label(_doc->GetData(),const_cast<char*>(anEntry.in()),aLabel);
  
  int aLength = 0 ;
  SALOMEDS::SObject_var aRefSO;
  TDF_ChildIterator aChildIter(aLabel,true);
  for(; aChildIter.More(); aChildIter.Next()){
    TDF_Label aLab = aChildIter.Value();
    Handle(TDataStd_Name) anAttr;
    if(aLab.FindAttribute(TDataStd_Name::GetID(),anAttr)){
      TCollection_AsciiString aString(anAttr->Get());
      if(strcmp(aString.ToCString(),theObjectName) == 0){
	aRefSO = SALOMEDS_SObject_i::NewRef(this,aLab)._retn();
	/* add to list */
	aLength++ ;
	aListOfSObj->length(aLength);
	aListOfSObj[aLength-1] = aRefSO;
      }
    }
  }

  return aListOfSObj._retn() ;
}



//============================================================================
/*! Function : FindObjectIOR
 *  Purpose  : Find an Object with IOR = anObjectIOR
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::FindObjectIOR(const char* theObjectIOR)
{
  SALOMEDS::Locker lock;

  // firstly searching in the datamap for optimization
  char* anIOR = const_cast<char*>(theObjectIOR);
  if(myIORLabels.IsBound(anIOR)){
    TDF_Label aLabel = myIORLabels.Find(anIOR);
    TSObjectHolder aSObjectHolder = SALOMEDS_SObject_i::New(this,aLabel);
    SALOMEDS_SObject_i* aSObjectPtr = aSObjectHolder.first;
    SALOMEDS::SObject_var aSObject = aSObjectHolder.second;
    // 11 oct 2002: forbidden attributes must be checked here
    SALOMEDS::GenericAttribute_var anAttr;
    if(!aSObjectPtr->FindAttribute(anAttr,"AttributeIOR")){
      myIORLabels.UnBind(anIOR);
    }else{
      return aSObject._retn();
    }
  }

  // Iterate to all components defined in the study
  // After testing the component name, iterate in all objects defined under
  // components (function _FindObject)
  bool aIsFound = false;
  SALOMEDS::SObject_var aRefSO;
  SALOMEDS_SComponentIterator_i aComponentIter(this,_doc);
  for(; aComponentIter.More() && !aIsFound; aComponentIter.Next()){
    TDF_Label aLab = aComponentIter.GetValue();
    Handle(SALOMEDS_IORAttribute) anAttr;
    if(aLab.FindAttribute(SALOMEDS_IORAttribute::GetID(),anAttr)){
      TCollection_AsciiString aString(anAttr->Get());
      if(strcmp(aString.ToCString(),theObjectIOR) == 0){
	aRefSO = SALOMEDS_SComponent_i::NewRef(this,aLab);
	aIsFound = true;
      }
    }
    if(!aIsFound) 
      aRefSO = _FindObjectIOR(aLab,theObjectIOR,aIsFound);
  }

  if(!aRefSO->_is_nil()) 
    MESSAGE("SALOMEDS_Study_i::FindObjectIOR: found label with old methods");

  return aRefSO._retn();
}

//============================================================================
/*! Function : FindObjectByPath
 *  Purpose  : Find an Object by its path = thePath
 */
//============================================================================
SALOMEDS::SObject_ptr SALOMEDS_Study_i::FindObjectByPath(const char* thePath)
{
  SALOMEDS::Locker lock;

  TCollection_AsciiString aPath(CORBA::string_dup(thePath)), aToken;
  SALOMEDS::SObject_var aSO = SALOMEDS::SObject::_nil();
  int i = 1, aLength = aPath.Length();
  bool isRelative = false;

  if(aLength == 0) {  //Empty path - return the current context
    return SALOMEDS_SObject_i::NewRef(this,_current)._retn();
  }

  if(aPath.Value(1) != '/')  //Relative path 
    isRelative = true;

  TDF_ChildIterator anIterator;
  TDF_Label aLabel;
  Handle(TDataStd_Name) anAttr;

  if(isRelative) {
    if(_current.IsNull()) throw SALOMEDS::Study::StudyInvalidContext(); 
    anIterator.Initialize(_current, Standard_False);
  }
  else {
    if(aPath.Length() == 1 && aPath.Value(1) == '/') {    //Root
      return SALOMEDS_SObject_i::NewRef(this,_doc->Main())._retn();
    }
    anIterator.Initialize(_doc->Main(), Standard_False);
  }

  while(i <= aLength) {

    aToken = aPath.Token("/", i);
    if(aToken.Length() == 0) break;

    for ( ; anIterator.More(); anIterator.Next() ) {
      aLabel = anIterator.Value();
      if(aLabel.FindAttribute(TDataStd_Name::GetID(), anAttr)) {
	if(anAttr->Get() == aToken) {
	  aToken = aPath.Token("/", i+1); //Check if it was the last part of the path
	  if(aToken.Length() == 0) {  //The searched label is found (no part of the path is left)
	    return SALOMEDS_SObject_i::NewRef(this,aLabel)._retn();
	  }

	  anIterator.Initialize(aLabel, Standard_False);
	  break;
	}
      }
    }

    i++;
  }

  return aSO._retn();
}

//============================================================================
/*! Function : GetObjectPath
 *  Purpose  : 
 */
//============================================================================
char* SALOMEDS_Study_i::GetObjectPath(CORBA::Object_ptr theObject)
{
  SALOMEDS::Locker lock;

  TCollection_AsciiString aPath("");
  if(CORBA::is_nil(theObject)) 
    return CORBA::string_dup(aPath.ToCString());

  SALOMEDS::SObject_var anObject = SALOMEDS::SObject::_narrow(theObject);
  if(anObject->_is_nil()) {
    CORBA::String_var anIOR = GetORB()->object_to_string(theObject);
    anObject = FindObjectIOR(anIOR);
    if(anObject->_is_nil()) 
      return CORBA::string_dup(aPath.ToCString());
  }

  SALOMEDS::GenericAttribute_var anAttr;
  if(anObject->FindAttribute(anAttr, "AttributeName")) {
    SALOMEDS::AttributeName_var aName = SALOMEDS::AttributeName::_narrow(anAttr);
    if(anAttr->_is_nil()) 
      return CORBA::string_dup(aPath.ToCString());
    TCollection_AsciiString aValue(aName->Value());
    aValue.Prepend("/");
    aValue += aPath;
    aPath = aValue;
    SALOMEDS::SObject_ptr aFather = anObject->GetFather();
    if(!aFather->_is_nil()) {
      TDF_Label aLabel;
      Handle(TDataStd_Name) aNameAttrib;
      TDF_Tool::Label(_doc->GetData(), aFather->GetID(), aLabel);
      if(aLabel.FindAttribute(TDataStd_Name::GetID(), aNameAttrib)) {
	  aValue = GetObjectPath(aFather);
	  aPath = aValue + aPath;
      }
    }
  }

  return  CORBA::string_dup(aPath.ToCString());
}


//============================================================================
/*! Function : SetContext
 *  Purpose  : Sets the current context
 */
//============================================================================
void SALOMEDS_Study_i::SetContext(const char* thePath) 
{
  SALOMEDS::Locker lock;

  if(thePath == NULL || strlen(thePath) == 0) throw SALOMEDS::Study::StudyInvalidDirectory();
  TCollection_AsciiString aPath(CORBA::string_dup(thePath)), aContext("");
  bool isInvalid = false;
  SALOMEDS::SObject_var aSO;
  
  if(aPath.Value(1) != '/') { //Relative path 
    aContext = TCollection_AsciiString(GetContext());
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

  if(isInvalid || aSO->_is_nil()) throw SALOMEDS::Study::StudyInvalidContext();

  TDF_Label aLabel;
  TDF_Tool::Label(_doc->GetData(), aSO->GetID(), aLabel);
  if(aLabel.IsNull()) throw SALOMEDS::Study::StudyInvalidContext();
  else
    _current = aLabel;  //Set the current context
  
}

//============================================================================
/*! Function : GetContext
 *  Purpose  : Gets the current context
 */
//============================================================================
char* SALOMEDS_Study_i::GetContext() 
{
  SALOMEDS::Locker lock;

  if(_current.IsNull()) 
    throw SALOMEDS::Study::StudyInvalidContext();   

  SALOMEDS::SObject_var aSObject = SALOMEDS_SObject_i::NewRef(this,_current);
  return GetObjectPath(aSObject);
}

//============================================================================
/*! Function : GetObjectNames
 *  Purpose  : method to get all object names in the given context (or in the current context, if 'theContext' is empty)
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetObjectNames(const char* theContext) {
  SALOMEDS::Locker lock;

  TColStd_SequenceOfExtendedString aResultSeq;
  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;
  TDF_Label aLabel;
  if (strlen(theContext) == 0) {
    if(_current.IsNull()) throw SALOMEDS::Study::StudyInvalidContext();   
    aLabel = _current;
  } else {
    TDF_Label aTmp = _current;
    SetContext(theContext);
    aLabel = _current;
    _current = aTmp;
  }
  TDF_ChildIterator anIter(aLabel, Standard_False); // iterate all subchildren at all sublevels
  for(; anIter.More(); anIter.Next()) {
    TDF_Label aLabel = anIter.Value();
//      Handle(TDF_Attribute) anAttribute;
//      if (aLabel.FindAttribute(SALOMEDS_IORAttribute::GetID(), anAttribute) ||
//  	aLabel.FindAttribute(SALOMEDS_LocalIDAttribute::GetID(), anAttribute)) {
    Handle(TDataStd_Name) aName;
    if (aLabel.FindAttribute(TDataStd_Name::GetID(), aName)) aResultSeq.Append(aName->Get());
//    }
  }
  // fill the result table
  int anIndex, aLength = aResultSeq.Length();
  aResult->length(aLength);
  for(anIndex = 0; anIndex < aLength; anIndex++) {
    aResult[anIndex] = CORBA::string_dup(TCollection_AsciiString(aResultSeq.Value(anIndex + 1)).ToCString());
  }
  return aResult._retn();
}

//============================================================================
/*! Function : GetDirectoryNames
 *  Purpose  : method to get all directory names in the given context (or in the current context, if 'theContext' is empty)
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetDirectoryNames(const char* theContext) {
  SALOMEDS::Locker lock;

  TColStd_SequenceOfExtendedString aResultSeq;
  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;
  TDF_Label aLabel;
  if (strlen(theContext) == 0) {
    if(_current.IsNull()) throw SALOMEDS::Study::StudyInvalidContext();   
    aLabel = _current;
  } else {
    TDF_Label aTmp = _current;
    SetContext(theContext);
    aLabel = _current;
    _current = aTmp;
  }
  TDF_ChildIterator anIter(aLabel, Standard_False); // iterate first-level children at all sublevels
  for(; anIter.More(); anIter.Next()) {
    TDF_Label aLabel = anIter.Value();
    Handle(SALOMEDS_LocalIDAttribute) anID;
    if (aLabel.FindAttribute(SALOMEDS_LocalIDAttribute::GetID(), anID)) {
      if (anID->Get() == DIRECTORYID) {
	Handle(TDataStd_Name) aName;
	if (aLabel.FindAttribute(TDataStd_Name::GetID(), aName)) {
	  aResultSeq.Append(aName->Get());
	}
      }
    }
  }
  // fill the result table
  int anIndex, aLength = aResultSeq.Length();
  aResult->length(aLength);
  for(anIndex = 0; anIndex < aLength; anIndex++) {
    aResult[anIndex] = CORBA::string_dup(TCollection_AsciiString(aResultSeq.Value(anIndex + 1)).ToCString());
  }
  return aResult._retn();
}

//============================================================================
/*! Function : GetFileNames
 *  Purpose  : method to get all file names in the given context (or in the current context, if 'theContext' is empty)
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetFileNames(const char* theContext) {
  SALOMEDS::Locker lock;

  TColStd_SequenceOfExtendedString aResultSeq;
  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;
  TDF_Label aLabel;
  if (strlen(theContext) == 0) {
    if(_current.IsNull()) throw SALOMEDS::Study::StudyInvalidContext();   
    aLabel = _current;
  } else {
    TDF_Label aTmp = _current;
    SetContext(theContext);
    aLabel = _current;
    _current = aTmp;
  }
  TDF_ChildIterator anIter(aLabel, Standard_False); // iterate all subchildren at all sublevels
  for(; anIter.More(); anIter.Next()) {
    TDF_Label aLabel = anIter.Value();
    Handle(SALOMEDS_LocalIDAttribute) anID;
    if (aLabel.FindAttribute(SALOMEDS_LocalIDAttribute::GetID(), anID)) {
      if (anID->Get() == FILELOCALID) {
	Handle(SALOMEDS_PersRefAttribute) aName;
	if(aLabel.FindAttribute(SALOMEDS_PersRefAttribute::GetID(), aName)) {
	  TCollection_ExtendedString aFileName = aName->Get();
	  if(aFileName.Length() > 0)
	    aResultSeq.Append(aFileName.Split(strlen(FILEID)));
	}
      }
    }
//      }
  }
  // fill the result table
  int anIndex, aLength = aResultSeq.Length();
  aResult->length(aLength);
  for(anIndex = 0; anIndex < aLength; anIndex++) {
    aResult[anIndex] = CORBA::string_dup(TCollection_AsciiString(aResultSeq.Value(anIndex + 1)).ToCString());
  }
  return aResult._retn();
}

//============================================================================
/*! Function : GetComponentNames
 *  Purpose  : method to get all components names
 */
//============================================================================
SALOMEDS::ListOfStrings* SALOMEDS_Study_i::GetComponentNames(const char* theContext) {
  SALOMEDS::Locker lock;

  TColStd_SequenceOfExtendedString aResultSeq;
  SALOMEDS::ListOfStrings_var aResult = new SALOMEDS::ListOfStrings;
  TDF_ChildIterator anIter(_doc->Main(), Standard_False); // iterate all subchildren at first level
  for(; anIter.More(); anIter.Next()) {
    TDF_Label aLabel = anIter.Value();
    Handle(TDataStd_Name) aName;
    if (aLabel.FindAttribute(TDataStd_Name::GetID(), aName)) aResultSeq.Append(aName->Get());
  }
  // fill the result table
  int anIndex, aLength = aResultSeq.Length();
  aResult->length(aLength);
  for(anIndex = 0; anIndex < aLength; anIndex++) {
    aResult[anIndex] = CORBA::string_dup(TCollection_AsciiString(aResultSeq.Value(anIndex + 1)).ToCString());
  }
  return aResult._retn();
}

//============================================================================
/*! Function : NewChildIterator
 *  Purpose  : Create a ChildIterator from an SObject
 */
//============================================================================
SALOMEDS::ChildIterator_ptr 
SALOMEDS_Study_i::NewChildIterator(SALOMEDS::SObject_ptr theSObject)
{
  SALOMEDS::Locker lock;

  SALOMEDS_ChildIterator_i* aServant = 
    new SALOMEDS_ChildIterator_i(GetChildIterator(theSObject));

  return aServant->_this();
}

SALOMEDS_ChildIterator_i 
SALOMEDS_Study_i::GetChildIterator(SALOMEDS::SObject_ptr theSObject)
{
  TDF_Label aLab;
  TDF_Tool::Label(_doc->GetData(),theSObject->GetID(),aLab);
  return SALOMEDS_ChildIterator_i(this,aLab);
}

//============================================================================
/*! Function : NewComponentIterator
 *  Purpose  : Create a SComponentIterator
 */
//============================================================================
SALOMEDS::SComponentIterator_ptr 
SALOMEDS_Study_i::NewComponentIterator()
{
  SALOMEDS::Locker lock;

  SALOMEDS_SComponentIterator_i* aServant = 
    new SALOMEDS_SComponentIterator_i(GetComponentIterator());

  return aServant->_this();
}

SALOMEDS_SComponentIterator_i 
SALOMEDS_Study_i::GetComponentIterator()
{
  return SALOMEDS_SComponentIterator_i(this,_doc);
}

//============================================================================
/*! Function : GetUseCaseBuilder
 *  Purpose  : Returns a UseCase builder
 */
//============================================================================
SALOMEDS::UseCaseBuilder_ptr SALOMEDS_Study_i::GetUseCaseBuilder() 
{
  SALOMEDS::Locker lock;

  return _UseCaseBuilder->_this();
}

//============================================================================
/*! Function : NewBuilder
 *  Purpose  : Create a StudyBuilder
 */
//============================================================================
SALOMEDS_StudyBuilder_i* SALOMEDS_Study_i::GetBuilder()
{
  return _Builder;
}
 
SALOMEDS::StudyBuilder_ptr SALOMEDS_Study_i::NewBuilder()
{
  SALOMEDS::Locker lock;

  return GetBuilder()->_this();
}
 
//============================================================================
/*! Function : Name
 *  Purpose  : get study name
 */
//============================================================================
char* SALOMEDS_Study_i::Name()
{
  SALOMEDS::Locker lock;

  return CORBA::string_dup(_name);
}

//============================================================================
/*! Function : Name
 *  Purpose  : set study name
 */
//============================================================================
void SALOMEDS_Study_i::Name(const char* name)
{
  SALOMEDS::Locker lock;

  _name = new char[strlen(name) +1];
  strcpy(_name,name);
}

//============================================================================
/*! Function : IsSaved
 *  Purpose  : get if study has been saved
 */
//============================================================================
CORBA::Boolean  SALOMEDS_Study_i::IsSaved()
{
  SALOMEDS::Locker lock;

  return _isSaved;
}

//============================================================================
/*! Function : IsSaved
 *  Purpose  : set if study has been saved
 */
//============================================================================
void SALOMEDS_Study_i::IsSaved(CORBA::Boolean save)
{
  SALOMEDS::Locker lock;

  _isSaved = save;
}

//============================================================================
/*! Function : IsModified
 *  Purpose  : Detect if a Study has been modified since it has been saved
 */
//============================================================================
CORBA::Boolean  SALOMEDS_Study_i::IsModified()
{
  SALOMEDS::Locker lock;

  // True if is modified and not saved
  if (_doc->IsModified())
    if (!_isSaved) return true;
    else return false;
  else return false;
}

//============================================================================
/*! Function : URL
 *  Purpose  : get URL of the study (persistent reference of the study)
 */
//============================================================================
char* SALOMEDS_Study_i::URL()
{
  SALOMEDS::Locker lock;

  if(!_URL) {
    _URL = new char[1];
    _URL[0] = (char)0;
  }
  return CORBA::string_dup(_URL);
}

//============================================================================
/*! Function : URL
 *  Purpose  : set URL of the study (persistent reference of the study)
 */
//============================================================================
void SALOMEDS_Study_i::URL(const char* url)
{
  SALOMEDS::Locker lock;

  if (_URL) delete [] _URL;
  _URL = new char[strlen(url) +1];
  strcpy(_URL,url);
  SCRUTE(_URL);

  char *aName = _URL;
  char *adr = strtok(aName, "/");
  while (adr)
    {
      aName = adr;
      adr = strtok(NULL, "/");
    }
  strcpy(_URL,url);
  Name(aName);
}


//============================================================================
/*! Function : _FindObject
 *  Purpose  : Find an Object with SALOMEDS::Name = anObjectName
 */
//============================================================================
SALOMEDS::SObject_ptr 
SALOMEDS_Study_i::_FindObject(TDF_Label theLabel,
			      const char* theObjectName, 
			      bool& theIsFound)
{
  theIsFound = false;
  // Iterate on each objects and subobjects of the component
  // If objectName find, stop the loop and get the object reference
  SALOMEDS::SObject_var aRefSO;
  TDF_ChildIterator aChildIter(theLabel,true);
  for(; aChildIter.More() && !theIsFound; aChildIter.Next()){
    TDF_Label aLab = aChildIter.Value();
    Handle(TDataStd_Name) anAttr;
    if(aLab.FindAttribute(TDataStd_Name::GetID(),anAttr)){
      TCollection_AsciiString aString(anAttr->Get());
      if(strcmp(aString.ToCString(),theObjectName) == 0){
	aRefSO = SALOMEDS_SObject_i::NewRef(this,aLab);
	theIsFound = true;
      }
    }
  }
  return aRefSO._retn();
}

//============================================================================
/*! Function : _FindObject
 *  Purpose  : Find an Object with SALOMEDS::IOR = anObjectIOR
 */
//============================================================================
SALOMEDS::SObject_ptr 
SALOMEDS_Study_i::_FindObjectIOR(TDF_Label theLabel,
				 const char* theObjectIOR, 
				 bool& theIsFound)
{
  // Iterate on each objects and subobjects of the component
  // If objectName find, stop the loop and get the object reference
  SALOMEDS::SObject_var aRefSO;
  TDF_ChildIterator aChildIter(theLabel,true);
  for(; aChildIter.More() && !theIsFound; aChildIter.Next()){
    TDF_Label aLab = aChildIter.Value();
    Handle(SALOMEDS_IORAttribute) anAttr;
    if(aLab.FindAttribute(SALOMEDS_IORAttribute::GetID(),anAttr)){
      TCollection_AsciiString aString(anAttr->Get());
      if(strcmp(aString.ToCString(),theObjectIOR) == 0){
	aRefSO = SALOMEDS_SObject_i::NewRef(this,aLab);
	theIsFound = true;
      }
    }
  }
  return aRefSO._retn();
}

CORBA::Short SALOMEDS_Study_i::StudyId()
{
  SALOMEDS::Locker lock;

  return _StudyId;
}

void SALOMEDS_Study_i::StudyId(CORBA::Short id)
{
  SALOMEDS::Locker lock;

  _StudyId = id;
}

void SALOMEDS_Study_i::UpdateIORLabelMap(const char* anIOR,const char* anEntry) {
  SALOMEDS::Locker lock;

  TDF_Label aLabel;
  CORBA::String_var anEn = CORBA::string_dup(anEntry);
  CORBA::String_var IOR = CORBA::string_dup(anIOR);
  TDF_Tool::Label(_doc->GetData(),anEn,aLabel, Standard_True);
  if (myIORLabels.IsBound(TCollection_ExtendedString(IOR))) myIORLabels.UnBind(TCollection_ExtendedString(IOR));
  myIORLabels.Bind(TCollection_ExtendedString(IOR), aLabel);
}

void SALOMEDS_Study_i::IORUpdated(const Handle(SALOMEDS_IORAttribute) theAttribute) {
  TCollection_AsciiString aString;
  TDF_Tool::Entry(theAttribute->Label(),aString);
  TCollection_AsciiString aValue(theAttribute->Get());
  UpdateIORLabelMap(aValue.ToCString(),aString.ToCString());
}

SALOMEDS::Study::ListOfSObject* SALOMEDS_Study_i::FindDependances(SALOMEDS::SObject_ptr anObject) {
  SALOMEDS::Locker lock;

  SALOMEDS::GenericAttribute_ptr aTarget;
  if (anObject->FindAttribute(aTarget,"AttributeTarget")) {
    return SALOMEDS::AttributeTarget::_narrow(aTarget)->Get();
  }
  SALOMEDS::Study::ListOfSObject* aList = new SALOMEDS::Study::ListOfSObject;
  aList->length(0);
  return aList;
}


SALOMEDS::AttributeStudyProperties_ptr SALOMEDS_Study_i::GetProperties(){
  SALOMEDS::Locker lock;

  SALOMEDS::SObject_var aSObject = FindObjectID("0:1");

  SALOMEDS::GenericAttribute_var anAttr =  
    GetBuilder()->FindOrCreateAttribute(aSObject,"AttributeStudyProperties");

  return SALOMEDS::AttributeStudyProperties::_narrow(anAttr);
}

char* SALOMEDS_Study_i::GetLastModificationDate() {
  SALOMEDS::Locker lock;

  SALOMEDS::AttributeStudyProperties_var aProp = GetProperties();
  SALOMEDS::StringSeq_var aNames;
  SALOMEDS::LongSeq_var aMinutes, aHours, aDays, aMonths, aYears;
  aProp->GetModificationsList(aNames , aMinutes ,aHours, aDays, aMonths, aYears, true);
  int aLastIndex = aNames->length() - 1;
  char aResult[20];
  sprintf(aResult, "%2.2d/%2.2d/%4.4d %2.2d:%2.2d", (int)(aDays[aLastIndex]),(int)(aMonths[aLastIndex]),
	  (int)(aYears[aLastIndex]), (int)(aHours[aLastIndex]), (int)(aMinutes[aLastIndex]));
  CORBA::String_var aResStr = CORBA::string_dup(aResult);
  return aResStr._retn();
}

SALOMEDS::ListOfDates* SALOMEDS_Study_i::GetModificationsDate() {
  SALOMEDS::Locker lock;

  SALOMEDS::AttributeStudyProperties_var aProp = GetProperties();
  SALOMEDS::StringSeq_var aNames;
  SALOMEDS::LongSeq_var aMinutes, aHours, aDays, aMonths, aYears;
  aProp->GetModificationsList(aNames , aMinutes ,aHours, aDays, aMonths, aYears, false);

  int anIndex, aLength = aNames->length();
  SALOMEDS::ListOfDates_var aDates = new SALOMEDS::ListOfDates;
  aDates->length(aLength);

  for(anIndex = 0; anIndex < aLength; anIndex++) {
    char aDate[20];
    sprintf(aDate, "%2.2d/%2.2d/%4.4d %2.2d:%2.2d", (int)(aDays[anIndex]), (int)(aMonths[anIndex]),
	    (int)(aYears[anIndex]), (int)(aHours[anIndex]), (int)(aMinutes[anIndex]));
    aDates[anIndex] = CORBA::string_dup(aDate);
  }
  return aDates._retn();
}



//============================================================================
/*! Function : Close
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::Close()
{
  SALOMEDS::Locker lock;

  SALOMEDS_SComponentIterator_i itcomponent(this,_doc);

  const CORBA::ORB_var& anORB = GetORB();
  for (; itcomponent.More(); itcomponent.Next()) {
    SALOMEDS::SComponent_var sco = itcomponent.Value();
	  
    MESSAGE ( "Look for an engine for data type :"<< sco->ComponentDataType());
    // if there is an associated Engine call its method for closing
    CORBA::String_var IOREngine;
    if (sco->ComponentIOR(IOREngine)) {
      // we have found the associated engine to write the data 
      MESSAGE ( "We have found an engine for data type :"<< sco->ComponentDataType());
      CORBA::Object_var obj = anORB->string_to_object(IOREngine);
      if (!CORBA::is_nil(obj)) {
	SALOMEDS::Driver_var anEngine = SALOMEDS::Driver::_narrow(obj) ;
	
	if (!anEngine->_is_nil()){
	  SALOMEDS::unlock();
	  anEngine->Close(sco);
	  SALOMEDS::lock();
	}
      }
    }
  }

  Handle(TDocStd_Application) anApp = Handle(TDocStd_Application)::DownCast(_doc->Application());
//    Handle(TDocStd_Owner) anOwner;
//    if (_doc->Main().Root().FindAttribute(TDocStd_Owner::GetID(), anOwner)) {
//      Handle(TDocStd_Document) anEmptyDoc;
//      anOwner->SetDocument(anEmptyDoc);
//    }
  if(!anApp.IsNull()) anApp->Close(_doc);
  _doc.Nullify();
}

//============================================================================
/*! Function : AddPostponed
 *  Purpose  : 
 */
 //============================================================================
void SALOMEDS_Study_i::AddPostponed(const char* theIOR) {
  SALOMEDS::Locker lock;

  if (!GetBuilder()->HasOpenCommand()) return;
  try {
    CORBA::Object_var obj = GetORB()->string_to_object(theIOR);
    if (!CORBA::is_nil(obj)) {
      SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj) ;
      if (!CORBA::is_nil(aGeneric)) {
	TCollection_AsciiString anIOR(const_cast<char*>(theIOR));
	anIOR.Prepend("d");
	myPostponedIORs.Append(anIOR); // add prefix: deleted
	myNbPostponed.SetValue(myNbPostponed.Length(), myNbPostponed.Last() + 1);
      }
    }
  } catch(...) {}
}

void SALOMEDS_Study_i::AddCreatedPostponed(const char* theIOR) {
  SALOMEDS::Locker lock;

  if (!GetBuilder()->HasOpenCommand()) return;
  try {
    CORBA::Object_var obj = GetORB()->string_to_object(theIOR);
    if (!CORBA::is_nil(obj)) {
      SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj) ;
      if (!CORBA::is_nil(aGeneric)) {
	TCollection_AsciiString anIOR(const_cast<char*>(theIOR));
	anIOR.Prepend("c");
	myPostponedIORs.Append(anIOR); // add prefix: created
	myNbPostponed.SetValue(myNbPostponed.Length(), myNbPostponed.Last() + 1);
      }
    }
  } catch(...) {}
}

//============================================================================
/*! Function : RemovePostponed
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::RemovePostponed(const CORBA::Long theUndoLimit) {
  SALOMEDS::Locker lock;

  int anIndex;
  int anOld;

  int aUndoLimit = theUndoLimit;
  if (theUndoLimit < 0) aUndoLimit = 0;

  const CORBA::ORB_var& anORB = GetORB();
  if (myNbUndos > 0) { // remove undone
    anOld = 0;
    for(anIndex = 1; anIndex < myNbPostponed.Length() - myNbUndos; anIndex++)
      anOld += myNbPostponed(anIndex);
    int aNew = myPostponedIORs.Length() - myNbPostponed.Last();

    for(anIndex = anOld + 1; anIndex <= aNew; anIndex++) {
      TCollection_AsciiString anIOR = myPostponedIORs(anIndex);
      if (anIOR.Value(1) == 'c') {
	CORBA::Object_var obj = anORB->string_to_object(anIOR.Split(1).ToCString());
	SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj);
	if (!CORBA::is_nil(aGeneric)) aGeneric->Destroy();
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
	CORBA::Object_var obj = anORB->string_to_object(anIOR.Split(1).ToCString());
	SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj);
	if (!CORBA::is_nil(aGeneric)) aGeneric->Destroy();
      }
    }
    if (anOld > 0) myPostponedIORs.Remove(1, anOld);
    myNbPostponed.Remove(1, myNbPostponed.Length() - aUndoLimit);
  }

  if (theUndoLimit == -1) { // remove all IORs from the study on the study close
    TDF_ChildIDIterator anIter(_doc->GetData()->Root(), SALOMEDS_IORAttribute::GetID(), Standard_True);
    for(; anIter.More(); anIter.Next()) {
      Handle(SALOMEDS_IORAttribute) anAttr = Handle(SALOMEDS_IORAttribute)::DownCast(anIter.Value());
      CORBA::String_var anIOR = CORBA::string_dup(TCollection_AsciiString(anAttr->Get()).ToCString());
      try {
	CORBA::Object_var obj = anORB->string_to_object(anIOR);
	SALOME::GenericObj_var aGeneric = SALOME::GenericObj::_narrow(obj);
	if (!CORBA::is_nil(aGeneric)) aGeneric->Destroy();
      } catch (...) {}
    }
  } else myNbPostponed.Append(0);
}

//============================================================================
/*! Function : UndoPostponed
 *  Purpose  : 
 */
//============================================================================
void SALOMEDS_Study_i::UndoPostponed(const CORBA::Long theWay) {
  SALOMEDS::Locker lock;

  myNbUndos += theWay;
  // remove current postponed
  if (myNbPostponed.Last() > 0)
    myPostponedIORs.Remove(myPostponedIORs.Length() - myNbPostponed.Last() + 1, myPostponedIORs.Length());
  myNbPostponed(myNbPostponed.Length()) = 0;
}
